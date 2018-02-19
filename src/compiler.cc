#include <iostream>
#include <functional>
#include "interpreter.hh"
#include "compiler.hh"
#include "ast.hh"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/Target/TargetMachine.h>
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "StackMapParser.cpp"

#ifdef DEBUG 
#define D(x) x
#else 
#define D(x)
#endif

using namespace llvm;
using llvm::legacy::PassManager;
using namespace MysoreScript;
using namespace AST;

namespace {
/**
 * Helper function that turns a pointer that is known at compile time into a
 * constant in the code.  Note that the garbage collector can't see the code (we
 * could tweak the memory manager to use the GC to allocate memory, but even
 * then constants are often materialised as immediate operands to two or more
 * instructions so the GC wouldn't necessarily see them).  This means that the
 * address must be a GC root or must be used to store pointers to non-GC'd
 * memory.
 */
template<typename T>
Value *staticAddress(Compiler::Context &c, T *ptr, Type *ty, const Twine &name="")
{
	return c.B.CreateIntToPtr(
		ConstantInt::get(c.ObjIntTy, reinterpret_cast<uintptr_t>(ptr)), ty, name);
}
/**
 * Generate a small integer object from an integer value.
 */
Value *compileSmallInt(Compiler::Context &c, Value *i)
{
	i = c.B.CreateShl(i, ConstantInt::get(c.ObjIntTy, 3));
	return c.B.CreateOr(i, ConstantInt::get(c.ObjIntTy, 1));
}
/**
 * Generate a small integer object from an integer constant.
 */
Value *compileSmallInt(Compiler::Context &c, intptr_t i)
{
	return ConstantInt::get(c.ObjIntTy, (i << 3) | 1);
}
/**
 * Get the specified value as the LLVM type used for object pointers.  This
 * inserts a bitcast or inttoptr instruction as appropriate for the source type.
 */
Value *getAsObject(Compiler::Context &c, Value *i)
{
	if (i->getType()->isPointerTy())
	{
		return c.B.CreateBitCast(i, c.ObjPtrTy);
	}
	return c.B.CreateIntToPtr(i, c.ObjPtrTy);
}
/**
 * Get the specified value as the LLVM type used for small integer objects.
 * This inserts a bitcast or ptrtoint instruction as appropriate for the source
 * type.
 */
Value *getAsSmallInt(Compiler::Context &c, Value *i)
{
	if (i->getType()->isPointerTy())
	{
		return c.B.CreatePtrToInt(i, c.ObjIntTy);
	}
	return c.B.CreateBitCast(i, c.ObjIntTy);
}
}

Compiler::Context::Context(Interpreter::SymbolTable &g) :
	globalSymbols(g),
	M(new Module("MysoreScript", C)),
	B(C),
	ObjPtrTy(Type::getInt8PtrTy(C)),
	ObjIntTy(Type::getInt64Ty(C)),
	SelTy(Type::getInt32Ty(C))
{
	// These functions do nothing, they just ensure that the correct modules
	// are not removed by the linker.
	LLVMInitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();
	LLVMLinkInMCJIT();

}

Value *Compiler::Context::lookupSymbolAddr(const std::string &str)
{
	// If the value is in the compiler's symbol table, then it's stored as an
	// LLVM `Value` representing the address of the variable, so just return it.
	if (Value *addr = symbols[str])
	{
		return addr;
	}
	// If it's in the global symbol table that we inherited from the interpreter
	// then it's a pointer, so construct an LLVM `Value` of the correct type and
	// value.
	if (Obj *global = globalSymbols[str])
	{
		return staticAddress(*this, global, ObjPtrTy->getPointerTo());
	}
	llvm_unreachable("Symbol not found");
}


// custong memory manager to get a hold of the stack map section address!
// RIGHT this doesn't work!!
/*class JITMemoryManager : public SectionMemoryManager                                                          
{                                                                                                             
	uint8_t *allocateDataSection(uintptr_t Size, unsigned Alignment, unsigned SectionID, StringRef SectionName, bool isReadOnly) {

		std::cerr << "DataSection: " << SectionID << std::endl;

		uint8_t *loc = SectionMemoryManager::allocateDataSection(Size, Alignment, SectionID, SectionName, isReadOnly);
		if (SectionName == ".llvm_stackmaps") {
			std::cerr << "Found LLVm stackmap section! At: " << static_cast<void*>(loc) << std::endl;
			std::cerr << "Size requested: " << Size << ", SectionID: " << SectionID << ", SectionName: " << SectionName.data()<< std::endl;
			
			// construct a new StackMapParser with this location
			StackMapParser smparser(loc);
			std::cerr << "Number of stackmap records: " << smparser.getNumRecords() << std::endl;

		}
        return loc;                                                                                           
     }                                                                                                         
};        
*/


AST::ClosureDecl* currentlyCompiling = nullptr;

// THIS WORKS!
// THANK YOU PYSTON & OPEN SOURCE
class StackmapJITEventListener : public llvm::JITEventListener {
	private:
	public:
    virtual void NotifyObjectEmitted(const llvm::object::ObjectFile&, const llvm::RuntimeDyld::LoadedObjectInfo&);
};

void StackmapJITEventListener::NotifyObjectEmitted(const llvm::object::ObjectFile& Obj,
		                                                   const llvm::RuntimeDyld::LoadedObjectInfo& L) {
	for (const auto& sec : Obj.sections()) {
		llvm::StringRef name;
		sec.getName(name);
		if (name == ".llvm_stackmaps") {
			uint64_t sm_addr = L.getSectionLoadAddress(sec);
			StackMapParser *smp = new StackMapParser(sm_addr);
			// register stackmap with runtime
			MysoreScript::registerStackMap(currentlyCompiling, smp);
		}	
	}
};

ClosureInvoke Compiler::Context::compile()
{
	// Construct a couple of pass managers to run the optimisations.
	llvm::legacy::FunctionPassManager FPM(M.get());
	PassManager MPM;
	PassManagerBuilder Builder;

// I think this is broken...
//	Builder.addExtension(PassManagerBuilder::EP_LoopOptimizerEnd, addSplitArithmeticPass);

	Builder.OptLevel = 2;
	Builder.populateFunctionPassManager(FPM);
	Builder.populateModulePassManager(MPM);

	// If you want to see the LLVM IR before optimisation, uncomment the
	// following line:
	// D(M->dump());	


	// Run the passes to optimise the function / module.
	MPM.run(*M);
	FPM.run(*F);


	// If you want to see the LLVM IR after optimisation, uncomment the
	// following line:
	// D(llvm::errs() << " --- Optimized IR --- " << '\n';
	// M->dump();)

	// create intercepting memory manager
//	std::unique_ptr<RTDyldMemoryManager> mm_ptr(new JITMemoryManager()); 	

	std::string FunctionName = F->getName();
	// D(llvm::errs() << "Compiled function or closure: " << FunctionName << "\n";)
	std::string err;
	EngineBuilder EB(std::move(M));
	TargetMachine * tm = EB.selectTarget();
	tm->Options.PrintMachineCode = true;	// print x86
	// Construct an execution engine (JIT)
	ExecutionEngine *EE = EB.setEngineKind(EngineKind::JIT)
		.setErrorStr(&err)
//		.setMCJITMemoryManager(std::move(mm_ptr))
	//	.create(tm);
		.create();

	EE->RegisterJITEventListener(new StackmapJITEventListener());
	if (!EE)
	{
		fprintf(stderr, "Failed to construct Execution Engine: %s\n",
				err.c_str());
		return nullptr;
	}


	// Use the execution engine to compile the code.  Note that we leave the
	// ExecutionEngine live here because it owns the memory for the function.
	// It would be better to provide our own JIT memory manager to manage the
	// memory (and allow us to GC the functions if their addresses don't exist
	// on the stack and they're replaced by specialised versions, but for now
	// it's fine to just leak)
	return reinterpret_cast<ClosureInvoke>(EE->getFunctionAddress(FunctionName));
}

llvm::FunctionType *Compiler::Context::getMethodType(int ivars, int args)
{
	PointerType *ObjTy = ObjPtrTy;
	if (ivars)
	{
		// The fields in a class are the class pointer and the array of instance
		// variables
		Type* fields[2];
		// isa (actually a class pointer not an object pointer)
		fields[0] = ObjPtrTy;
		fields[1] = ArrayType::get(ObjPtrTy, ivars);
		ObjTy = StructType::create(fields)->getPointerTo();
	}
	// Set up the argument types for the closure invoke function.
	SmallVector<Type*, 10> paramTypes;
	// First two parameters are the implicit receiver and selector parameters.
	paramTypes.push_back(ObjTy);
	paramTypes.push_back(SelTy);
	// All parameters are objects.
	paramTypes.insert(paramTypes.end(), args, ObjPtrTy);
	return FunctionType::get(ObjPtrTy, paramTypes, false);
}

llvm::FunctionType *Compiler::Context::getClosureType(int bound, int args)
{
	// Create an opaque structure type to use as the invoke parameter
	auto *ClosureTy = StructType::create(C);
	// Collect the parameter types.  We don't expect more than 10 parameters,
	// so allocate any smaller number of types on the stack.
	SmallVector<Type*, 10> paramTypes;
	paramTypes.push_back(ClosureTy->getPointerTo());
	// All parameters are objects.
	paramTypes.insert(paramTypes.end(), args, ObjPtrTy);
	auto *invokeTy = FunctionType::get(ObjPtrTy, paramTypes, false);
	SmallVector<Type*, 6> fields;
	// isa (actually a class pointer not an object pointer)
	fields.push_back(ObjPtrTy);
	// Number of parameters
	fields.push_back(ObjPtrTy);
	// Invoke function.
	fields.push_back(invokeTy->getPointerTo());
	// AST pointer
	fields.push_back(ObjPtrTy);
	if (bound)
	{
		// The array of bound variables.
		fields.push_back(ArrayType::get(ObjPtrTy, bound));
	}
	ClosureTy->setBody(fields);
	return invokeTy;
}



CompiledMethod ClosureDecl::compileMethod(Class *cls,
                                          Interpreter::SymbolTable &globalSymbols)
{
	auto &params = parameters->arguments;
	Compiler::Context c(globalSymbols);
	currentlyCompiling = this;
	// Get the type of the method as an LLVM type
	FunctionType *ClosureInvokeTy = c.getMethodType(cls->indexedIVarCount,
			params.size());
	// Create the LLVM function
	c.F = Function::Create(ClosureInvokeTy, GlobalValue::ExternalLinkage,
			"invoke", c.M.get());
	// Insert the first basic block and store all of the parameters.
	BasicBlock *entry = BasicBlock::Create(c.C, "entry", c.F);
	c.B.SetInsertPoint(entry);

// record in the runtime which function is currently being executed
// TODO this might not be the best way to find associated stackmap
// potential performance loss here!
// Also need to set it back to the same value when a `Call` returns
	Value *currently_executing_ptr = staticAddress(c, &MysoreScript::cur_jit_function, c.ObjPtrTy->getPointerTo(), "Runtime ptr cur jit func");
	Value *this_addr = staticAddress(c, currentlyCompiling, c.ObjPtrTy, "This ClosureDecl");
	Value *v = c.B.CreateStore(this_addr, currently_executing_ptr);
//	v->dump();



	// We'll store the arguments into stack allocations so that they can be
	// written to.
	auto AI = c.F->arg_begin();
	// The first two arguments are the two implicit parameters, self and cmd
	auto selfPtr = c.B.CreateAlloca(c.ObjPtrTy);
	auto cmdPtr = c.B.CreateAlloca(c.ObjPtrTy);
	// Add these two stack locations to the symbol table.
	c.symbols["self"] = selfPtr;
	c.symbols["cmd"] = cmdPtr;
	// We're now going to create stack allocations for all of the parameters and
	// all of the locals.  We do this *before* we initialise any of them because
	// that makes life easier for the LLVM pass that generates SSA form, which
	// only guarantees to promote allocas that occur before any other
	// instructions to SSA registers.
	SmallVector<Value*, 10> paramAllocas;
	SmallVector<Value*, 10> localAllocas;

	// Create a stack allocation for each explicit parameter
	for (size_t i=0 ; i<params.size() ; i++)
	{
		paramAllocas.push_back(c.B.CreateAlloca(c.ObjPtrTy));
	}
	// Create a stack allocation for each local variable
	for (size_t i=0 ; i<decls.size() ; i++)
	{
		localAllocas.push_back(c.B.CreateAlloca(c.ObjPtrTy));
	}
	// Now we'll initialise the parameter allocations
	auto alloca = paramAllocas.begin();
	// First store the self pointer in its slot (which is already in the symbol
	// table)
	c.B.CreateStore(c.B.CreateBitCast(&*(AI++), c.ObjPtrTy), selfPtr);
	// The selector is a 32-bit integer, promote it to an object and store it.
	c.B.CreateStore(getAsObject(c, compileSmallInt(c, c.B.CreateZExt(&*(AI++),
						c.ObjIntTy))), cmdPtr);
	for (auto &arg : params)
	{
		// Set the name of the stack slot.  This isn't necessary, but makes
		// reading the generated IR a bit easier.
		(*alloca)->setName(*arg.get());
		// Store the argument in the stack slot.
		c.B.CreateStore(&*(AI++), *alloca);
		// Set this slot (specifically, the address of this stack allocation) to
		// the address used when looking up the name of the argument.
		c.symbols[*arg.get()] = *(alloca++);
	}
	alloca = localAllocas.begin();
	// Now do almost the same thing for locals
	for (auto &local : decls)
	{
		(*alloca)->setName(local);
		// Initialise all local variables with null.
		c.B.CreateStore(ConstantPointerNull::get(c.ObjPtrTy), *alloca);

		

		c.symbols[local] = *(alloca++);
	}
	// Next we need to add the instance variables to the symbol table.
	if (cls->indexedIVarCount)
	{
		// The type of the object pointer argument
		PointerType *ArgTy = cast<PointerType>(ClosureInvokeTy->params()[0]);
		// The type of the object 
		StructType *ObjTy =
			cast<StructType>(cast<PointerType>(ArgTy)->getElementType());
		// This does pointer arithmetic on the first argument to get the address
		// of the array of instance variables.
		Value *iVarsArray = c.B.CreateStructGEP(ObjTy, &*c.F->arg_begin(), 1);
		// The type of the instance variables array
		Type *iVarsArrayTy = ObjTy->elements()[1];
		// The type of the arguments array
		for (int i=0 ; i<cls->indexedIVarCount ; i++)
		{
			const char *name = cls->indexedIVarNames[i];
			// Now we do similar arithmetic on the array to get the address of
			// each instance variable.
			c.symbols[name] =
				c.B.CreateStructGEP(iVarsArrayTy, iVarsArray, i, name);
		}
	}
	// Compile the statements in the method.
	body->compile(c);
	// If we hit a return statement, it will clear the insert block, so if there
	// is still a valid insert block then the function ends with an unterminated
	// basic block.
	if (c.B.GetInsertBlock() != nullptr)
	{
		// Return null if there's no explicit return
		c.B.CreateRet(ConstantPointerNull::get(c.ObjPtrTy));
	}
	// Generate the compiled code.
	return reinterpret_cast<CompiledMethod>(c.compile());
}


// NOTE second entry point to compilation process!
ClosureInvoke ClosureDecl::compileClosure(Interpreter::SymbolTable &globalSymbols)
{
	auto &params = parameters->arguments;
	Compiler::Context c(globalSymbols);
	currentlyCompiling = this;
	// Get the LLVM type of the closure invoke function
	FunctionType *ClosureInvokeTy = c.getClosureType(boundVars.size(),
			params.size());
	// Create the LLVM function
	c.F = Function::Create(ClosureInvokeTy, GlobalValue::ExternalLinkage, "invoke", c.M.get());
	// Insert the first basic block and store all of the parameters.
	BasicBlock *entry = BasicBlock::Create(c.C, "entry", c.F);


	c.B.SetInsertPoint(entry);

// record in the runtime which function is currently being executed
// TODO this might not be the best way to find associated stackmap
// potential performance loss here!
// Also need to set it back to the same value when a `Call` returns
	Value *currently_executing_ptr = staticAddress(c, &MysoreScript::cur_jit_function, c.ObjPtrTy->getPointerTo(), "Runtime ptr cur jit func");
	Value *this_addr = staticAddress(c, currentlyCompiling, c.ObjPtrTy, "This ClosureDecl");
	Value *v = c.B.CreateStore(this_addr, currently_executing_ptr);
//	v->dump();


	auto AI = c.F->arg_begin();
	auto selfPtr = c.B.CreateAlloca(c.ObjPtrTy);
	c.symbols["self"] = selfPtr;
	SmallVector<Value*, 10> paramAllocas;
	SmallVector<Value*, 10> localAllocas;

	for (size_t i=0 ; i<params.size() ; i++)
	{
		paramAllocas.push_back(c.B.CreateAlloca(c.ObjPtrTy));
	}
	for (size_t i=0 ; i<decls.size() ; i++)
	{
		localAllocas.push_back(c.B.CreateAlloca(c.ObjPtrTy));
	}
	auto alloca = paramAllocas.begin();

	c.B.CreateStore(c.B.CreateBitCast(&*(AI++), c.ObjPtrTy), selfPtr);
	for (auto &arg : params)
	{
		(*alloca)->setName(*arg.get());
		c.B.CreateStore(&*(AI++), *alloca);
		c.symbols[*arg.get()] = *(alloca++);
	}
	alloca = localAllocas.begin();
	for (auto &local : decls)
	{
		(*alloca)->setName(local);
		c.B.CreateStore(ConstantPointerNull::get(c.ObjPtrTy), *alloca);
		c.symbols[local] = *(alloca++);
	}
	// If this closure refers to external variables then they'll have been
	// copied into the closure structure.  Add those to the symbol table in the
	// same way that we added instance variables for methods.
	if (!boundVars.empty())
	{
		// The type of the closure pointer argument
		PointerType *ArgTy = cast<PointerType>(ClosureInvokeTy->params()[0]);
		// The type of the closure object
		StructType *ObjTy =
			cast<StructType>(cast<PointerType>(ArgTy)->getElementType());
		// The type of the instance variables array
		Type *boundVarsArrayTy = ObjTy->elements()[4];

		Value *boundVarsArray = c.B.CreateStructGEP(ObjTy, &*c.F->arg_begin(), 4);
		int i=0;
		for (auto &bound : boundVars)
		{
			c.symbols[bound] = c.B.CreateStructGEP(boundVarsArrayTy, boundVarsArray, i++, bound);
		}
	}
	body->compile(c);
	if (c.B.GetInsertBlock() != nullptr)
	{
		c.B.CreateRet(ConstantPointerNull::get(c.ObjPtrTy));
	}
	return c.compile();
}

Value *ClosureDecl::compileExpression(Compiler::Context &c)
{
	// Make sure that we know what the bound variables are.
	check();
	auto &params = parameters->arguments;
	// Get the type of the invoke function
	FunctionType *invokeTy = c.getClosureType(boundVars.size(), params.size());
	// Get a pointer to the invoke function type, which we will cast our
	// function pointer to.
	PointerType *invokePtrTy = invokeTy->getPointerTo();
	// Get the type of the first parameter (a pointer to the closure structure)
	PointerType *closurePtrTy = cast<PointerType>(invokeTy->getParamType(0));
	// The type of the closure.
	StructType *closureTy = cast<StructType>(closurePtrTy->getElementType());
	// The size of the closure is the size of `Closure`, which contains all of
	// the fields shared by all closures, and then space for all of the bound
	// variables.
	size_t closureSize = sizeof(struct Closure) + boundVars.size() * sizeof(Obj);
	// Insert the `GC_malloc` function (provided by libgc) into the module,
	// bitcast to return a pointer to our closure type.
	Constant *allocFn = c.M->getOrInsertFunction("GC_malloc", closurePtrTy,
			c.ObjIntTy);
	// Allocate GC'd memory for the closure.  Note that it would often be more
	// efficient to do this on the stack, but only if we can either statically
	// prove that the closure is not captured by anything that is called or if
	// we can promote it to the heap if it is.
	Value *closure = c.B.CreateCall(allocFn, ConstantInt::get(c.ObjIntTy,
				closureSize));
	// Set the isa pointer to the closure class.
	c.B.CreateStore(staticAddress(c, &ClosureClass, c.ObjPtrTy),
			c.B.CreateStructGEP(closureTy, closure, 0));
	// Set the parameters pointer to the number of parameters.
	c.B.CreateStore(getAsObject(c, compileSmallInt(c, params.size())),
			c.B.CreateStructGEP(closureTy, closure, 1));
	// If we've already compiled the function for this closure, then insert a
	// pointer to it into the closure, otherwise use the trampoline that calls
	// back into the interpreter.
	// Note: This means that if we later compile this closure we should
	// recompile the enclosing function or the invocation of the closure will be
	// expensive.
	ClosureInvoke closureFn = compiledClosure ? compiledClosure :
		Interpreter::closureTrampolines[params.size()];
	// Store the invoke pointer
	c.B.CreateStore(
		c.B.CreateBitCast(staticAddress(c, closureFn, c.ObjPtrTy), invokePtrTy),
		c.B.CreateStructGEP(closureTy, closure, 2));
	// Set the AST pointer
	c.B.CreateStore(staticAddress(c, this, c.ObjPtrTy),
		c.B.CreateStructGEP(closureTy, closure, 3));
	if (!boundVars.empty())
	{
		// Get a pointer to the arrayjof bound variables
		Value *boundVarsArray = c.B.CreateStructGEP(closureTy, closure, 4);
		Type *boundVarsArrayTy = closureTy->elements()[4];
		int i=0;
		for (auto &var : boundVars)
		{
			// Load each bound variable and then insert it into the closure at the
			// correct index.
			c.B.CreateStore(c.B.CreateLoad(c.lookupSymbolAddr(var)),
				c.B.CreateStructGEP(boundVarsArrayTy, boundVarsArray, i++, var));
		}
	}
	// Add this closure to our symbol table.
	c.B.CreateStore(c.B.CreateBitCast(closure, c.ObjPtrTy), c.symbols[name]);
	return closure;
}




Value *Call::compileExpression(Compiler::Context &c)
{
	SmallVector<Value*, 10> args;
	// Compile the expression that evaluates to the object being called.
	Value *obj = getAsObject(c, callee->compileExpression(c));
	assert(obj);
	// For both closure and method invocations, the object being invoked is the
	// first argument
	args.push_back(obj);
	// If this is a method invocation, then the next argument is the selector.
	if (method)
	{
		Selector sel = lookupSelector(*method.get());
		args.push_back(ConstantInt::get(c.SelTy, sel));
	}
	// Now add each of the explicit arguments.
	auto &argsAST = arguments->arguments;
	for (auto &arg : argsAST)
	{
		args.push_back(getAsObject(c, arg->compileExpression(c)));
	}
	// If there's no method, then we're trying to invoke a closure.
	if (!method)
	{
		// Get the closure invoke type. 
		FunctionType *invokeFnTy = c.getClosureType(0, args.size() - 1);
		// Get the type of a pointer to the closure object 
		PointerType *closurePtrTy = cast<PointerType>(invokeFnTy->getParamType(0));
		// Get the closure type.  Note: This will need to be changed once
		// typeless pointers are here, so that `getClosureType` will return the
		// struct type as well as the function type.
		Type *closureTy = closurePtrTy->getElementType();
		// Cast the called object to a closure
		Value *closure = c.B.CreateBitCast(obj, closurePtrTy);
		// Make sure that the closure argument is the correct type
		args[0] = closure;
		// Compute the address of the pointer to the closure invoke function
		Value *invokeFn = c.B.CreateStructGEP(closureTy, closure, 2);
		// Load the address of the invoke function
		invokeFn = c.B.CreateLoad(invokeFn);
		// Insert the call	
		Value *invoke_call = c.B.CreateCall(invokeFn, args, "call_closure");
	// after the call, need to reset currently_executing ptr
	Value *currently_executing_ptr = staticAddress(c, &MysoreScript::cur_jit_function, c.ObjPtrTy->getPointerTo(), "Runtime ptr cur jit func");
	Value *this_addr = staticAddress(c, currentlyCompiling, c.ObjPtrTy, "This ClosureDecl");
	c.B.CreateStore(this_addr, currently_executing_ptr);
		// return c.B.CreateCall(invokeFn, args, "call_closure");
		return invoke_call;
	}




BasicBlock* entry = c.B.GetInsertBlock();


BasicBlock *cache_hit = BasicBlock::Create(c.C, "cache_hit", c.F);
BasicBlock *cache_miss = BasicBlock::Create(c.C, "cache_miss", c.F);

Value* not_null_obj /* i1 */ = c.B.CreateIsNotNull(obj, "not_null_obj");
BasicBlock* if_not_null_obj_body = BasicBlock::Create(c.C, "if_not_null", c.F);
c.B.CreateCondBr(not_null_obj, if_not_null_obj_body, cache_miss);

c.B.SetInsertPoint(if_not_null_obj_body);

// Value* int_obj /* i64 */ = getAsSmallInt(c, obj);
//Value* int_obj = c.B.CreatePtrToInt(obj, c.ObjIntTy);
//Value* tmp /* i64 */ = c.B.CreateAnd(int_obj, ConstantInt::get(c.ObjIntTy, ~7), "tmp");
//Value* is_int_obj /* i1 */ = c.B.CreateIsNotNull(tmp, "is_int_obj");



if (type_assumption == (intptr_t)&SmallIntClass) {
//	D(std::cerr << " Type assumption: " << (void*) type_assumption << " = SmallIntClass\n";)
	// confirm object is an int	
	Value *intConst = ConstantInt::get(Type::getInt64Ty(c.C), 7);
	Value *objAsInt = c.B.CreatePtrToInt(obj, Type::getInt64Ty(c.C)); 
	Value *isInt = c.B.CreateAnd(objAsInt, intConst);

	c.B.CreateCondBr(isInt, cache_hit, cache_miss);


} else {
//	D(std::cerr << " Type assumption: " << (void*) type_assumption << " != SmallIntClass\n";)
	Value *expected_cls = staticAddress(c, (void*)type_assumption, c.ObjPtrTy); // just a constant
	Value* cls_addr /* i8* */ = c.B.CreateConstGEP1_64(obj, offsetof(Object, isa), "cls_addr");
	cls_addr /* i8** */ = c.B.CreateBitCast(cls_addr, cls_addr->getType()->getPointerTo(), "cls_addr_cast");
	Value* cls /* i8* */ = c.B.CreateLoad(cls_addr, "cls");

	Value* same /* i1 */ = c.B.CreateICmpEQ(expected_cls, cls, "same");
	c.B.CreateCondBr(same, cache_hit, cache_miss);	

}


/* --------------- SLOW PATH BB --------------- */
c.B.SetInsertPoint(cache_miss);
// no type accounting to do here since it's all done in resume interpreter!

//-------patchpoints/stackmaps!-------

	std::vector<Type *> arg_types;
	std::vector<Value *> stackmap_args;
	
	// insert return type first?
//	arg_types.push_back(Type::getVoidTy(c.C));	


	// stackmap ID
	// ask runtime for next stackmap ID
	uint64_t stackmap_id = get_next_stackmap_id();
	Value *id = ConstantInt::get(c.ObjIntTy, stackmap_id); 
	Type *id_type = id->getType();
	arg_types.push_back(id_type);
	stackmap_args.push_back(id);

	// reverved bytes - not doing code patching so 0 bytes
	// apparently need to reserve some space for the call instruction
	// 15 is least that works
	
	Value *reserved_bytes = ConstantInt::get(Type::getInt32Ty(c.C), 15);
	Type *bytes_type = reserved_bytes->getType();
	arg_types.push_back(bytes_type);
	stackmap_args.push_back(reserved_bytes);

	// create call to stackmap intrinsic

//	// don't actually need to pass types to intrisic declaration!
//	// https://stackoverflow.com/questions/27569967/adding-intrinsics-using-an-llvm-pass
	
	// ---patchpoint not stackmap---
	// function to call
	Constant *func = c.M->getOrInsertFunction("x86_trampoline", c.ObjPtrTy);
	Value *func_i8ptr = c.B.CreateBitCast(func, Type::getInt8PtrTy(c.C));
	stackmap_args.push_back(func_i8ptr); // needs an i8*... not sure what getOrInsert returns

	// need to insert the number of arguments to the function
	uint32_t num_args = 3; // AST node to resume at, self, and cmd
	num_args += currentlyCompiling->parameters->arguments.size();
	num_args += currentlyCompiling->decls.size(); // local vars
	num_args += currentlyCompiling->boundVars.size(); // bound vars
//	D(std::cerr << "Number of params+ locals + bound vars: " << num_args - 3 << std::endl;)
	stackmap_args.push_back(ConstantInt::get(Type::getInt32Ty(c.C), num_args)); 

	
	// --- arguments for the anyregcc call ---
	// idea: insert, in order:
	// 1. pointer to AST node to resume at
	// 3. values of all parameters
	// 2. values of all local decls (using standard iterator)
	// 3. values of all bound vars (using standard iterator)


	// AST Node to resume at
	stackmap_args.push_back(staticAddress(c, this, c.ObjPtrTy));

	// self and cmd pointers
	if (c.symbols["self"]) {
//		D(std::cerr << "This closure has a self ptr" << std::endl;)	
		stackmap_args.push_back(c.symbols["self"]);
	} else {
//		D(std::cerr << "No self ptr, pushing nullptr" << std::endl;)
		stackmap_args.push_back(ConstantPointerNull::get(c.ObjPtrTy));
	}
	if (c.symbols["cmd"]) {
//		D(std::cerr << "This Closure has a CMD/sel ptr" << std::endl;)
		stackmap_args.push_back(c.symbols["cmd"]);
	} else {
//		std::cerr << "No CMD/sel ptr, pushing nullptr" << std::endl;
		stackmap_args.push_back(ConstantPointerNull::get(c.ObjPtrTy));
	}
//	stackmap_args.push_back(c.B.CreateLoad(c.symbols["self"]));
//	stackmap_args.push_back(c.symbols["self"]);
//	stackmap_args.push_back(c.B.CreateLoad(c.symbols["cmd"]));
//	stackmap_args.push_back(c.symbols["cmd"]);
	// params
	for (auto &param : currentlyCompiling->parameters->arguments) {
//		stackmap_args.push_back(c.B.CreateLoad(c.symbols[*param.get()]));
		stackmap_args.push_back(c.symbols[*param.get()]);
	}


	// decls
	// rely on deterministic hashing...
	for (auto &local : currentlyCompiling->decls) {
//		D(std::cerr << "Saving local c.symbols[" << local << "] = " << c.symbols[local] << std::endl;)
//		stackmap_args.push_back(staticAddress(c, c.symbols[local], c.ObjPtrTy->getPointerTo()));
//		stackmap_args.push_back(c.B.CreateLoad(c.symbols[local]));
		stackmap_args.push_back(c.symbols[local]);
	}

	// bound vars
	if (!currentlyCompiling->boundVars.empty()) {
		for (auto &bound : currentlyCompiling->boundVars) {
//			stackmap_args.push_back(staticAddress(c, c.symbols[bound], c.ObjPtrTy->getPointerTo()));
//			stackmap_args.push_back(c.B.CreateLoad(c.symbols[bound]));			
			stackmap_args.push_back(c.symbols[bound]);
	
		}
	}


	Function *fun = Intrinsic::getDeclaration(c.M.get(), Intrinsic::experimental_patchpoint_i64);
	CallInst *func_call = CallInst::Create(fun, stackmap_args);
	func_call->setCallingConv(CallingConv::AnyReg);
	Value *value_from_interpreter = c.B.Insert(func_call);

	Value *result_as_obj = getAsObject(c, value_from_interpreter);
	// print to check
/*
	ArrayRef<Type*> aref(c.ObjPtrTy);
	Value *CalleeF = c.M->getOrInsertFunction("print_msg", FunctionType::get(c.SelTy, aref, false));
	std::vector<Value *> ArgsV;
	ArgsV.push_back(result_as_obj);
	CallInst *print_call = CallInst::Create(CalleeF, ArgsV);
	print_call->setCallingConv(CallingConv::C);
	c.B.Insert(print_call);
*/
	
	c.B.CreateRet(result_as_obj);

// ------ Cache Hit ------	
c.B.SetInsertPoint(cache_hit);

if (cachedMethod == nullptr) {
	D(std::cerr << "ERROR: cachedMethod being hardcoded into compiled code is NULL\n";)
}

// instrumentation, reset alternative path counter
Value* alt_type_count_addr = staticAddress(c, &alternative_type_count, c.ObjPtrTy->getPointerTo());
c.B.CreateStore(getAsObject(c, ConstantInt::get(c.ObjIntTy, 0)), alt_type_count_addr);

FunctionType *methodType = c.getMethodType(0, args.size() - 2);
Value* method_ptr_ptr = staticAddress(c, cachedMethod, methodType->getPointerTo()->getPointerTo());
Value* cm /* (A -> B)* */ = c.B.CreateLoad(method_ptr_ptr, "method_ptr");
Value* res1 /* B */ = c.B.CreateCall(cm, args, "res1_call_cached_method");

// need to reset the currently executing function ptr in the runtime	
Value *currently_executing_ptr = staticAddress(c, &MysoreScript::cur_jit_function, c.ObjPtrTy->getPointerTo(), "Runtime ptr cur jit func");
Value *this_addr = staticAddress(c, currentlyCompiling, c.ObjPtrTy, "this ClosureDecl");
Value *s = c.B.CreateStore(this_addr, currently_executing_ptr);

return res1;


/* type assumptions mean no more of these checks
Value *intConst = ConstantInt::get(Type::getInt64Ty(c.C), 7);
Value *objAsInt = c.B.CreatePtrToInt(obj, Type::getInt64Ty(c.C)); 
Value *isInt = c.B.CreateAnd(objAsInt, intConst);
// this needs to be a 1 bit wide int apparently!	
Value *is_int_obj = c.B.CreateIntCast(isInt, Type::getInt1Ty(c.C), false);

BasicBlock* if_is_int_obj_then = BasicBlock::Create(c.C, "if_is_int_obj.then", c.F);
BasicBlock* if_is_int_obj_else = BasicBlock::Create(c.C, "if_is_int_obj.else", c.F);
c.B.CreateCondBr(is_int_obj, if_is_int_obj_then, if_is_int_obj_else);


c.B.SetInsertPoint(if_is_int_obj_then);

	ArrayRef<Type*> tmpref3(c.ObjPtrTy);
	Value *dbgprint3 = c.M->getOrInsertFunction("print_msg", FunctionType::get(c.SelTy, tmpref3, false));
	std::vector<Value *> printargs3;
	printargs3.push_back(c.B.CreateIntToPtr(ConstantInt::get(c.ObjIntTy, 3), c.ObjPtrTy));
//	printargs.push_back(staticAddress(c, (void*)type_assumption, c.ObjPtrTy));
	CallInst *print_call_tmp3 = CallInst::Create(dbgprint3, printargs3);
	c.B.Insert(print_call_tmp3);
*/

// Value* cls_1 /* i8* */ = staticAddress(c, &SmallIntClass, c.ObjPtrTy);
// BasicBlock* if_is_int_obj_end /* i1 */ = BasicBlock::Create(c.C, "if_is_int_obj.end", c.F);
// c.B.CreateBr(if_is_int_obj_end);

// c.B.SetInsertPoint(if_is_int_obj_else);

// Value* cls_addr /* i8* */ = c.B.CreateConstGEP1_64(obj, offsetof(Object, isa), "cls_addr");
// cls_addr /* i8** */ = c.B.CreateBitCast(cls_addr, cls_addr->getType()->getPointerTo(), "cls_addr_cast");
// Value* cls_2 /* i8* */ = c.B.CreateLoad(cls_addr, "cls_2");


//	Value *isa = c.B.CreateLoad(obj); // this is the 'isa' ptr ie a pointer to a class
//	Value *cls_2 = c.B.CreateIntToPtr(isa, c.ObjPtrTy); // convert to right type of pointer

/*
	ArrayRef<Type*> tmpref(c.ObjPtrTy);
	Value *dbgprint = c.M->getOrInsertFunction("print_msg", FunctionType::get(c.SelTy, tmpref, false));
	std::vector<Value *> printargs;
	Value *cls_as_obj = getAsObject(c, cls_2);
	printargs.push_back(cls_2);
//	printargs.push_back(c.B.CreateIntToPtr(ConstantInt::get(c.ObjIntTy, 4), c.ObjPtrTy));
//	printargs.push_back(staticAddress(c, (void*)type_assumption, c.ObjPtrTy));
	CallInst *print_call_tmp = CallInst::Create(dbgprint, printargs);
	c.B.Insert(print_call_tmp);
*/

// c.B.CreateBr(if_is_int_obj_end);
// /* DEBUG */ if_is_int_obj_else->dump();


// c.B.SetInsertPoint(if_is_int_obj_end);
/* And this */
// PHINode* cls_assigned /* i8* */ = c.B.CreatePHI(c.ObjPtrTy, 2, "cls_assigned");
// cls_assigned->addIncoming(cls_1, if_is_int_obj_then);
// cls_assigned->addIncoming(cls_2, if_is_int_obj_else);
// c.B.CreateBr(if_not_null_obj_cont);

// c.B.SetInsertPoint(if_not_null_obj_cont);
/* Forgot about this one first time around */
// PHINode* cls /* i8* */ = c.B.CreatePHI(c.ObjPtrTy, 2, "cls");
// cls->addIncoming(ConstantPointerNull::get(c.ObjPtrTy), entry);
// cls->addIncoming(cls_assigned, if_is_int_obj_end);

// make this into a permanent choice!!
// Value *prev_cls_val = staticAddress(c, (void*)type_assumption, c.ObjPtrTy); // just a constant


// Value* prev_cls_addr /* i8** */ = staticAddress(c, &type_assumption, c.ObjPtrTy->getPointerTo());
// Value* prev_cls_val /* i8* */ = c.B.CreateLoad(prev_cls_addr, "prev_cls_val");
// Value* same /* i1 */ = c.B.CreateICmpEQ(prev_cls_val, cls, "same");
/*

	ArrayRef<Type*> tmpref4(c.ObjPtrTy);
	Value *dbgprint4 = c.M->getOrInsertFunction("print_msg", FunctionType::get(c.SelTy, tmpref4, false));
	std::vector<Value *> printargs4a;
	std::vector<Value *> printargs4b;
	std::vector<Value *> printargs4c;
	printargs4a.push_back(c.B.CreateIntToPtr(same, c.ObjPtrTy));
	printargs4b.push_back(prev_cls_val);
	printargs4c.push_back(cls);
//	printargs.push_back(staticAddress(c, (void*)type_assumption, c.ObjPtrTy));
	c.B.CreateCall(dbgprint4, printargs4a);
	c.B.CreateCall(dbgprint4, printargs4b);
	c.B.CreateCall(dbgprint4, printargs4c);

*/
/*
BasicBlock* if_same_then = BasicBlock::Create(c.C, "if_same.then", c.F);
BasicBlock* if_same_else = BasicBlock::Create(c.C, "if_same.else", c.F);
c.B.CreateCondBr(same, if_same_then, if_same_else);
*/

// FAST PATH
// c.B.SetInsertPoint(if_same_then);
// reset alternative type count
// Value* alt_type_count_addr = staticAddress(c, &alternative_type_count, c.ObjPtrTy->getPointerTo());
// c.B.CreateStore(getAsObject(c, ConstantInt::get(c.ObjIntTy, 0)), alt_type_count_addr);

// If we are invoking a method, then we must first look up the method, then call it.


//FunctionType *methodType = c.getMethodType(0, args.size() - 2);
//Value* prev_cm_addr = staticAddress(c, &cachedMethod, methodType->getPointerTo()->getPointerTo()->getPointerTo());
//Value* prev_cm_val /* (A -> B)** */ = c.B.CreateLoad(prev_cm_addr, "prev_cm_val");
//Value* valid_prev_cm /* i1 */ = c.B.CreateIsNotNull(prev_cm_val, "valid_prev_cm");
//BasicBlock* if_valid_prev_cm_body = BasicBlock::Create(c.C, "if_valid_prev_cm.body", c.F);
//BasicBlock* if_same_end = BasicBlock::Create(c.C, "if_same.end", c.F);
//c.B.CreateCondBr(valid_prev_cm, if_valid_prev_cm_body, if_same_end);
// /* DEBUG */ if_same_then->dump();

//c.B.SetInsertPoint(if_valid_prev_cm_body);
//Value* cm /* (A -> B)* */ = c.B.CreateLoad(prev_cm_val, "cm");
//Value* res1 /* B */ = c.B.CreateCall(cm, args, "res1_call_cached_method");
//BasicBlock* ans = BasicBlock::Create(c.C, "ans", c.F);
//c.B.CreateBr(ans);
// /* DEBUG */ if_valid_prev_cm_body->dump();





/*
 
methodType->getReturnType();
Constant *lookupFn =
	c.M->getOrInsertFunction(
	"ptrToCompiledMethodForSelector",
	methodType->getPointerTo()->getPointerTo(),
	obj->getType(),
	c.SelTy
);


// Insert the call to the function that performs the lookup.  This will
// always return *something* that we can call, even if it's just a function
// that reports an error.

Value* methodFn  = c.B.CreateCall(lookupFn, {obj, args[1]}, "methodFn");
c.B.CreateStore(methodFn , prev_cm_addr);
// Call the method

Value* deref_methodFn = c.B.CreateLoad(methodFn);
Value* res2  = c.B.CreateCall(deref_methodFn, args, "res2_call_method");
c.B.CreateBr(ans);
// if_same_end->dump();
*/

// c.B.SetInsertPoint(ans);
// PHINode* result /* B */ = c.B.CreatePHI(c.ObjPtrTy, 1, "cls");
// result->addIncoming(res1 /* B */, if_valid_prev_cm_body);
// result->addIncoming(res2 /* B */, if_same_end);
// /* DEBUG */ ans->dump();




/*
 * My old code follows, reference
 *
 *
// create a branch here mirroring interpreter check
//  ie. create a static pointer to function pointer cachedMethod
//  create static pointer to cached Selector
//  create static pointer to cachedClass
//  check *(ptr to cachedMethod) is not null
//   and *(ptr to cachedClass) == getClassOf(obj)
//  getClassOf needs to be implemented in IR
//
//  create two basic blocks, one with the usual lookupFn
//  as well as rewriting the *(ptr)s as required
//  and the other bb invoking the *(ptr to cachedMethod) directly

	
//	FunctionType *methodType = c.getMethodType(0, 1);
//	Constant *getClassTypeFn = c.m->getOrInsertFunction("getClassFor", methodType->getPointerTo(), obj->getType());
//	Value *classType = c.B.CreateCall(getClassTypeFn, {obj});

// need to extract the 'isa' class from the obj
//  use GEP to get address 0th member of this struct
//  this is the _address_ of the 'isa' member
//  ie. a pointer to a class pointer

	llvm::errs() << "running inline cache optimisation\n";

	// set up slow ang fast path asic blocks
//	BasicBlock* fast = BasicBlock::Create(c.C, "cached", c.F);
	BasicBlock* slow = BasicBlock::Create(c.C, "cacheMiss", c.F);
	BasicBlock *rejoinBB = BasicBlock::Create(c.C, "rejoin", c.F);

	// set up first conditional to check if object is null or small int
	// Value *isObjNull = c.B.CreateIsNull(obj); // ignore this for now

	BasicBlock *smallIntBB = BasicBlock::Create(c.C, "intClass", c.F);
	BasicBlock *clsBB = BasicBlock::Create(c.C, "otherClass", c.F);
	BasicBlock *clsJoin = BasicBlock::Create(c.C, "clsJoin", c.F);

	if (Compiler::DEBUG_JIT >= DEBUG_NORMAL) {
	    llvm::errs() << "entry: " << c.B.GetInsertBlock() << '\n';			
		llvm::errs() << "slow: " << slow << '\n';
		llvm::errs() << "rejoinBB: " << rejoinBB << '\n';
		llvm::errs() << "smallIntBB: " << smallIntBB << '\n';
		llvm::errs() << "clsBB: " << clsBB << '\n';
		llvm::errs() << "clsJoin: " << clsJoin << '\n';
	}

	Value *intConst = ConstantInt::get(Type::getInt64Ty(c.C), 7);
	Value *objAsInt = c.B.CreatePtrToInt(obj, Type::getInt64Ty(c.C)); 
	
	Value *isInt = c.B.CreateAnd(objAsInt, intConst);

	// this needs to be a 1 bit wide int apparently!	
	isInt = c.B.CreateIntCast(isInt, Type::getInt1Ty(c.C), false);

	Value *cond = c.B.CreateCondBr(isInt, smallIntBB, clsBB);

	
	// Basic Block to get cls if it is any Object other than SmallIntClass
	c.B.SetInsertPoint(clsBB);


	// create the object struct type
	StructType *obj_struct_type = llvm::StructType::create(c.C, "object struct");
	std::vector<llvm::Type*> members;
	members.push_back(Type::getInt8PtrTy(c.C)); // the 'isa' member we want hopefully
	obj_struct_type->setBody(members);

// TODO this needs to be done properly
//	Value *isaPtr = c.B.CreateStructGEP(obj_struct_type->getPointerTo(), obj, 0); // addr of isa ptr TODO needs type
//	Value *isaPtr = c.B.CreateLoad(obj);
//	Value *isa = c.B.CreateLoad(isaPtr); // now we have the Class Ptr ignore null case
//
//  ditch the GEP and just access it directly since it's the first thing in the object struct
// NOT good should use GEP

	Value *isa = c.B.CreateLoad(obj); // this is the 'isa' ptr ie a pointer to a class
	isa = c.B.CreateIntToPtr(isa, c.ObjPtrTy); // convert to right type of pointer
	c.B.CreateBr(clsJoin);

// Basic Block to get cls if it is SmallIntClass
	c.B.SetInsertPoint(smallIntBB);
	Value *clsPtr = staticAddress(c, &SmallIntClass, c.ObjPtrTy, "smallIntClass"); // apprently this is the right type
	c.B.CreateBr(clsJoin);




	//Basic Block to rejoin to once class has been obtained
	c.B.SetInsertPoint(clsJoin);
	PHINode *phiObjClass = c.B.CreatePHI(c.ObjPtrTy, 2, "classsRejoin"); // now have either small int or class
	phiObjClass->addIncoming(clsPtr, smallIntBB);
	phiObjClass->addIncoming(isa, clsBB);

// type we need for function
	FunctionType *methodType = c.getMethodType(0, args.size() - 2);
	PointerType *methodPtr = methodType->getPointerTo();
	PointerType *methodPtrPtr = methodPtr->getPointerTo();

	// set up some static pointers
	PointerType *ptrTy = c.ObjPtrTy;
	PointerType *ptrPtrTy = PointerType::getUnqual(ptrTy);
	Value *ptrToCachedMethod = staticAddress(c, &cachedMethod, methodPtrPtr); // TODO needs type!
	Value *ptrToCachedClass = staticAddress(c, &cachedClass, ptrPtrTy); // TODO needs type
	Value *cachedMeth = c.B.CreateLoad(ptrToCachedMethod);
	Value *haveCached = c.B.CreateIsNotNull(cachedMeth);

	BasicBlock *notNull = BasicBlock::Create(c.C, "cachedNotNull", c.F);

	if (Compiler::DEBUG_JIT >= DEBUG_NORMAL) {
		llvm::errs() << "cachedNotNull: " << notNull << '\n';
	}
	c.B.CreateCondBr(haveCached, notNull, slow);

	c.B.SetInsertPoint(notNull);
	Value *cachedClass = c.B.CreateLoad(ptrToCachedClass); // get class pointer
	cachedClass = c.B.CreateIntToPtr(cachedClass, c.ObjPtrTy); // need to get a ptr type out
	Value *classesMatch = c.B.CreateICmpEQ(cachedClass, phiObjClass);

	c.B.CreateCondBr(classesMatch, rejoinBB, slow);


	// slow path
	c.B.SetInsertPoint(slow);



//attempt at patchpoints/stackmaps!

	std::vector<Type *> arg_types;
	std::vector<Value *> stackmap_args;
	
	// insert return type first?
//	arg_types.push_back(Type::getVoidTy(c.C));	


	// stackmap ID
	// ask runtime for next stackmap ID
	uint64_t stackmap_id = get_next_stackmap_id();
	Value *id = ConstantInt::get(c.ObjIntTy, stackmap_id); 
	Type *id_type = id->getType();
	arg_types.push_back(id_type);
	stackmap_args.push_back(id);

	// reverved bytes - not doing code patching so 0 bytes
	// apparently need to reserve some space for the call instruction
	// 15 is least that works
	
	Value *reserved_bytes = ConstantInt::get(Type::getInt32Ty(c.C), 15);
	Type *bytes_type = reserved_bytes->getType();
	arg_types.push_back(bytes_type);
	stackmap_args.push_back(reserved_bytes);

	// create call to stackmap intrinsic

//	ArrayRef<Type*> args_aref(arg_types.data(), arg_types.size());

//	// don't actually need to pass types to intrisic declaration!
//	// https://stackoverflow.com/questions/27569967/adding-intrinsics-using-an-llvm-pass
//	Function *fun = Intrinsic::getDeclaration(c.M.get(), Intrinsic::experimental_stackmap);//, args_aref);
//	c.B.CreateCall(fun, stackmap_args);


	
	// ---patchpoint not stackmap---
	// function to call
	Constant *func = c.M->getOrInsertFunction("x86_trampoline", c.ObjPtrTy);
	Value *func_i8ptr = c.B.CreateBitCast(func, Type::getInt8PtrTy(c.C));
	stackmap_args.push_back(func_i8ptr); // needs an i8*... not sure what getOrInsert returns

	// need to insert the number of arguments to the function
	uint32_t num_args = 2; // AST node to resume at, self, and //cmd
	num_args += currentlyCompiling->parameters->arguments.size();
	num_args += currentlyCompiling->decls.size(); // local vars
	num_args += currentlyCompiling->boundVars.size(); // bound vars
//	num_args++; // testing
	std::cerr << "Number of locals + bound vars: " << num_args - 2 << std::endl;
	stackmap_args.push_back(ConstantInt::get(Type::getInt32Ty(c.C), num_args)); 
// test value
//	stackmap_args.push_back(ConstantInt::get(Type::getInt64Ty(c.C), 199));

	
	// --- arguments for the anyregcc call ---
	// idea: insert, in order:
	// 1. pointer to AST node to resume at
	// 3. values of all parameters
	// 2. values of all local decls (using standard iterator)
	// 3. values of all bound vars (using standard iterator)


	std::cerr << "'this' ptr to Call that we exited at: " << (void*) this << std::endl;

	// AST Node to resume at
	stackmap_args.push_back(staticAddress(c, this, c.ObjPtrTy));

	// self and cmd pointers
//	stackmap_args.push_back(c.B.CreateLoad(c.symbols["self"]));
	stackmap_args.push_back(c.symbols["self"]);
//	stackmap_args.push_back(c.B.CreateLoad(c.symbols["cmd"]));
//	stackmap_args.push_back(c.symbols["cmd"]);
	// params
	for (auto &param : currentlyCompiling->parameters->arguments) {
//		stackmap_args.push_back(c.B.CreateLoad(c.symbols[*param.get()]));
		stackmap_args.push_back(c.symbols[*param.get()]);
	}


	// decls
	// rely on deterministic hashing...
	for (auto &local : currentlyCompiling->decls) {
		std::cerr << "Saving local c.symbols[" << local << "] = " << c.symbols[local] << std::endl;
//		stackmap_args.push_back(staticAddress(c, c.symbols[local], c.ObjPtrTy->getPointerTo()));
//		stackmap_args.push_back(c.B.CreateLoad(c.symbols[local]));
		stackmap_args.push_back(c.symbols[local]);
	}

	// bound vars
	if (!currentlyCompiling->boundVars.empty()) {
		for (auto &bound : currentlyCompiling->boundVars) {
//			stackmap_args.push_back(staticAddress(c, c.symbols[bound], c.ObjPtrTy->getPointerTo()));
//			stackmap_args.push_back(c.B.CreateLoad(c.symbols[bound]));			
			stackmap_args.push_back(c.symbols[bound]);
	
		}
	}


	Function *fun = Intrinsic::getDeclaration(c.M.get(), Intrinsic::experimental_patchpoint_i64);
	CallInst *func_call = CallInst::Create(fun, stackmap_args);
	func_call->setCallingConv(CallingConv::AnyReg);
	Value *value_from_interpreter = c.B.Insert(func_call);

	// print to check
	ArrayRef<Type*> aref(c.ObjPtrTy);
	Value *CalleeF = c.M->getOrInsertFunction("print_msg", FunctionType::get(c.SelTy, aref, false));
	std::vector<Value *> ArgsV;
	Value *result_as_obj = getAsObject(c, value_from_interpreter);
	ArgsV.push_back(result_as_obj);
	CallInst *print_call = CallInst::Create(CalleeF, ArgsV);
	print_call->setCallingConv(CallingConv::C);
	c.B.Insert(print_call);
//	c.B.CreateCall(CalleeF, ArgsV, "printCall");

	c.B.CreateRet(result_as_obj);
//	c.B.CreateRet(value_from_interpreter);	
	


//	Constant *lookupFn = c.M->getOrInsertFunction("compiledMethodForSelector",
//			methodType->getPointerTo(), obj->getType(), c.SelTy);
	// Insert the call to the function that performs the lookup.  This will
	// always return *something* that we can call, even if it's just a function
	// that reports an error.
//	Value *methodFn = c.B.CreateCall(lookupFn, {obj, args[1]});

//	methodFn->getType()->print(llvm::errs());
//	llvm::errs() << '\n';
//	ptrToCachedMethod->getType()->print(llvm::errs());
//	llvm::errs() << '\n';
//	llvm::errs() << methodFn->getType() << '\n';
//	llvm::errs() << ptrToCachedMethod->getType() << '\n';

//	c.B.CreateStore(methodFn, ptrToCachedMethod);
//	c.B.CreateStore(phiObjClass, ptrToCachedClass);
//	c.B.CreateBr(rejoinBB);

	// rejoin 
	c.B.SetInsertPoint(rejoinBB);
	PHINode *rejoined = c.B.CreatePHI(cachedMeth->getType(), 1, "rejoin");
//	rejoined->addIncoming(methodFn, slow);
	rejoined->addIncoming(cachedMeth, notNull);

	// Call the method
	Value *call_method = c.B.CreateCall(rejoined, args, "call_method");
	// need to reset the currently executing function ptr in the runtime	
	Value *currently_executing_ptr = staticAddress(c, &MysoreScript::cur_jit_function, c.ObjPtrTy->getPointerTo(), "Runtime ptr cur jit func");
	Value *this_addr = staticAddress(c, currentlyCompiling, c.ObjPtrTy, "this ClosureDecl");
	Value *s = c.B.CreateStore(this_addr, currently_executing_ptr);

//	return c.B.CreateCall(rejoined, args, "call_method");
	return call_method;
*/
}

void Statements::compile(Compiler::Context &c)
{
	for (auto &s : statements)
	{
		// If we've hit a return statement then any subsequent statements in
		// this block are dead so just return.
		if (c.B.GetInsertBlock() == nullptr)
		{
			return;
		}
		s->compile(c);
	}
}



void Return::compile(Compiler::Context &c)
{
	// Insert a return instruction with the correct value
	Value *ret = expr->compileExpression(c);
	c.B.CreateRet(getAsObject(c, ret));
	// Clear the insert point so nothing else tries to insert instructions after
	// the basic block terminator
	c.B.ClearInsertionPoint();
}

void IfStatement::compile(Compiler::Context &c)
{
	// Compute the condition
	Value *cond = condition->compileExpression(c);
	// Create the basic block that we'll branch to if the condition is false and
	// after executing if the condition is true.
	BasicBlock *cont = BasicBlock::Create(c.C, "if.cont", c.F);
	// Create the block that contains the body of the if statement
	BasicBlock *ifBody = BasicBlock::Create(c.C, "if.body", c.F);
	// Cast the condition to a small int.  We aren't unconditionally
	// interpreting it as a small int, we just want to be able to do some
	// arithmetic on it...
	cond = getAsSmallInt(c, cond);
	// Now right shift it by 3.  If it is an object pointer, it will now be 0 if
	// it were a null pointer before the shift.  If it is a small integer, then
	// it will now contain its numeric value.  Either way, a value of 0 means
	// either a zero integer or null, and a value of anything else means a valid
	// object or non-zero integer.
	cond = c.B.CreateLShr(cond, ConstantInt::get(c.ObjIntTy, 3));
	// Create a comparison with 0 that we can then branch on
	cond = c.B.CreateIsNotNull(cond);
	// Branch to the body if it's not 0, to the continuation block if it is
	c.B.CreateCondBr(cond, ifBody, cont);
	// Compile the body of the if statement.
	c.B.SetInsertPoint(ifBody);
	body->compile(c);
	// If the if statement didn't end with a return, then return control flow to
	// the block after the if statement.
	if (c.B.GetInsertBlock() != nullptr)
	{
		c.B.CreateBr(cont);
	}
	// Continue from the end
	c.B.SetInsertPoint(cont);
}
void WhileLoop::compile(Compiler::Context &c)
{
	// Create three blocks, one for the body of the test (which we will
	// unconditionally branch back to at the end of the body), one for the loop
	// body (which we will skip if the condition is false) and one for the end,
	// which we will branch to after the loop has finished.
	BasicBlock *condBlock = BasicBlock::Create(c.C, "while.cond", c.F);
	BasicBlock *cont = BasicBlock::Create(c.C, "while.cont", c.F);
	BasicBlock *whileBody = BasicBlock::Create(c.C, "while.body", c.F);
	// Unconditionally branch to the block for the condition and compile it
	c.B.CreateBr(condBlock);
	c.B.SetInsertPoint(condBlock);
	// Compile the condition expression
	Value *cond = condition->compileExpression(c);
	// Convert it to an integer and test that it isn't null
	cond = getAsSmallInt(c, cond);
	cond = c.B.CreateLShr(cond, ConstantInt::get(c.ObjIntTy, 3));
	cond = c.B.CreateIsNotNull(cond);
	// Branch into the loop body or past it, depending on the condition value.
	c.B.CreateCondBr(cond, whileBody, cont);
	c.B.SetInsertPoint(whileBody);
	// Compile the body of the loop.
	body->compile(c);
	// Unconditionally branch back to the condition to check it again.
	c.B.CreateBr(condBlock);
	// Set the insert point to the block after the loop.
	c.B.SetInsertPoint(cont);
}

Value *AST::StringLiteral::compileExpression(Compiler::Context &c)
{
	// If we don't have a cached string object for this literal, then poke the
	// interpreter to generate one.
	if (!static_cast<Obj>(cache))
	{
		Interpreter::Context ic;
		cache = evaluateExpr(ic);
	}
	// Return a constant pointer to the cached value.
	return staticAddress(c, static_cast<Obj>(cache), c.ObjPtrTy);
}
Value *Number::compileExpression(Compiler::Context &c)
{
	// Construct a constant small integer value
	return compileSmallInt(c, value);
}

void Decl::compile(Compiler::Context &c)
{
	// Decls are collected and stack space allocated for them when we compile a
	// closure, but we still want to initialise them at their first use.
	if (init)
	{
		assert(c.symbols[name]);
		Value *v = init->compileExpression(c);
		// v->dump();
		c.B.CreateStore(getAsObject(c, v),
				c.symbols[name]);
	}
}
void Assignment::compile(Compiler::Context &c)
{
	assert(c.symbols[target->name]);
	// Store the result of the expression in the address of the named variable.
	Value *v = getAsObject(c, expr->compileExpression(c));
	// v->dump();
	c.B.CreateStore(v,
			c.symbols[target->name]);
}

Value *VarRef::compileExpression(Compiler::Context &c)
{
	return c.B.CreateLoad(c.lookupSymbolAddr(name));
}
Value *NewExpr::compileExpression(Compiler::Context &c)
{
	// Look up the class statically
	Class *cls = lookupClass(className);
	// Create a value corresponding to the class pointer
	Value *clsPtr = staticAddress(c, cls, c.ObjPtrTy);
	// Look up the function that creates instances of objects
	Constant *newFn = c.M->getOrInsertFunction("newObject", c.ObjPtrTy,
			c.ObjPtrTy);
	// Call the function with the class pointer as the argument
	return c.B.CreateCall(newFn, clsPtr, "new");
}

namespace {
/**
 * A function type that is used by the `compileBinaryOp` function.  Each
 * `compileBinOp` method for arithmetic operations provides an instance of this
 * type to insert the correct instruction for performing its operation on the
 * two arguments.
 */
typedef std::function<Value*(Compiler::Context &c, Value*, Value*)> BinOpFn;

/**
 * Helper function that inserts all of the code required for comparison
 * operations.
 */
Value *compileCompare(Compiler::Context &c, CmpInst::Predicate Op,
                    Value *LHS, Value *RHS)
{
	// Comparisons work on small integers or pointers, so just insert the
	// relevant compare instruction.
	Value *cmp = c.B.CreateICmp(Op, getAsSmallInt(c, LHS),
	                             getAsSmallInt(c, RHS), "cmp");
	// The result is an i1 (one-bit integer), so zero-extend it to the size of a
	// small integer
	cmp = c.B.CreateZExt(cmp, c.ObjIntTy, "cmp_object");
	// Then set the low bit to make it an object.
	return compileSmallInt(c, cmp);

}

/**
 * Helper function that inserts all of the code required for small integer
 * operations, either calling the relevant method or doing the arithmetic.  For
 * real objects, the function named by the `slowCallFnName` parameter is called,
 * which then invokes the correct method.  For integers, the function inserts
 * the binary op specified by `Op`.
 */
Value *compileBinaryOp(Compiler::Context &c, Value *LHS, Value *RHS,
		Instruction::BinaryOps Op, const char *slowCallFnName)
{

/*


	// Get the two operands as integer values
	Value *LHSInt = getAsSmallInt(c, LHS);
	Value *RHSInt = getAsSmallInt(c, RHS);
	// And them together.  If they are both small ints, then the low bits of the
	// result will be 001.
	Value *isSmallInt = c.B.CreateAnd(LHSInt, RHSInt);
	// Now mask off the low 3 bits.
	isSmallInt = c.B.CreateAnd(isSmallInt, ConstantInt::get(c.ObjIntTy, 7));
	// If the low three bits are 001, then it is a small integer
	isSmallInt = c.B.CreateICmpEQ(isSmallInt, ConstantInt::get(c.ObjIntTy, 1));
	// Create three basic blocks, one for the small int case, one for the real
	// object case, and one for when the two join together again.
	BasicBlock *cont = BasicBlock::Create(c.C, "cont", c.F);
	// Set its value to the result of whichever basic block we arrived from
	result->addIncoming(intResult, small);
	result->addIncoming(objResult, obj);
	// Return the result
	return result;

*/

	LHS = getAsObject(c, LHS);
	RHS = getAsObject(c, RHS);

	// Call the function that handles the object case
	Value *objResult = c.B.CreateCall(c.M->getOrInsertFunction(slowCallFnName,
				c.ObjPtrTy, LHS->getType(), RHS->getType()),
			{LHS, RHS});
	
	return objResult;
}
} // End anonymous namespace

Value *CmpNe::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileCompare(c, CmpInst::Predicate::ICMP_NE, LHS, RHS);
}
Value *CmpEq::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileCompare(c, CmpInst::Predicate::ICMP_EQ, LHS, RHS);
}
Value *CmpGt::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileCompare(c, CmpInst::Predicate::ICMP_SGT, LHS, RHS);
}
Value *CmpLt::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileCompare(c, CmpInst::Predicate::ICMP_SLT, LHS, RHS);
}
Value *CmpGE::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileCompare(c, CmpInst::Predicate::ICMP_SGE, LHS, RHS);
}
Value *CmpLE::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileCompare(c, CmpInst::Predicate::ICMP_SLE, LHS, RHS);
}
Value *Subtract::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileBinaryOp(c, LHS, RHS, Instruction::Sub, "mysoreScriptSub");
}
Value *Add::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileBinaryOp(c, LHS, RHS, Instruction::Add, "mysoreScriptAdd");
}
Value *Multiply::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileBinaryOp(c, LHS, RHS, Instruction::Mul, "mysoreScriptMul");
}
Value *Divide::compileBinOp(Compiler::Context &c, Value *LHS, Value *RHS)
{
	return compileBinaryOp(c, LHS, RHS, Instruction::SDiv, "mysoreScriptDiv");
}
