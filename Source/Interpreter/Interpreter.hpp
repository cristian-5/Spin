
/*!
 *
 *    + --------------------------------------- +
 *    |  Interpreter.hpp                        |
 *    |                                         |
 *    |           Language Interpreter          |
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

#ifndef SPININTERPRETER
#define SPININTERPRETER

namespace Spin {

	/* Expressions */

	Object * Interpreter::visitAssignmentExpression(Assignment * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> value);
			Object * obj = nullptr;
			try {
				obj = memory -> getReference(e -> name -> lexeme);
			} catch (VariableNotFoundException & r) {
				if (expression) delete expression;
				expression = nullptr;
				throw EvaluationError(
					"Unexpected identifier '" +
					e -> name -> lexeme + "'!", * e -> name
				);
			}
			CPU -> applyAssignment(e -> o, obj, expression);
			return expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	Object * Interpreter::visitBinaryExpression(Binary * e) {
		Object * l = nullptr;
		Object * r = nullptr;
		Object * result = nullptr;
		try {
			l = evaluateExpression(e -> l);
			r = evaluateExpression(e -> r);
			result = CPU -> applyBinaryOperator(e -> o, l, r);
			delete r; delete l; return result;
		} catch (Exception & exc) {
			if (l) delete l;
			if (r) delete r;
			if (result) delete result;
			throw;
		}
	}
	Object * Interpreter::visitBraExpression(Bra * e) {
		Object * v = nullptr;
		try {
			v = memory -> getValue(e -> name);
		} catch (VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> name + "'!", * e -> bra
			);
		}
		if (v -> type != BasicType::VectorType) {
			delete v;
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> name + "'!", * e -> bra
			);
		}
		((Vector *) v -> value) -> inBraForm();
		return v;
	}
	Object * Interpreter::visitCallExpression(Call * e) {
		Object * callee = nullptr;
		CallProtocol * function = nullptr;
		Array<Object *> arguments = Array<Object *>();
		try {
			callee = evaluateExpression(e -> callee);
			for (Expression * a : * e -> arguments) {
				// TODO: push reference if ref keyword
				Object * evaluation = evaluateExpression(a);
				arguments.push(evaluation -> copy());
				delete evaluation;
			}
			if (!(callee -> isCallable()) ||
				!(callee -> value)) {
				throw EvaluationError(
					"Failed call of invalid function!",
					* e -> parenthesis
				);
			}
			function = (CallProtocol *)(callee -> value);
			if (function -> isInstanceOf<Class>()) {
				if (!e -> isConstructor)	{
					throw EvaluationError(
						"Invalid constructor call missing 'new' keyword!",
						* e -> parenthesis
					);
				}
			} else if (e -> isConstructor) {
				throw EvaluationError(
					"Function call forbids unnecessary 'new' keyword!",
					* e -> parenthesis
				);
			}
			if (arguments.size() != function -> arity()) {
				throw EvaluationError(
					"Call of " + function -> stringValue() + " doesn't match the predefined parameters!",
					* e -> parenthesis
				);
			}
			Object * result = function -> call(this, arguments, e -> parenthesis);
			return result;
		} catch (Exception & exc) {
			if (callee) delete callee;
			for (Object * o : arguments) delete o;
			throw;
		}
	}
	Object * Interpreter::visitComparisonExpression(Comparison * e) {
		Object * l = nullptr;
		Object * r = nullptr;
		Object * result = nullptr;
		try {
			l = evaluateExpression(e -> l);
			r = evaluateExpression(e -> r);
			result = CPU -> applyComparisonOperator(e -> o, l, r);
			delete r; delete l; return result;
		} catch (Exception & exc) {
			if (l) delete l;
			if (r) delete r;
			if (result) delete result;
			throw;
		}
	}
	Object * Interpreter::visitGetExpression(Get * e) {
		Object * object = nullptr;
		try {
			object = evaluateExpression(e -> object);
			if (object -> type == BasicType::InstanceType) {
				try {
					return ((Instance *) object -> value) -> getValue(
						e -> name -> lexeme
					);
				} catch (UndefinedException & u) {
					throw EvaluationError(
						"The resolved object does not contain any property named '" +
						e -> name -> lexeme + "'!", * e -> name
					);
				}
			}
			throw EvaluationError(
				"The resolved object is not an instance and does not contain properties!",
				* e -> name
			);
		} catch (Exception & exc) {
			if (object) delete object;
			throw;
		}
		return object;
	}
	Object * Interpreter::visitGroupingExpression(Grouping * e) {
		try { return evaluateExpression(e -> expression); }
		catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitInnerExpression(Inner * e) {
		Object * b = nullptr;
		Object * k = nullptr;
		try {
			b = memory -> getValue(e -> bra);
		} catch (VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> bra + "'!", * e -> inner
			);
		}
		try {
			k = memory -> getValue(e -> ket);
		} catch (VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> ket + "'!", * e -> inner
			);
		}
		if (b -> type != BasicType::VectorType) {
			delete b; delete k;
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> bra + "'!", * e -> inner
			);
		}
		if (k -> type != BasicType::VectorType) {
			delete b; delete k;
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> ket + "'!", * e -> inner
			);
		}
		((Vector *) b -> value) -> inBraForm();
		((Vector *) k -> value) -> inKetForm();
		try {
			return CPU -> applyInnerProduct(e -> inner, b, k);
		} catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitKetExpression(Ket * e) {
		Object * v = nullptr;
		try {
			v = memory -> getValue(e -> name);
		} catch (VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> name + "'!", * e -> ket
			);
		}
		if (v -> type != BasicType::VectorType) {
			delete v;
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> name + "'!", * e -> ket
			);
		}
		((Vector *) v -> value) -> inKetForm();
		return v;
	}
	Object * Interpreter::visitListExpression(List * e) {
		Array<Object *> * elements = new Array<Object *>();
		try {
			for (Expression * ex : * e -> values) {
				elements -> push(evaluateExpression(ex));
			}
		} catch (Exception & exc) {
			for (Object * o : * elements) delete o;
			delete elements;
			throw;
		}
		return new Object(BasicType::ArrayType, new ArrayList(elements));
	}
	Object * Interpreter::visitLiteralExpression(Literal * e) {
		try {
			if (!(e -> object)) {
				e -> object = Converter::literalToObject(e -> token);
			}
			return e -> object -> copy();
		} catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitLogicalExpression(Logical * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> l);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Invalid Bool expression found on left side of circuit operator '||'!",
					* e -> o
				);
			}
			if ((e -> o -> type) == TokenType::OR) {
				if (expression -> getBoolValue()) return expression;
			} else if (!(expression -> getBoolValue())) return expression;
			return evaluateExpression(e -> r);
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	Object * Interpreter::visitMutableExpression(Mutable * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> value);
			Object * obj = nullptr;
			try {
				obj = memory -> getReference(e -> name -> lexeme);
			} catch (VariableNotFoundException & r) {
				throw EvaluationError(
					"Unexpected identifier '" +
					e -> name -> lexeme + "'!", * e -> name
				);
			}
			CPU -> applyMutableAssignment(e -> o, obj, expression);
			return obj -> copy();
		} catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitOuterExpression(Outer * e) {
		return nullptr;
	}
	Object * Interpreter::visitSetExpression(Set * e) {
		/* Work in progress */
		Object * object = nullptr;
		Object * value = nullptr;
		try {
			object = evaluateExpression(e -> object);
			if (object -> type != BasicType::InstanceType) {
				throw EvaluationError(
					"The resolved object is not an instance and does not contain properties!",
					* e -> name
				);
			}
			value = evaluateExpression(e -> value);
			// TODO: finish this.
		} catch (Exception & exc) {
			if (object) delete object;
			if (value) delete value;
			throw;
		}
		return nullptr;
	}
	Object * Interpreter::visitSubscriptExpression(Subscript * e) {
		Object * item = nullptr;
		Object * expression = nullptr;
		Object * result = nullptr;
		try {
			item = evaluateExpression(e -> item);
			if (!(item -> isSubscriptable()) ||
				!(item -> value)) {
				throw EvaluationError(
					"The selected object doesn't support subscription!",
					* e -> bracket
				);
			}
			expression = evaluateExpression(e -> expression);
			result = CPU -> applySubscriptOperator(e -> bracket, item, expression);
			delete item; delete expression;
			return result;
		} catch (Exception & exc) {
			if (item) delete item;
			if (expression) delete expression;
			if (result) delete result;
			throw;
		}
	}
	Object * Interpreter::visitSuperExpression(Super * e) { return nullptr; }
	Object * Interpreter::visitThisExpression(This * e) { return nullptr; }
	Object * Interpreter::visitUnaryExpression(Unary * e) {
		Object * expression = nullptr;
		Object * result = nullptr;
		try {
			expression = evaluateExpression(e -> r);
			result = CPU -> applyUnaryOperator(e -> o, expression);
			delete expression; return result;
		} catch (Exception & exc) {
			if (expression) delete expression;
			if (result) delete result;
			throw;
		}
	}
	Object * Interpreter::visitIdentifierExpression(Identifier * e) {
		try {
			return memory -> getValue(e -> name -> lexeme);
		} catch (VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected identifier '" +
				e -> name -> lexeme + "'!", * e -> name
			);
		}
	}

	Object * Interpreter::evaluateExpression(Expression * e) {
		try { return e -> accept(this); }
		catch (Exception & exc) { throw; }
	}

	/* Statements */

	void Interpreter::visitBlockStatement(BlockStatement * e) {
		try {
			executeBlock(e -> statements, new Environment(memory));
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitBreakStatement(BreakStatement * e) {
		broken = true;
	}
	void Interpreter::visitClassStatement(ClassStatement * e) {
		try {
			memory -> define(
				e -> name -> lexeme,
				new Object(
					BasicType::ClassType,
					new Class(e -> name -> lexeme)
				)
			);
		} catch (VariableRedefinitionException & r) {
			throw EvaluationError(
				"Object redefinition! The object '" +
				e -> name -> lexeme + "' was already declared with type '" +
				r.getType() + "'!", * e -> name
			);
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitContinueStatement(ContinueStatement * e) {
		continued = true;
	}
	void Interpreter::visitDeleteStatement(DeleteStatement * e) {
		try {
			memory -> forget(e -> name -> lexeme);
		} catch (VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected identifier '" + e -> name -> lexeme +
				"'! You can only delete variables defined in the innermost scope.",
				* e -> name
			);
		}
	}
	void Interpreter::visitDoWhileStatement(DoWhileStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> expression);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical condition in iteration statement!",
					* e -> whileToken
				);
			}
			Bool condition = expression -> getBoolValue();
			executeStatement(e -> body);
			if (broken) { broken = false; return; }
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				expression = evaluateExpression(e -> expression);
				condition = expression -> getBoolValue();
			}
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	void Interpreter::visitExpressionStatement(ExpressionStatement * e) {
		try { evaluateExpression(e -> e); }
		catch (Exception & exc) { throw; }
	}
	void Interpreter::visitFileStatement(FileStatement * e) {
		if (fileName) delete fileName;
		fileName = new String(* e -> name);
		fileContents = e -> file;
	}
	void Interpreter::visitForStatement(ForStatement * e) {
		Object * expression = nullptr;
		try {
			executeStatement(e -> declaration);
			expression = evaluateExpression(e -> expression);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> forToken
				);
			}
			Bool condition = expression -> getBoolValue();
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				evaluateExpression(e -> stepper);
				expression = evaluateExpression(e -> expression);
				condition = expression -> getBoolValue();
			}
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	void Interpreter::visitFunctionStatement(FunctionStatement * e) {
		try {
			Object * function = new Object(BasicType::FunctionType, new Function(e, memory));
			memory -> define(e -> name -> lexeme, function);
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitIfStatement(IfStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> expression);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in conditional statement!",
					* e -> ifToken
				);
			}
			if (expression -> getBoolValue()) {
				executeStatement(e -> thenBranch);
			} else if (e -> elseBranch) {
				executeStatement(e -> elseBranch);
			}
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	void Interpreter::visitLoopStatement(LoopStatement * e) {
		try {
			while (true) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
			}
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitPrintStatement(PrintStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> e);
			Output << expression -> getObjectStringValue() << endLine;
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	void Interpreter::visitProcedureStatement(ProcedureStatement * e) {
		try {
			Object * procedure = new Object(BasicType::FunctionType, new Procedure(e, memory));
			memory -> define(e -> name -> lexeme, procedure);
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitRepeatUntilStatement(RepeatUntilStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> expression);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> untilToken
				);
			}
			Bool condition = expression -> getBoolValue();
			executeStatement(e -> body);
			if (broken) { broken = false; return; }
			while (!condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				expression = evaluateExpression(e -> expression);
				condition = expression -> getBoolValue();
			}
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	void Interpreter::visitRestStatement(RestStatement * e) {
	}
	void Interpreter::visitReturnStatement(ReturnStatement * e) {
		Object * expression = nullptr;
		try {
			if (e -> e) expression = evaluateExpression(e -> e);
			throw InterpreterReturn(expression, new Token(* e -> returnToken));
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitUntilStatement(UntilStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> expression);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> untilToken
				);
			}
			Bool condition = expression -> getBoolValue();
			while (!condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				expression = evaluateExpression(e -> expression);
				condition = expression -> getBoolValue();
			}
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	void Interpreter::visitVariableStatement(VariableStatement * e) {
		Object * expression = nullptr;
		try {
			if (e -> initialiser) {
				expression = evaluateExpression(e -> initialiser);
				Object * o = new Object(e -> type);
				CPU -> applyAssignment(e -> equal, o, expression);
				delete expression; expression = o;
			} else if (e -> object) {
				Object * o = nullptr;
				try { o = memory -> getValue(e -> object -> lexeme); }
				catch (Exception & oex) {
					throw EvaluationError(
						"Object definition not found!", * e -> object
					);
				}
				Instance * i = new Instance((Class *)o -> value);
				expression = new Object(e -> type, i);
			} else expression = new Object(e -> type);
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
		try {
			memory -> define(e -> name -> lexeme, expression);
		} catch (VariableRedefinitionException & r) {
			throw EvaluationError(
				"Variable redefinition! The identifier '" +
				e -> name -> lexeme + "' was already declared with type '" +
				r.getType() + "'!", * e -> name
			);
		}
	}
	void Interpreter::visitVectorStatement(VectorStatement * e) {
		Object * expression = nullptr;
		try {
			if (e -> initialiser) {
				expression = evaluateExpression(e -> initialiser);
				Object * o = new Object(
					BasicType::VectorType,
					/* A ket starts with '|': */
					new Vector(e -> vector -> lexeme[0] == '|' ? 0 : 1)
				);
				CPU -> applyVectorAssignment(e -> equal, o, expression);
				delete expression; expression = o;
			} else {
				expression = new Object(
					BasicType::VectorType,
					/* A ket starts with '|': */
					new Vector(e -> vector -> lexeme[0] == '|' ? 0 : 1)
				);
			}
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
		try {
			memory -> define(e -> name, expression);
		} catch (VariableRedefinitionException & r) {
			throw EvaluationError(
				"Vector redefinition! The identifier '" +
				e -> name + "' was already declared with type '" +
				r.getType() + "'!", * e -> vector
			);
		}
	}
	void Interpreter::visitWhileStatement(WhileStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluateExpression(e -> expression);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> whileToken
				);
			}
			Bool condition = expression -> getBoolValue();
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				expression = evaluateExpression(e -> expression);
				condition = expression -> getBoolValue();
			}
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}

	void Interpreter::executeStatement(Statement * statement) {
		try { statement -> accept(this); }
		catch (Exception & exc) { throw; }
	}
	void Interpreter::executeBlock(Array<Statement *> * statements, Environment * environment) {
		Environment * previous = memory;
		try {
			memory = environment;
			for (Statement * statement : * statements) {
				executeStatement(statement);
				if (broken || continued) {
					continued = false; break;
				}
			}
		} catch (Exception & exc) {
			memory = previous;
			delete environment;
			throw;
		}
		memory = previous;
		delete environment;
	}
	void Interpreter::executeFunction(BlockStatement * block, Environment * environment) {
		try {
			executeBlock(block -> statements, environment);
		} catch (Exception & exc) { throw; }
	}

	Object * Interpreter::evaluate(Expression * expression, String * input, String fileName) {
		try {
			return expression -> accept(this);
		} catch (EvaluationError & e) {
			const UInt32 cursor = e.getToken().position;
			SizeType line = Linker::getLine(fileContents, cursor);
			throw InterpreterErrorException(
				e.getMessage(), line, fileName
			);
		}
	}

	Interpreter::Interpreter() {
		globals = new Environment();
		memory = globals;
	}

	void Interpreter::evaluate(SyntaxTree * syntaxTree) {
		if (!syntaxTree) return;
		if (syntaxTree -> standardLibrary) Standard::defineLibrary(globals);
		if (syntaxTree -> mathsLibrary) Maths::defineLibrary(globals);
		if (syntaxTree -> kronosLibrary) Kronos::defineLibrary(globals);
		try {
			for (Statement * statement : * syntaxTree -> statements) {
				executeStatement(statement);
			}
		} catch (EvaluationError & e) {
			const SizeType cursor = e.getToken().position;
			SizeType line = Linker::getLine(fileContents, cursor);
			if (!fileName) fileName = new String("Unknown File");
			throw InterpreterErrorException(
				e.getMessage(), line, * fileName
			);
		}
	}

}

#endif
