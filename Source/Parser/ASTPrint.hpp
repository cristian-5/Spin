
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

		void parenthesise(const String & name, ArrayList<Expression *> * e) {
			stream << '(' << name;
			for (Expression * ex : * e) {
				if (ex == nullptr) continue;
				stream << ' ';
				ex -> accept(this);
			}
			stream << ')';
		}

		void visitBinaryExpression(Binary * e) override {
			ArrayList<Expression *> * exes = new ArrayList<Expression *>();
			exes -> push(e -> l); exes -> push(e -> r);
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
			for (Expression * ex : e -> arguments) {
				stream << ' '; ex -> accept(this);
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
			ArrayList<Expression *> * exes = new ArrayList<Expression *>();
			exes -> push(e -> l); exes -> push(e -> r);
			parenthesise(e -> o -> lexeme, exes);
			delete exes;
		}
		void visitSetExpression(Set * e) override {
			ArrayList<Expression *> * exes = new ArrayList<Expression *>();
			exes -> push(e -> object); exes -> push(e -> value);
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

		String print(ArrayList<Expression *> * statements) {
			for (Expression * ex : * statements) {
				ex -> accept(this); stream << '\n';
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
