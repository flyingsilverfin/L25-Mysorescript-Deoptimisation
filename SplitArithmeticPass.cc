#include <set>
#include <map>

#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/CFG.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Type.h"

using namespace llvm;

// LLVM passes are normally defined in the anonymous namespace, as they should
// only ever be exposed via their superclass interface
namespace {


struct SplitArithmeticPass : FunctionPass, InstVisitor<SplitArithmeticPass>
{
  /// The module that we're currently working on
  Module *M = 0;
  /// The data layout of the current module.
  const DataLayout *DL = 0;
  /// Unique value.  Its address is used to identify this class.
  static char ID;
  
  LLVMContext *c;
  


  /// Call the superclass constructor with the unique identifier as the
  /// (by-reference) argument.
  SplitArithmeticPass() : FunctionPass(ID) {}


  /// Return the name of the pass, for debugging.
  StringRef getPassName() const override {
    return "Arithmetic fast/slow split pass";
  }

  /// doInitialization - called when the pass manager begins running this
  /// pass on a module.  A single instance of the pass may be run on multiple
  /// modules in sequence.
  bool doInitialization(Module &Mod) override {
    M = &Mod;
    DL = &Mod.getDataLayout();
    
    c = &M->getContext();
    
    // Return false on success.
    return false;
  }

  /// doFinalization - called when the pass manager has finished running this
  /// pass on a module.  It is possible that the pass will be used again on
  /// another module, so reset it to its initial state.
  bool doFinalization(Module &Mod) override {
    assert(&Mod == M);
    M = nullptr;
    DL = nullptr;
            
    // Return false on success.
    return false;
  }
  
  void clear() {
  	arithmetic_blocks.clear();
  	use_counts.clear();
  }


  void visitCallInst(CallInst &CI) {
  	use_counts[&CI] = 0;
  	/* TODO CORNER CASE: U could use value twice? */
  	for (User *U : CI.users()) {
  		use_counts[&CI]++;
		llvm::errs() << "Found: " << CI << " **at** " << *U << '\n';
  	}
  }


  /*
	Idea:
		1. Find all contiguous sets of 'call'ing arithmetic instructions
		2. Split before and after contigous blocks
		3. Create a slow and a fast path
			Fast path needs to bit shift all arguments since we know they're 61 bit ints, then do math, then return the in object pointer form again
		4. Confirm all values are actually integers with some bit manipulation
		5. Create branch conditional on all being integers to fast or slow path
  */
  
  std::map<CallInst*, std::pair<std::set<Value*>,Instruction*> > arithmetic_blocks;	// map with variables to check, and terminating instructions of sequence
  std::map<Value*, uint32_t> use_counts;
  
  bool runOnFunction(Function &F) override {

llvm::errs() << F << '\n';
    
  	clear();
  	
  	visit(F);
  	// we now have the number of times each CallInst return value is used... // CONFIRMED WORKS
  	
  	for (auto &entry : use_counts) {
  		llvm::errs() << entry.first << "    " << entry.second <<'\n';
 	}
  	
  	  
    for (auto &BB : F) {

      CallInst* arithmetic_block_start = nullptr;
  	  std::set<Value *> args_to_check;
  	  bool end_contiguous_block = false;
  	  std::set<Value *> computed_this_block;

      for (auto &I : BB) {
		CallInst *CI = dyn_cast<CallInst>(&I);
llvm::errs() << I << '\n';
		if (CI && isArithmeticCall(CI)) {





			// if we have found a "first" math callsite
			if (!arithmetic_block_start) {
				arithmetic_block_start = CI;
				
				// we know these are binary operators so can just hardcode :D
				Value *arg1 = CI->getArgOperand(0);
				Value *arg2 = CI->getArgOperand(1);
				
//llvm::errs() << I << '\n';
//Instruction *test0 = dyn_cast<Instruction>(arg1);	 // RIGHT so the inner inttoptr is an "operator" not an instruction, with opcode 46 but NO clue what that is			
//llvm::errs() << "HIIIIIIIi" << test0 << "  " << test0->getOpcode() << "  " <<  arg1 << '\n';

//Instruction *test1 = dyn_cast<Instruction>(arg2);	 // RIGHT so the inner inttoptr is an "operator" not an instruction, with opcode 46 but NO clue what that is			
//llvm::errs() << test1 << "  " <<  arg2  << '\n';		

				// we only need to save it to check if it's not a constant!
				if (dyn_cast<Operator>(arg1)->getOpcode() != 46) {	//46 is opcode of inttoptr?

//				if (!dyn_cast<Instruction>(arg1)) {
					//llvm::errs() << "1. Inserted an arg into args to check: " << arg1 << '\n';
					args_to_check.insert(arg1);
				}
				if (dyn_cast<Operator>(arg2)->getOpcode() != 46) {
//				if (!dyn_cast<Instruction>(arg2)) {
					args_to_check.insert(arg2);
				}
				
			} else {
llvm::errs() << *CI << '\n';
				// we know these are binary operators
				Value *arg1 = CI->getArgOperand(0);
				Value *arg2 = CI->getArgOperand(1);
				std::vector<Value *> these_args;
				these_args.push_back(arg1);
				these_args.push_back(arg2);
				for (auto &arg : these_args) {
					auto count = use_counts.find(arg);
					// if this argument value is in use_counts
					if (count != use_counts.end()) {
						count->second--; //decrement the value
						// if the count is NOT 0, then we can't include this instruction in the contiguous block
						if (count->second > 0) {
							end_contiguous_block = true;
						}
					} else {	// argument not in use_counts
						if (dyn_cast<Operator>(arg)->getOpcode() != 46) {	// if not inttoptr
							args_to_check.insert(arg);			
						} // else it's a constant and we can ignore!
					}
					
				}
			
				// here we have a second or third or fourth contiguous arithmetic instruction
				// if an arg is a Value in use_counts, it is computed so decrement it. If this 0 then continue
				// otherwise, we terminate the current arithmetic block and add copy of arg set to pair
				// if there's an arg that is is not previously computed, add it to the set of Values to check
				
			}



			// in either case, we end contiguous block if the argument computed here
			// does not have all its references used up in the following block of arithmetic calls
			//   NOT quite sure this 100% works...
			Instruction *computed = CI;
			BasicBlock::iterator it(computed);
llvm::errs() << "Iterator " << *it << '\n';
			uint32_t num_required = use_counts[computed];
			while (!end_contiguous_block) {	// either toggle this or break out manually
				Instruction *next = &*it;
llvm::errs() << "checking forward instruction: " << *next << '\n';
				CallInst *next_call = dyn_cast<CallInst>(next);
				if (next_call && isArithmeticCall(next_call)) {
					Value *arg1 = next_call->getArgOperand(0);
					Value *arg2 = next_call->getArgOperand(1);

					if (arg1 == computed) {
						num_required--;
					} 
					if (arg2 == computed) {
						num_required--;
					}

				} else {
					if (num_required > 0) {
						end_contiguous_block = true;
					}
					break;	// stop checking forward at end of arithmetic block
				}
				it++;
			}


/*

			// here we've got the last K results computed in a sequence of arithmetic
			// we need to terminate this block if any of their results are used after the current instruction
			for (auto &value : computed_this_block) {
				auto count = use_counts.find(value);
				if (count->second > 0) {
					end_contiguous_block = true;
				}
			}
			computed_this_block.insert(CI); // keep track of vars generated this block
*/


		} 
		if (end_contiguous_block || (arithmetic_block_start && !isArithmeticCall(CI)))  {	// if we previously had a sequence of arithmetic, it is now terminated
			
			// copy the arg set to pair and save it in the arithmetic blocks
			std::pair<std::set<Value *>, Instruction*> p;
			p.first = args_to_check;
			p.second = &I;
			arithmetic_blocks[arithmetic_block_start] = p;
			
			// reset now this sequence been saved!
			arithmetic_block_start = nullptr;
			end_contiguous_block = false;
			args_to_check.clear();
			computed_this_block.clear();
			
		}
  	  }
  	  
	}
	
	 	 
//	 	 std::map<CallInst*, std::pair<std::set<Value*>,Instruction*> > arithmetic_blocks
// keeping these in breaks something? some implicit type conversion that sticks around
// LOL - 2AM	 	 
  	for (auto &entry : arithmetic_blocks) {
 		llvm::errs() << *entry.first << "    " << *entry.second.second << "    " << entry.second.first.size() <<'\n';
  		for (auto &value : entry.second.first) {
 			llvm::errs() << "  " << value << '\n';
		  }
  	}
  	


	// at this point, we should have all continuous sequences of call arithmetic that need to be put into their own blocks
	//  check the args we've put aside before, then branch into fast/slow pass



	for (auto const &entry : arithmetic_blocks) {
		CallInst *first_call = entry.first;	// first math instruction
		std::set<Value*> args_to_check = entry.second.first; // args required to pass integer check for fast pass
		Instruction *split_after = entry.second.second;	// instruction after last math instruction
		BasicBlock::iterator i(split_after);
		++i;
		Instruction *after_calls = &*i;
		

		BasicBlock *bb = first_call->getParent();	// parent BB
		
		BasicBlock *slow_bb = bb->splitBasicBlock(first_call, "slow_path");	// splits before this instruction
		BasicBlock *rejoin_bb = slow_bb->splitBasicBlock(after_calls, "rejoin");	// splits after sequence of math
		BasicBlock *fast_bb = BasicBlock::Create(*c, "fast", &F, rejoin_bb); // empty fast path for now

		IRBuilder<> original_builder(bb);
		IRBuilder<> fast_builder(fast_bb);

		// TODO logic for AND'ing all the args to check together!
		Value *are_all_ints = ConstantInt::get(*c, llvm::APInt(/*nbits*/1, 1, /*bool*/false));

//llvm::errs() << "type " << are_all_ints->getType()->isIntegerTy(1) << '\n';		

		// replace unconditional branch with conditional branch in original block
		bb->getTerminator()->eraseFromParent();
		original_builder.CreateCondBr(are_all_ints, fast_bb, slow_bb); // cond, true, false

		// create PHI node in rejoin_bb
		IRBuilder<> rejoin_builder(&*rejoin_bb->begin());	//create IRBuilder for inserting before first instruction
		PHINode *phi = rejoin_builder.CreatePHI(Type::getInt8PtrTy(*c), 2, "arith_rejoin"); // this might need to change to get a different type or dependent on the result of a computation


//llvm::errs() << F << '\n';
		// at the top of the fast path, convert all required args to actual int type
		// here we can assume they are ints since they've all been checked before
		std::map<Value *, Value*> arg_map;

		for (auto &a : args_to_check) {
			Value *v = getAsSmallInt(fast_builder, a);
			Value *val = fast_builder.CreateAShr(v, ConstantInt::get(Type::getInt64Ty(*c), 3));
			arg_map[a] = val;
		}
		

		std::map<Value *, Value*> arg_dependencies;		// remap slow path data dependencies into fast path data dependencies within new basic block!

		//BasicBlock::iterator it(first_call);
		
		Value *fast_result;
		Value *slow_last_instruction;
		// here we iterate over the block of arithmetic
		// keep a hold of the last slow instruction to add an edge to the PHI node after
		//for ( ; &*it != after_calls; ++it) {	// deref iterator gives instruction, get addr to get Instruction. Compare to other instruction pointers!


		for (auto &inst : *slow_bb) {
//llvm::errs() << inst << '\n';
			if (!dyn_cast<CallInst>(&inst) || &inst == after_calls ) {
				break;
			}


			//CallInst *ci = dyn_cast<CallInst>(&*it);
			CallInst *ci = dyn_cast<CallInst>(&inst);
			slow_last_instruction = ci; 	// should automatically upcast

			// clone this call instruction across to this BB
			// then replace it so that the register saved into is correctly referenced by next instruction
			// CallInst *clone = dyn_cast<CallInst>(ci->clone());	// To be replaced later
			//fast_bb->getInstList().push_back(clone);

			// get arguments
			Value *lhs = ci->getArgOperand(0);
			Value *rhs = ci->getArgOperand(1);

			Value *lhs_int;
			Value *rhs_int;
			
			
			if (arg_map.count(lhs) > 0) { // get from map if is external arg
				lhs_int = arg_map[lhs];
			} else if (arg_dependencies.count(lhs) > 0) { // if its internal arg, get it from remapping
				lhs_int = arg_dependencies[lhs];
			} else {	// else it is a constant
				//Instruction *
				lhs_int = lhs;
			}
			
			if (arg_map.count(rhs) > 0) { // get from map if is external arg
				rhs_int = arg_map[rhs];
			} else if (arg_dependencies.count(rhs) > 0) {	// if its internal arg, get it from remapping
				rhs_int = arg_dependencies[rhs];
			} else {	// else it is a constant
				/*TODO*/
				rhs_int = rhs;
			}			

//llvm::errs() << "  " << lhs->getType() << "  " << rhs->getType() << '\n';
//llvm::errs() << "  " << lhs_int->getType() << "  " << rhs_int->getType() << '\n';
			BinaryOperator *op;
			if (isMysoreAdd(ci)) {
				op = BinaryOperator::Create(Instruction::Add, lhs_int, rhs_int, "add", fast_bb);
			} else if (isMysoreSub(ci)) {
				op = BinaryOperator::Create(Instruction::Sub, lhs_int, rhs_int, "sub", fast_bb);
			} else if (isMysoreMul(ci)) {
				op = BinaryOperator::Create(Instruction::Mul, lhs_int, rhs_int, "mul", fast_bb);
			} else if (isMysoreDiv(ci)) {
				op = BinaryOperator::Create(Instruction::SDiv, lhs_int, rhs_int, "div", fast_bb); //SDIV = signed div?
			} else {
				llvm::errs() << "Some unknown call that isn't add, sub, mult, or div has made it in here!" << '\n';
				return true; // signal error
			}

			// we have now computed BinaryOp
			Value *computed = op;

			// check which Instruction depended on the corresponding slow path value
			// and set up a future pointer to the fast path value!
			for (User *U : slow_last_instruction->users()) {
				if (CallInst *CI = dyn_cast<CallInst>(U)) {
					Value *slow_value = slow_last_instruction;
					// need to figure out which one dependended on this result...
					Value *arg1 = CI->getArgOperand(0);
					Value *arg2 = CI->getArgOperand(1);

					if (arg1 == slow_value) {
						llvm::errs() << "redirecting this argument to new fast path " << *CI << '\n';
						arg_dependencies[arg1] = computed; 	// in the future have it point to the FAST path value
					}
					if (arg2 == slow_value) {
						llvm::errs() << "redirecting this argument to new fast path " << *CI << '\n';
						arg_dependencies[arg2] = computed; 	// in the future have it point to the FAST path value
					}
					if (arg1 != slow_value && arg2 != slow_value) {
						llvm::errs() << "HELP neither argument is a User even though the call instruction is" << '\n';
					}
			  	}
			}

			

			

			//ReplaceInstWithInst(static_cast<Instruction*>(clone), static_cast<Instruction*>(op));	// this doesn't work... :(
			
			fast_result = op; // should be able to upcast cast from BinaryOperator to Value automatically
		}

		// bit shift left, add correct low bits
		Value *shifted = fast_builder.CreateShl(fast_result, ConstantInt::get(Type::getInt64Ty(*c), 3));
		Value *ored = fast_builder.CreateOr(shifted, ConstantInt::get(Type::getInt64Ty(*c), 1));
		// now we have it in the right bit pattern, cast to pointer type like everything else
		Value *result = getAsObject(fast_builder, ored);		
		// add branch to rejoin bottom bb
		fast_builder.CreateBr(rejoin_bb);
		
		// add edge to PHI node using result and basic block from fast path
		phi->addIncoming(result, fast_bb);
		// add edge to phi node from slow path's result
		phi->addIncoming(slow_last_instruction, slow_bb);


		// rewrite the previous users of the result of the slow path to use the PHI node result instead
		std::vector<User*> users;
		for (User *U : slow_last_instruction->users()) {
			users.push_back(U);	// not allowed to modify directly in iterator it seems - invalidates iterator
		}
		for (auto &U : users) {	// do modification here
			llvm::errs() << "Replacing with phi results: " <<  *U << '\n';
			if (U != phi) {
				U->replaceUsesOfWith(slow_last_instruction, phi);
			}
		}
		
		
	}

	llvm::errs() << F << '\n';
	
   
    return false;
  }



	/*
	helper 
	https://stackoverflow.com/questions/11686951/how-can-i-get-function-name-from-callinst-in-llvm
	*/
	StringRef get_function_name(CallInst *call)
	{
		if (call == nullptr) {
			return StringRef("nullptr!");	
		}
		Function *fun = call->getCalledFunction();
		if (fun) 
		    return fun->getName(); // inherited from llvm::Value
		else
		    return StringRef("indirect call");
	}

	/*
	  Helpers
	*/

	bool isArithmeticCall(CallInst *ci) {
		if (ci == nullptr) {
			return false;	
		}
		return  isMysoreAdd(ci) || isMysoreSub(ci) || isMysoreDiv(ci) || isMysoreMul(ci);
	}

	bool isMysoreAdd(CallInst *c) {
//llvm::errs() << "mysoreadd?: " << c << '\n';
		Function *fun = c->getCalledFunction();
		if (!fun) {
			return false;
		}
		StringRef function_name = fun->getName();
		return function_name.equals(StringRef("mysoreScriptAdd"));
	}

	bool isMysoreSub(CallInst *c) {
		Function *fun = c->getCalledFunction();
		if (!fun) {
			return false;
		}
		StringRef function_name = fun->getName();
		return function_name.equals(StringRef("mysoreScriptSub"));
	}
	bool isMysoreDiv(CallInst *c) {
		Function *fun = c->getCalledFunction();
		if (!fun) {
			return false;
		}
		StringRef function_name = fun->getName();
		return function_name.equals(StringRef("mysoreScriptDiv"));
	}
	bool isMysoreMul(CallInst *c) {
		Function *fun = c->getCalledFunction();
		if (!fun) {
			return false;
		}
		StringRef function_name = fun->getName();
		return function_name.equals(StringRef("mysoreScriptMul"));
	}



	Value *getAsSmallInt(IRBuilder<> &builder, Value *i) {
		if (i->getType()->isPointerTy())
		{
			return builder.CreatePtrToInt(i, Type::getInt64Ty(*c));
		}
		return builder.CreateBitCast(i, Type::getInt64Ty(*c));
	}

	Value *getAsObject(IRBuilder<> &builder, Value *i) {
		if (i->getType()->isPointerTy())
		{
			return builder.CreateBitCast(i, Type::getInt8PtrTy(*c));
		}
		return builder.CreateIntToPtr(i, Type::getInt8PtrTy(*c));
	}


};

/*
void get_all_refs_to_check(CallInst* start, Instruction *end, std::set<Value *> &refs) {

	std::set<Value *> computed;

	//BasicBlock *parent = start->getParent();
	BasicBlock::Iterator it(start);	// TODO make sure this works 
	
	for ( ; &*it != end; it++) {	// deref iterator gives instruction, get addr to get Instruction
		CallInst *ci = dyn_cast<CallInst*>(&*it);	// TODO make sure this works. dyn_cast should not be necessary 
		

		for (auto &arg : ci->arg_operands()) {
			Value *a = dyn_cast<Value*> &arg;
			// if this value is NOT in computed set, add it to refs
			if (computed.count(a) == 0) {
				refs.insert(a);
			}
		}
		computed.add(ci->getReturnedArgOperand());	// TODO this does the right thing 
	}
	
}
*/







char SplitArithmeticPass::ID;

/// This function is called by the PassManagerBuilder to add the pass to the
/// pass manager.  You can query the builder for the optimisation level and so
/// on to decide whether to insert the pass.
void addSplitArithmeticPass(const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
  PM.add(new SplitArithmeticPass());
}

/*

RegisterStandardPasses SOpt(PassManagerBuilder::EP_EarlyAsPossible,
                         addSplitArithmeticPass);
/// Register the pass to run at -O0.  This is useful for debugging the pass,
/// though modifications to this pass will typically want to disable this, as
/// most passes don't make sense to run at -O0.
RegisterStandardPasses S(PassManagerBuilder::EP_EnabledOnOptLevel0,
                         addSplitArithmeticPass);

*/                         
}




