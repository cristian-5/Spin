
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

#include "../Aliases/Prototypes/Manager.hpp"
#include "../Aliases/Prototypes/Libraries.hpp"
#include "../Aliases/Prototypes/Environment.hpp"
#include "../Aliases/Prototypes/Routines.hpp"
#include "../Aliases/Prototypes/Array.hpp"
#include "../Aliases/Prototypes/Class.hpp"
#include "../Aliases/Prototypes/Vector.hpp"

namespace Spin {

	/* Exceptions */

	InterpreterErrorException::InterpreterErrorException(String m, UInt64 l, String n): message(m), line(l), fileName(n) { }
	const String & InterpreterErrorException::getMessage() const {
		return message;
	}
	const UInt64 & InterpreterErrorException::getLine() const {
		return line;
	}
	const String & InterpreterErrorException::getFileName() const {
		return fileName;
	}

	InterpreterReturn::InterpreterReturn(Object * v, Token * t): value(v), returnToken(t) { }
	InterpreterReturn::~InterpreterReturn() {
		if (returnToken) delete returnToken;
	}
	Object * InterpreterReturn::getReturnValue() const {
		return value;
	}
	Token * InterpreterReturn::getReturnToken() const {
		return returnToken;
	}

	/* Expressions */

	Object * Interpreter::visitAssignmentExpression(Assignment * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> value);
			Object * obj = nullptr;
			try {
				obj = memory -> getReference(e -> name -> lexeme);
			} catch (Environment::VariableNotFoundException & r) {
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
			l = evaluate(e -> l);
			r = evaluate(e -> r);
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
		} catch (Environment::VariableNotFoundException & exc) {
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
			callee = evaluate(e -> callee);
			for (Expression * a : * e -> arguments) {
				Object * evaluation = evaluate(a);
				arguments.push_back(evaluation -> copy());
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
			Bool lost = false;
			if (function -> isInstanceOf<Class>()) {
				if (!e -> isConstructor) {
					throw EvaluationError(
						"Constructor call is missing 'new' operator!",
						* e -> parenthesis
					);
				}
				lost = true;
				// Needs a reference instead of a copy of the definition:
				Object * backUpCallee = callee;
				callee = memory -> getReference(
					((Class *) callee -> value) -> name
				);
				delete backUpCallee;
				function = (CallProtocol *)(callee -> value);
			} else if (e -> isConstructor) {
				throw EvaluationError(
					"Operator 'new' doesn't support operands of type 'Routine'!",
					* e -> parenthesis
				);
			}
			Object * result = function -> call(arguments, e -> parenthesis);
			if (lost) memory -> lose(result);
			return result;
		} catch (Exception & exc) {
			// There's no need to delete the arguments since
			// they always get deleted in the function call.
			if (callee) delete callee;
			throw;
		}
	}
	Object * Interpreter::visitComparisonExpression(Comparison * e) {
		Object * l = nullptr;
		Object * r = nullptr;
		Object * result = nullptr;
		try {
			l = evaluate(e -> l);
			r = evaluate(e -> r);
			result = CPU -> applyComparisonOperator(e -> o, l, r);
			delete r; delete l; return result;
		} catch (Exception & exc) {
			if (l) delete l;
			if (r) delete r;
			if (result) delete result;
			throw;
		}
	}
	Object * Interpreter::visitDynamicGetExpression(DynamicGet * e) {
		Object * object = nullptr;
		try {
			object = evaluate(e -> object);
			if (object -> type != BasicType::InstanceType) {
				throw EvaluationError(
					"The resolved object is not an instance and does not contain properties!",
					* e -> name
				);
			}
			try {
				Object * value = nullptr;
				// Check if we are inside a method:
				if (e -> selfReference) {
					value = ((Instance *) object -> value) -> getInnerValue(
						e -> name -> lexeme
					);
				} else {
					value = ((Instance *) object -> value) -> getValue(
						e -> name -> lexeme
					);
				}
				if (value -> type == BasicType::RoutineType) {
					// Bind self to the function:
					((CallProtocol *) value -> value) -> self = object;
				} else delete object;
				return value;
			} catch (Environment::VariableNotFoundException & u) {
				throw EvaluationError(
					"The resolved object does not contain any attribute, field or method named '" +
					e -> name -> lexeme + "'!", * e -> name
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
				throw EvaluationError(
					"The resolved object is not an instance and does not contain properties!",
					* e -> name
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
				throw EvaluationError(
					"The resolved object does not contain any attribute, field or method named '" +
					e -> name -> lexeme + "'!", * e -> name
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
		try {
			return memory -> getValue(e -> name -> lexeme);
		} catch (Environment::VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected identifier '" +
				e -> name -> lexeme + "'!", * e -> name
			);
		}
	}
	Object * Interpreter::visitInnerExpression(Inner * e) {
		Object * b = nullptr;
		Object * k = nullptr;
		try {
			b = memory -> getValue(e -> bra);
		} catch (Environment::VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected Vector identifier '" +
				e -> bra + "'!", * e -> inner
			);
		}
		try {
			k = memory -> getValue(e -> ket);
		} catch (Environment::VariableNotFoundException & exc) {
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
		} catch (Environment::VariableNotFoundException & exc) {
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
		try {
			if (!(e -> object)) {
				e -> object = Object::fromLiteral(e -> token);
			}
			return e -> object -> copy();
		} catch (Exception & exc) { throw; }
	}
	Object * Interpreter::visitLogicalExpression(Logical * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> l);
			if (!(expression -> isBool())) {
				throw EvaluationError(
					"Invalid Bool expression found on left side of circuit operator '||'!",
					* e -> o
				);
			}
			if ((e -> o -> type) == TokenType::OR) {
				if (expression -> getBoolValue()) return expression;
			} else if (!(expression -> getBoolValue())) return expression;
			return evaluate(e -> r);
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	Object * Interpreter::visitMutableExpression(Mutable * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> value);
			Object * obj = nullptr;
			try {
				obj = memory -> getReference(e -> name -> lexeme);
			} catch (Environment::VariableNotFoundException & r) {
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
	Object * Interpreter::visitSelfExpression(Self * e) {
		try {
			return memory -> getValue("self");
		} catch (Environment::VariableNotFoundException & exc) {
			throw EvaluationError(
				"Unexpected use of 'self' outside of allowed context!",
				* e -> keyword
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
				throw EvaluationError(
					"The resolved object is not a definition that provides a static context!",
					* e -> name
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
				throw EvaluationError(
					"The resolved object does not contain any static attribute, field or method named '" +
					e -> name -> lexeme + "'!", * e -> name
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
				throw EvaluationError(
					"The resolved object is not a definition that provides a static context!",
					* e -> name
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
				throw EvaluationError(
					"The resolved object does not contain any attribute, field or method named '" +
					e -> name -> lexeme + "'!", * e -> name
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
		Object * item = nullptr;
		Object * expression = nullptr;
		Object * result = nullptr;
		try {
			item = evaluate(e -> item);
			if (!(item -> isSubscriptable()) ||
				!(item -> value)) {
				throw EvaluationError(
					"The selected object doesn't support subscription!",
					* e -> bracket
				);
			}
			expression = evaluate(e -> expression);
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
	Object * Interpreter::visitUnaryExpression(Unary * e) {
		Object * expression = nullptr;
		Object * result = nullptr;
		try {
			expression = evaluate(e -> r);
			result = CPU -> applyUnaryOperator(e -> o, expression);
			delete expression; return result;
		} catch (Exception & exc) {
			if (expression) delete expression;
			if (result) delete result;
			throw;
		}
	}

	Object * Interpreter::evaluate(Expression * e) {
		try { return e -> accept(this); }
		catch (Exception & exc) { throw; }
	}

	/* Statements */

	void Interpreter::visitAttributeStatement(AttributeStatement * e) {
		try {
			currentModifier = e -> modifier;
			e -> field -> accept(this);
		} catch (Exception & exc) { throw; }
	}
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
			classDefinition = new Class(e -> name -> lexeme, e -> dynamicAttributes);
			if (e -> atCreate) {
				classDefinition -> atCreate = new Procedure(
					e -> atCreate, new Environment()
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
				throw EvaluationError(
					"Object redefinition! The object '" +
					e -> name -> lexeme + "' was already declared with type '" +
					r.getType() + "'!", * e -> name
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
		try {
			memory -> forget(e -> name -> lexeme);
		} catch (Environment::VariableNotFoundException & exc) {
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
			expression = evaluate(e -> expression);
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
				expression = evaluate(e -> expression);
				condition = expression -> getBoolValue();
			}
			if (expression) delete expression;
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
		}
	}
	void Interpreter::visitExpressionStatement(ExpressionStatement * e) {
		try { evaluate(e -> e); }
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
			expression = evaluate(e -> expression);
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
					new Function(e, new Environment())
				);
				try {
					instanceDefinition -> defineDynamic(
						e -> name -> lexeme, currentModifier, function
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (function) delete function;
					throw EvaluationError(
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!", * e -> name
					);
				}
			} else if (classDefinition) {
				Object * function = new Object(
					BasicType::RoutineType,
					new Function(e, new Environment())
				);
				try {
					classDefinition -> defineStatic(
						e -> name -> lexeme, currentModifier, function
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (function) delete function;
					throw EvaluationError(
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!", * e -> name
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
					throw EvaluationError(
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!", * e -> name
					);
				}
			}
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitIfStatement(IfStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> expression);
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
	void Interpreter::visitProcedureStatement(ProcedureStatement * e) {
		try {
			if (instanceDefinition) {
				Object * procedure = new Object(
					BasicType::RoutineType,
					new Procedure(e, new Environment())
				);
				try {
					instanceDefinition -> defineDynamic(
						e -> name -> lexeme, currentModifier, procedure
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (procedure) delete procedure;
					throw EvaluationError(
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!", * e -> name
					);
				}
			} else if (classDefinition) {
				Object * procedure = new Object(
					BasicType::RoutineType,
					new Procedure(e, new Environment())
				);
				try {
					classDefinition -> defineStatic(
						e -> name -> lexeme, currentModifier, procedure
					);
				} catch (Environment::VariableRedefinitionException & r) {
					if (procedure) delete procedure;
					throw EvaluationError(
						"Function redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!", * e -> name
					);
				}
			} else {
				Object * procedure = new Object(BasicType::RoutineType, new Procedure(e, memory));
				try { memory -> define(e -> name -> lexeme, procedure); }
				catch (Environment::VariableRedefinitionException & r) {
					if (procedure) delete procedure;
					throw EvaluationError(
						"Procedure redefinition! The object '" +
						e -> name -> lexeme + "' was already declared within the current scope!", * e -> name
					);
				}
			}
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitRepeatUntilStatement(RepeatUntilStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> expression);
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
		try {
			if (e -> e) expression = evaluate(e -> e);
			throw InterpreterReturn(expression, new Token(* e -> returnToken));
		} catch (Exception & exc) { throw; }
	}
	void Interpreter::visitUntilStatement(UntilStatement * e) {
		Object * expression = nullptr;
		try {
			expression = evaluate(e -> expression);
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
		try {
			if (e -> object) {
				// This is the class definition:
				try { definition = memory -> getReference(e -> object -> lexeme); }
				catch (Environment::VariableNotFoundException & oex) {
					throw EvaluationError(
						"Object definition not found!", * e -> object
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
					expression = evaluate(e -> initialiser);
					// This should work if the definitions are compatible.
					CPU -> applyAssignment(e -> equal, instance, expression);
					// There's no need to delete the expression since the original
					// object has already been sent to lost and found.
				} else if (definedClass -> arity() > 0) {
					throw EvaluationError(
						"Object instantiation requires constructor call when it's defined!",
						* e -> name
					);
				}
				expression = instance;
			} else {
				// Normal variables:
				if (e -> initialiser) {
					expression = evaluate(e -> initialiser);
					Object * o = new Object(e -> type);
					CPU -> applyAssignment(e -> equal, o, expression);
					delete expression; expression = o;
				} else expression = new Object(e -> type);
			}
		} catch (Exception & exc) {
			if (expression) delete expression;
			if (instance) delete instance;
			throw;
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
			throw EvaluationError(
				"Variable redefinition! The identifier '" +
				e -> name -> lexeme + "' was already declared with type '" +
				r.getType() + "' in current scope!", * e -> name
			);
		}
	}
	void Interpreter::visitVectorStatement(VectorStatement * e) {
		Object * expression = nullptr;
		try {
			if (e -> initialiser) {
				expression = evaluate(e -> initialiser);
				Object * o = new Object(
					BasicType::VectorType,
					// A ket starts with '|':
					new Vector(e -> vector -> lexeme[0] == '|' ? 0 : 1)
				);
				CPU -> applyVectorAssignment(e -> equal, o, expression);
				delete expression; expression = o;
			} else {
				expression = new Object(
					BasicType::VectorType,
					// A ket starts with '|':
					new Vector(e -> vector -> lexeme[0] == '|' ? 0 : 1)
				);
			}
		} catch (Exception & exc) {
			if (expression) delete expression;
			throw;
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
			expression = evaluate(e -> expression);
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

	void Interpreter::evaluate(SyntaxTree * syntaxTree) {
		if (!syntaxTree) return;
		globals = new Environment();
		memory = globals;
		if (syntaxTree -> consoleLibrary) Console::defineLibrary(globals);
		if (syntaxTree -> mathsLibrary) Maths::defineLibrary(globals);
		if (syntaxTree -> kronosLibrary) Kronos::defineLibrary(globals);
		try {
			for (Statement * statement : * syntaxTree -> statements) {
				executeStatement(statement);
			}
		} catch (EvaluationError & e) {
			const SizeType cursor = e.getToken().position;
			SizeType line = Manager::getLine(fileContents, cursor);
			if (!fileName) fileName = new String("Unknown File");
			throw InterpreterErrorException(
				e.getMessage(), line, * fileName
			);
		}
		delete memory;
		memory = nullptr;
		globals = nullptr;
	}

}

#endif
