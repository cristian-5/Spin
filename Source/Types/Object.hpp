
/*!
 *
 *    + --------------------------------------- +
 *    |  Object.hpp                             |
 *    |                                         |
 *    |               Stack Object              |
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

#ifndef STACKOBJECT
#define STACKOBJECT

#include "../Aliases/Aliases.hpp"
#include "../Token/Token.hpp"

#include "Complex.hpp"
#include "Colour.hpp"

namespace Stack {

	enum BasicType {

		BoolType,
		CharacterType,

		ByteType,
		
		Int64Type,

		RealType,

		ColourType,

		ImaginaryType,
		ComplexType,

		StringType,

		ArrayListType,

		VectorType,

		ClassType,
		StructureType,
		ExceptionType,

		UnknownType

	};

	class Object {

		public:

		void * value = nullptr;

		BasicType type = BasicType::UnknownType;

		Object() { }

		Object(BasicType t) {
			// TODO: Handle all types.
			type = t;
			switch (type) {
				case BasicType::BoolType: value = new Bool(false); return;
				case BasicType::CharacterType: value = new Character(0); return;
				case BasicType::ByteType: value = new UInt8(0); return;
				case BasicType::Int64Type: value = new Int64(0); return;
				case BasicType::ColourType: value = new Colour(); return;
				case BasicType::RealType: value = new Real(0.0); return;
				case BasicType::ImaginaryType: value = new Real(0.0); return;
				case BasicType::ComplexType: value = new Complex(); return;
				case BasicType::StringType: value = new String(""); return;
				case BasicType::ArrayListType: return;
				case BasicType::VectorType: return;
				case BasicType::ClassType: return;
				case BasicType::StructureType: return;
				case BasicType::ExceptionType: return;
				case BasicType::UnknownType: default: return;
			}
		}

		Object(BasicType t, void * v) {
			type = t; value = v;
		}

		~Object() {
			// TODO: Handle all deallocations.
			if (!value) return;
			switch (type) {
				case BasicType::BoolType: delete (Bool *) value; return;
				case BasicType::CharacterType: delete (Character *) value; return;
				case BasicType::ByteType: delete (UInt8 *) value; return;
				case BasicType::Int64Type: delete (Int64 *) value; return;
				case BasicType::ColourType: delete (Colour *) value; return;
				case BasicType::RealType: delete (Real *) value; return;
				case BasicType::ImaginaryType: delete (Real *) value; return;
				case BasicType::ComplexType: delete (Complex *) value; return;
				case BasicType::StringType: delete (String *) value; return;
				case BasicType::ArrayListType: return;
				case BasicType::VectorType: return;
				case BasicType::ClassType: return;
				case BasicType::StructureType: return;
				case BasicType::ExceptionType: return;
				case BasicType::UnknownType: default: return;
			}
		}

		inline Bool isByte() const { return type == BasicType::ByteType; }
		inline Bool isInteger() const { return type == BasicType::Int64Type; }
		inline Bool isReal() const { return type == BasicType::RealType; }

		inline Bool isComplexType() const {
			return type == BasicType::ComplexType ||
				   type == BasicType::ImaginaryType;
		}			

		inline Bool isNumericType() const {
			return isInteger() || isReal() || isComplexType();
		}

		inline Bool isBool() const { return type == BasicType::BoolType; }
		inline Bool isString() const { return type == BasicType::StringType; }
		inline Bool isColour() const { return type == BasicType::ColourType; }
		inline Bool isCharacter() const { return type == BasicType::CharacterType; }

		String getObjectName() const {
			switch (type) {
				case BasicType::BoolType: return "Bool";
				case BasicType::CharacterType: return "Character";
				case BasicType::ByteType: return "Byte";
				case BasicType::Int64Type: return "Integer";
				case BasicType::ColourType: return "Colour";
				case BasicType::RealType: return "Real";
				case BasicType::ImaginaryType: return "Imaginary";
				case BasicType::ComplexType: return "Complex";
				case BasicType::StringType: return "String";
				case BasicType::ArrayListType: return "Array";
				case BasicType::VectorType: return "Vector";
				case BasicType::ClassType: {
					return "Class";
					// TODO: Ask the class for its type.
				}
				case BasicType::StructureType: return "Structure";
				case BasicType::ExceptionType: return "Exception";
				case BasicType::UnknownType: default: return "Unknown";
			}
		}

		Object * copy() const {
			Object * copy = new Object();
			copy -> type = type;
			switch (type) {
				case BasicType::BoolType: copy -> value = new Bool(* ((Bool *) value)); break;
				case BasicType::CharacterType: copy -> value = new Character(* ((Character *) value)); break;
				case BasicType::ByteType: copy -> value = new UInt8(* ((UInt8 *) value)); break;
				case BasicType::Int64Type: copy -> value = new Int64(* ((Int64 *) value)); break;
				case BasicType::ColourType: copy -> value = new Colour(* ((Colour *) value)); break;
				case BasicType::RealType: copy -> value = new Real(* ((Real *) value)); break;
				case BasicType::ImaginaryType: copy -> value = new Real(* ((Real *) value)); break;
				case BasicType::ComplexType: copy -> value = new Complex(* ((Complex *) value)); break;
				case BasicType::StringType: copy -> value = new String(* ((String *) value)); break;
				case BasicType::ArrayListType: /* TODO: Ask the class for its copy. */ break;
				case BasicType::VectorType: /* TODO: Ask the class for its copy. */ break;
				case BasicType::ClassType: /* TODO: Ask the class for its copy. */ break;
				case BasicType::StructureType: /* TODO: Ask the class for its copy. */ break;
				case BasicType::ExceptionType: /* TODO: Ask the class for its copy. */ break;
				case BasicType::UnknownType: default: break;
			}
			return copy;
		}

		String getObjectStringValue() const {
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
				case BasicType::Int64Type: {
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
				case BasicType::ArrayListType: {
					// TODO: fix list value.
					return "SomeList";
				}
				case BasicType::VectorType: {
					// TODO: fix vector value.
					return "SomeVector";
				}
				case BasicType::ClassType: {
					if (!value) return "null";
					// TODO: fix class value.
					return "SomeClass";
				}
				case BasicType::StructureType: {
					// TODO: fix structure value.
					return "SomeStructure";
				}
				case BasicType::ExceptionType: {
					// TODO: fix exception value.
					return "SomeException";
				}
				case BasicType::UnknownType: default: {
					return "Unknown"; break;
				}
			}
		}

		Bool isUnknown() const { return type == BasicType::UnknownType; }

		Bool getBoolValue() const {
			if (isBool()) return *((Bool *)value);
			else return false;
		}

	};

}

#endif
