
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

		Object value = Object();

		Object literalToObject(Token * t) {
			Object o = Object();
			if (t == nullptr) return o;
			if (!t -> isTypeLiteral()) return o;
			switch (t -> type) {
				case TokenType::boolLiteral: {
					o.type = BasicType::BooleanType;
					Boolean * v = new Boolean;
					* v = Converter::stringToBool(t -> lexeme);
					o.value = v;
				} break;
				case TokenType::intLiteral: {
					o.type = BasicType::UInt64Type;
					UInt64 * v = new UInt64;
					* v = Converter::stringToUInt64(t -> lexeme);
					o.value = v;
				} break;
				case TokenType::stringLiteral: {
					o.type = BasicType::StringType;
					String * v = new String;
					* v = Converter::escapeString(t -> lexeme);
					o.value = v;
				} break;
				case TokenType::charLiteral: {
					o.type = BasicType::StringType;
					Character * v = new Character;
					* v = Converter::escapeChar(t -> lexeme);
					o.value = v;
				} break;
				case TokenType::realLiteral: {
					o.type = BasicType::RealType;
					Real * v = new Real;
					* v = Converter::stringToReal(t -> lexeme);
					o.value = v;
				} break;
				case TokenType::nullLiteral: {
					o.type = BasicType::ClassType;
					o.value = nullptr;
				} break;
				default: return o; break;
			}
			return o;
		}

		void visitBinaryExpression(Binary * e) override { }
		void visitAssignmentExpression(Assignment * e) override { }
		void visitCallExpression(Call * e) override { }
		void visitGetExpression(Get * e) override { }
		void visitGroupingExpression(Grouping * e) override {
			try { evaluateExpression(e); }
			catch (Exception & e) { throw; }
		}
		void visitLiteralExpression(Literal * e) override {
			try { value = literalToObject(e -> token); }
			catch (Exception & e) { throw; }
		}
		void visitLogicalExpression(Logical * e) override { }
		void visitSetExpression(Set * e) override { }
		void visitSuperExpression(Super * e) override { }
		void visitThisExpression(This * e) override { }
		void visitUnaryExpression(Unary * e) override {
			try {
				evaluateExpression(e -> r);
				value = Processor::applyUnaryOperand(e -> o, & value);
			} catch (Exception & e) { throw; }
		}
		void visitVariableExpression(Variable * e) override { }

		void evaluateExpression(Expression * e) {
			try { e -> accept(this); }
			catch (Exception & e) { throw; }
		}

		public:

		Interpreter() { }

		Object * evaluate(Expression * e) {
			try {
				e -> accept(this);
				return new Object(value);
			}
			catch (Exception & e) { throw; }
		}

	};

}

#endif
