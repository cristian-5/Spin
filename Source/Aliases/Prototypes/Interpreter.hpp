
#include "../Aliases.hpp"

#ifndef SPIN_INTERPRETER_HPP
#define SPIN_INTERPRETER_HPP

#include "Exception.hpp"
#include "SyntaxTree.hpp"
#include "Processor.hpp"

namespace Spin {

	class Environment;
	class Class;
	class Instance;
	class CodeUnit;

	class Interpreter: public Expression::Visitor, public Statement::Visitor {
		private:
		Processor * CPU = Processor::self();
		Environment * memory = nullptr;
		Bool broken = false;
		Bool continued = false;
		Class * classDefinition = nullptr;
		Modifier currentModifier = Modifier::publicAccess;
		Object * visitAssignmentExpression(Assignment * e) override;
		Object * visitBinaryExpression(Binary * e) override;
		Object * visitBraExpression(Bra * e) override;
		Object * visitCallExpression(Call * e) override;
		Object * visitComparisonExpression(Comparison * e) override;
		Object * visitDynamicGetExpression(DynamicGet * e) override;
		Object * visitDynamicSetExpression(DynamicSet * e) override;
		Object * visitGroupingExpression(Grouping * e) override;
		Object * visitIdentifierExpression(Identifier * e) override;
		Object * visitInnerExpression(Inner * e) override;
		Object * visitKetExpression(Ket * e) override;
		Object * visitListExpression(List * e) override;
		Object * visitLiteralExpression(Literal * e) override;
		Object * visitLogicalExpression(Logical * e) override;
		Object * visitMutableExpression(Mutable * e) override;
		Object * visitOuterExpression(Outer * e) override;
		Object * visitSelfExpression(Self * e) override;
		Object * visitStaticGetExpression(StaticGet * e) override;
		Object * visitStaticSetExpression(StaticSet * e) override;
		Object * visitSubscriptExpression(Subscript * e) override;
		Object * visitUnaryExpression(Unary * e) override;
		Object * evaluate(Expression * e);
		void visitAttributeStatement(AttributeStatement * e) override;
		void visitBlockStatement(BlockStatement * e) override;
		void visitBreakStatement(BreakStatement * e) override;
		void visitClassStatement(ClassStatement * e) override;
		void visitContinueStatement(ContinueStatement * e) override;
		void visitDeleteStatement(DeleteStatement * e) override;
		void visitDoWhileStatement(DoWhileStatement * e) override;
		void visitExpressionStatement(ExpressionStatement * e) override;
		void visitFileStatement(FileStatement * e) override;
		void visitForStatement(ForStatement * e) override;
		void visitFunctionStatement(FunctionStatement * e) override;
		void visitIfStatement(IfStatement * e) override;
		void visitLoopStatement(LoopStatement * e) override;
		void visitProcedureStatement(ProcedureStatement * e) override;
		void visitRepeatUntilStatement(RepeatUntilStatement * e) override;
		void visitRestStatement(RestStatement * e) override;
		void visitReturnStatement(ReturnStatement * e) override;
		void visitSwapStatement(SwapStatement * e) override;
		void visitUntilStatement(UntilStatement * e) override;
		void visitVariableStatement(VariableStatement * e) override;
		void visitVectorStatement(VectorStatement * e) override;
		void visitWhileStatement(WhileStatement * e) override;
		void executeStatement(Statement * statement);
		void executeBlock(Array<Statement *> * statements, Environment * environment);
		Interpreter() = default;
		~Interpreter() = default;
		public:
		CodeUnit * currentUnit;
		class Return: public Exception {
			private:
			Object * value = nullptr;
			Token * returnToken = nullptr;
			public:
			Return(Object * v, Token * t);
			~Return();
			Object * getReturnValue() const;
			Token * getReturnToken() const;
		};
		Instance * instanceDefinition = nullptr;
		Interpreter(const Interpreter &) = delete;
		Interpreter(Interpreter &&) = delete;
		Interpreter & operator = (const Interpreter &) = delete;
		Interpreter & operator = (Interpreter &&) = delete;
		static Interpreter * self() {
			static Interpreter instance;
			return & instance;
		}
		Environment * globals = nullptr;
		void executeFunction(BlockStatement * block, Environment * environment);
		void evaluate(SyntaxTree * syntaxTree);
	};

}

#endif
