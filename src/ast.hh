#include "Pegmatite/ast.hh"
#include "runtime.hh"
#include "interpreter.hh"
#include <unordered_set>
#include <functional>
#include <time.h>

#include <iostream>

#ifdef DEBUG 
#define D(x) x
#else 
#define D(x)
#endif

namespace Compiler
{
	class Context;
}

namespace llvm
{
	class Value;
}

namespace AST 
{
	using pegmatite::ASTPtr;
	using pegmatite::ASTChild;
	using pegmatite::ASTList;
	using pegmatite::ErrorReporter;
	using MysoreScript::Obj;

	/**
	 * The abstract superclass for all statements.
	 */
	struct Statement : pegmatite::ASTContainer
	{
		// only used by a couple subclasses	
		intptr_t type_assumption = 0; 
		intptr_t alternative_type = 0;
		uint64_t alternative_type_count = 0;
	
		virtual void skip_to(Interpreter::Context &c, Statement* ast_node) = 0;

		 /* Execute this statement in the interpreter. */
		virtual void interpret(Interpreter::Context &c) = 0;
		/**
		 * Compile this statement to LLVM IR.
		 */
		virtual void compile(Compiler::Context &c) {}
		/**
		 * Recursively visit all of the children of this statement, collecting
		 * variables that are declared and used in it.  This is used to
		 * allocate space for all declared variables and to bind variables to
		 * closures.
		 */
		virtual void collectVarUses(std::unordered_set<std::string> &decls,
		                            std::unordered_set<std::string> &uses) = 0;
	};

	/**
	 * Abstract superclass for expressions (statements that evaluate to
	 * something).
	 */
	class Expression : public Statement
	{
		protected:
		
		/**
		 * Cached result for constant expression.
		 */
		Interpreter::Value cache;
		/**
		 * Evaluate the expression.
		 */
		virtual Obj evaluateExpr(Interpreter::Context &c) = 0;
		public:

		void skip_to(Interpreter::Context &c, Statement* ast_node) override {
			expr_skip_to(c, ast_node);	
		}

		virtual Obj expr_skip_to(Interpreter::Context &c, Statement* ast_node) = 0;

		/**
		 * Returns true if the expression is constant and therefore doesn't
		 * need interpreting every time.  The result will be cached after the
		 * first time.
		 */
		virtual bool isConstantExpression() { return false; }
		/**
		 * Evaluate the expression, caching the result if it's a constant
		 * expression.
		 */
		Obj evaluate(Interpreter::Context &c);
		/**
		 * Interpret this expression as if it were a statement.
		 */
		void interpret(Interpreter::Context &c) override final
		{
			evaluate(c);
		}
			/**
			 * Compile this expression as if it were a statement.
		 */
		void compile(Compiler::Context &c) override final
		{
			compileExpression(c);
		}
		/**
		 * Compile the expression, returning the LLVM value that represents the
		 * result of the expression.
		 */
		virtual llvm::Value *compileExpression(Compiler::Context &c) = 0;
	};
	/**
	 * Block of statements.
	 */
	struct Statements : pegmatite::ASTContainer
	{

		void skip_to(Interpreter::Context &c, Statement* ast_node);				
		/**
		 * Interprets each of the statements in turn.  If the context is marked
		 * as returning, then aborts execution and returns.
		 */
		void interpret(Interpreter::Context &c);
		/**
		 * Compiles each statement in turn.
		 */
		void compile(Compiler::Context &c);
		/**
		 * Visits each statement to collect its variable uses and definitions.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses);
		private:
		/**
		 * The statements in this block, built by the parser.
		 */
		ASTList<Statement> statements;
	};
	/**
	 * A number literal.
	 */
	struct Number : public Expression
	{
		
		Obj expr_skip_to(Interpreter::Context &c, Statement* ast_node) override {
			if (ast_node == this) {
				D(std::cerr << "HELP Number matched ast_node??" << std::endl;)
				c.astNodeFound = true;
			}
			return evaluateExpr(c);
		}

		// Can use default skip_to provided in Statement

		/**
		 * The value of this literal.  MysoreScript integers are 61 bits, so a
		 * 64-bit integer is enough to store the value.
		 */
		int64_t value;
		/**
		 * Constructs the class from the source range.  Numbers are terminals,
		 * so this will construct the numeric value from the text.
		 */
		bool construct(const pegmatite::InputRange &r,
		               pegmatite::ASTStack &st, const ErrorReporter&) override
		{
			pegmatite::constructValue(r, value);
			return true;
		}
		/**
		 * All literals are constant expressions.
		 */
		bool isConstantExpression() override { return true; }
		protected:
		/**
		 * Construct the small integer (integer in a pointer with the low bit
		 * set to 1) value corresponding to this literal.
		 */
		Obj evaluateExpr(Interpreter::Context &c) override
		{
			return reinterpret_cast<Obj>((value << 3) | 1);
		}
		/**
		 * Compile the expression, returning an LLVM constant integer.
		 */
		llvm::Value *compileExpression(Compiler::Context &c) override;
		/**
		 * Literals do not define or use any values.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			return;
		}
	};
	/**
	 * A string literal.  MysoreScript strings are instances of the String
	 * class.
	 */
	struct StringLiteral : public Expression, pegmatite::ASTString
	{

		Obj expr_skip_to(Interpreter::Context &c, Statement* ast_node) override {
			if (ast_node == this) {
				D(std::cerr << "HELP StringLiteral matched ast_node" << std::endl;)
				c.astNodeFound = true;
			}
			return evaluateExpr(c);
		}

		/**
		 * Construct the string from the source text.
		 */
		bool construct(const pegmatite::InputRange &r,
		               pegmatite::ASTStack &st, const ErrorReporter &er) override
		{
			pegmatite::ASTString::construct(r, st, er);
			std::string::size_type newline;
			while ((newline = find("\\n")) != std::string::npos)
			{
				replace(newline, 2, "\n");
			}
			return true;
		}
		/**
		 * Literals are constant expressions.
		 */
		bool isConstantExpression() override { return true; }
		/**
		 * Evaluate the 
		 */
		Obj evaluateExpr(Interpreter::Context &c) override;
		/**
		 * Compile the string.  Generates a string object and returns a constant
		 * pointer value that refers to it.
		 */
		llvm::Value *compileExpression(Compiler::Context &c) override;
		/**
		 * Literals do not define or use any values.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			return;
		}
	};
	/**
	 * Abstract superclass for binary operators.
	 */
	struct BinOpBase : public Expression
	{
	    Obj expr_skip_to(Interpreter::Context &c, Statement* ast_node) override;
	
		Obj performOp(Interpreter::Context &c, Obj LHS, Obj RHS);	

		/**
		 * The left-hand side of the operation.
		 */
		ASTPtr<Expression> lhs;
		/**
		 * The right-hand side of the operation.
		 */
		ASTPtr<Expression> rhs;
		/**
		 * Returns whether this operation is a comparison.  In MysoreScript,
		 * comparisons work on primitive values, but other binary operators can
		 * become methods if invoked with operands that are not integers.
		 */
		virtual bool isComparison() { return false; }
		/**
		 * A binary operation is a constant expression if both of its operands
		 * are binary expressions.  This is not quite true in the general case,
		 * as we could potentially add methods to the String class that take
		 * other constant expression values as arguments but have side effects.
		 * We don't currently though.
		 */
		bool isConstantExpression() override
		{
			return lhs->isConstantExpression() && rhs->isConstantExpression();
		}
		/**
		 * Evaluates the expression, either invoking a subclass
		 * `evaluateWithIntegers` method or by calling the relevant method on
		 * the left-hand side.
		 */
		Obj evaluateExpr(Interpreter::Context &c) override;
		/**
		 * Compile this binary operation with the two sides already already
		 * compiled.
		 */
		virtual llvm::Value *compileBinOp(Compiler::Context &c,
		                                  llvm::Value *LHS,
		                                  llvm::Value *RHS) = 0;
		/**
		 * Compile the expression by compiling the two sides and then calling
		 * `compileBinOp` (implemented in subclasses) to compile the operation.
		 */
		llvm::Value *compileExpression(Compiler::Context &c) override
		{
			return compileBinOp(c,
			                    lhs->compileExpression(c),
			                    rhs->compileExpression(c));
		}
		/**
		 * Return the method name that this operator expands to if the operands
		 * are not small integers.
		 */
		virtual const char *methodName() = 0;
		/**
		 * Collect the uses and declarations in this expression.  Neither side
		 * of a binary operation can contain declarations, but both sides can
		 * reference variables.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			lhs->collectVarUses(decls, uses);
			rhs->collectVarUses(decls, uses);
		}
		/**
		 * Evaluate (interpret) this expression, having already determined that
		 * the two sides are integer values.
		 */
		virtual intptr_t evaluateWithIntegers(intptr_t lhs, intptr_t rhs) = 0;
	};
	/**
	 * Template superclass for binary operators.  The template argument
	 * provides the implementation of the function that handles the
	 * small-integer implementation.
	 */
	template<class Op>
	struct BinOp : public BinOpBase
	{
		/**
		 * Evaluate (interpret) this expression, having already determined that
		 * the two sides are integer values.
		 */
		intptr_t evaluateWithIntegers(intptr_t lhs, intptr_t rhs) override
		{
			Op o;
			return o(lhs, rhs);
		}
	};
	/**
	 * Multiply operation.
	 */
	struct Multiply : public BinOp<std::multiplies<intptr_t>>
	{
		/**
		 * Multiply operations become the `mul()` method invocations on
		 * non-integer objects.
		 */
		const char *methodName() override { return "mul"; }
		/**
		 * Compile the multiply expression.
		 */
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Divide operation.
	 */
	struct Divide   : public BinOp<std::divides<intptr_t>>
	{
		/**
		 * Divide operations become `div()` method invocations on non-integer
		 * objects.
		 */
		const char *methodName() override { return "div"; }
		/**
		 * Compile the divide expression.
		 */
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Add expression.
	 */
	struct Add      : public BinOp<std::plus<intptr_t>>
	{
		/**
		 * Add operations become `add()` method invocations on non-integer
		 * objects.
		 */
		const char *methodName() override { return "add"; }
		/**
		 * Compile the add expression.
		 */
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Subtract expression.
	 */
	struct Subtract : public BinOp<std::minus<intptr_t>>
	{
		/**
		 * Evaluate on integer values by subtracting the right side from the
		 * left..
		 */
		intptr_t evaluateWithIntegers(intptr_t lhs, intptr_t rhs) override
		{
			return lhs - rhs;
		}
		/**
		 * Subtract operations become `sub()` method invocations on non-integer
		 * objects.
		 */
		const char *methodName() override { return "sub"; }
		/**
		 * Compile the subtract expression.
		 */
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Superclass for comparison operations.  
	 */
	template<class T>
	struct Comparison : public BinOp<T>
	{
		/**
		 * All comparisons are (obviously) comparisons.
		 */
		bool isComparison() override { return true; }
		/**
		 * Comparisons don't map to any method name.
		 */
		const char *methodName() override { return nullptr; }
	};
	/**
	 * Equality comparison.
	 */
	struct CmpEq    : public Comparison<std::equal_to<intptr_t>>
	{
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Non-equality comparison.
	 */
	struct CmpNe    : public Comparison<std::not_equal_to<intptr_t>>
	{
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Less-than comparison.
	 */
	struct CmpLt    : public Comparison<std::less<intptr_t>>
	{
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Greater-than comparison.
	 */
	struct CmpGt    : public Comparison<std::greater<intptr_t>>
	{
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Less-than-or-equal-to comparison.
	 */
	struct CmpLE    : public Comparison<std::less_equal<intptr_t>>
	{
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Greater-than-or-equal-to comparison.
	 */
	struct CmpGE    : public Comparison<std::greater_equal<intptr_t>>
	{
		llvm::Value *compileBinOp(Compiler::Context &c,
		                          llvm::Value *LHS,
		                          llvm::Value *RHS) override;
	};
	/**
	 * Any identifier in the source.  Identifiers are not AST nodes in their own
	 * right, but are terminals that refer  to any identifier in the source
	 * code.  
	 */
	struct Identifier : public pegmatite::ASTString { };
	/**
	 * A parameter list for a closure declaration.  This contains list of
	 * identifiers, one for each parameters.
	 *
	 * This class exists to simplify automatic AST building: Pegmatite will pop
	 * all identifiers that are within the range of the of the parameter list as
	 * described in the grammar when constructing this class.  If the
	 * `ASTList<Identifier>` were just within the `ClosureDecl` then it would
	 * also pop the closure name onto the stack.
	 */
	struct ParamList : public pegmatite::ASTContainer
	{
		/**
		 * The arguments in this parameter list.
		 */
		ASTList<Identifier> arguments;
	};
	/**
	 * A closure declaration.  Closures in MysoreScript represent methods,
	 * functions, or true closures.
	 */
	struct ClosureDecl : Expression
	{

		bool recompile = false;

		// this is the root of any skip_ to call
		// Context c has already been assembled correctly
		// need to find sub-expression which represents node we stopped executing at before
		Obj expr_skip_to(Interpreter::Context &c, Statement *ast_node) override; 

		
		// ast_node has a member which is pointer called 'jit_type_assumption'
		// mapped to an alternative type, and a counter
//		std::unordered_map<Statement* ast_node, std::pair<intptr_t, uint64_t>> type_assumptions;

		/**
		 * The bound variables - variables that are declared outside of this
		 * closure, but referenced inside it.
		 */
		std::unordered_set<std::string> boundVars;
		/**
		 * Local variables declared inside this closure.
		 */
		std::unordered_set<std::string> decls;



		/**
		 * The name of this closure.
		 */
		ASTChild<Identifier> name;
		/**
		 * The parameter list.
		 */
		ASTPtr<ParamList> parameters;
		/**
		 * The statements that make up the body of the closure.
		 */
		ASTPtr<Statements> body;
		/**
		 * Interprets the closure in the current context.  When closures are
		 * created, one of their instance variables is the AST for that
		 * closure, the remaining fields are the bound variables 
		 */
		Obj interpretClosure(Interpreter::Context &c,
		                     MysoreScript::Closure *self,
		                     Obj *args);
		/**
		 * Interprets the closure, assuming that it is a method.  The AST is
		 * stored in the class, so can be looked up when the method must be
		 * executed.
		 */
		Obj interpretMethod(Interpreter::Context &c,
		                    MysoreScript::Method *mth,
		                    Obj self,
		                    MysoreScript::Selector sel,
		                    Obj *args);
		/**
		 * Compile as if this is a method.
		 */
		MysoreScript::CompiledMethod compileMethod(MysoreScript::Class *cls,
		                                           Interpreter::SymbolTable &globalSymbols);
		/**
		 * Compile as if this is a closure.
		 */
		MysoreScript::ClosureInvoke compileClosure(Interpreter::SymbolTable &globalSymbols);
		/**
		 * If this closure has already been compiled once, then the compiled
		 * functions is cached.
		 */
		MysoreScript::ClosureInvoke compiledClosure = nullptr;
		protected:
		/**
		 * Evaluate this closure, returning the closure object representing it.
		 * This is not invoked for methods, because classes are responsible for
		 * creating the method table entries corresponding to their methods.
		 */
		Obj evaluateExpr(Interpreter::Context &c) override;
		/**
		 * Compile the closure declaration.  This does *not* compile the closure
		 * itself, it merely generates code that constructs a `Closure` object
		 * with the correct bound variables.
		 */
		llvm::Value *compileExpression(Compiler::Context &c) override;
		private:
		/**
		 * The number of times a closure should be interpreted before it's
		 * compiled.
		 *
		 * Try varying this value and seeing what effect it has on performance.
		 * The interpreter is faster to start than the compiler, but is vastly
		 * slower.
		 */
		static const int compileThreshold = 10;
		/**
		 * The number of times this closure has been interpreted.  Used to
		 * determine whether it is now worthwhile to compile it.
		 */
		int executionCount = 0;
		/**
		 * A flag indicating whether we've already worked out what the bound
		 * variables and locals are in this closure.
		 */
		bool checked = false;
		/**
		 * Collect the bound variables and declarations from the statements
		 * inside this closure.
		 */
		void check();
		/**
		 * Collect the name of this closure as a declaration and all of the
		 * bound variables as uses.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override;
	};
	/**
	 * Reference to a variable.
	 */
	struct VarRef : public Expression
	{

		Obj expr_skip_to(Interpreter::Context &c, Statement* ast_node) override {
			if (this == ast_node) {
				D(std::cerr << "HELP VarRef matched ast_node, it shouldn't" << std::endl;)
				c.astNodeFound = true;
			}
			return evaluateExpr(c);
		}

		/**
		 * The name of the referenced variable.
		 */
		ASTChild<Identifier> name;
		/**
		 * Add this variable to the set of referenced variables.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			uses.insert(name);
		}
		protected:
		/**
		 * Evaluate this by looking up the variable in the interpreter's symbol
		 * table and loading it.
		 */
		Obj evaluateExpr(Interpreter::Context &c) override;
		/**
		 * Compile the reference by looking up the value in the compiler's
		 * symbol table and generating code to load it.
		 */
		llvm::Value *compileExpression(Compiler::Context &c) override;
	};
	/**
	 * Assignment statements, setting the value of a variable.  Note that
	 * MysoreScript does not provide compound-assignment operators.
	 */
	struct Assignment : Statement
	{

		void skip_to(Interpreter::Context &c, Statement* ast_node) override;


		/**
		 * The variable being assigned to.
		 */
		ASTPtr<VarRef> target;
		/**
		 * The expression being assigned.
		 */
		ASTPtr<Expression> expr;
		/**
		 * Interpret the assignment, updating the stored value.
		 */
		void interpret(Interpreter::Context &c) override;
		/**
		 * Compile the assignment, looking up the address of the target and
		 * storing the result of compiling the expression in it.
		 */
		void compile(Compiler::Context &c) override;
		/**
		 * Collect any variables use in this expression.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			target->collectVarUses(decls, uses);
			expr->collectVarUses(decls, uses);
		}
	};
	/**
	 * Argument list for a call expression.  This exists for the same reason as
	 * ParamList.
	 */
	struct ArgList : public pegmatite::ASTContainer
	{
		/**
		 * The expressions that will be evaluated to give the arguments to the
		 * called function.
		 */
			ASTList<Expression> arguments;
	};
	/**
	 * A call expression.  This either invokes a closure or a method.
	 */
	struct Call : public Expression
	{
		
		MysoreScript::CompiledMethod *cachedMethod; // cached function pointer for this call site
		// essentially the type check, moved up to superclass
//	    MysoreScript::Class *cachedClass; // class the object isa type of 


		/**
		 * The callee, if this is calling a closure, or the object that is
		 * having a method invoked on it if it is a method invocation.
		 */
		ASTPtr<Expression> callee;
		/**
		 * The name of the method, if this is a method invocation.
		 */
		ASTPtr<Identifier, /*optional*/true> method;
		/**
		 * The arguments to this call.
		 */
		ASTPtr<ArgList> arguments;


		Obj expr_skip_to(Interpreter::Context &c, Statement* ast_node) override;
		
		protected:


		Obj do_call(Interpreter::Context &c, Obj obj); // refactored




		/**
		 * Call the relevant method or closure.
		 */
		Obj evaluateExpr(Interpreter::Context &c) override;
		/**
		 * Generate code to call the relevant object.
		 */
		llvm::Value *compileExpression(Compiler::Context &c) override;
		/**
		 * Collect the variables referenced by this call.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			callee->collectVarUses(decls, uses);
			for (auto &arg : arguments->arguments)
			{
				arg->collectVarUses(decls, uses);
			}
		}
	};
	/**
	 * A variable declaration.
	 */
	struct Decl : Statement
	{

		void skip_to(Interpreter::Context &c, Statement* ast_node) override;



		/**
		 * The name of the variable.
		 */
		ASTChild<Identifier> name;
		/**
		 * The initialiser for this variable, if there is one.
		 */
		ASTPtr<Expression, /*optional*/true> init;
		/**
		 * Sets the variable to the value in the initialisation.
		 */
		void interpret(Interpreter::Context &c) override;
		/**
		 * Compiles the initialiser, if one exists.
		 */
		void compile(Compiler::Context &c) override;
		/**
		 * Adds this variable to the set that are defined.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			decls.insert(name);
		}
	};
	/**
	 * Return statement.
	 */
	struct Return : Statement
	{

		void skip_to(Interpreter::Context &c, Statement* ast_node) override;

		/**
		 * The expression that is returned.
		 */
		ASTPtr<Expression> expr;
		/**
		 * Interpret the returned expression and then indicate in the context
		 * that we have hit a return statement and so should stop interpreting.
		 */
		void interpret(Interpreter::Context &c) override;
		/**
		 * Compile the return statement.
		 */
		virtual void compile(Compiler::Context &c) override;
		/**
		 * Collect any variables that are referenced.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			expr->collectVarUses(decls, uses);
		}
	};
	/**
	 * If statement.
	 */
	struct IfStatement : Statement
	{
	
		void skip_to(Interpreter::Context &c, Statement* ast_node) override;


		/**
		 * The condition.  This is interpreted as true if it is either a
		 * non-zero integer or a non-null object.
		 */
		ASTPtr<Expression> condition;
		/**
		 * The body of the if statement.
		 */
		ASTPtr<Statements> body;
		/**
		 * Interpret the condition, then interpret the body if the condition is
		 * true.
		 */
		void interpret(Interpreter::Context &c) override;
		/**
		 * Compile the if statement.
		 */
		virtual void compile(Compiler::Context &c) override;
		/**
		 * Collect all of the variables used and defined in this statement.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			condition->collectVarUses(decls, uses);
			body->collectVarUses(decls, uses);
		}
	};
	/**
	 * A while loop.
	 */
	class WhileLoop : public Statement
	{
	
		void skip_to(Interpreter::Context &c, Statement* ast_node) override;

		/**
		 * The condition.  This is interpreted as true if it is either a
		 * non-zero integer or a non-null object.
		 */
		ASTPtr<Expression> condition;
		/**
		 * The loop body.
		 */
		ASTPtr<Statements> body;
		protected:
		/**
		 * Interpret the body as long as the condition remains true.
		 */
		void interpret(Interpreter::Context &c) override;
		/**
		 * Collect the variables used and declared in the loop.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override
		{
			condition->collectVarUses(decls, uses);
			body->collectVarUses(decls, uses);
		}
		/**
		 * Compile the loop.
		 */
		void compile(Compiler::Context &c) override;
	};
	/**
	 * A class declaration.  Classes contain instance variables and methods.
	 */
	struct ClassDecl : Statement
	{

		// dummy implementation
		void skip_to(Interpreter::Context &c, Statement* ast_node) override {
			return;
		}

		/**
		 * The name of the class.  AST construction happens depth-first, so this
		 * is optional even though it is really the superclass name that is
		 * optional.  If a superclass is not specified, then the class name is
		 * in the `superclassName` field.  This is because the first identifier
		 * will be popped off the AST stack when constructing `superclassName`,
		 * even if it were marked as optional, because the two identifiers can't
		 * be distinguished.
		 */
		ASTPtr<Identifier, /*optional*/true>   name;
		/**
		 * The superclass name (or class name if there is no superclass).
		 */
		ASTChild<Identifier> superclassName;
		/**
		 * The instance variables declared in this class.
		 */
		ASTList<Decl>        ivars;
		/**
		 * The methods declared in this class.
		 */
		ASTList<ClosureDecl> methods;
		/**
		 * Interpret, but constructing the class.  Note that there is no compile
		 * method for classes.  They are always interpreted, although their
		 * methods may be compiled.
		 */
		void interpret(Interpreter::Context &c) override;
		/**
		 * Classes are not allowed to be declared inside closures, so there is
		 * never a need to collect their declarations.
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override {}
	};
	/**
	 * A `new` expression, which constructs a new instance of a class.
	 */
	class NewExpr : public Expression
	{
		
		Obj expr_skip_to(Interpreter::Context &c, Statement* ast_node) override {
			if (this == ast_node) {
				D(std::cerr << "HELP NewExpr matches ast node??" << std::endl;)
				c.astNodeFound = true;
			}
			return evaluateExpr(c);
		}

		/**
		 * The name of the class being instantiated.
		 */
		ASTChild<Identifier> className;
		/**
		 * Construct a new instance of the class in the interpreter.
		 */
		Obj evaluateExpr(Interpreter::Context &c) override;
		/**
		 * Construct a call to the function that constructs a new instance.
		 */
		llvm::Value *compileExpression(Compiler::Context &c) override;
		/**
		 * The only 'variable' that is referenced by a new expression is the
		 * class name, which is in the class table managed by the runtime and
		 * not the symbol table managed by the interpreter context..
		 */
		void collectVarUses(std::unordered_set<std::string> &decls,
		                    std::unordered_set<std::string> &uses) override {}
	};
}
