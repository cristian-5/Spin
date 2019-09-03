
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

#include "Converter.hpp"
#include "Environment.hpp"
#include "Processor.hpp"

namespace Stack {

	class InterpreterErrorException: public Exception {
		private:
		const String _message;
		const FilePosition _position;
		const String _fileName;
		public:
		const String & getMessage() const { return _message; }
		const FilePosition & getPosition() const { return _position; }
		const String & getFileName() const { return _fileName; }
		InterpreterErrorException(String message, FilePosition position, String name):
		Exception(),  _message(message), _position(position), _fileName(name) { }
	};

	class Interpreter: public Expression::Visitor, public Statement::Visitor {

		private:

		Object * value = nullptr;

		Processor * CPU = Processor::self();

		Environment * memory = new Environment();

		/* Core */

		void deleteValue() { delete value; value = nullptr; }

		void resetValue() { value = nullptr; }

		void setValue(Object * o) {
			if (value == o) return;
			if (value) delete value;
			value = o;
		}

		/* Expressions */

		void visitAssignmentExpression(Assignment * e) override {
			try {
				evaluate(e -> value);
				Object * o = nullptr;
				try {
					o = memory -> getReference(e -> name-> lexeme);
				} catch (VariableNotFoundException & r) {
					throw EvaluationError(
						"Unexpected identifier '" +
						e -> name -> lexeme + "'!", * e -> name
					);
				}
				CPU -> applyAssignment(e -> name, o, value);
			} catch (EvaluationError & r) { throw; }
		}
		void visitBinaryExpression(Binary * e) override {
			try {
				evaluateExpression(e -> l);
				Object * l = value -> copy();
				evaluateExpression(e -> r);
				Object * r = value -> copy();
				setValue(CPU -> applyBinaryOperator(e -> o, l, r));
				delete r; delete l;
			} catch (EvaluationError & r) { throw; }
		}
		void visitCallExpression(Call * e) override { }
		void visitComparisonExpression(Comparison * e) override {
			try {
				evaluateExpression(e -> l);
				Object * l = value -> copy();
				evaluateExpression(e -> r);
				Object * r = value -> copy();
				setValue(CPU -> applyComparisonOperator(e -> o, l, r));
				delete r; delete l;
			} catch (EvaluationError & r) { throw; }
		}
		void visitGetExpression(Get * e) override { }
		void visitGroupingExpression(Grouping * e) override {
			try { evaluateExpression(e -> expression); }
			catch (EvaluationError & r) { throw; }
		}
		void visitLiteralExpression(Literal * e) override {
			try {
				if (!(e -> object)) {
					e -> object = Converter::literalToObject(e -> token);
				}
				setValue(e -> object);
			} catch (EvaluationError & r) { throw; }
		}
		void visitLogicalExpression(Logical * e) override {
			try {
				evaluateExpression(e -> l);
				if (!(value -> isBool())) {
					throw EvaluationError(
						"Invalid Bool expression found on left side of circuit operator '||'!",
						* e -> o
					);
				}
				if ((e -> o -> type) == TokenType::OR) {
					if (value -> getBoolValue()) return;
				} else if (!(value -> getBoolValue())) return;
				deleteValue();
				evaluateExpression(e -> r);
			} catch (EvaluationError & r) { throw; }
		}
		void visitSetExpression(Set * e) override { }
		void visitSuperExpression(Super * e) override { }
		void visitThisExpression(This * e) override { }
		void visitUnaryExpression(Unary * e) override {
			try {
				evaluateExpression(e -> r);
				setValue(CPU -> applyUnaryOperator(e -> o, value));
			} catch (EvaluationError & r) { throw; }
		}
		void visitIdentifierExpression(Identifier * e) override {
			try {
				setValue(memory -> getValue(e -> name -> lexeme));
			} catch (VariableNotFoundException & r) {
				throw EvaluationError(
					"Unexpected identifier '" +
					e -> name -> lexeme + "'!", * e -> name
				);
			}
		}

		void evaluateExpression(Expression * e) {
			try { e -> accept(this); }
			catch (EvaluationError & r) { throw; }
		}

		/* Statements */

		void visitBlockStatement(BlockStatement * e) override {
			try {
				executeBlock(e -> statements, new Environment(memory));
			} catch (EvaluationError & r) { throw; }
		}
		void visitExpressionStatement(ExpressionStatement * e) override {
			try { evaluateExpression(e -> e); }
			catch (EvaluationError & r) { throw; }
		}
		void visitIfStatement(IfStatement * e) override {
			try {
				evaluateExpression(e -> expression);
				if (!(value -> isBool())) {
					throw EvaluationError(
						"Unsupported evaluation of non logical expression in conditional statement!",
						* e -> ifToken
					);
				}
				if (value -> getBoolValue()) {
					execute(e -> thenBranch);
				} else if (e -> elseBranch) {
					execute(e -> elseBranch);
				}
			} catch (EvaluationError & r) { throw; }
		}
		void visitPrintStatement(PrintStatement * e) override {
			try {
				evaluateExpression(e -> e);
				// TO FIX: Find a better way to output things.
				std::cout << value -> getObjectStringValue() << std::endl;
			} catch (EvaluationError & r) { throw; }
		}
		void visitVariableStatement(VariableStatement * e) override {
			try {
				if (e -> initializer) {
					evaluate(e -> initializer);
					Object * o = new Object(e -> type);
					CPU -> applyAssignment(e -> name, o, value);
					setValue(o);
				} else setValue(new Object(e -> type));
			} catch (EvaluationError & r) { throw; }
			try {
				memory -> define(e -> name -> lexeme, value);
			} catch (VariableRedefinitionException & r) {
				throw EvaluationError(
					"Variable redefinition! The identifier '" +
					e -> name -> lexeme + "' was already declared with type '" +
					r.getType() + "'!", * e -> name
				);
			}
			resetValue();
		}

		void execute(Statement * statement) {
			try { statement -> accept(this); }
			catch (EvaluationError & r) { throw; }
		}

		void executeBlock(ArrayList<Statement *> statements,
						  Environment * environment) {
			 Environment * previous = memory;                         
			try {
				memory = environment;
				for (Statement * statement : statements) {
					execute(statement);
				}
			} catch (EvaluationError & r) {
				memory = previous;
				delete environment;
				throw;
			}
			memory = previous;
			delete environment;
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

		void evaluate(ArrayList<Statement *> * statements,
					  String * input = nullptr,
					  String fileName = "Unknown File") {
			try {
				for (Statement * statement : (* statements)) {
					execute(statement);
				}
			} catch (EvaluationError & e) {
				const UInt32 cursor = e.getToken().position;
				FilePosition fp = Linker::getPosition(input, cursor);
				throw InterpreterErrorException(
					e.getMessage(), fp, fileName
				);
			}
		}

		Object * evaluate(Expression * expression,
						  String * input = nullptr,
						  String fileName = "Unknown File") {
			try {
				expression -> accept(this);
			} catch (EvaluationError & e) {
				const UInt32 cursor = e.getToken().position;
				FilePosition fp = Linker::getPosition(input, cursor);
				throw InterpreterErrorException(
					e.getMessage(), fp, fileName
				);
			}
			return value;
		}

	};

}

#endif
