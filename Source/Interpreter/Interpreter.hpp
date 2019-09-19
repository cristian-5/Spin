
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

#include "../Aliases/Prototypes.hpp"

#ifndef STACKINTERPRETER
#define STACKINTERPRETER

namespace Stack {

	void Interpreter::deleteValue() { delete value; value = nullptr; }
	void Interpreter::resetValue() { value = nullptr; }
	void Interpreter::setValue(Object * o) {
		if (value == o) return;
		if (value) delete value;
		value = o;
	}

	/* Expressions */

	void Interpreter::visitArrayExpression(Array * e) { }
	void Interpreter::visitAssignmentExpression(Assignment * e) {
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
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitBinaryExpression(Binary * e) {
		try {
			evaluateExpression(e -> l);
			Object * l = value -> copy();
			evaluateExpression(e -> r);
			Object * r = value -> copy();
			setValue(CPU -> applyBinaryOperator(e -> o, l, r));
			delete r; delete l;
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitCallExpression(Call * e) {
		try {
			evaluateExpression(e -> callee);
			Object * callee = value -> copy();
			deleteValue();
			ArrayList<Object *> arguments = ArrayList<Object *>();
			for (Expression * a : e -> arguments) {
				evaluateExpression(a);
				// TODO: push reference if ref keyword
				arguments.push(value -> copy());
				deleteValue();
			}
			if (!(callee -> isFunction()) ||
				!(callee -> value)) {
				throw EvaluationError(
					"Failed call of invalid function!",
					* e -> parenthesis
				);
			}
			auto function = (CallProtocol *)(callee -> value);
			if (arguments.size() != function -> arity()) {
				throw EvaluationError(
					"Call of " + function -> stringValue() + " doesn't match the predefined parameters!",
					* e -> parenthesis
				);
			}
			try { setValue(function -> call(this, arguments, e -> parenthesis)); }
			catch (InterpreterReturn & ret) { }
		} catch (EvaluationError & r) { throw; }
	}
	void Interpreter::visitComparisonExpression(Comparison * e) {
		try {
			evaluateExpression(e -> l);
			Object * l = value -> copy();
			evaluateExpression(e -> r);
			Object * r = value -> copy();
			setValue(CPU -> applyComparisonOperator(e -> o, l, r));
			delete r; delete l;
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitGetExpression(Get * e) { }
	void Interpreter::visitGroupingExpression(Grouping * e) {
		try { evaluateExpression(e -> expression); }
		catch (Exception & r) { throw; }
	}
	void Interpreter::visitLiteralExpression(Literal * e) {
		try {
			if (!(e -> object)) {
				e -> object = Converter::literalToObject(e -> token);
			}
			setValue((e -> object) -> copy());
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitLogicalExpression(Logical * e) {
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
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitSetExpression(Set * e) { }
	void Interpreter::visitSuperExpression(Super * e) { }
	void Interpreter::visitThisExpression(This * e) { }
	void Interpreter::visitUnaryExpression(Unary * e) {
		try {
			evaluateExpression(e -> r);
			setValue(CPU -> applyUnaryOperator(e -> o, value));
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitIdentifierExpression(Identifier * e) {
		try {
			setValue(memory -> getValue(e -> name -> lexeme));
		} catch (VariableNotFoundException & r) {
			throw EvaluationError(
				"Unexpected identifier '" +
				e -> name -> lexeme + "'!", * e -> name
			);
		}
	}

	void Interpreter::evaluateExpression(Expression * e) {
		try { e -> accept(this); }
		catch (Exception & r) { throw; }
	}

	/* Statements */

	void Interpreter::visitBlockStatement(BlockStatement * e) {
		try {
			executeBlock(e -> statements, new Environment(memory));
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitBreakStatement(BreakStatement * e) {
		broken = true;
	}
	void Interpreter::visitContinueStatement(ContinueStatement * e) {
		continued = true;
	}
	void Interpreter::visitDoWhileStatement(DoWhileStatement * e) {
		try {
			evaluateExpression(e -> expression);
			if (!(value -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> whileToken
				);
			}
			Bool condition = value -> getBoolValue();
			executeStatement(e -> body);
			if (broken) { broken = false; return; }
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				evaluateExpression(e -> expression);
				condition = value -> getBoolValue();
			}
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitExpressionStatement(ExpressionStatement * e) {
		try { evaluateExpression(e -> e); }
		catch (Exception & r) { throw; }
	}
	void Interpreter::visitForStatement(ForStatement * e) {
		try {
			executeStatement(e -> declaration);
			evaluateExpression(e -> expression);
			if (!(value -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> forToken
				);
			}
			Bool condition = value -> getBoolValue();
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				evaluateExpression(e -> stepper);
				evaluateExpression(e -> expression);
				condition = value -> getBoolValue();
			}
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitFunctionStatement(FunctionStatement * e) {
		Object * function = new Object(BasicType::FunctionType, new Function(e, memory));
		memory -> define(e -> name -> lexeme, function);
	}
	void Interpreter::visitIfStatement(IfStatement * e) {
		try {
			evaluateExpression(e -> expression);
			if (!(value -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in conditional statement!",
					* e -> ifToken
				);
			}
			if (value -> getBoolValue()) {
				executeStatement(e -> thenBranch);
			} else if (e -> elseBranch) {
				executeStatement(e -> elseBranch);
			}
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitLoopStatement(LoopStatement * e) {
		try {
			while (true) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
			}
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitPrintStatement(PrintStatement * e) {
		try {
			evaluateExpression(e -> e);
			// TO FIX: Find a better way to output things.
			std::cout << value -> getObjectStringValue() << std::endl;
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitRepeatUntilStatement(RepeatUntilStatement * e) {
		try {
			evaluateExpression(e -> expression);
			if (!(value -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> untilToken
				);
			}
			Bool condition = value -> getBoolValue();
			executeStatement(e -> body);
			if (broken) { broken = false; return; }
			while (!condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				evaluateExpression(e -> expression);
				condition = value -> getBoolValue();
			}
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitRestStatement(RestStatement * e) {	
	}
	void Interpreter::visitReturnStatement(ReturnStatement * e) {
		try {
			if (e -> e) evaluateExpression(e -> e);
			else deleteValue();
			throw InterpreterReturn();
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitUntilStatement(UntilStatement * e) {
		try {
			evaluateExpression(e -> expression);
			if (!(value -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> untilToken
				);
			}
			Bool condition = value -> getBoolValue();
			while (!condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				evaluateExpression(e -> expression);
				condition = value -> getBoolValue();
			}
		} catch (Exception & r) { throw; }
	}
	void Interpreter::visitVariableStatement(VariableStatement * e) {
		try {
			if (e -> initializer) {
				evaluate(e -> initializer);
				Object * o = new Object(e -> type);
				CPU -> applyAssignment(e -> name, o, value);
				setValue(o);
			} else setValue(new Object(e -> type));
		} catch (Exception & r) { throw; }
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
	void Interpreter::visitWhileStatement(WhileStatement * e) {
		try {
			evaluateExpression(e -> expression);
			if (!(value -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> whileToken
				);
			}
			Bool condition = value -> getBoolValue();
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				evaluateExpression(e -> expression);
				condition = value -> getBoolValue();
			}
		} catch (Exception & r) { throw; }
	}

	void Interpreter::executeStatement(Statement * statement) {
		try { statement -> accept(this); }
		catch (Exception & r) { throw; }
	}
	void Interpreter::executeBlock(ArrayList<Statement *> statements,
						Environment * environment) {
		Environment * previous = memory;
		try {
			memory = environment;
			for (Statement * statement : statements) {
				executeStatement(statement);
				if (broken || continued) {
					continued = false; break;
				}
			}
		} catch (Exception & r) {
			memory = previous;
			delete environment;
			throw;
		}
		memory = previous;
		delete environment;
	}
	void Interpreter::executeFunction(BlockStatement * block,
							Environment * environment) {
		try {
			executeBlock(block -> statements, environment);
		} catch (Exception & r) { throw; }
	}

	Object * Interpreter::getCurrentValue() const { return value; }

	Interpreter::Interpreter() {
		globals = new Environment();
		memory = globals;
		globals -> define(
			"clock", new Object(BasicType::FunctionType,
				new NativeFunction(
					[] (Interpreter * i, ArrayList<Object *> a) {
						Int64 time = std::chrono::duration_cast
									<std::chrono::milliseconds>
									(std::chrono::system_clock::now()
									.time_since_epoch()).count();
						return new Object(BasicType::Int64Type, new Int64(time));
					}
				)
			)
		);
	}

	void Interpreter::evaluate(ArrayList<Statement *> * statements, String * input, String fileName) {
		try {
			for (Statement * statement : (* statements)) {
				executeStatement(statement);
			}
		} catch (EvaluationError & e) {
			const UInt32 cursor = e.getToken().position;
			FilePosition fp = Linker::getPosition(input, cursor);
			throw InterpreterErrorException(
				e.getMessage(), fp, fileName
			);
		}
	}

	Object * Interpreter::evaluate(Expression * expression, String * input, String fileName) {
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

}

#endif
