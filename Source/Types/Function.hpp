
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
		Environment * parameters = new Environment(closure);
		SizeType j = 0;
		for (Parameter * param : * declaration -> params) {
			if ((param -> type) != (a[j] -> type)) {
				delete parameters;
				throw EvaluationError(
					"Call of " + stringValue() + " doesn't match the predefined parameters!",
					* (param -> tokenType)
				);
			} else if (param -> type == BasicType::ClassType) {
				if ((param -> tokenType -> lexeme) !=
					(a[j] -> getObjectName())) {
					delete parameters;
					throw EvaluationError(
						"Call of " + stringValue() + " doesn't match the predefined parameters!",
						* (param -> tokenType)
					);
				}
			}
			parameters -> define(param -> name -> lexeme, a[j]);
			j += 1;
		}
		Environment * environment = new Environment(parameters);
		try {
			i -> executeFunction(declaration -> body, environment);
		} catch (InterpreterReturn & ret) {
			Object * value = ret.getReturnValue();
			String type = declaration -> returnType -> tokenType -> lexeme;
			// Safely delete function parameters:
			delete parameters;
			parameters = nullptr;
			if (value && type == value -> getObjectName()) return value;
			throw EvaluationError(
				"Function " + stringValue() + " reached bottom of body without returning a valid '" +
				(declaration -> returnType -> tokenType -> lexeme) + "' value!", * ret.getReturnToken()
			);
		}
		// Safely delete function parameters:
		if (parameters) delete parameters;
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
		Environment * parameters = new Environment(closure);
		SizeType j = 0;
		for (Parameter * param : * declaration -> params) {
			if ((param -> type) != (a[j] -> type)) {
				delete parameters;
				throw EvaluationError(
					"Call of " + stringValue() + " doesn't match the predefined parameters!",
					* (param -> tokenType)
				);
			} else if (param -> type == BasicType::ClassType) {
				if ((param -> tokenType -> lexeme) !=
					(a[j] -> getObjectName())) {
					delete parameters;
					throw EvaluationError(
						"Call of " + stringValue() + " doesn't match the predefined parameters!",
						* (param -> tokenType)
					);
				}
			}
			parameters -> define(param -> name -> lexeme, a[j]);
			j += 1;                                        
		}
		Environment * environment = new Environment(parameters);
		try {
			i -> executeFunction(declaration -> body, environment);
		} catch (InterpreterReturn & ret) {
			if (ret.getReturnValue() -> value) {
				// Safely delete function parameters:
				delete parameters;
				parameters = nullptr;
				throw EvaluationError(
					"Procedure " + stringValue() + " reached invalid return statement with value of type '" +
					ret.getReturnValue() -> getObjectName() + "' value!", * ret.getReturnToken()
				);
			}
		}
		// Safely delete function parameters:
		if (parameters) delete parameters;
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
				deallocate(a);
				throw EvaluationError(
					"Call of " + stringValue() + " doesn't match the predefined parameters!",
					* (param -> tokenType)
				);
			} else if (param -> type == BasicType::ClassType) {
				if ((param -> tokenType -> lexeme) != (a[j] -> getObjectName())) {
					deallocate(a);
					throw EvaluationError(
						"Call of " + stringValue() + " doesn't match the predefined parameters!",
						* (param -> tokenType)
					);
				}
			}
			j += 1;                                        
		}
		Object * result = nullptr;
		try {
			result = lambda(i, a, c);
		} catch (Exception & e) {
			deallocate(a);
			throw;
		}
		deallocate(a);
		return result;
	}
	void NativeFunction::deallocate(Array<Object *> & parameters) {
		for (Object * parameter : parameters) delete parameter;
	}
	String NativeFunction::stringValue() const { return name; }
	UInt32 NativeFunction::arity() const { return params -> size(); }
	CallProtocol * NativeFunction::copy() const {
		return new NativeFunction(* this);
	}

	NativeProcedure::NativeProcedure(NativeLambda l, Array<Parameter *> * p, String n) {
		lambda = l; params = p; name = n;
	}
	Object * NativeProcedure::call(Interpreter * i, Array<Object *> a, Token * c) {
		SizeType j = 0;
		for (Parameter * param : * params) {
			if (!param) { j += 1; continue; }
			if ((param -> type) != (a[j] -> type)) {
				deallocate(a);
				throw EvaluationError(
					"Call of " + stringValue() + " doesn't match the predefined parameters!",
					* (param -> tokenType)
				);
			} else if (param -> type == BasicType::ClassType) {
				if ((param -> tokenType -> lexeme) != (a[j] -> getObjectName())) {
					deallocate(a);
					throw EvaluationError(
						"Call of " + stringValue() + " doesn't match the predefined parameters!",
						* (param -> tokenType)
					);
				}
			}
			j += 1;                                        
		}
		try {
			lambda(i, a, c);
		} catch (Exception & e) {
			deallocate(a);
			throw;
		}
		deallocate(a);
		return nullptr;
	}
	void NativeProcedure::deallocate(Array<Object *> & parameters) {
		for (Object * parameter : parameters) delete parameter;
	}
	String NativeProcedure::stringValue() const { return name; }
	UInt32 NativeProcedure::arity() const { return params -> size(); }
	CallProtocol * NativeProcedure::copy() const {
		return new NativeProcedure(* this);
	}

}

#endif
