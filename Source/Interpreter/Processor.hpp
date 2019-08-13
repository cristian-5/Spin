
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
#include "../Types/Object.hpp"

#include "Exceptions.hpp"

namespace Stack {

	class Processor {

		private:
		
		Processor() { }

		static Object * applySubtraction(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		static Object * applyAddition(Token * t, Object * l, Object * r) {
			Object * o = nullptr;
			
			return o;
		}

		static Object * applyMultiplication(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		static Object * applyDivision(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		static Object * applyModulus(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		public:

		static Object * applyBinaryOperator(Token * t, Object * l, Object * r) {
			switch (t -> type) {
				case TokenType::minus: {
					try { return applySubtraction(t, l, r); }
					catch (Exception & e) { throw; }
				} break;
				case TokenType::plus: {
					try { return applyAddition(t, l, r); }
					catch (Exception & e) { throw; }
				} break;
				case TokenType::star: {
					try { return applyMultiplication(t, l, r); }
					catch (Exception & e) { throw; }
				} break;
				case TokenType::slash: {
					try { return applyDivision(t, l, r);}
					catch (Exception & e) { throw; }
				} break;
				case TokenType::modulus: {
					try { return applyModulus(t, l, r); }
					catch (Exception & e) { throw; }
				} break;
				default: break;
			}
			return nullptr;
		}

		static Object * applyUnaryOperator(Token * t, Object * o) {
			switch (t -> type) {
				case TokenType::minus: {
					switch (o -> type) {
						case BasicType::Int8Type: {
							Int8 * i = (Int8 *) o -> value;
							i = new Int8(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::Int16Type: {
							Int16 * i = (Int16 *) o -> value;
							i = new Int16(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::Int32Type: {
							Int32 * i = (Int32 *) o -> value;
							i = new Int32(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::Int64Type: {
							Int64 * i = (Int64 *) o -> value;
							i = new Int64(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::UInt8Type: {
							UInt8 * j = (UInt8 *) o -> value;
							Int16 * i = new Int16(-(* j));
							return new Object(BasicType::Int16Type, i);
						} break;
						case BasicType::UInt16Type: {
							UInt16 * j = (UInt16 *) o -> value;
							Int32 * i = new Int32(-(* j));
							return new Object(BasicType::Int32Type, i);
						} break;
						case BasicType::UInt32Type: {
							UInt32 * j = (UInt32 *) o -> value;
							Int64 * i = new Int64(-(* j));
							return new Object(BasicType::Int64Type, i);
						} break;
						case BasicType::UInt64Type: {
							UInt64 * j = (UInt64 *) o -> value;
							Int64 * i = new Int64(-(* j));
							return new Object(BasicType::Int64Type, i);
						} break;
						case BasicType::FloatType: {
							Float * i = (Float *) o -> value;
							i = new Float(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::DoubleType: {
							Double * i = (Double *) o -> value;
							i = new Double(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::RealType:
						case BasicType::ImaginaryType: {
							Real * i = (Real *) o -> value;
							i = new Real(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::ComplexType: {
							Complex * c = (Complex *) o -> value;
							c = new Complex(-(* c));
							return new Object(o -> type, c);
						} break;
						default: throw RunTimeUnaryOperandException(
							t -> lexeme, o -> getObjectName()
						); break;
					}
				} break;
				case TokenType::plus: {
					switch (o -> type) {
						case BasicType::CharacterType:
						case BasicType::Int8Type:
						case BasicType::Int16Type:
						case BasicType::Int32Type:
						case BasicType::Int64Type:
						case BasicType::UInt8Type:
						case BasicType::UInt16Type:
						case BasicType::UInt32Type:
						case BasicType::UInt64Type:
						case BasicType::FloatType:
						case BasicType::DoubleType:
						case BasicType::RealType:
						case BasicType::ImaginaryType:
						case BasicType::ComplexType: return o;
						default: throw RunTimeUnaryOperandException(
							t -> lexeme, o -> getObjectName()
						); break;
					}
				} break;
				case TokenType::tilde: {
					switch (o -> type) {
						case BasicType::Int8Type: {
							Int8 * i = (Int8 *) o -> value;
							i = new Int8(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::Int16Type: {
							Int16 * i = (Int16 *) o -> value;
							i = new Int16(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::Int32Type: {
							Int32 * i = (Int32 *) o -> value;
							i = new Int32(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::Int64Type: {
							Int64 * i = (Int64 *) o -> value;
							i = new Int64(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::UInt8Type: {
							UInt8 * i = (UInt8 *) o -> value;
							i = new UInt8(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::UInt16Type: {
							UInt16 * i = (UInt16 *) o -> value;
							i = new UInt16(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::UInt32Type: {
							UInt32 * i = (UInt32 *) o -> value;
							i = new UInt32(-(* i));
							return new Object(o -> type, i);
						} break;
						case BasicType::UInt64Type: {
							UInt64 * i = (UInt64 *) o -> value;
							i = new UInt64(-(* i));
							return new Object(o -> type, i);
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
						return new Object(o -> type, b);
					} else throw RunTimeUnaryOperandException(
						t -> lexeme, o -> getObjectName()
					);
				} break;
				default: throw RunTimeUnaryOperandException(
					t -> lexeme, o -> getObjectName()
				); break;
			}
			return nullptr;
		}
		
	};

}

#endif
