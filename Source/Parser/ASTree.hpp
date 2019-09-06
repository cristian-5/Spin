
/*!
 *
 *    + --------------------------------------- +
 *    |  ASTree.hpp                             |
 *    |                                         |
 *    |           Abstract Syntax Tree          |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
 */

#ifndef STACKASTREE
#define STACKASTREE

#include "../Aliases/Aliases.hpp"
#include "../Token/Token.hpp"
#include "../Types/Object.hpp"

namespace Stack {

	class Assignment;
	class Binary;
	class Call;
	class Comparison;
	class Get;
	class Grouping;
	class Literal;
	class Logical;
	class Set;
	class Super;
	class This;
	class Unary;
	class Identifier;

	class Expression {

		public:

		virtual ~Expression() = default;

		class Visitor {
			public:
			virtual void visitAssignmentExpression(Assignment * e) = 0;
			virtual void visitBinaryExpression(Binary * e) = 0;
			virtual void visitCallExpression(Call * e) = 0;
			virtual void visitComparisonExpression(Comparison * e) = 0;
			virtual void visitGetExpression(Get * e) = 0;
			virtual void visitGroupingExpression(Grouping * e) = 0;
			virtual void visitLiteralExpression(Literal * e) = 0;
			virtual void visitLogicalExpression(Logical * e) = 0;
			virtual void visitSetExpression(Set * e) = 0;
			virtual void visitSuperExpression(Super * e) = 0;
			virtual void visitThisExpression(This * e) = 0;
			virtual void visitUnaryExpression(Unary * e) = 0;
			virtual void visitIdentifierExpression(Identifier * e) = 0;
		};

		virtual void accept(Visitor *) { }

		template<typename t>
		Bool isInstanceOf() {
			return (dynamic<t *>(this));
		}

	};

	class BlockStatement;
	class BreakStatement;
	class ContinueStatement;
	class DoWhileStatement;
	class ExpressionStatement;
	class ForStatement;
	class IfStatement;
	class LoopStatement;
	class PrintStatement;
	class RepeatUntilStatement;
	class RestStatement;
	class UntilStatement;
	class VariableStatement;
	class WhileStatement;

	class Statement {

		public:

		virtual ~Statement() = default;

		class Visitor {
			public:
			virtual void visitBlockStatement(BlockStatement * e) = 0;
			virtual void visitBreakStatement(BreakStatement * e) = 0;
			virtual void visitContinueStatement(ContinueStatement * e) = 0;
			virtual void visitDoWhileStatement(DoWhileStatement * e) = 0;
			virtual void visitExpressionStatement(ExpressionStatement * e) = 0;
			virtual void visitForStatement(ForStatement * e) = 0;
			virtual void visitIfStatement(IfStatement * e) = 0;
			virtual void visitLoopStatement(LoopStatement * e) = 0;
			virtual void visitPrintStatement(PrintStatement * e) = 0;
			virtual void visitRepeatUntilStatement(RepeatUntilStatement * e) = 0;
			virtual void visitRestStatement(RestStatement * e) = 0;
			virtual void visitUntilStatement(UntilStatement * e) = 0;
			virtual void visitVariableStatement(VariableStatement * e) = 0;
			virtual void visitWhileStatement(WhileStatement * e) = 0;
		};

		virtual void accept(Visitor *) { }

		template<typename t>
		Bool isInstanceOf() {
			return (dynamic<t *>(this));
		}

	};

	/* Expressions */

	class Assignment: public Expression {
		public:
		Token * name = nullptr;
		Expression * value = nullptr;
		Assignment(Token * name, Expression * value) {
			this -> name = name;
			this -> value = value;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitAssignmentExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Assignment() { delete name; delete value; }
	};
	class Binary: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Binary(Expression * ls, Token * op, Expression * rs) {
			r = rs, l = ls; o = op;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitBinaryExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Binary() { delete r; delete l; delete o; }
	};
	class Call: public Expression {
		public:
		Token * paren = nullptr;
		Expression * callee = nullptr;
		ArrayList<Expression *> arguments = ArrayList<Expression *>();
		Call(Expression * c, Token * p, ArrayList<Expression *> a) {
			paren = p; callee = c; arguments = a;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitCallExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Call() {
			delete paren; delete callee;
			for (Expression * argument : arguments) {
				delete argument;
			}
		}
	};
	class Comparison: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Comparison(Expression * ls, Token * op, Expression * rs) {
			r = rs, l = ls; o = op;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitComparisonExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Comparison() { delete r; delete l; delete o; }
	};
	class Get: public Expression {
		public:
		Expression * object = nullptr;
		Token * name = nullptr;
		Get(Expression * o, Token * n) {
			object = o; name = n;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitGetExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Get() { delete object; delete name; }
	};
	class Grouping: public Expression {
		public:
		Expression * expression = nullptr;
		Grouping(Expression * e) { expression = e; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitGroupingExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Grouping() { delete expression; }
	};
	class Literal: public Expression {
		public:
		Token * token = nullptr;
		Object * object = nullptr;
		Literal(Token * t) { token = t; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitLiteralExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Literal() { delete token; }
	};
	class Logical: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Logical(Expression * ls, Token * op, Expression * rs) {
			r = rs, l = ls; o = op;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitLogicalExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Logical() { delete r; delete l; delete o; }
	};
	class Set: public Expression {
		public:
		Expression * object = nullptr;
		Token * name = nullptr;
		Expression * value = nullptr;
		Set(Expression * o, Token * n, Expression * v) {
			object = o; name = n; value = v;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitSetExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Set() { delete object; delete name; delete value; }
	};
	class Super: public Expression {
		public:
		Token * keyword = nullptr;
		Token * method = nullptr;
		Super(Token * k, Token * m) { keyword = k; method = m; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitSuperExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Super() { delete keyword; delete method; }
	};
	class This: public Expression {
		public:
		Token * keyword = nullptr;
		This(Token * k) { keyword = k; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitThisExpression(this); }
			catch (Exception & e) { throw; }
		}
		~This() { delete keyword; }
	};
	class Unary: public Expression {
		public:
		Expression * r = nullptr;
		Token * o = nullptr;
		Unary(Token * op, Expression * rs) { o = op; r = rs; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitUnaryExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Unary() { delete r; delete o; }
	};
	class Identifier: public Expression {
		public:
		Token * name = nullptr;
		Identifier(Token * n) { name = n; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitIdentifierExpression(this); }
			catch (Exception & e) { throw; }
		}
		~Identifier() { delete name; }
	};

	/* Statements */

	class BlockStatement: public Statement {
		public:
		ArrayList<Statement *> statements = ArrayList<Statement *>();
		BlockStatement(ArrayList<Statement *> s) { statements = s; }
		BlockStatement(Statement * s) { statements.push(s); }
		void accept(Visitor * visitor) override {
			try { visitor -> visitBlockStatement(this); }
			catch (Exception & e) { throw; }
		}
		~BlockStatement() {
			for (Statement * statement : statements) {
				delete statement;
			}
		}
	};
	class BreakStatement: public Statement {
		public:
		Token * breakToken = nullptr;
		BreakStatement(Token * b) { breakToken = b; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitBreakStatement(this); }
			catch (Exception & e) { throw; }
		}
		~BreakStatement() { delete breakToken; }
	};
	class ContinueStatement: public Statement {
		public:
		Token * continueToken = nullptr;
		ContinueStatement(Token * c) { continueToken = c; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitContinueStatement(this); }
			catch (Exception & e) { throw; }
		}
		~ContinueStatement() { delete continueToken; }
	};
	class DoWhileStatement: public Statement {
		public:
		Token * whileToken = nullptr;
		Expression * expression = nullptr;
		Statement * body = nullptr;
		DoWhileStatement(Statement * b, Expression * e, Token * w) {
			expression = e; body = b; whileToken = w;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitDoWhileStatement(this); }
			catch (Exception & e) { throw; }
		}
		~DoWhileStatement() {
			delete whileToken;
			delete expression;
			delete body;
		}
	};
	class ExpressionStatement: public Statement {
		public:
		Expression * e = nullptr;
		ExpressionStatement(Expression * ex) { e = ex; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitExpressionStatement(this); }
			catch (Exception & e) { throw; }
		}
		~ExpressionStatement() { delete e; }
	};
	class ForStatement: public Statement {
		public:
		Token * forToken = nullptr;
		Statement * declaration = nullptr;
		Expression * expression = nullptr;
		Expression * stepper = nullptr;
		Statement * body = nullptr;
		ForStatement(Statement * d, Expression * e,
					 Expression * s, Statement * b, Token * f) {
			declaration = d; expression = e;
			stepper = s; body = b; forToken = f;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitForStatement(this); }
			catch (Exception & e) { throw; }
		}
		~ForStatement() {
			delete forToken;
			delete declaration;
			delete expression;
			delete stepper;
			delete body;
		}
	};
	class IfStatement: public Statement {
		public:
		Token * ifToken = nullptr;
		Expression * expression = nullptr;
		Statement * thenBranch = nullptr;
		Statement * elseBranch = nullptr;
		IfStatement(Expression * x, Statement * t,
					Statement * e, Token * i) {
			expression = x; thenBranch = t;
			elseBranch = e; ifToken = i;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitIfStatement(this); }
			catch (Exception & e) { throw; }
		}
		~IfStatement() {
			delete expression;
			delete thenBranch;
			delete elseBranch;
		}
	};
	class LoopStatement: public Statement {
		public:
		Token * loopToken = nullptr;
		Statement * body = nullptr;
		LoopStatement(Statement * b, Token * l) {
			body = b; loopToken = l;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitLoopStatement(this); }
			catch (Exception & e) { throw; }
		}
		~LoopStatement() {
			delete loopToken;
			delete body;
		}
	};	
	class PrintStatement: public Statement {
		public:
		Expression * e = nullptr;
		PrintStatement(Expression * ex) { e = ex; }
		void accept(Visitor * visitor) override {
			try { visitor -> visitPrintStatement(this); }
			catch (Exception & e) { throw; }
		}
		~PrintStatement() { delete e; }
	};
	class RepeatUntilStatement: public Statement {
		public:
		Token * untilToken = nullptr;
		Statement * body = nullptr;
		Expression * expression = nullptr;
		RepeatUntilStatement(Statement * b, Expression * e, Token * u) {
			expression = e; body = b; untilToken = u;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitRepeatUntilStatement(this); }
			catch (Exception & e) { throw; }
		}
		~RepeatUntilStatement() {
			delete untilToken;
			delete expression;
			delete body;
		}
	};
	class RestStatement: public Statement {
		public:
		RestStatement() { }
		void accept(Visitor * visitor) override {
			try { visitor -> visitRestStatement(this); }
			catch (Exception & e) { throw; }
		}
	};
	class UntilStatement: public Statement {
		public:
		Token * untilToken = nullptr;
		Expression * expression = nullptr;
		Statement * body = nullptr;
		UntilStatement(Expression * e, Statement * b, Token * u) {
			expression = e; body = b; untilToken = u;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitUntilStatement(this); }
			catch (Exception & e) { throw; }
		}
		~UntilStatement() {
			delete untilToken;
			delete expression;
			delete body;
		}
	};
	class VariableStatement: public Statement {
		public:
		Token * name = nullptr;
		BasicType type = BasicType::UnknownType;
		Expression * initializer = nullptr;
		VariableStatement(Token * n, Expression * i, BasicType t) {
			name = n; initializer = i; type = t;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitVariableStatement(this); }
			catch (Exception & e) { throw; }
		}
		~VariableStatement() { delete name; }
	};
	class WhileStatement: public Statement {
		public:
		Token * whileToken = nullptr;
		Expression * expression = nullptr;
		Statement * body = nullptr;
		WhileStatement(Expression * e, Statement * b, Token * w) {
			expression = e; body = b; whileToken = w;
		}
		void accept(Visitor * visitor) override {
			try { visitor -> visitWhileStatement(this); }
			catch (Exception & e) { throw; }
		}
		~WhileStatement() {
			delete whileToken;
			delete expression;
			delete body;
		}
	};

}

#endif
