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



  /*
	Idea:
		1. Find all contiguous sets of 'call'ing arithmetic instructions
		2. Split before and after contigous blocks
		3. Create a slow and a fast path
			Fast path needs to bit shift all arguments since we know they're 61 bit ints
		4. Confirm all values are actually integers with some bit manipulation
		5. Create branch conditional on all being integers to fast or slow path
  */
  std::map<CallInst*, CallInst*> arithmetic_blocks;
  bool runOnFunction(Function &F) override {


    for (auto &BB : F) {

	  CallInst* arithmetic_block_start;
      for (auto &I : BB) {
		if (CallInst* ci = dyn_cast<CallInst*>(&I)) {
			// we have found a "first" callsite
			if (!arithmetic_block_start) {
				arithmetic_block_start = ci;
			}
			// if its the second, third.. etc. we just want the first that is not arithmetic now

		} else {
			// we want to split again before this instruction
			arithmetic_blocks[arithmetic_block_start] = dyn_cast<CallInst*>(&I);	
			arithmetic_block_start = nullptr;	// reset now this sequence been saved!
		}
  	  }
	}

	// at this point, we should have all continuous sequences of call arithmetic
	
	
   
    return false;
  }



};

/*
helper 
https://stackoverflow.com/questions/11686951/how-can-i-get-function-name-from-callinst-in-llvm
*/
StringRef get_function_name(CallInst &call)
{
    Function *fun = call.getCalledFunction();
    if (fun) 
        return fun->getName(); // inherited from llvm::Value
    else
        return StringRef("indirect call");
}

/*
  Helper
*/

bool isArithmeticCall(CallInst &c) {
	StringRef function_name = get_function_name(c);
	return (function_name.equals_lower(StringRef("mysoreScriptAdd") ||
		function_name.equals_lower(StringRef("mysoreScriptSub") ||
		function_name.equals_lower(StringRef("mysoreScriptDiv") ||
		function_name.equals_lower(StringRef("mysoreScriptMult"));
}


char SplitArithmeticPass::ID;

/// This function is called by the PassManagerBuilder to add the pass to the
/// pass manager.  You can query the builder for the optimisation level and so
/// on to decide whether to insert the pass.
void addSplitArithmeticPass(const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
  PM.add(new SplitArithmeticPass());
}
}




