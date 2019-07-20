
/*!
 *
 *    + --------------------------------------- +
 *    |  ASTPrint.hpp                           |
 *    |                                         |
 *    |       Abstract Syntax Tree Printer      |
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

#ifndef STACKASTPRINT
#define STACKASTPRINT

#include "../Aliases/Aliases.hpp"

#include "ASTree.hpp"

namespace Stack {

	class ASTPrinter: public Expression::Visitor {

		private:

		StringStream stream = StringStream();

		void parenthesise(const String & name, Expression * e) {
			if (e == nullptr) return;
			stream << '(' << name << ' ';
			e -> accept(this);
			stream << ')';
		}

		void parenthesise(const String & name, StrongList<Expression *> * e) {
			stream << '(' << name;
			for (UInt32 i = 0; i < e -> count(); i++) {
				if (e -> getNode(i) == nullptr) continue;
				stream << ' ';
				e -> getNode(i) -> accept(this);
			}
			stream << ')';
		}

		void visitBinaryExpression(Binary * e) override {
			StrongList<Expression *> * exes = new StrongList<Expression *>();
			exes -> link(e -> l); exes -> link(e -> r);
			parenthesise(e -> o -> lexeme, exes);
			delete exes;
		}
		void visitAssignmentExpression(Assignment * e) override {
			const String name = "setq " + e -> name -> lexeme;
			parenthesise(name, e -> value);
		}
		void visitCallExpression(Call * e) override {
			stream << '(';
    		e -> callee -> accept(this);
			for (UInt32 i = 0; i < e -> arguments.count(); i++) {
				stream << ' ';
				e -> arguments[i] -> accept(this);
			}
			stream << ')';
		}
		void visitGetExpression(Get * e) override {
			parenthesise("get " + e -> name -> lexeme, e -> object);
		}
		void visitGroupingExpression(Grouping * e) override {
			parenthesise("group", e -> expression);
		}
		void visitLiteralExpression(Literal * e) override {
			stream << "(literal " << e -> token -> lexeme << ")";
		}
		void visitLogicalExpression(Logical * e) override {
			StrongList<Expression *> * exes = new StrongList<Expression *>();
			exes -> link(e -> l); exes -> link(e -> r);
			parenthesise(e -> o -> lexeme, exes);
			delete exes;
		}
		void visitSetExpression(Set * e) override {
			StrongList<Expression *> * exes = new StrongList<Expression *>();
			exes -> link(e -> object); exes -> link(e -> value);
			parenthesise("set " + e -> name -> lexeme, exes);
			delete exes;
		}
		void visitSuperExpression(Super * e) override {
			stream << e -> keyword -> lexeme << ' ';
			stream << e -> method -> lexeme;
		}
		void visitThisExpression(This * e) override {
			stream << e -> keyword -> lexeme;
		}
		void visitUnaryExpression(Unary * e) override {
			parenthesise(e -> o -> lexeme, e -> r);
		}
		void visitVariableExpression(Variable * e) override {
			stream << e -> name -> lexeme;
		}

		public:

		ASTPrinter() { }

		String print(StrongList<Expression *> * statements) {
			for (UInt32 i = 0; i < statements -> count(); i++) {
				// This might not work because of reference:
				statements -> getNode(i) -> accept(this);
				stream << '\n';
			}
			return stream.str();
		}

		String print(Expression * statement) {
			statement -> accept(this);
			return stream.str();
		}

	};

}

#endif
