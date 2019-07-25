
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

namespace Stack {

	class Assignment;
	class Binary;
	class Call;
	class Get;
	class Grouping;
	class Literal;
	class Logical;
	class Set;
	class Super;
	class This;
	class Unary;
	class Variable;

	class Expression {

		public:

		virtual ~Expression() = default;

		class Visitor {
			public:
			virtual void visitAssignmentExpression(Assignment * e) = 0;
			virtual void visitBinaryExpression(Binary * e) = 0;
			virtual void visitCallExpression(Call * e) = 0;
			virtual void visitGetExpression(Get * e) = 0;
			virtual void visitGroupingExpression(Grouping * e) = 0;
			virtual void visitLiteralExpression(Literal * e) = 0;
			virtual void visitLogicalExpression(Logical * e) = 0;
			virtual void visitSetExpression(Set * e) = 0;
			virtual void visitSuperExpression(Super * e) = 0;
			virtual void visitThisExpression(This * e) = 0;
			virtual void visitUnaryExpression(Unary * e) = 0;
			virtual void visitVariableExpression(Variable * e) = 0;
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
			try {
				visitor -> visitAssignmentExpression(this);
			} catch (Exception & e) { throw; }
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
			try {
				visitor -> visitBinaryExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Binary() { delete r; delete l; delete o; }
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
			try {
				visitor -> visitCallExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Call() {
			delete paren; delete callee;
			for (UInt32 i = 0; i < arguments.count(); i++) {
				delete arguments.getNode(i);
			}
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
			try {
				visitor -> visitGetExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Get() { delete object; delete name; }
	};

	class Grouping: public Expression {
		public:
		Expression * expression = nullptr;
		Grouping(Expression * e) {
			expression = e;
		}
		void accept(Visitor * visitor) override {
			try {
				visitor -> visitGroupingExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Grouping() { delete expression; }
	};

	class Literal: public Expression {
		public:
		Token * token = nullptr;
		Literal(Token * t) { token = t; }
		void accept(Visitor * visitor) override {
			try {
				visitor -> visitLiteralExpression(this);
			} catch (Exception & e) { throw; }
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
			try {
				visitor -> visitLogicalExpression(this);
			} catch (Exception & e) { throw; }
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
			try {
				visitor -> visitSetExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Set() { delete object; delete name; delete value; }
	};

	class Super: public Expression {
		public:
		Token * keyword = nullptr;
		Token * method = nullptr;
		Super(Token * k, Token * m) {
			keyword = k; method = m;
		}
		void accept(Visitor * visitor) override {
			try {
				visitor -> visitSuperExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Super() { delete keyword; delete method; }
	};

	class This: public Expression {
		public:
		Token * keyword = nullptr;
		This(Token * k) { keyword = k; }
		void accept(Visitor * visitor) override {
			try {
				visitor -> visitThisExpression(this);
			} catch (Exception & e) { throw; }
		}
		~This() { delete keyword; }
	};

	class Unary: public Expression {
		public:
		Expression * r = nullptr;
		Token * o = nullptr;
		Unary(Token * op, Expression * rs) {
			o = op; r = rs;
		}
		void accept(Visitor * visitor) override {
			try {
				visitor -> visitUnaryExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Unary() { delete r; delete o; }
	};

	class Variable: public Expression {
		public:
		Token * name = nullptr;
		Variable(Token * n) { name = n; }
		void accept(Visitor * visitor) override {
			try {
				visitor -> visitVariableExpression(this);
			} catch (Exception & e) { throw; }
		}
		~Variable() { delete name; }
	};

}

#endif
