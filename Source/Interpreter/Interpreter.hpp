
/*!
 *
 *    + --------------------------------------- +
 *    |  Interpreter.hpp                        |
 *    |                                         |
 *    |            Stack Interpreter            |
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

#ifndef STACKINTERPRETER
#define STACKINTERPRETER

#include "../Aliases/Aliases.hpp"
#include "../Parser/ASTree.hpp"

#include "Processor.hpp"
#include "Converter.hpp"

namespace Stack {

	class Interpreter: public Expression::Visitor {

		private:

		Object * value = nullptr;

		void setValue(Object * o) {
			if (value == o) return;
			if (value != nullptr) delete value;
			value = o;
		}

		void visitBinaryExpression(Binary * e) override {
			try {
				evaluateExpression(e -> l);
				//value = Processor::applyUnaryOperator(e -> o, & value);
			} catch (Exception & e) { throw; }
		}
		void visitAssignmentExpression(Assignment * e) override { }
		void visitCallExpression(Call * e) override { }
		void visitGetExpression(Get * e) override { }
		void visitGroupingExpression(Grouping * e) override {
			try { evaluateExpression(e -> expression); }
			catch (Exception & e) { throw; }
		}
		void visitLiteralExpression(Literal * e) override {
			try {
				if (e -> object == nullptr) {
					e -> object = Converter::literalToObject(e -> token);
				}
				setValue(e -> object);
			}
			catch (Exception & e) { throw; }
		}
		void visitLogicalExpression(Logical * e) override { }
		void visitSetExpression(Set * e) override { }
		void visitSuperExpression(Super * e) override { }
		void visitThisExpression(This * e) override { }
		void visitUnaryExpression(Unary * e) override {
			try {
				evaluateExpression(e -> r);
				setValue(Processor::applyUnaryOperator(e -> o, value));
			} catch (Exception & e) { throw; }
		}
		void visitVariableExpression(Variable * e) override { }

		void evaluateExpression(Expression * e) {
			try { e -> accept(this); }
			catch (Exception & e) { throw; }
		}

		Interpreter() = default;
		~Interpreter() = default;

		public:

		Interpreter(const Interpreter &) = delete;
		Interpreter(Interpreter &&) = delete;
		Interpreter & operator = (const Interpreter &) = delete;
		Interpreter & operator = (Interpreter &&) = delete;

		static Interpreter * self() {
			static Interpreter instance;
			return & instance;
		}

		Object * evaluate(Expression * e) {
			try {
				e -> accept(this);
				return value;
			}
			catch (Exception & e) { throw; }
		}

	};

}

#endif
