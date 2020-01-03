
/*!
 *
 *    + --------------------------------------- +
 *    |  Object.hpp                             |
 *    |                                         |
 *    |               Object Type               |
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

#ifndef SPINOBJECT
#define SPINOBJECT

namespace Spin {
	
	Object::Object(BasicType t) {
		type = t;
		switch (type) {
			case BasicType::BoolType: value = new Bool(false); return;
			case BasicType::CharacterType: value = new Character(0); return;
			case BasicType::ByteType: value = new UInt8(0); return;
			case BasicType::IntegerType: value = new Int64(0); return;
			case BasicType::ColourType: value = new Colour(); return;
			case BasicType::RealType: value = new Real(0.0); return;
			case BasicType::ImaginaryType: value = new Real(0.0); return;
			case BasicType::ComplexType: value = new Complex(); return;
			case BasicType::StringType: value = new String(); return;
			case BasicType::ArrayType: value = new ArrayList(); return;
			case BasicType::VectorType: value = new Vector(); return;
			case BasicType::InstanceType: value = new Instance(nullptr);
			case BasicType::UnknownType: default: return;
		}
	}
	Object::Object(BasicType t, void * v) {
		type = t; value = v;
	}
	Object::~Object() {
		if (!value) return;
		switch (type) {
			case BasicType::BoolType: delete (Bool *) value; return;
			case BasicType::CharacterType: delete (Character *) value; return;
			case BasicType::ByteType: delete (UInt8 *) value; return;
			case BasicType::IntegerType: delete (Int64 *) value; return;
			case BasicType::ColourType: delete (Colour *) value; return;
			case BasicType::RealType: delete (Real *) value; return;
			case BasicType::ImaginaryType: delete (Real *) value; return;
			case BasicType::ComplexType: delete (Complex *) value; return;
			case BasicType::StringType: delete (String *) value; return;
			case BasicType::ArrayType: delete (ArrayList *) value; return;
			case BasicType::VectorType: delete (Vector *) value; return;
			case BasicType::FunctionType: delete (CallProtocol *) value; return;
			case BasicType::ClassType: delete (Class *) value; return;
			case BasicType::InstanceType: delete (Instance *) value; return;
			case BasicType::UnknownType: default: return;
		}
	}
	void Object::safeDestroy() {
		if (type == BasicType::InstanceType) {
			((Instance *) value) -> destroy();
		}
		delete this;
	}
	inline Bool Object::isByte() const { return type == BasicType::ByteType; }
	inline Bool Object::isInteger() const { return type == BasicType::IntegerType; }
	inline Bool Object::isReal() const { return type == BasicType::RealType; }
	inline Bool Object::isComplexType() const {
		return type == BasicType::ComplexType ||
			   type == BasicType::ImaginaryType;
	}			
	inline Bool Object::isNumericType() const {
		return isInteger() || isReal() || isComplexType();
	}
	inline Bool Object::isBool() const { return type == BasicType::BoolType; }
	inline Bool Object::isString() const { return type == BasicType::StringType; }
	inline Bool Object::isColour() const { return type == BasicType::ColourType; }
	inline Bool Object::isCharacter() const { return type == BasicType::CharacterType; }
	String Object::getObjectName() const {
		switch (type) {
			case BasicType::BoolType: return "Bool";
			case BasicType::CharacterType: return "Character";
			case BasicType::ByteType: return "Byte";
			case BasicType::IntegerType: return "Integer";
			case BasicType::ColourType: return "Colour";
			case BasicType::RealType: return "Real";
			case BasicType::ImaginaryType: return "Imaginary";
			case BasicType::ComplexType: return "Complex";
			case BasicType::StringType: return "String";
			case BasicType::ArrayType: return "Array";
			case BasicType::VectorType: return "Vector";
			case BasicType::FunctionType: return "Function";
			case BasicType::ClassType: return "Definition";
			case BasicType::InstanceType: return "Instance";
			case BasicType::UnknownType: default: return "Unknown";
		}
	}
	Object * Object::copy() const {
		Object * copy = new Object();
		copy -> type = type;
		switch (type) {
			case BasicType::BoolType: copy -> value = new Bool(* ((Bool *) value)); break;
			case BasicType::CharacterType: copy -> value = new Character(* ((Character *) value)); break;
			case BasicType::ByteType: copy -> value = new UInt8(* ((UInt8 *) value)); break;
			case BasicType::IntegerType: copy -> value = new Int64(* ((Int64 *) value)); break;
			case BasicType::ColourType: copy -> value = new Colour(* ((Colour *) value)); break;
			case BasicType::RealType: copy -> value = new Real(* ((Real *) value)); break;
			case BasicType::ImaginaryType: copy -> value = new Real(* ((Real *) value)); break;
			case BasicType::ComplexType: copy -> value = new Complex(* ((Complex *) value)); break;
			case BasicType::StringType: copy -> value = new String(* ((String *) value)); break;
			case BasicType::ArrayType: copy -> value = ((ArrayList *) value) -> copy(); break;
			case BasicType::VectorType: copy -> value = ((Vector *) value) -> copy(); break;
			case BasicType::FunctionType: copy -> value = ((CallProtocol *) value) -> copy(); break;
			case BasicType::ClassType: copy -> value = ((CallProtocol *) value) -> copy(); break;
			case BasicType::InstanceType: copy -> value = ((Instance *) value) -> copy(); break;
			case BasicType::UnknownType: default: copy -> value = value; break;
		}
		return copy;
	}
	String Object::getObjectStringValue() const {
		switch (type) {
			case BasicType::BoolType: {
				Bool * b = (Bool *) value;
				if (* b) return "true";
				else return "false";
			}
			case BasicType::CharacterType: {
				Character * c = (Character *) value;
				return String(c);
			}
			case BasicType::ByteType: {
				UInt8 * i = (UInt8 *) value;
				return toString((UInt64)(* i));
			}
			case BasicType::IntegerType: {
				Int64 * i = (Int64 *) value;
				return intToString(* i);
			}
			case BasicType::ColourType: {
				Colour * c = (Colour *) value;
				return c -> stringValue();
			}
			case BasicType::RealType: {
				Real * i = (Real *) value;
				return realToString(* i);
			}
			case BasicType::ImaginaryType: {
				Real * i = (Real *) value;
				return realToString(* i) + "i";
			}
			case BasicType::ComplexType: {
				Complex * c = (Complex *) value;
				return c -> stringValue();
			}
			case BasicType::StringType: {
				return * ((String *) value);
			}
			case BasicType::ArrayType: {
				ArrayList * a = (ArrayList *) value;
				return a -> stringValue();
			}
			case BasicType::VectorType: {
				Vector * v = (Vector *) value;
				return v -> stringValue();
			}
			case BasicType::FunctionType: {
				if (!value) return "<void>";
				return ((CallProtocol *) value) -> stringValue();
			}
			case BasicType::ClassType: {
				if (!value) return "<empty>";
				return ((Class *) value) -> stringValue();
			}
			case BasicType::InstanceType: {
				if (!value) return "<empty>";
				return ((Instance *) value) -> stringValue();
			}
			case BasicType::UnknownType: default: {
				return "Unknown"; break;
			}
		}
	}
	Bool Object::isUnknown() const { return type == BasicType::UnknownType; }
	Bool Object::isFunction() const { return type == BasicType::FunctionType; }
	Bool Object::isCallable() const {
		return type == BasicType::FunctionType ||
			   type == BasicType::ClassType;
	}
	Bool Object::isArray() const { return type == BasicType::ArrayType; }
	Bool Object::isSubscriptable() const {
		return type == BasicType::StringType ||
			   type == BasicType::ArrayType;
	}
	Bool Object::getBoolValue() const {
		if (isBool()) return *((Bool *)value);
		else return false;
	}

}

#endif
