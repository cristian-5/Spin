
/*!
 *
 *    + --------------------------------------- +
 *    |  Processor.hpp                          |
 *    |                                         |
 *    |             Stack Processor             |
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

#ifndef STACKPROCESSOR
#define STACKPROCESSOR

#include "../Aliases/Aliases.hpp"

#include "Object.hpp"

namespace Stack {

	class Processor {

		private:
		
		Processor() { }

		public:

		static Object applyUnaryOperand(Token * t, Object * o) {
			switch (t -> type) {
				case TokenType::minus: {
					switch (o -> type) {
						case BasicType::CharacterType: {
							Character * i = (Character *) o -> value;
							i = new Character(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::Int8Type: {
							Int8 * i = (Int8 *) o -> value;
							i = new Int8(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::Int16Type: {
							Int16 * i = (Int16 *) o -> value;
							i = new Int16(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::Int32Type: {
							Int32 * i = (Int32 *) o -> value;
							i = new Int32(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::Int64Type: {
							Int64 * i = (Int64 *) o -> value;
							i = new Int64(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::UInt8Type: {
							UInt8 * j = (UInt8 *) o -> value;
							Int16 * i = new Int16(-(* j));
							return Object(BasicType::Int16Type, i);
						} break;
						case BasicType::UInt16Type: {
							UInt16 * j = (UInt16 *) o -> value;
							Int32 * i = new Int32(-(* j));
							return Object(BasicType::Int32Type, i);
						} break;
						case BasicType::UInt32Type: {
							UInt32 * j = (UInt32 *) o -> value;
							Int64 * i = new Int64(-(* j));
							return Object(BasicType::Int64Type, i);
						} break;
						case BasicType::UInt64Type: {
							UInt64 * j = (UInt64 *) o -> value;
							Int64 * i = new Int64(-(* j));
							return Object(BasicType::Int64Type, i);
						} break;
						case BasicType::FloatType: {
							Float * i = (Float *) o -> value;
							i = new Float(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::DoubleType: {
							Double * i = (Double *) o -> value;
							i = new Double(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::RealType: {
							Real * i = (Real *) o -> value;
							i = new Real(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::ImaginaryType: {
							// TODO: Ask the classes.
						} break;
						case BasicType::ComplexType: {
							// TODO: Ask the classes.
						} break;
						default: throw RunTimeUnaryOperandException(
							t -> lexeme, o -> getObjectName()
						);
					}
				} break;
				case TokenType::plus: {
					
				} break;
				case TokenType::tilde: {
					switch (o -> type) {
						case BasicType::Int8Type: {
							Int8 * i = (Int8 *) o -> value;
							i = new Int8(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::Int16Type: {
							Int16 * i = (Int16 *) o -> value;
							i = new Int16(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::Int32Type: {
							Int32 * i = (Int32 *) o -> value;
							i = new Int32(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::Int64Type: {
							Int64 * i = (Int64 *) o -> value;
							i = new Int64(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::UInt8Type: {
							UInt8 * i = (UInt8 *) o -> value;
							i = new UInt8(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::UInt16Type: {
							UInt16 * i = (UInt16 *) o -> value;
							i = new UInt16(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::UInt32Type: {
							UInt32 * i = (UInt32 *) o -> value;
							i = new UInt32(-(* i));
							return Object(o -> type, i);
						} break;
						case BasicType::UInt64Type: {
							UInt64 * i = (UInt64 *) o -> value;
							i = new UInt64(-(* i));
							return Object(o -> type, i);
						} break;
						default: throw RunTimeUnaryOperandException(
							t -> lexeme, o -> getObjectName()
						); break;
					}
				} break;
				case TokenType::exclamationMark: {
					if (o -> type == BasicType::BooleanType) {
						Boolean * b = (Boolean *) o -> value;
						b = new Boolean(!(* b));
						return Object(o -> type, b);
					} else throw RunTimeUnaryOperandException(
						t -> lexeme, o -> getObjectName()
					);
				} break;
				default: throw RunTimeUnaryOperandException(
					t -> lexeme, o -> getObjectName()
				); break;
			}
			return Object();
		}
		
	};

}

#endif
