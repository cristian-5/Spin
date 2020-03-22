
#include "../Aliases.hpp"

#ifndef SPIN_SYNTAX_HPP
#define SPIN_SYNTAX_HPP

#include <vector>

#include "Exception.hpp"
#include "Token.hpp"
#include "Object.hpp"

namespace Spin {

	class Object;
	class CodeUnit;

	/* Base Classes */

	class Assignment;
	class Binary;
	class Bra;
	class Call;
	class Comparison;
	class DynamicGet;
	class DynamicSet;
	class Grouping;
	class Identifier;
	class Inner;
	class Ket;
	class List;
	class Literal;
	class Logical;
	class Mutable;
	class Outer;
	class Self;
	class StaticGet;
	class StaticSet;
	class Subscript;
	class Unary;

	class AttributeStatement;
	class BlockStatement;
	class BreakStatement;
	class ClassStatement;
	class ContinueStatement;
	class DeleteStatement;
	class DoWhileStatement;
	class ExpressionStatement;
	class FileStatement;
	class ForStatement;
	class FunctionStatement;
	class IfStatement;
	class LoopStatement;
	class ProcedureStatement;
	class RepeatUntilStatement;
	class RestStatement;
	class ReturnStatement;
	class SwapStatement;
	class UntilStatement;
	class VariableStatement;
	class VectorStatement;
	class WhileStatement;

	class Expression {
		public:
		virtual ~Expression() = default;
		class Visitor {
			public:
			virtual Object * visitAssignmentExpression(Assignment * e) = 0;
			virtual Object * visitBinaryExpression(Binary * e) = 0;
			virtual Object * visitBraExpression(Bra * e) = 0;
			virtual Object * visitCallExpression(Call * e) = 0;
			virtual Object * visitComparisonExpression(Comparison * e) = 0;
			virtual Object * visitDynamicGetExpression(DynamicGet * e) = 0;
			virtual Object * visitDynamicSetExpression(DynamicSet * e) = 0;
			virtual Object * visitGroupingExpression(Grouping * e) = 0;
			virtual Object * visitIdentifierExpression(Identifier * e) = 0;
			virtual Object * visitInnerExpression(Inner * e) = 0;
			virtual Object * visitKetExpression(Ket * e) = 0;
			virtual Object * visitListExpression(List * e) = 0;
			virtual Object * visitLiteralExpression(Literal * e) = 0;
			virtual Object * visitLogicalExpression(Logical * e) = 0;
			virtual Object * visitMutableExpression(Mutable * e) = 0;
			virtual Object * visitOuterExpression(Outer * e) = 0;
			virtual Object * visitSelfExpression(Self * e) = 0;
			virtual Object * visitStaticGetExpression(StaticGet * e) = 0;
			virtual Object * visitStaticSetExpression(StaticSet * e) = 0;
			virtual Object * visitSubscriptExpression(Subscript * e) = 0;
			virtual Object * visitUnaryExpression(Unary * e) = 0;
		};
		virtual Object * accept(Visitor *) = 0;
		template<typename t>
		Boolean isInstanceOf() {
			return (DynamicCast<t *>(this));
		}
	};
	class Statement {
		public:
		virtual ~Statement() = default;
		class Visitor {
			public:
			virtual void visitAttributeStatement(AttributeStatement * e) = 0;
			virtual void visitBlockStatement(BlockStatement * e) = 0;
			virtual void visitBreakStatement(BreakStatement * e) = 0;
			virtual void visitClassStatement(ClassStatement * e) = 0;
			virtual void visitContinueStatement(ContinueStatement * e) = 0;
			virtual void visitDeleteStatement(DeleteStatement * e) = 0;
			virtual void visitDoWhileStatement(DoWhileStatement * e) = 0;
			virtual void visitExpressionStatement(ExpressionStatement * e) = 0;
			virtual void visitFileStatement(FileStatement * e) = 0;
			virtual void visitForStatement(ForStatement * e) = 0;
			virtual void visitFunctionStatement(FunctionStatement * e) = 0;
			virtual void visitIfStatement(IfStatement * e) = 0;
			virtual void visitLoopStatement(LoopStatement * e) = 0;
			virtual void visitProcedureStatement(ProcedureStatement * e) = 0;
			virtual void visitRepeatUntilStatement(RepeatUntilStatement * e) = 0;
			virtual void visitRestStatement(RestStatement * e) = 0;
			virtual void visitReturnStatement(ReturnStatement * e) = 0;
			virtual void visitSwapStatement(SwapStatement * e) = 0;
			virtual void visitUntilStatement(UntilStatement * e) = 0;
			virtual void visitVariableStatement(VariableStatement * e) = 0;
			virtual void visitVectorStatement(VectorStatement * e) = 0;
			virtual void visitWhileStatement(WhileStatement * e) = 0;
		};
		virtual void accept(Visitor *) = 0;
		template<typename t>
		Boolean isInstanceOf() {
			return (DynamicCast<t *>(this));
		}
	};

	/* Parameter */

	class Parameter {
		public:
		BasicType type = BasicType::UnknownType;
		Token * tokenType = nullptr;
		Token * name = nullptr;
		Parameter() = default;
		Parameter(BasicType b, Token * t, Token * n) {
			type = b; tokenType = t; name = n;
		}
		Parameter * copy() const {
			return new Parameter(
				type,
				new Token(* tokenType),
				new Token(* name)
			);
		}
		~Parameter() {
			if (tokenType) delete tokenType;
			if (name) delete name;
		}
	};

	/* ASTree */

	class Assignment: public Expression {
		public:
		Token * name;
		Expression * value;
		Token * o;
		Assignment(Token * name, Expression * value, Token * o);
		Object * accept(Visitor * visitor) override;
		~Assignment();
	};
	class Binary: public Expression {
		public:
		Expression * r;
		Expression * l;
		Token * o;
		Binary(Expression * ls, Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Binary();
	};
	class Bra: public Expression {
		public:
		String name;
		Token * bra;
		Bra(Token * b, String & n);
		Object * accept(Visitor * visitor) override;
		~Bra();
	};
	class Call: public Expression {
		public:
		Token * parenthesis;
		Expression * callee;
		Array<Expression *> * arguments;
		Boolean isConstructor;
		Call(Expression * c, Token * p, Array<Expression *> * a, Boolean i);
		Object * accept(Visitor * visitor) override;
		~Call();
	};
	class Comparison: public Expression {
		public:
		Expression * r;
		Expression * l;
		Token * o;
		Comparison(Expression * ls, Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Comparison();
	};
	class DynamicGet: public Expression {
		public:
		Expression * object;
		Token * name;
		Boolean selfReference;
		DynamicGet(Expression * o, Token * n, Boolean s = false);
		Object * accept(Visitor * visitor) override;
		~DynamicGet();
	};
	class DynamicSet: public Expression {
		public:
		Expression * object;
		Token * name;
		Token * equals;
		Expression * value;
		Boolean selfReference;
		DynamicSet(Expression * o, Token * n, Expression * v, Token * e, Boolean s = false);
		Object * accept(Visitor * visitor) override;
		~DynamicSet();
	};
	class Grouping: public Expression {
		public:
		Expression * expression;
		Grouping(Expression * e);
		Object * accept(Visitor * visitor) override;
		~Grouping();
	};
	class Identifier: public Expression {
		public:
		Token * name;
		Identifier(Token * n);
		Object * accept(Visitor * visitor) override;
		~Identifier();
	};
	class Inner: public Expression {
		public:
		String bra;
		String ket;
		Token * inner;
		Inner(Token * i, String & b, String & k);
		Object * accept(Visitor * visitor) override;
		~Inner();
	};
	class Ket: public Expression {
		public:
		String name;
		Token * ket;
		Ket(Token * k, String & n);
		Object * accept(Visitor * visitor) override;
		~Ket();
	};
	class List: public Expression {
		public:
		Array<Expression *> * values;
		List(Array<Expression *> * v);
		Object * accept(Visitor * visitor) override;
		~List();
	};
	class Literal: public Expression {
		public:
		Token * token;
		Object * object;
		Literal(Token * t);
		Object * accept(Visitor * visitor) override;
		~Literal();
	};
	class Logical: public Expression {
		public:
		Expression * r;
		Expression * l;
		Token * o;
		Logical(Expression * ls, Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Logical();
	};
	class Mutable: public Expression {
		public:
		Token * name;
		Expression * value;
		Token * o;
		Mutable(Token * name, Expression * value, Token * o);
		Object * accept(Visitor * visitor) override;
		~Mutable();
	};
	class Outer: public Expression {
		public:
		String ket;
		String bra;
		Token * outer;
		Outer(Token * o, String & k, String & b);
		Object * accept(Visitor * visitor) override;
		~Outer();
	};
	class Self: public Expression {
		public:
		Token * keyword;
		Self(Token * k);
		Object * accept(Visitor * visitor) override;
		~Self();
	};
	class StaticGet: public Expression {
		public:
		Expression * object;
		Token * name;
		Boolean selfReference;
		StaticGet(Expression * o, Token * n, Boolean s = false);
		Object * accept(Visitor * visitor) override;
		~StaticGet();
	};
	class StaticSet: public Expression {
		public:
		Expression * object;
		Token * name;
		Token * equals;
		Expression * value;
		Boolean selfReference;
		StaticSet(Expression * o, Token * n, Expression * v, Token * e, Boolean s = false);
		Object * accept(Visitor * visitor) override;
		~StaticSet();
	};
	class Subscript: public Expression {
		public:
		Token * bracket;
		Expression * item;
		Expression * expression;
		Subscript(Expression * i, Token * b, Expression * e);
		Object * accept(Visitor * visitor) override;
		~Subscript();
	};
	class Unary: public Expression {
		public:
		Expression * r;
		Token * o;
		Unary(Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Unary();
	};

	class AttributeStatement: public Statement {
		public:
		Statement * field;
		Modifier modifier;
		AttributeStatement(Statement * f, Modifier m);
		void accept(Visitor * visitor) override;
		~AttributeStatement();
	};
	class BlockStatement: public Statement {
		public:
		Array<Statement *> * statements;
		BlockStatement(Array<Statement *> * s);
		BlockStatement(Statement * s);
		void accept(Visitor * visitor) override;
		~BlockStatement();
	};
	class BreakStatement: public Statement {
		public:
		Token * breakToken;
		BreakStatement(Token * b);
		void accept(Visitor * visitor) override;
		~BreakStatement();
	};
	class ClassStatement: public Statement {
		public:
		Token * name;
		Array<AttributeStatement *> * staticAttributes;
		Array<AttributeStatement *> * dynamicAttributes;
		ProcedureStatement * atCreate;
		ClassStatement(Token * n, Array<AttributeStatement *> * sF, Array<AttributeStatement *> * dF, ProcedureStatement * atCreate);
		void accept(Visitor * visitor) override;
		~ClassStatement();
	};
	class ContinueStatement: public Statement {
		public:
		Token * continueToken;
		ContinueStatement(Token * c);
		void accept(Visitor * visitor) override;
		~ContinueStatement();
	};
	class DeleteStatement: public Statement {
		public:
		Token * name;
		DeleteStatement(Token * n);
		void accept(Visitor * visitor) override;
		~DeleteStatement();
	};
	class DoWhileStatement: public Statement {
		public:
		Token * whileToken;
		Expression * expression;
		Statement * body;
		DoWhileStatement(Statement * b, Expression * e, Token * w);
		void accept(Visitor * visitor) override;
		~DoWhileStatement();
	};
	class ExpressionStatement: public Statement {
		public:
		Expression * e;
		ExpressionStatement(Expression * ex);
		void accept(Visitor * visitor) override;
		~ExpressionStatement();
	};
	class FileStatement: public Statement {
		public:
		CodeUnit * file;
		FileStatement(CodeUnit * f);
		void accept(Visitor * visitor) override;
	};
	class ForStatement: public Statement {
		public:
		Token * forToken;
		Statement * declaration;
		Expression * expression;
		Expression * stepper;
		Statement * body;
		ForStatement(Statement * d, Expression * e, Expression * s, Statement * b, Token * f);
		void accept(Visitor * visitor) override;
		~ForStatement();
	};
	class FunctionStatement: public Statement {
		public:
		Token * name;
		Array<Parameter *> * params;
		BlockStatement * body;
		Parameter * returnType;
		FunctionStatement(Token * n, Array<Parameter *> * p, BlockStatement * b, Parameter * r);
		void accept(Visitor * visitor) override;
		~FunctionStatement();
	};
	class IfStatement: public Statement {
		public:
		Token * ifToken;
		Expression * expression;
		Statement * thenBranch;
		Statement * elseBranch;
		IfStatement(Expression * x, Statement * t, Statement * e, Token * i);
		void accept(Visitor * visitor) override;
		~IfStatement();
	};
	class LoopStatement: public Statement {
		public:
		Token * loopToken;
		Statement * body;
		LoopStatement(Statement * b, Token * l);
		void accept(Visitor * visitor) override;
		~LoopStatement();
	};
	class ProcedureStatement: public Statement {
		public:
		Token * name;
		Array<Parameter *> * params;
		BlockStatement * body;
		ProcedureStatement(Token * n, Array<Parameter *> * p, BlockStatement * b);
		void accept(Visitor * visitor) override;
		~ProcedureStatement();
	};
	class RepeatUntilStatement: public Statement {
		public:
		Token * untilToken;
		Statement * body;
		Expression * expression;
		RepeatUntilStatement(Statement * b, Expression * e, Token * u);
		void accept(Visitor * visitor) override;
		~RepeatUntilStatement();
	};
	class RestStatement: public Statement {
		public:
		RestStatement() = default;
		void accept(Visitor * visitor) override;
	};
	class ReturnStatement: public Statement {
		public:
		Token * returnToken;
		Expression * e;
		ReturnStatement(Expression * ex, Token * rt);
		void accept(Visitor * visitor) override;
		~ReturnStatement();
	};
	class SwapStatement: public Statement {
		public:
		Token * l;
		Token * r;
		Token * swap;
		SwapStatement(Token * l, Token * r, Token * s);
		void accept(Visitor * visitor) override;
		~SwapStatement();
	};
	class UntilStatement: public Statement {
		public:
		Token * untilToken;
		Expression * expression;
		Statement * body;
		UntilStatement(Expression * e, Statement * b, Token * u);
		void accept(Visitor * visitor) override;
		~UntilStatement();
	};
	class VariableStatement: public Statement {
		public:
		Token * name;
		BasicType type;
		Expression * initialiser;
		Token * object;
		Token * equal;
		VariableStatement(Token * n, Expression * i, BasicType t, Token * e, Token * o = nullptr);
		void accept(Visitor * visitor) override;
		~VariableStatement();
	};
	class VectorStatement: public Statement {
		public:
		String name;
		Token * vector;
		Expression * initialiser;
		Token * equal;
		VectorStatement(Token * v, String & n, Expression * i, Token * e);
		void accept(Visitor * visitor) override;
		~VectorStatement();
	};
	class WhileStatement: public Statement {
		public:
		Token * whileToken;
		Expression * expression;
		Statement * body;
		WhileStatement(Expression * e, Statement * b, Token * w);
		void accept(Visitor * visitor) override;
		~WhileStatement();
	};

	/* Syntax Tree */

	class SyntaxTree {
		public:
		Array<String> * libraries = nullptr;
		Array<Statement *> * statements = nullptr;
		SyntaxTree() = default;
		SyntaxTree(Array<Statement *> * s);
		~SyntaxTree() {
			if (libraries) delete libraries;
			if (!statements) return;
			for (Statement * s : * statements) delete s;
			delete statements;
		}
	};

}

#endif
