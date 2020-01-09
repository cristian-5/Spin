
/*!
 *
 *    + --------------------------------------- +
 *    |  Function.hpp                           |
 *    |                                         |
 *    |              Function Type              |
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

#ifndef SPINFUNCTION
#define SPINFUNCTION

namespace Spin {

	Function::Function(FunctionStatement * d, Environment * c) {
		declaration = d;
		closure = c;
	}
	Object * Function::call(Interpreter * i, Array<Object *> a, Token * c) {
		Environment * environment = new Environment(closure);
		SizeType j = 0;
		for (Parameter * param : * declaration -> params) {
			if ((param -> type) != (a[j] -> type)) {
				throw EvaluationError(
					"Call of " + stringValue() + " doesn't match the predefined parameters!",
					* (param -> tokenType)
				);
			} else if (param -> type == BasicType::ClassType) {
				if ((param -> tokenType -> lexeme) !=
					(a[j] -> getObjectName())) {
					throw EvaluationError(
						"Call of " + stringValue() + " doesn't match the predefined parameters!",
						* (param -> tokenType)
					);
				}
			}
			environment -> define(param -> name -> lexeme, a[j]);
			j += 1;                                        
		}
		try {
			i -> executeFunction(declaration -> body, environment);
		} catch (InterpreterReturn & ret) {
			Object * value = ret.getReturnValue();
			String type = declaration -> returnType -> tokenType -> lexeme;
			if (value && type == value -> getObjectName()) return value;
			throw EvaluationError(
				"Function " + stringValue() + " reached bottom of body without returning a valid '" +
				(declaration -> returnType -> tokenType -> lexeme) + "' value!", * ret.getReturnToken()
			);
		}
		throw EvaluationError(
			"Function " + stringValue() + " reached bottom of body without returning a valid '" +
			(declaration -> returnType -> tokenType -> lexeme) + "' value!", * c
		);
	}
	String Function::stringValue() const {
		return "<func " + (declaration -> name -> lexeme) + ">";
	}
	UInt32 Function::arity() const { return declaration -> params -> size(); }
	CallProtocol * Function::copy() const {
		return new Function(* this);
	}

	Procedure::Procedure(ProcedureStatement * d, Environment * c) {
		declaration = d;
		closure = c;
	}
	Object * Procedure::call(Interpreter * i, Array<Object *> a, Token * c) {
		Environment * environment = new Environment(closure);
		SizeType j = 0;
		for (Parameter * param : * declaration -> params) {
			if ((param -> type) != (a[j] -> type)) {
				throw EvaluationError(
					"Call of " + stringValue() + " doesn't match the predefined parameters!",
					* (param -> tokenType)
				);
			} else if (param -> type == BasicType::ClassType) {
				if ((param -> tokenType -> lexeme) !=
					(a[j] -> getObjectName())) {
					throw EvaluationError(
						"Call of " + stringValue() + " doesn't match the predefined parameters!",
						* (param -> tokenType)
					);
				}
			}
			environment -> define(param -> name -> lexeme, a[j]);
			j += 1;                                        
		}
		try {
			i -> executeFunction(declaration -> body, environment);
		} catch (InterpreterReturn & ret) {
			if (ret.getReturnValue() -> value) {
				throw EvaluationError(
					"Procedure " + stringValue() + " reached invalid return statement with value of type '" +
					ret.getReturnValue() -> getObjectName() + "' value!", * ret.getReturnToken()
				);
			}
		}
		return nullptr;
	}
	String Procedure::stringValue() const {
		return "<proc " + (declaration -> name -> lexeme) + ">";
	}
	UInt32 Procedure::arity() const { return declaration -> params -> size(); }
	CallProtocol * Procedure::copy() const {
		return new Procedure(* this);
	}

	NativeFunction::NativeFunction(NativeLambda l, Array<Parameter *> * p, String n) {
		lambda = l; params = p; name = n;
	}
	Object * NativeFunction::call(Interpreter * i, Array<Object *> a, Token * c) {
		SizeType j = 0;
		for (Parameter * param : * params) {
			if (!param) { j += 1; continue; }
			if ((param -> type) != (a[j] -> type)) {
				throw EvaluationError(
					"Call of " + stringValue() + " doesn't match the predefined parameters!",
					* (param -> tokenType)
				);
			} else if (param -> type == BasicType::ClassType) {
				if ((param -> tokenType -> lexeme) !=
					(a[j] -> getObjectName())) {
					throw EvaluationError(
						"Call of " + stringValue() + " doesn't match the predefined parameters!",
						* (param -> tokenType)
					);
				}
			}
			j += 1;                                        
		}
		try { return lambda(i, a, c); }
		catch (Exception & e) { throw; }
	}
	String NativeFunction::stringValue() const { return name; }
	UInt32 NativeFunction::arity() const { return params -> size(); }
	CallProtocol * NativeFunction::copy() const {
		return new NativeFunction(* this);
	}

}

#endif
