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



  void visitCallInst(CallInst &CI) {

  }


  bool runOnFunction(Function &F) override {

    // The visit method is inherited by InstVisitor.  This will call each
    // of the visit*() methods, allowing individual functions to be inspected.
//    visit(F);

    for (auto &BB : F) {

      for (auto &I : BB) {
			 llvm::errs() << "hi" << '\n';
  	  }

	}

    // Note that we *must not* modify the IR in either of the previous methods,
    // because doing so will invalidate the iterators that we're using to
    // explore it.  After we've finished using the iterators, it is safe to do
    // the modifications.
   
    return false;
  }



};

char SplitArithmeticPass::ID;

/// This function is called by the PassManagerBuilder to add the pass to the
/// pass manager.  You can query the builder for the optimisation level and so
/// on to decide whether to insert the pass.
void addSplitArithmeticPass(const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
  PM.add(new SplitArithmeticPass());
}
}




