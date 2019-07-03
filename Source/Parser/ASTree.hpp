
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

#include "STable.hpp"

namespace Stack {

	class Assignment;
	class Binary;
	class Call;
	class Get;
	class Grouping;
	class Literal;
	class Logical;
	class Unary;

	class Expression {

		public:

		//virtual ~Expression() = default;

		class Visitor {
			public:
			virtual void visitAssignmentExpression(Assignment * e) = 0;
			virtual void visitBinaryExpression(Binary * e) = 0;
			virtual void visitCallExpression(Call * e) = 0;
			virtual void visitGetExpression(Get * e) = 0;
			virtual void visitGroupingExpression(Grouping * e) = 0;
			virtual void visitLiteralExpression(Literal * e) = 0;
			virtual void visitLogicalExpression(Logical * e) = 0;
			virtual void visitUnaryExpression(Unary * e) = 0;
		};

		virtual void accept(Visitor *) { }

	};

	class Assignment: public Expression {
		public:
		Token * name = nullptr;
		Expression * value = nullptr;
		Assignment(Token * n, Expression * v) {
			name = n; value = v;
		}
		void accept(Visitor * visitor) override {
			visitor -> visitAssignmentExpression(this);
		}
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
			visitor -> visitBinaryExpression(this);
		}
	};

	class Call: public Expression {
		public:
		Token * paren = nullptr;
		Expression * callee = nullptr;
		StrongList<Expression *> arguments = StrongList<Expression *>();
		Call(Expression * c, Token * p, StrongList<Expression *> a) {
			paren = p; callee = c; arguments = a;
		}
		void accept(Visitor * visitor) override {
			visitor -> visitCallExpression(this);
		}
	};

	class Get: public Expression {
		public:
		Expression * object = nullptr;
		Token * name = nullptr;
		Get(Expression * o, Token * n) {
			object = o; name = n;
		}
		void accept(Visitor * visitor) override {
			visitor -> visitGetExpression(this);
		}
	};

	class Grouping: public Expression {
		public:
		Expression * expression = nullptr;
		Grouping(Expression * e) {
			expression = e;
		}
		void accept(Visitor * visitor) override {
			visitor -> visitGroupingExpression(this);
		}
	};

	class Literal: public Expression {
		public:
		String lexeme = "";
		// Value value
		Literal(String l) {
			lexeme = l;
		}
		void accept(Visitor * visitor) override {
			visitor -> visitLiteralExpression(this);
		}
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
			visitor -> visitLogicalExpression(this);
		}
	};

	class Unary: public Expression {
		public:
		Expression * r = nullptr;
		Token * o = nullptr;
		Unary(Token * op, Expression * rs) {
			o = op; r = rs;
		}
		void accept(Visitor * visitor) override {
			visitor -> visitUnaryExpression(this);
		}
	};

}

#endif
