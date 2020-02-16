
/*!
 *
 *    + --------------------------------------- +
 *    |  Processor.cpp                          |
 *    |                                         |
 *    |             Object Processor            |
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

#include "../Aliases/Prototypes/Processor.hpp"

#ifndef SPIN_PROCESSOR
#define SPIN_PROCESSOR

#include "../Aliases/Prototypes/Class.hpp"
#include "../Aliases/Prototypes/Complex.hpp"
#include "../Aliases/Prototypes/Vector.hpp"
#include "../Aliases/Prototypes/Converter.hpp"

namespace Spin {

	EvaluationError::EvaluationError(String m, Token t): message(m), token(t) { }
	const String & EvaluationError::getMessage() const {
		return message;
	}
	const Token & EvaluationError::getToken() const {
		return token;
	}

	Processor::Processor() {
		unaryNegation = Dictionary<BasicType, UnaryHandler>({
			{
				BasicType::IntegerType,
				[] (Object * o) -> Object * {
					Int64 * i = (Int64 *) o -> value;
					return new Object(o -> type, new Int64(-(* i)));
				}
			},
			{
				BasicType::RealType,
				[] (Object * o) -> Object * {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ImaginaryType,
				[] (Object * o) -> Object * {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ComplexType,
				[] (Object * o) -> Object * {
					Complex * c = (Complex *) o -> value;
					return new Object(o -> type, new Complex(-(* c)));
				}
			},
			{
				BasicType::VectorType,
				[] (Object * o) -> Object * {
					Vector * v = (Vector *) o -> value;
					v = v -> getAdditiveInverse();
					return new Object(o -> type, v);
				}
			}
		});
		unaryInversion = Dictionary<BasicType, UnaryHandler>({
			{
				BasicType::ByteType,
				[] (Object * o) -> Object * {
					UInt8 * b = (UInt8 *) o -> value;
					b = new UInt8(~(* b));
					return new Object(o -> type, b);
				}
			},
			{
				BasicType::IntegerType,
				[] (Object * o) -> Object * {
					Int64 * i = (Int64 *) o -> value;
					i = new Int64(~(* i));
					return new Object(o -> type, i);
				}
			}
		});
		binaryAddition = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) + (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* b) + (* a));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) + (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* a), (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) + b -> a, b -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b) + a -> a, a -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) + (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a, a -> b + (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) + (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) + (* b));
					return new Object(BasicType::ByteType, c);
				}
			}
		});
		stringAddition = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::StringType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String((* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					String * c = new String(* a);
					c -> push_back(* b);
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					String * c = new String((* a) + Converter::realToString(* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					String * c = new String((* a) + Converter::realToString(* b) + "i");
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					String * c = new String((* a) + Converter::integerToString(* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Complex * b = (Complex *) r -> value;
					String * c = new String((* a) + b -> stringValue());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String((* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(Converter::realToString(* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(Converter::realToString(* a) + "i" + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(Converter::integerToString(* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(a -> stringValue() + (* b));
					return new Object(BasicType::StringType, c);
				}
			}
		});
		binarySubtraction = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) - (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) - (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(-(Real)(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* b), -(* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(-(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a), -(* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a, (* a).b - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* b).a, (* a) - (* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a - (* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex((* a).a - (Real)(* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((Real)(* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) - (* b));
					return new Object(BasicType::CharacterType, c);
				}
			}
		});
		binaryMultiplication = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) * (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) * (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) * (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) * (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real(-((* a) * (* b)));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* a) * b -> b, (* a) * b -> a);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a * (* b), a -> b * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(a -> a * (Real)(* b), a -> b * (Real)(* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::VectorType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Vector * v = ((Vector *) l -> value) -> copy();
					Complex c = * ((Complex *) r -> value);
					v -> multiplyByScalarComplex(c);
					return new Object(BasicType::VectorType, v);
				}
			},
			{
				compose(BasicType::VectorType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Vector * v = ((Vector *) l -> value) -> copy();
					Real i = * ((Real *) r -> value);
					v -> multiplyByScalarImaginary(i);
					return new Object(BasicType::VectorType, v);
				}
			},
			{
				compose(BasicType::VectorType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Vector * v = ((Vector *) l -> value) -> copy();
					Real i = * ((Real *) r -> value);
					v -> multiplyByScalarReal(i);
					return new Object(BasicType::VectorType, v);
				}
			},
			{
				compose(BasicType::VectorType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Vector * v = ((Vector *) l -> value) -> copy();
					Int64 i = * ((Int64 *) r -> value);
					v -> multiplyByScalarInteger(i);
					return new Object(BasicType::VectorType, v);
				}
			}
		});
		binaryDivision = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((* a) / (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return nullptr;
					Real * c = new Real((Real)(* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return nullptr;
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return nullptr;
					UInt8 * c = new UInt8((* a) / (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((* a) / (Int64)(* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					if ((* b) == 0) return nullptr;
					Character * c = new Character((* a) / (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return nullptr;
					Character * c = new Character((* a) / (UInt8)(* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return nullptr;
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0) return nullptr;
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = nullptr;
					try { c = new Complex((* a) / (* b)); }
					catch (ComplexDBZException & e) { return nullptr; }
					return new Object(BasicType::ComplexType, c);
				}
			}
		});
		binaryAND = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) & (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) & (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) & (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) && (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryXOR = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) ^ (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) ^ (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) ^ (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) != (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryOR = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) | (* b));
					return new Object(BasicType::IntegerType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) | (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) | (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) || (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryStrictEquality = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryMixedEquality = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> b) && (b -> a == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> a) && (b -> b == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool(false);
					if (a -> length() != 1) {
						return new Object(BasicType::BoolType, c);
					}
					* c = (a -> at(0) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryMajor = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryMajorEqual = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryMinor = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		binaryMinorEqual = Dictionary<BasicTypes, BinaryHandler>({
			{
				compose(BasicType::IntegerType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::IntegerType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		});
		pureAssignment = Dictionary<BasicType, AssignmentHandler>({
			{
				BasicType::IntegerType,
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = * b;
				}
			},
			{
				BasicType::BoolType,
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					* a = * b;
				}
			},
			{
				BasicType::ByteType,
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = * b;
				}
			},
			{
				BasicType::StringType,
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					* a = String(* b);
				}
			},
			{
				BasicType::RealType,
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					* a = * b;
				}
			},
			{
				BasicType::CharacterType,
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					* a = * b;
				}
			},
			{
				BasicType::ImaginaryType,
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					* a = * b;
				}
			},
			{
				BasicType::ComplexType,
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					* a = * b;
				}
			},
			{
				BasicType::VectorType,
				[] (Object * l, Object * r) {
					delete (Vector *) l -> value;
					l -> value = ((Vector *) r -> value) -> copy();
				}
			}
		});
		mixedAssignment = Dictionary<BasicTypes, AssignmentHandler>({
			{
				compose(BasicType::IntegerType, BasicType::RealType),
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				compose(BasicType::RealType, BasicType::IntegerType),
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (Real)(* b);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::IntegerType),
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = Complex(* b, 0.0);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					* a = Complex(* b, 0.0);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					* a = Complex(0.0, * b);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::CharacterType),
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				compose(BasicType::IntegerType, BasicType::ByteType),
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::IntegerType),
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::ByteType, BasicType::IntegerType),
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				compose(BasicType::StringType, BasicType::CharacterType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					* a = String(b, 1);
				}
			},
			{
				compose(BasicType::StringType, BasicType::IntegerType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = String(Converter::integerToString(* b));
				}
			},
			{
				compose(BasicType::StringType, BasicType::RealType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					* a = String(Converter::realToString(* b));
				}
			},
			{
				compose(BasicType::StringType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					* a = String(Converter::realToString(* b));
				}
			},
			{
				compose(BasicType::StringType, BasicType::ComplexType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Complex * b = (Complex *) r -> value;
					* a = b -> stringValue();
				}
			},
			{
				compose(BasicType::StringType, BasicType::BoolType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Bool * b = (Bool *) r -> value;
					* a = (* b) ? "true" : "false";
				}
			}
		});
	}

	void Processor::applyAdditionAssignment(Token * t, Object * l, Object * r) {
		if (l -> isString() || r -> isString()) {
			auto search = stringAddition.find(compose(l -> type, r -> type));
			if (search != stringAddition.end()) {
				auto handler = search -> second;
				Object * temp = handler(l, r);
				try { applyAssignment(t, l, temp); }
				catch (EvaluationError & e) { throw; }
				delete temp; return;
			}
			throw EvaluationError(
				"Binary operator '+=' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}
		auto search = binaryAddition.find(compose(l -> type, r -> type));
		if (search != binaryAddition.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		search = binaryAddition.find(compose(r -> type, l -> type));
		if (search != binaryAddition.end()) {
			auto handler = search -> second;
			Object * temp = handler(r, l);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '+=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applySubtractionAssignment(Token * t, Object * l, Object * r) {
		auto search = binarySubtraction.find(compose(l -> type, r -> type));
		if (search != binarySubtraction.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '-=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyMultiplicationAssignment(Token * t, Object * l, Object * r) {
		auto search = binaryMultiplication.find(compose(l -> type, r -> type));
		if (search != binaryMultiplication.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		search = binaryMultiplication.find(compose(r -> type, l -> type));
		if (search != binaryMultiplication.end()) {
			auto handler = search -> second;
			Object * temp = handler(r, l);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '*=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyDivisionAssignment(Token * t, Object * l, Object * r) {
		auto search = binaryDivision.find(compose(l -> type, r -> type));
		if (search != binaryDivision.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			if (!temp) {
				// Check if in try catch block.
				throw EvaluationError(
					"Binary operator '/=' threw division by 0 exception!", * t
				);
			}
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '/=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyModulusAssignment(Token * t, Object * l, Object * r) {
		if (l -> type == BasicType::IntegerType &&
			r -> type == BasicType::IntegerType) {
			Int64 * a = (Int64 *) l -> value;
			Int64 * b = (Int64 *) r -> value;
			if ((* b) == 0) {
				// Check if in try catch block.
				throw EvaluationError(
					"Binary operator '%=' threw division by 0 exception!", * t
				);
			}
			Int64 * c = new Int64((* a) % (* b));
			Object * temp = new Object(BasicType::IntegerType, c);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '%=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyORAssignment(Token * t, Object * l, Object * r) {
		auto search = binaryOR.find(compose(l -> type, r -> type));
		if (search != binaryOR.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '|=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyANDAssignment(Token * t, Object * l, Object * r) {
		auto search = binaryAND.find(compose(l -> type, r -> type));
		if (search != binaryAND.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '&=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyXORAssignment(Token * t, Object * l, Object * r) {
		auto search = binaryXOR.find(compose(l -> type, r -> type));
		if (search != binaryXOR.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			try { applyAssignment(t, l, temp); }
			catch (EvaluationError & e) { throw; }
			delete temp; return;
		}
		throw EvaluationError(
			"Binary operator '^=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}

	Object * Processor::applyAddition(Token * t, Object * l, Object * r) {
		if (l -> isString() || r -> isString()) {
			auto search = stringAddition.find(compose(l -> type, r -> type));
			if (search != stringAddition.end()) {
				auto handler = search -> second;
				return handler(l, r);
			}
			throw EvaluationError(
				"Binary operator '+' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}
		auto search = binaryAddition.find(compose(l -> type, r -> type));
		if (search != binaryAddition.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		search = binaryAddition.find(compose(r -> type, l -> type));
		if (search != binaryAddition.end()) {
			auto handler = search -> second;
			return handler(r, l);
		}
		throw EvaluationError(
			"Binary operator '+' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applySubtraction(Token * t, Object * l, Object * r) {
		auto search = binarySubtraction.find(compose(l -> type, r -> type));
		if (search != binarySubtraction.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Binary operator '-' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyMultiplication(Token * t, Object * l, Object * r) {
		auto search = binaryMultiplication.find(compose(l -> type, r -> type));
		if (search != binaryMultiplication.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		search = binaryMultiplication.find(compose(r -> type, l -> type));
		if (search != binaryMultiplication.end()) {
			auto handler = search -> second;
			return handler(r, l);
		}
		if (l -> type == BasicType::VectorType &&
			r -> type == BasicType::VectorType) {
			Vector * a = (Vector *) l -> value;
			Vector * b = (Vector *) r -> value;
			if (a -> getDirection() == b -> getDirection()) {
				throw EvaluationError(
					"Binary operator '*' doesn't support Vectors that occupy the same space!", * t
				);
			}
			if (a -> getSize() != b -> getSize()) {
				throw EvaluationError(
					"Binary operator '*' doesn't support Vectors with different dimensions!", * t
				);
			}
			if (a -> getDirection()) {
				/* Bra * Ket */
				Complex * c = new Complex();
				for (SizeType i = 0; i < a -> getSize(); i += 1) {
					(* c) = (* c) + (a -> at(i) * b -> at(i));
				}
				return new Object(BasicType::ComplexType, c);
			} else {
				/* Ket * Bra */
				// TODO: Outer Product.
				return nullptr;
			}
		}
		throw EvaluationError(
			"Binary operator '*' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyDivision(Token * t, Object * l, Object * r) {
		auto search = binaryDivision.find(compose(l -> type, r -> type));
		if (search != binaryDivision.end()) {
			auto handler = search -> second;
			Object * temp = handler(l, r);
			if (!temp) {
				// Check if in try catch block.
				throw EvaluationError(
					"Binary operator '/' threw division by 0 exception!", * t
				);
			}
			return temp;
		}
		throw EvaluationError(
			"Binary operator '/' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyModulus(Token * t, Object * l, Object * r) {
		if (l -> type == BasicType::IntegerType &&
			r -> type == BasicType::IntegerType) {
			Int64 * a = (Int64 *) l -> value;
			Int64 * b = (Int64 *) r -> value;
			if ((* b) == 0) {
				// Check if in try catch block.
				throw EvaluationError(
					"Binary operator '%' threw division by 0 exception!", * t
				);
			}
			Int64 * c = new Int64((* a) % (* b));
			return new Object(BasicType::IntegerType, c);
		}
		throw EvaluationError(
			"Binary operator '%' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyAND(Token * t, Object * l, Object * r) {
		auto search = binaryAND.find(compose(l -> type, r -> type));
		if (search != binaryAND.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Binary operator '&' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyXOR(Token * t, Object * l, Object * r) {
		auto search = binaryXOR.find(compose(l -> type, r -> type));
		if (search != binaryXOR.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Binary operator '^' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyOR(Token * t, Object * l, Object * r) {
		auto search = binaryOR.find(compose(l -> type, r -> type));
		if (search != binaryOR.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Binary operator '|' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyEquality(Token * t, Object * l, Object * r) {
		if (l -> type == r -> type) {
			auto search = binaryStrictEquality.find(compose(l -> type, r -> type));
			if (search != binaryStrictEquality.end()) {
				auto handler = search -> second;
				return handler(l, r);
			}
			throw EvaluationError(
				"Logical operator '==' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}
		auto search = binaryMixedEquality.find(compose(l -> type, r -> type));
		if (search != binaryMixedEquality.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		search = binaryMixedEquality.find(compose(r -> type, l -> type));
		if (search != binaryMixedEquality.end()) {
			auto handler = search -> second;
			return handler(r, l);
		}
		throw EvaluationError(
			"Logical operator '==' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyInequality(Token * t, Object * l, Object * r) {
		Object * a = applyEquality(t, l, r);
		Bool * b = (Bool *) a -> value;
		* b = !(* b); return a;
	}
	Object * Processor::applyMajor(Token * t, Object * l, Object * r) {
		auto search = binaryMajor.find(compose(l -> type, r -> type));
		if (search != binaryMajor.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Logical operator '>' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyMajorEqual(Token * t, Object * l, Object * r) {
		auto search = binaryMajorEqual.find(compose(l -> type, r -> type));
		if (search != binaryMajorEqual.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Logical operator '>=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyMinor(Token * t, Object * l, Object * r) {
		auto search = binaryMinor.find(compose(l -> type, r -> type));
		if (search != binaryMinor.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Logical operator '<' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyMinorEqual(Token * t, Object * l, Object * r) {
		auto search = binaryMinorEqual.find(compose(l -> type, r -> type));
		if (search != binaryMinorEqual.end()) {
			auto handler = search -> second;
			return handler(l, r);
		}
		throw EvaluationError(
			"Logical operator '<=' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}

	BasicTypes Processor::compose(BasicType a, BasicType b) {
		return (BasicTypes)(((BasicTypes)a << 8) | b);
	}

	Object * Processor::applyComparisonOperator(Token * t, Object * l, Object * r) {
		switch (t -> type) {
			case TokenType::equality: {
				try { return applyEquality(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::inequality: {
				try { return applyInequality(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::major: {
				try { return applyMajor(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::majorEqual: {
				try { return applyMajorEqual(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::minor: {
				try { return applyMinor(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::minorEqual: {
				try { return applyMinorEqual(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			default: break;
		}
		throw EvaluationError(
			"Comparison operator '" + t -> lexeme + "' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyBinaryOperator(Token * t, Object * l, Object * r) {
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
			case TokenType::ampersand: {
				try { return applyAND(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::dollar: {
				try { return applyXOR(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::pipe: {
				try { return applyOR(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			case TokenType::modulus: {
				try { return applyModulus(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} break;
			default: break;
		}
		throw EvaluationError(
			"Binary operator '" + t -> lexeme + "' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applySubscriptOperator(Token * t, Object * l, Object * r) {
		if (l -> type == BasicType::StringType) {
			if (r -> type == BasicType::IntegerType) {
				Int64 * i = (Int64 *) r -> value;
				String * s = (String *) l -> value;
				if (((* i) >= 0) && (s -> length() > (* i))) {
					Character * c = new Character(s -> at(* i));
					return new Object(BasicType::CharacterType, c);
				}
				throw EvaluationError(
					"Subscript operator '[ ]' threw index out of range exception!", * t
				);
			}
			throw EvaluationError(
				"Subscript operator '[ ]' doesn't support operand of type '" +
				r -> getObjectName() + "' on inner expression of type '" +
				l -> getObjectName() + "'!", * t
			);
		} else if (l -> type == BasicType::ArrayType) {
			/* TODO: Ask the array... */
		}
		throw EvaluationError(
			"Subscript operator '[ ]' doesn't support operand of type '" +
			r -> getObjectName() + "' on inner expression of type '" +
			l -> getObjectName() + "'!", * t
		);
	}
	Object * Processor::applyUnaryOperator(Token * t, Object * o) {
		switch (t -> type) {
			case TokenType::minus: {
				auto search = unaryNegation.find(o -> type);
				if (search != unaryNegation.end()) {
					auto handler = search -> second;
					return handler(o);
				}
				throw EvaluationError(
					"Unary operator '-' doesn't support any operand of type '" +
					o -> getObjectName() + "'!", * t
				);
			} break;
			case TokenType::plus: {
				switch (o -> type) {
					case BasicType::CharacterType:
					case BasicType::ByteType:
					case BasicType::IntegerType:
					case BasicType::RealType:
					case BasicType::ImaginaryType:
					case BasicType::ComplexType:
					case BasicType::VectorType: return o -> copy();
					default: {
						throw EvaluationError(
							"Unary operator '+' doesn't support any operand of type '" +
							o -> getObjectName() + "'!", * t
						);
					} break;
				}
			} break;
			case TokenType::exclamationMark: {
				if (o -> type == BasicType::BoolType) {
					Bool * b = (Bool *) o -> value;
					b = new Bool(!(* b));
					return new Object(o -> type, b);
				}
				throw EvaluationError(
					"Unary operator '!' doesn't support any operand of type '" +
					o -> getObjectName() + "'!", * t
				);
			} break;
			case TokenType::conjugate: {
				if (o -> type == BasicType::ComplexType) {
					Complex * c = (Complex *) o -> value;
					c = new Complex(* c); c -> conjugate();
					return new Object(o -> type, c);
				} else if (o -> type == BasicType::VectorType) {
					Vector * v = ((Vector *) o -> value) -> getConjugate();
					return new Object(o -> type, v);
				}
				throw EvaluationError(
					"Unary operator '°' doesn't support any operand of type '" +
					o -> getObjectName() + "'!", * t
				);
			} break;
			case TokenType::transpose: {
				if (o -> type == BasicType::VectorType) {
					Vector * v = ((Vector *) o -> value) -> getTransposed();
					return new Object(o -> type, v);
				}
				throw EvaluationError(
					"Unary operator '^' doesn't support any operand of type '" +
					o -> getObjectName() + "'!", * t
				);
			} break;
			case TokenType::dagger: {
				if (o -> type == BasicType::VectorType) {
					Vector * v = ((Vector *) o -> value) -> getConjugateTranspose();
					return new Object(o -> type, v);
				}
				throw EvaluationError(
					"Unary operator ''' doesn't support any operand of type '" +
					o -> getObjectName() + "'!", * t
				);
			} break;
			case TokenType::tilde: {
				auto search = unaryInversion.find(o -> type);
				if (search != unaryInversion.end()) {
					auto handler = search -> second;
					return handler(o);
				}
				throw EvaluationError(
					"Unary operator '~' doesn't support any operand of type '" +
					o -> getObjectName() + "'!", * t
				);
			} break;
			default: break;
		}
		throw EvaluationError(
			"Unary operator '" + t -> lexeme +
			"' doesn't support any operand of type '" +
			o -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyAssignment(Token * t, Object * l, Object * r) {
		if (l -> type == r -> type) {
			if (l -> type == BasicType::InstanceType) {
				Instance * a = (Instance *) l -> value;
				Instance * b = (Instance *) r -> value;
				if (a -> type != b -> type) {
					throw EvaluationError(
						"Assignment operator '" + t -> lexeme + "' doesn't support types '" +
						a -> stringValue() + "' and '" +
						b -> stringValue() + "' generated from different class definitions!", * t
					);
				}
				b = b -> copyByValue();
				// We leave b intact because classes
				// assignations are copies by value.
				// We delete a because it's been
				// overwritten by the new copy.
				a -> destroy(); delete a;
				l -> value = b; return;
			}
			auto search = pureAssignment.find(l -> type);
			if (search != pureAssignment.end()) {
				auto handler = search -> second;
				handler(l, r); return;
			}
		}
		auto search = mixedAssignment.find(compose(l -> type, r -> type));
		if (search != mixedAssignment.end()) {
			auto handler = search -> second;
			handler(l, r); return;
		}
		throw EvaluationError(
			"Assignment operator '" + t -> lexeme + "' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyVectorAssignment(Token * t, Object * l, Object * r) {
		/* This void takes in consideration eventual conjugate transpose. */
		if (l -> type == BasicType::VectorType &&
			r -> type == BasicType::VectorType) {
			Vector * a = (Vector *) l -> value;
			Vector * b = (Vector *) r -> value;
			if (a -> getDirection()) {
				b -> inBraForm();
			} else b -> inKetForm();
			delete a; l -> value = b -> copy();
			return;
		}
		throw EvaluationError(
			"Assignment operator '" + t -> lexeme + "' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}
	void Processor::applyMutableAssignment(Token * t, Object * l, Object * r) {
		switch (t -> type) {
			case TokenType::plusEqual: {
				try { applyAdditionAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			case TokenType::minusEqual: {
				try { applySubtractionAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			case TokenType::starEqual: {
				try { applyMultiplicationAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			case TokenType::slashEqual: {
				try { applyDivisionAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			case TokenType::modulusEqual: {
				try { applyModulusAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			case TokenType::pipeEqual: {
				try { applyORAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			case TokenType::ampersandEqual: {
				try { applyANDAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			case TokenType::dollarEqual: {
				try { applyXORAssignment(t, l, r); }
				catch (EvaluationError & e) { throw; }
			} return;
			default: break;
		}
		throw EvaluationError(
			"Mutable Assignment operator '" + t -> lexeme + "' doesn't support operands of type '" +
			l -> getObjectName() + "' and '" +
			r -> getObjectName() + "'!", * t
		);
	}

	Object * Processor::applyInnerProduct(Token * t, Object * l, Object * r) {
		/* We assume that the Interpreter passed us the right order <Bra|Ket>. */
		if (l -> type == BasicType::VectorType &&
			r -> type == BasicType::VectorType) {
			Vector * a = (Vector *) l -> value;
			Vector * b = (Vector *) r -> value;
			if (a -> getDirection() == b -> getDirection()) {
				throw EvaluationError(
					"Inner product '<Bra|Ket>' doesn't support Vectors that occupy the same space!", * t
				);
			}
			if (a -> getSize() != b -> getSize()) {
				throw EvaluationError(
					"Inner product '<Bra|Ket>' doesn't support Vectors with different dimensions!", * t
				);
			}
			Complex * c = new Complex();
			for (SizeType i = 0; i < a -> getSize(); i += 1) {
				(* c) = (* c) + (a -> at(i) * b -> at(i));
			}
			return new Object(BasicType::ComplexType, c);
		}
		throw EvaluationError(
			"Could not resolve invalid inner product '<Bra|Ket>'!", * t
		);
	}
	Object * Processor::applyOuterProduct(Token * t, Object * l, Object * r) {
		return nullptr;
	}

}

#endif
