
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
#include "../Linker/Linker.hpp"
#include "../Types/Object.hpp"

namespace Stack {

	class EvaluationError: public Exception {
		private:
		const String _message;
		const Token _token;
		public:
		const String & getMessage() const { return _message; }
		const Token & getToken() const { return _token; }
		EvaluationError(String message, Token token):
		Exception(), _message(message), _token(token) { }
	};

	class Processor {

		private:

		typedef Function<Object * (Object *)> UnaryHandler;
		typedef Function<Object * (Object *, Object *)> BinaryHandler;

		typedef Pair<BasicType, BasicType> BasicTypes;

		Map<BasicType, UnaryHandler> unaryNegation = {
			{
				BasicType::Int64Type,
				[] (Object * o) {
					Int64 * i = (Int64 *) o -> value;
					return new Object(o -> type, new Int64(-(* i)));
				}
			},
			{
				BasicType::RealType,
				[] (Object * o) {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ImaginaryType,
				[] (Object * o) {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ComplexType,
				[] (Object * o) {
					Complex * c = (Complex *) o -> value;
					return new Object(o -> type, new Complex(-(* c)));
				}
			}
		};
		Map<BasicType, UnaryHandler> unaryInversion = {
			{
				BasicType::ByteType,
				[] (Object * o) {
					UInt8 * b = (UInt8 *) o -> value;
					b = new UInt8(~(* b));
					return new Object(o -> type, b);
				}
			},
			{
				BasicType::ColourType,
				[] (Object * o) {
					Colour * c = (Colour *) o -> value;
					c = new Colour(~(* c));
					return new Object(o -> type, c);
				}
			},
			{
				BasicType::Int64Type,
				[] (Object * o) {
					Int64 * i = (Int64 *) o -> value;
					i = new Int64(~(* i));
					return new Object(o -> type, i);
				}
			}
		};

		Map<BasicTypes, BinaryHandler> binaryAddition = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) + (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) + (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* a), (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) + b -> a, b -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b) + a -> a, a -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a, a -> b + (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			}
		};

		Object * applyAddition(Token * t, Object * l, Object * r) {
			auto search = binaryAddition.find({ l -> type, r -> type });
			if (search != binaryAddition.end()) {
				auto handler = search -> second;
				return handler(l, r);
			}
			search = binaryAddition.find({ r -> type, l -> type });
			if (search != binaryAddition.end()) {
				auto handler = search -> second;
				return handler(r, l);
			}
			throw EvaluationError(
				"Binary operator '+' doesn't match operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}

		Object * applySubtraction(Token * t, Object * l, Object * r) {
			return nullptr;
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
				case TokenType::plus: {
					try { return applyAddition(t, l, r); }
					catch (EvaluationError & e) { throw; }
				} break;
				case TokenType::minus: {
					try { return applySubtraction(t, l, r); }
					catch (EvaluationError & e) { throw; }
				} break;
				case TokenType::star: {
					try { return applyMultiplication(t, l, r); }
					catch (EvaluationError & e) { throw; }
				} break;
				case TokenType::slash: {
					try { return applyDivision(t, l, r);}
					catch (EvaluationError & e) { throw; }
				} break;
				case TokenType::modulus: {
					try { return applyModulus(t, l, r); }
					catch (EvaluationError & e) { throw; }
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
					}
					throw EvaluationError(
						"Unary operator '-' doesn't match any operand of type '" +
						o -> getObjectName() + "'!", * t
					);
				} break;
				case TokenType::plus: {
					switch (o -> type) {
						case BasicType::CharacterType:
						case BasicType::ByteType:
						case BasicType::Int64Type:
						case BasicType::RealType:
						case BasicType::ImaginaryType:
						case BasicType::ComplexType: return o -> copy();
						default: {
							throw EvaluationError(
								"Unary operator '+' doesn't match any operand of type '" +
								o -> getObjectName() + "'!", * t
							);
						} break;
					}
				} break;
				case TokenType::exclamationMark: {
					if (o -> type == BasicType::BooleanType) {
						Boolean * b = (Boolean *) o -> value;
						b = new Boolean(!(* b));
						return new Object(o -> type, b);
					} else {
						throw EvaluationError(
							"Unary operator '!' doesn't match any operand of type '" +
							o -> getObjectName() + "'!", * t
						);
					}
				} break;
				case TokenType::tilde: {
					auto search = unaryInversion.find(o -> type);
					if (search != unaryInversion.end()) {
						auto handler = search -> second;
						return handler(o);
					} else {
						throw EvaluationError(
							"Unary operator '~' doesn't match any operand of type '" +
							o -> getObjectName() + "'!", * t
						);	
					}
				} break;
				default: break;
			}
			throw EvaluationError(
				"Unary operator '" + t -> lexeme +
				"' doesn't match any operand of type '" +
				o -> getObjectName() + "'!", * t
			);
		}
		
	};

}

#endif
