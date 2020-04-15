
/*!
 *
 *    + --------------------------------------- +
 *    |  Interpreter.cpp                        |
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
!*/

#include "../Aliases/Prototypes/Interpreter.hpp"

#ifndef SPIN_INTERPRETER
#define SPIN_INTERPRETER

#include "../Aliases/Prototypes/Libraries.hpp"
#include "../Aliases/Prototypes/Environment.hpp"
#include "../Aliases/Prototypes/Routines.hpp"
#include "../Aliases/Prototypes/Array.hpp"
#include "../Aliases/Prototypes/Class.hpp"
#include "../Aliases/Prototypes/Vector.hpp"
#include "../Aliases/Prototypes/Program.hpp"

namespace Spin {

	/* Exceptions */

	Interpreter::Return::Return(Object * v, Token * t): value(v), returnToken(t) { }
	Interpreter::Return::~Return() {
		if (returnToken) delete returnToken;
	}
	Object * Interpreter::Return::getReturnValue() const {
		return value;
	}
	Token * Interpreter::Return::getReturnToken() const {
		return returnToken;
	}

	/* Expressions */

	Object * Interpreter::visitAssignmentExpression(Assignment * e) {
		Object * expression;
		Object * obj;
		try { expression = evaluate(e -> value); }
		catch (Exception & exc) { throw; }
		try { obj = memory -> getReference(e -> name -> lexeme); }
		catch (Environment::VariableNotFoundException & r) {
			delete expression;
			throw Program::Error(
				currentUnit,
				"Unexpected identifier '" +
				e -> name -> lexeme + "'!",
				* e -> name, ErrorCode::evl
			);
		}
		try { CPU -> applyAssignment(e -> o, obj, expression); }
		catch (Program::Error & p) {
			delete expression;
			throw;
		}
		return expression;
	}
	Object * Interpreter::visitBinaryExpression(Binary * e) {
		Object * l = nullptr;
		Object * r = nullptr;
		Object * result;
		try {
			l = evaluate(e -> l);
			r = evaluate(e -> r);
			result = CPU -> applyBinaryOperator(e -> o, l, r);
		} catch (Exception & exc) {
			if (l) delete l;
			if (r) delete r;
			throw;
		}
		delete r;
		delete l;
		return result;
	}
	Object * Interpreter::visitBraExpression(Bra * e) {
		Object * v;
		try { v = memory -> getValue(e -> name); }
		catch (Environment::VariableNotFoundException & exc) {
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> name + "'!",
				* e -> bra, ErrorCode::evl
			);
		}
		if (v -> type != BasicType::VectorType) {
			delete v;
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> name + "'!",
				* e -> bra, ErrorCode::evl
			);
		}
		((Vector *) v -> value) -> inBraForm();
		return v;
	}
	Object * Interpreter::visitCallExpression(Call * e) {
		Array<Object *> arguments;
		Object * callee;
		Object * result;
		try { callee = evaluate(e -> callee); }
		catch (Exception & exc) { throw; }
		if (!(callee -> isCallable()) || !(callee -> value)) {
			delete callee;
			throw Program::Error(
				currentUnit,
				"Failed call of invalid function!",
				* e -> parenthesis, ErrorCode::evl
			);
		}
		Object * evaluation;
		for (Expression * a : * e -> arguments) {
			try { evaluation = evaluate(a); }
			catch (Exception & exc) {
				delete callee;
				for (auto a : arguments) delete a;
				throw;
			}
			arguments.push_back(evaluation);
		}
		CallProtocol * function = (CallProtocol *)(callee -> value);
		if (function -> isInstanceOf<Class>()) {
			if (!(e -> isConstructor)) {
				delete callee;
				for (auto a : arguments) delete a;
				throw Program::Error(
					currentUnit,
					"Constructor call is missing 'new' operator!",
					* e -> parenthesis, ErrorCode::evl
				);
			}
			// Needs a reference instead of a copy of the definition:
			Object * backUpCallee = callee;
			callee = memory -> getReference(
				((Class *) callee -> value) -> name
			);
			delete backUpCallee;
			function = (CallProtocol *)(callee -> value);
			try { result = function -> call(arguments, e -> parenthesis); }
			catch (Exception & exc) {
				delete callee;
				for (auto a : arguments) delete a;
				throw;
			}
			memory -> lose(result);
			delete callee;
			return result;
		} else if (e -> isConstructor) {
			delete callee;
			for (auto a : arguments) delete a;
			throw Program::Error(
				currentUnit,
				"Operator 'new' doesn't support operands of type 'Routine'!",
				* e -> parenthesis, ErrorCode::evl
			);
		}
		try { result = function -> call(arguments, e -> parenthesis); }
		catch (Exception & exc) {
			delete callee;
			for (auto a : arguments) delete a;
			throw;
		}
		delete callee;
		return result;
	}
	Object * Interpreter::visitComparisonExpression(Comparison * e) {
		Object * l = nullptr;
		Object * r = nullptr;
		Object * result;
		try {
			l = evaluate(e -> l);
			r = evaluate(e -> r);
			result = CPU -> applyComparisonOperator(e -> o, l, r);
		} catch (Exception & exc) {
			if (l) delete l;
			if (r) delete r;
			throw;
		}
		delete r;
		delete l;
		return result;
	}
	Object * Interpreter::visitDynamicGetExpression(DynamicGet * e) {
		Object * object = nullptr;
		try {
			object = evaluate(e -> object);
			if (object -> type == BasicType::UnknownType) {
				throw Program::Error(
					currentUnit,
					"The resolved object is not an instance and does not contain properties!",
					* e -> name, ErrorCode::evl
				);
			}
			try {
				Object * value = nullptr;
				// Check if we are inside a method:
				if (e -> selfReference) {
					value = ((Instance *) object -> value) -> getInnerValue(
						e -> name -> lexeme
					);
				} else value = object -> getAttribute(e -> name -> lexeme);
				if (value -> type == BasicType::RoutineType) {
					// Bind self to the function:
					((CallProtocol *) value -> value) -> self = object;
				} else delete object;
				return value;
			} catch (Environment::VariableNotFoundException & u) {
				throw Program::Error(
					currentUnit,
					"The resolved object does not contain any attribute, field or method named '" +
					e -> name -> lexeme + "'!",
					* e -> name, ErrorCode::evl
				);
			}
		} catch (Exception & exc) {
			if (object) delete object;
			throw;
		}
		return object;
	}
	Object * Interpreter::visitDynamicSetExpression(DynamicSet * e) {
		Object * object = nullptr;
		Object * value = nullptr;
		try {
			object = evaluate(e -> object);
			if (object -> type != BasicType::InstanceType) {
				throw Program::Error(
					currentUnit,
					"The resolved object is not an instance and does not contain properties!",
					* e -> name, ErrorCode::evl
				);
			}
			value = evaluate(e -> value);
			Object * field = nullptr;
			try {
				// Check if we are inside a method:
				if (e -> selfReference) {
					field = ((Instance *) object -> value) -> getInnerReference(
						e -> name -> lexeme
					);
				} else {
					field = ((Instance *) object -> value) -> getReference(
						e -> name -> lexeme
					);
				}
			} catch (Environment::VariableNotFoundException & u) {
				throw Program::Error(
					currentUnit,
					"The resolved object does not contain any attribute, field or method named '" +
					e -> name -> lexeme + "'!",
					* e -> name, ErrorCode::evl
				);
			}
			CPU -> applyAssignment(e -> equals, field, value);
		} catch (Exception & exc) {
			if (object) delete object;
			if (value) delete value;
			throw;
		}
		return value;
	}
	Object * Interpreter::visitGroupingExpression(Grouping * e) {
		try { return evaluate(e -> expression); }
		catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitIdentifierExpression(Identifier * e) {
		try { return memory -> getValue(e -> name -> lexeme); }
		catch (Environment::VariableNotFoundException & exc) {
			throw Program::Error(
				currentUnit,
				"Unexpected identifier '" +
				e -> name -> lexeme + "'!",
				* e -> name, ErrorCode::evl
			);
		}
	}
	Object * Interpreter::visitInnerExpression(Inner * e) {
		Object * b;
		Object * k;
		try { b = memory -> getValue(e -> bra); }
		catch (Environment::VariableNotFoundException & exc) {
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> bra + "'!",
				* e -> inner, ErrorCode::evl
			);
		}
		try { k = memory -> getValue(e -> ket); }
		catch (Environment::VariableNotFoundException & exc) {
			delete b;
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> ket + "'!",
				* e -> inner, ErrorCode::evl
			);
		}
		if (b -> type != BasicType::VectorType) {
			delete b; delete k;
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> bra + "'!",
				* e -> inner, ErrorCode::evl
			);
		}
		if (k -> type != BasicType::VectorType) {
			delete b; delete k;
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> ket + "'!",
				* e -> inner, ErrorCode::evl
			);
		}
		((Vector *) b -> value) -> inBraForm();
		((Vector *) k -> value) -> inKetForm();
		try { return CPU -> applyInnerProduct(e -> inner, b, k); }
		catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitKetExpression(Ket * e) {
		Object * v;
		try { v = memory -> getValue(e -> name); }
		catch (Environment::VariableNotFoundException & exc) {
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> name + "'!",
				* e -> ket, ErrorCode::evl
			);
		}
		if (v -> type != BasicType::VectorType) {
			delete v;
			throw Program::Error(
				currentUnit,
				"Unexpected Vector identifier '" +
				e -> name + "'!",
				* e -> ket, ErrorCode::evl
			);
		}
		((Vector *) v -> value) -> inKetForm();
		return v;
	}
	Object * Interpreter::visitListExpression(List * e) {
		Array<Object *> * elements = new Array<Object *>();
		try {
			for (Expression * ex : * e -> values) {
				elements -> push_back(evaluate(ex));
			}
		} catch (Exception & exc) {
			for (Object * o : * elements) delete o;
			delete elements;
			throw;
		}
		return new Object(BasicType::ArrayType, new ArrayList(elements));
	}
	Object * Interpreter::visitLiteralExpression(Literal * e) {
		try { return e -> object -> copy(); }
		catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitLogicalExpression(Logical * e) {
		Object * expression;
		try { expression = evaluate(e -> l); }
		catch (Exception & exc) { throw; }
		if (!(expression -> isBoolean())) {
			delete expression;
			throw Program::Error(
				currentUnit,
				"Invalid Boolean expression found on left side of circuit operator '||'!",
				* e -> o, ErrorCode::evl
			);
		}
		if ((e -> o -> type) == TokenType::OR) {
			if (expression -> getBoolValue()) return expression;
		} else if (!(expression -> getBoolValue())) return expression;
		delete expression;
		try { return evaluate(e -> r); }
		catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitMutableExpression(Mutable * e) {
		Object * expression;
		Object * obj;
		try { expression = evaluate(e -> value); }
		catch (Exception & exc) { throw; }
		try { obj = memory -> getReference(e -> name -> lexeme); }
		catch (Environment::VariableNotFoundException & r) {
			delete expression;
			throw Program::Error(
				currentUnit,
				"Unexpected identifier '" +
				e -> name -> lexeme + "'!",
				* e -> name, ErrorCode::evl
			);
		}
		try { CPU -> applyMutableAssignment(e -> o, obj, expression); }
		catch (Exception & exc) {
			delete expression;
			throw;
		}
		return expression;
	}
	Object * Interpreter::visitOuterExpression(Outer * e) {
		return nullptr;
	}
	Object * Interpreter::visitSelfExpression(Self * e) {
		try { return memory -> getValue("self"); }
		catch (Environment::VariableNotFoundException & exc) {
			throw Program::Error(
				currentUnit,
				"Unexpected use of 'self' outside of allowed context!",
				* e -> keyword, ErrorCode::evl
			);
		}
	}
	Object * Interpreter::visitStaticGetExpression(StaticGet * e) {
		Object * object = nullptr;
		try {
			object = evaluate(e -> object);
			Class * definition = nullptr;
			if (object -> type == BasicType::InstanceType) {
				definition = ((Instance *) object -> value) -> type;
			} else if (object -> type == BasicType::ClassType) {
				definition = (Class *)object -> value;
			} else {
				throw Program::Error(
					currentUnit,
					"The resolved object is not a definition that provides a static context!",
					* e -> name, ErrorCode::evl
				);
			}
			try {
				Object * value = nullptr;
				// Check if we are inside a method:
				if (e -> selfReference) {
					value = definition -> getInnerValue(
						e -> name -> lexeme
					);
				} else {
					value = definition -> getValue(
						e -> name -> lexeme
					);
				}
				if (value -> type == BasicType::RoutineType) {
					// Bind self to the function:
					((CallProtocol *) value -> value) -> self = object;
				} else delete object;
				return value;
			} catch (Environment::VariableNotFoundException & u) {
				throw Program::Error(
					currentUnit,
					"The resolved object does not contain any static attribute, field or method named '" +
					e -> name -> lexeme + "'!",
					* e -> name, ErrorCode::evl
				);
			}
		} catch (Exception & exc) {
			if (object) delete object;
			throw;
		}
		return object;
	}
	Object * Interpreter::visitStaticSetExpression(StaticSet * e) {
		Object * object = nullptr;
		Object * value = nullptr;
		try {
			object = evaluate(e -> object);
			Class * definition = nullptr;
			if (object -> type == BasicType::InstanceType) {
				definition = ((Instance *) object -> value) -> type;
			} else if (object -> type == BasicType::ClassType) {
				definition = (Class *) object -> value;
			} else {
				throw Program::Error(
					currentUnit,
					"The resolved object is not a definition that provides a static context!",
					* e -> name, ErrorCode::evl
				);
			}
			value = evaluate(e -> value);
			Object * field = nullptr;
			try {
				// Check if we are inside a method:
				if (e -> selfReference) {
					field = definition -> getInnerReference(
						e -> name -> lexeme
					);
				} else {
					field = definition -> getReference(
						e -> name -> lexeme
					);
				}
			} catch (Environment::VariableNotFoundException & u) {
				throw Program::Error(
					currentUnit,
					"The resolved object does not contain any attribute, field or method named '" +
					e -> name -> lexeme + "'!",
					* e -> name, ErrorCode::evl
				);
			}
			CPU -> applyAssignment(e -> equals, field, value);
		} catch (Exception & exc) {
			if (object) delete object;
			if (value) delete value;
			throw;
		}
		return value;
	}
	Object * Interpreter::visitSubscriptExpression(Subscript * e) {
		Object * item;
		Object * expression;
		Object * result;
		try { item = evaluate(e -> item); }
		catch (Exception & exc) { throw; }
		if (!(item -> isSubscriptable()) || !(item -> value)) {
			delete item;
			throw Program::Error(
				currentUnit,
				"The selected object doesn't support subscription!",
				* e -> bracket, ErrorCode::evl
			);
		}
		try { expression = evaluate(e -> expression); }
		catch (Exception & exc) {
			delete item;
			throw;
		}
		try { result = CPU -> applySubscriptOperator(e -> bracket, item, expression); }
		catch (Exception & exc) {
			delete item;
			delete expression;
			throw;
		}
		delete item;
		delete expression;
		return result;
	}
	Object * Interpreter::visitUnaryExpression(Unary * e) {
		Object * expression;
		Object * result;
		try { expression = evaluate(e -> r); }
		catch (Exception & exc) { throw; }
		try { result = CPU -> applyUnaryOperator(e -> o, expression); }
		catch (Exception & exc) {
			delete expression;
			throw;
		}
		delete expression;
		return result;
	}

	Object * Interpreter::evaluate(Expression * e) {
		try { return e -> accept(this); }
		catch (Exception & exc) { throw; }
	}

	/* Statements */

	void Interpreter::visitAttributeStatement(AttributeStatement * e) {
		currentModifier = e -> modifier;
		try { e -> field -> accept(this); }
		catch (Exception & exc) { throw; }
	}
	void Interpreter::visitBlockStatement(BlockStatement * e) {
		try { executeBlock(e -> statements, new Environment(memory)); }
		catch (Exception & exc) { throw; }
	}
	void Interpreter::visitBreakStatement(BreakStatement * e) {
		broken = true;
	}
	void Interpreter::visitClassStatement(ClassStatement * e) {
		try {
			classDefinition = new Class(e -> name -> lexeme, e -> dynamicAttributes);
			if (e -> atCreate) {
				classDefinition -> atCreate = new Procedure(
					e -> atCreate, memory
				);
			}
			String name;
			try {
				for (AttributeStatement * attribute : * e -> staticAttributes) {
					attribute -> accept(this);
				}
			} catch (Exception & exc) { throw; }
			try {
				memory -> define(
					e -> name -> lexeme,
					new Object(BasicType::ClassType, classDefinition)
				);
				classDefinition = nullptr;
			} catch (Environment::VariableRedefinitionException & r) {
				throw Program::Error(
					currentUnit,
					"Object redefinition! The object '" +
					e -> name -> lexeme + "' was already declared with type '" +
					r.getType() + "'!",
					* e -> name, ErrorCode::evl
				);
			}
		} catch (Exception & exc) {
			classDefinition = nullptr;
			throw;
		}
	}
	void Interpreter::visitContinueStatement(ContinueStatement * e) {
		continued = true;
	}
	void Interpreter::visitDeleteStatement(DeleteStatement * e) {
		try { memory -> forget(e -> name -> lexeme); }
		catch (Environment::VariableNotFoundException & exc) {
			throw Program::Error(
				currentUnit,
				"Unexpected identifier '" + e -> name -> lexeme +
				"'! You can only delete variables defined in the innermost scope.",
				* e -> name, ErrorCode::evl
			);
		}
	}
	void Interpreter::visitDoWhileStatement(DoWhileStatement * e) {
		Object * expression;
		try { expression = evaluate(e -> expression); }
		catch (Exception & exc) { throw; }
		if (!(expression -> isBoolean())) {
			delete expression;
			throw Program::Error(
				currentUnit,
				"Unsupported evaluation of non logical condition in iteration statement!",
				* e -> whileToken, ErrorCode::evl
			);
		}
		try { executeStatement(e -> body); }
		catch (Exception & exc) {
			delete expression;
			throw;
		}
		Boolean condition = expression -> getBoolValue();
		if (broken) {
			broken = false;
			delete expression;
			return;
		}
		while (condition) {
			try { executeStatement(e -> body); }
			catch (Exception & exc) {
				delete expression;
				throw;
			}
			delete expression;
			if (broken) {
				broken = false;
				break;
			}
			try { expression = evaluate(e -> expression); }
			catch (Exception & exc) { throw; }
			condition = expression -> getBoolValue();
		}
		delete expression;
	}
	void Interpreter::visitExpressionStatement(ExpressionStatement * e) {
		try { delete evaluate(e -> e); }
		catch (Exception & exc) { throw; }
	}
	void Interpreter::visitFileStatement(FileStatement * e) {
		currentUnit = e -> file;
		CPU -> currentUnit = e -> file;
	}
	void Interpreter::visitForStatement(ForStatement * e) {
		Object * expression = nullptr;
		try {
			executeStatement(e -> declaration);
			expression = evaluate(e -> expression);
			if (!(expression -> isBoolean())) {
				throw Program::Error(
					currentUnit,
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> forToken, ErrorCode::evl
				);
			}
			Boolean condition = expression -> getBoolValue();
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				evaluate(e -> stepper);
				expression = evaluate(e -> expression);
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
			if (instanceDefinition) {
				Object * function = new Object(
					BasicType::RoutineType,
					new Function(e, memory)
				);
				try {
					instanceDefinition -> defineDynamic(
						e -> name -> lexeme, currentModifier, function
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (function) delete function;
					throw Program::Error(
						currentUnit,
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!",
						* e -> name, ErrorCode::evl
					);
				}
			} else if (classDefinition) {
				Object * function = new Object(
					BasicType::RoutineType,
					new Function(e, memory)
				);
				try {
					classDefinition -> defineStatic(
						e -> name -> lexeme, currentModifier, function
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (function) delete function;
					throw Program::Error(
						currentUnit,
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!",
						* e -> name, ErrorCode::evl
					);
				}
			} else {
				Object * function = new Object(
					BasicType::RoutineType,
					new Function(e, memory)
				);
				try { memory -> define(e -> name -> lexeme, function); }
				catch (Environment::VariableRedefinitionException & r) {
					if (function) delete function;
					throw Program::Error(
						currentUnit,
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!",
						* e -> name, ErrorCode::evl
					);
				}
			}
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitIfStatement(IfStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> expression);
			if (!(expression -> isBoolean())) {
				throw Program::Error(
					currentUnit,
					"Unsupported evaluation of non logical expression in conditional statement!",
					* e -> ifToken, ErrorCode::evl
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
		while (true) {
			try { executeStatement(e -> body); }
			catch (Exception & exc) { throw; }
			if (broken) { broken = false; break; }
		}
	}
	void Interpreter::visitProcedureStatement(ProcedureStatement * e) {
		try {
			if (instanceDefinition) {
				Object * procedure = new Object(
					BasicType::RoutineType,
					new Procedure(e, memory)
				);
				try {
					instanceDefinition -> defineDynamic(
						e -> name -> lexeme, currentModifier, procedure
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (procedure) delete procedure;
					throw Program::Error(
						currentUnit,
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!",
						* e -> name, ErrorCode::evl
					);
				}
			} else if (classDefinition) {
				Object * procedure = new Object(
					BasicType::RoutineType,
					new Procedure(e, memory)
				);
				try {
					classDefinition -> defineStatic(
						e -> name -> lexeme, currentModifier, procedure
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (procedure) delete procedure;
					throw Program::Error(
						currentUnit,
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!",
						* e -> name, ErrorCode::evl
					);
				}
			} else {
				Object * procedure = new Object(BasicType::RoutineType, new Procedure(e, memory));
				try { memory -> define(e -> name -> lexeme, procedure); }
				catch (Environment::VariableRedefinitionException & r) {
					if (procedure) delete procedure;
					throw Program::Error(
						currentUnit,
						"Procedure redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!",
						* e -> name, ErrorCode::evl
					);
				}
			}
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitRepeatUntilStatement(RepeatUntilStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> expression);
			if (!(expression -> isBoolean())) {
				throw Program::Error(
					currentUnit,
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> untilToken, ErrorCode::evl
				);
			}
			Boolean condition = expression -> getBoolValue();
			executeStatement(e -> body);
			if (broken) { broken = false; return; }
			while (!condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				expression = evaluate(e -> expression);
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
		if (e -> e) {
			try { expression = evaluate(e -> e); }
			catch (Exception & exc) { throw; }
		}
		throw Return(expression, new Token(* e -> returnToken));
	}
	void Interpreter::visitSwapStatement(SwapStatement * e) {
		Object * l = nullptr;
		Object * r = nullptr;
		try {
			l = memory -> getReference(e -> l -> lexeme);
			r = memory -> getReference(e -> r -> lexeme);
			if (l -> type == r -> type) {
				memory -> setReference(e -> l -> lexeme, r);
				memory -> setReference(e -> r -> lexeme, l);
				return;
			}
			throw Program::Error(
				currentUnit,
				"Swap failed over incompatible types '" + 
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!",
				* (e -> swap), ErrorCode::evl
			);
		} catch (Environment::VariableNotFoundException & exc) {
			Token errorToken = * (l ? e -> l : e -> r);
			throw Program::Error(
				currentUnit,
				"Unexpected identifier '" + errorToken.lexeme + "'!",
				errorToken, ErrorCode::evl
			);
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitUntilStatement(UntilStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> expression);
			if (!(expression -> isBoolean())) {
				throw Program::Error(
					currentUnit,
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> untilToken, ErrorCode::evl
				);
			}
			Boolean condition = expression -> getBoolValue();
			while (!condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				expression = evaluate(e -> expression);
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
		Object * definition = nullptr;
		Object * instance = nullptr;
		CallProtocol * constructor = nullptr;
		if (e -> object) {
			// This is the class definition:
			try { definition = memory -> getReference(e -> object -> lexeme); }
			catch (Environment::VariableNotFoundException & oex) {
				throw Program::Error(
					currentUnit,
					"Object definition not found!",
					* e -> object, ErrorCode::evl
				);
			}
			// Empty instance from class definition:
			Class * definedClass = (Class *) definition -> value;
			instance = new Object(
				BasicType::InstanceType,
				new Instance(definedClass)
			);
			// Call of eventual custom constructor:
			if (e -> initialiser) {
				// This returns a new instance with the specified definition.
				try { expression = evaluate(e -> initialiser); }
				catch (Exception & exc) {
					delete instance;
					throw;
				}
				// This should work if the definitions are compatible.
				try { CPU -> applyAssignment(e -> equal, instance, expression); }
				catch (Exception & exc) {
					delete instance;
					delete expression;
					throw;
				}
				// There's no need to delete the expression since the original
				// object has already been sent to lost and found.
			} else if (definedClass -> arity() > 0) {
				// TODO: Also check if it has 0 arity and it's
				//       defined, we should call it automatically.
				//       (It probably already happens but IDK)
				throw Program::Error(
					currentUnit,
					"Object instantiation requires constructor call when it's defined!",
					* e -> name, ErrorCode::evl
				);
			}
			expression = instance;
		} else {
			// Normal variables:
			if (e -> initialiser) {
				Object * o = new Object(e -> type);
				try {
					// We use an assignment so it can
					// cast an object into the other.
					expression = evaluate(e -> initialiser);
					CPU -> applyAssignment(e -> equal, o, expression);
				} catch (Exception & exc) {
					if (expression) delete expression;
					delete o;
					throw;
				}
				delete expression;
				expression = o;
			} else expression = new Object(e -> type);
		}
		try {
			if (instanceDefinition) {
				instanceDefinition -> defineDynamic(
					e -> name -> lexeme, currentModifier, expression
				);
			} else if (classDefinition) {
				classDefinition -> defineStatic(
					e -> name -> lexeme, currentModifier, expression
				);
			} else {
				memory -> define(e -> name -> lexeme, expression);
			}
		} catch (Environment::VariableRedefinitionException & r) {
			if (expression) delete expression;
			throw Program::Error(
				currentUnit,
				"Variable redefinition! The identifier '" +
				e -> name -> lexeme + "' was already declared with type '" +
				r.getType() + "' in current scope!",
				* e -> name, ErrorCode::evl
			);
		}
	}
	void Interpreter::visitVectorStatement(VectorStatement * e) {
		Object * expression = nullptr;
		if (e -> initialiser) {
			Object * o = new Object(
				BasicType::VectorType,
				// A ket starts with '|':
				new Vector(e -> vector -> lexeme[0] == '|' ? 0 : 1)
			);
			try {
				expression = evaluate(e -> initialiser);
				CPU -> applyVectorAssignment(e -> equal, o, expression);
			} catch (Exception & exc) {
				if (expression) delete expression;
				delete o;
				throw;
			}
			delete expression; expression = o;
		} else {
			expression = new Object(
				BasicType::VectorType,
				// A ket starts with '|':
				new Vector(e -> vector -> lexeme[0] == '|' ? 0 : 1)
			);
		}
		try {
			if (instanceDefinition) {
				instanceDefinition -> defineDynamic(
					e -> name, currentModifier, expression
				);
			} if (classDefinition) {
				classDefinition -> defineStatic(
					e -> name, currentModifier, expression
				);
			} else {
				memory -> define(e -> name, expression);
			}
		} catch (Environment::VariableRedefinitionException & r) {
			throw Program::Error(
				currentUnit,
				"Vector redefinition! The identifier '" +
				e -> name + "' was already declared with type '" +
				r.getType() + "'!",
				* e -> vector, ErrorCode::evl
			);
		}
	}
	void Interpreter::visitWhileStatement(WhileStatement * e) {
		// IMPROVE: try catch.
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> expression);
			if (!(expression -> isBoolean())) {
				throw Program::Error(
					currentUnit,
					"Unsupported evaluation of non logical expression in iteration statement!",
					* e -> whileToken, ErrorCode::evl
				);
			}
			Boolean condition = expression -> getBoolValue();
			while (condition) {
				executeStatement(e -> body);
				if (broken) { broken = false; break; }
				delete expression; expression = nullptr;
				expression = evaluate(e -> expression);
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
		memory = environment;
		for (Statement * statement : * statements) {
			try { executeStatement(statement); }
			catch (Exception & exc) {
				memory = previous;
				delete environment;
				throw;
			}
			if (broken || continued) {
				continued = false;
				break;
			}
		}
		memory = previous;
		delete environment;
	}
	void Interpreter::executeFunction(BlockStatement * block, Environment * environment) {
		try { executeBlock(block -> statements, environment); }
		catch (Exception & exc) { throw; }
	}

	void Interpreter::evaluate(SyntaxTree * syntaxTree) {
		if (!syntaxTree) return;
		globals = new Environment();
		memory = globals;
		Library::defineVirual(globals);
		if (syntaxTree -> libraries) {
			for (String & lib : * (syntaxTree -> libraries)) {
				Library::define(lib, globals);
			}
		}
		for (Statement * statement : * syntaxTree -> statements) {
			try { executeStatement(statement); }
			catch (Program::Error & e) { throw; }
		}
		delete globals;
		memory = nullptr;
		globals = nullptr;
	}

}

#endif
