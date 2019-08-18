
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

		typedef Function<Object * (Object *)> UnaryHandler;
		typedef Function<Object * (Object *, Object *)> BinaryHandler;

		struct BasicTypes {
			BasicType rs = BasicType::UnknownType;
			BasicType ls = BasicType::UnknownType;
		};
 
		Map<BasicType, UnaryHandler> unaryNegation = {
			{
				BasicType::Int8Type,
				[] (Object * o) {
					Int8 * i = (Int8 *) o -> value;
					i = new Int8(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::Int16Type,
				[] (Object * o) {
					Int16 * i = (Int16 *) o -> value;
					i = new Int16(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::Int32Type,
				[] (Object * o) {
					Int32 * i = (Int32 *) o -> value;
					i = new Int32(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::Int64Type,
				[] (Object * o) {
					Int64 * i = (Int64 *) o -> value;
					i = new Int64(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::UInt8Type,
				[] (Object * o) {
					UInt8 * j = (UInt8 *) o -> value;
					Int16 * i = new Int16(-(* j));
					return new Object(BasicType::Int16Type, i);
				}
			},
			{
				BasicType::UInt16Type,
				[] (Object * o) {
					UInt16 * j = (UInt16 *) o -> value;
					Int32 * i = new Int32(-(* j));
					return new Object(BasicType::Int32Type, i);
				}
			},
			{
				BasicType::UInt32Type,
				[] (Object * o) {
					UInt32 * j = (UInt32 *) o -> value;
					Int64 * i = new Int64(-(* j));
					return new Object(BasicType::Int64Type, i);
				}
			},
			{
				BasicType::UInt64Type,
				[] (Object * o) {
					UInt64 * j = (UInt64 *) o -> value;
					Int64 * i = new Int64(-(* j));
					return new Object(BasicType::Int64Type, i);
				}
			},
			{
				BasicType::FloatType,
				[] (Object * o) {
					Float * i = (Float *) o -> value;
					i = new Float(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::DoubleType,
				[] (Object * o) {
					Double * i = (Double *) o -> value;
					i = new Double(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::RealType,
				[] (Object * o) {
					Real * i = (Real *) o -> value;
					i = new Real(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::ImaginaryType,
				[] (Object * o) {
					Real * i = (Real *) o -> value;
					i = new Real(-(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::ComplexType,
				[] (Object * o) {
					Complex * c = (Complex *) o -> value;
					c = new Complex(-(* c));
					return new Object(o -> type, c);
				}
			}
		};

		Map<BasicType, UnaryHandler> unaryInversion = {
			{
				BasicType::Int8Type,
				[] (Object * o) {
					Int8 * i = (Int8 *) o -> value;
					i = new Int8(~(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::Int16Type,
				[] (Object * o) {
					Int16 * i = (Int16 *) o -> value;
					i = new Int16(~(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::Int32Type,
				[] (Object * o) {
					Int32 * i = (Int32 *) o -> value;
					i = new Int32(~(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::Int64Type,
				[] (Object * o) {
					Int64 * i = (Int64 *) o -> value;
					i = new Int64(~(* i));
					return new Object(o -> type, i);
				}
			},
			{
				BasicType::UInt8Type,
				[] (Object * o) {
					UInt8 * j = (UInt8 *) o -> value;
					Int16 * i = new Int16(~(* j));
					return new Object(BasicType::Int16Type, i);
				}
			},
			{
				BasicType::UInt16Type,
				[] (Object * o) {
					UInt16 * j = (UInt16 *) o -> value;
					Int32 * i = new Int32(~(* j));
					return new Object(BasicType::Int32Type, i);
				}
			},
			{
				BasicType::UInt32Type,
				[] (Object * o) {
					UInt32 * j = (UInt32 *) o -> value;
					Int64 * i = new Int64(~(* j));
					return new Object(BasicType::Int64Type, i);
				}
			},
			{
				BasicType::UInt64Type,
				[] (Object * o) {
					UInt64 * j = (UInt64 *) o -> value;
					Int64 * i = new Int64(~(* j));
					return new Object(BasicType::Int64Type, i);
				}
			}
		};

		Object * applySubtraction(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		Object * applyAddition(Token * t, Object * l, Object * r) {
			Object * o = nullptr;
			
			return o;
		}

		Object * applyMultiplication(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		Object * applyDivision(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		Object * applyModulus(Token * t, Object * l, Object * r) {
			return nullptr;
		}

		Processor() = default;
		~Processor() = default;

		public:

		Processor(const Processor &) = delete;
		Processor(Processor &&) = delete;
		Processor & operator = (const Processor &) = delete;
		Processor & operator = (Processor &&) = delete;

		static Processor * self() {
			static Processor instance;
			return & instance;
		}

		Object * applyBinaryOperator(Token * t, Object * l, Object * r) {
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

		Object * applyUnaryOperator(Token * t, Object * o) {
			switch (t -> type) {
				case TokenType::minus: {
					auto search = unaryNegation.find(o -> type);
					if (search != unaryNegation.end()) {
						auto handler = search -> second;
						return handler(o);
					} else {
						throw RunTimeUnaryOperatorException(
							t -> lexeme, o -> getObjectName()
						);
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
						case BasicType::ComplexType: return new Object(* o);
						default: throw RunTimeUnaryOperatorException(
							t -> lexeme, o -> getObjectName()
						); break;
					}
				} break;
				case TokenType::exclamationMark: {
					if (o -> type == BasicType::BooleanType) {
						Boolean * b = (Boolean *) o -> value;
						b = new Boolean(!(* b));
						return new Object(o -> type, b);
					} else throw RunTimeUnaryOperatorException(
						t -> lexeme, o -> getObjectName()
					);
				} break;
				case TokenType::tilde: {
					auto search = unaryInversion.find(o -> type);
					if (search != unaryInversion.end()) {
						auto handler = search -> second;
						return handler(o);
					} else {
						throw RunTimeUnaryOperatorException(
							t -> lexeme, o -> getObjectName()
						);
					}
				} break;
				default: throw RunTimeUnaryOperatorException(
					t -> lexeme, o -> getObjectName()
				); break;
			}
			return nullptr;
		}
		
	};

}

#endif
