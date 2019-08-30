
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
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* b) + (* a));
					return new Object(BasicType::RealType, c);
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
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) + (* b));
					return new Object(BasicType::ImaginaryType, c);
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
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) + (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) + (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) + (* b));
					return new Object(BasicType::ByteType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> stringAddition = {
			{
				{ BasicType::StringType, BasicType::StringType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String((* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::RealType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b) << "i";
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Colour * b = (Colour *) r -> value;
					String * c = new String((* a) + b -> stringValue());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Complex * b = (Complex *) r -> value;
					String * c = new String((* a) + b -> stringValue());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					String * b = (String *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					StringStream s = StringStream();
					s << (* a) << "i" << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::StringType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					String * b = (String *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(a -> stringValue() + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::ComplexType,  BasicType::StringType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(a -> stringValue() + (* b));
					return new Object(BasicType::StringType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binarySubtraction = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) - (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(-(Real)(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* b), -(* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(-(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a), -(* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a, (* a).b - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* b).a, (* a) - (* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a - (* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex((* a).a - (Real)(* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((Real)(* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) - (* b));
					return new Object(BasicType::CharacterType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMultiplication = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) * (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) * (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) * (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) * (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real(-((* a) * (* b)));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* a) * b -> b, (* a) * b -> a);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a * (* b), a -> b * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(a -> a * (Real)(* b), a -> b * (Real)(* b));
					return new Object(BasicType::ComplexType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryDivision = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return new Object();
					Real * c = new Real((Real)(* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return new Object();
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return new Object();
					UInt8 * c = new UInt8((* a) / (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((* a) / (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					if ((* b) == 0) return new Object();
					Character * c = new Character((* a) / (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return new Object();
					Character * c = new Character((* a) / (UInt8)(* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) / (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return new Object();
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0) return new Object();
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = nullptr;
					try { c = new Complex((* a) / (* b)); }
					catch (ComplexDBZException & e) { return new Object(); }
					return new Object(BasicType::ComplexType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryAND = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) & (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) & (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) & (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) && (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryXOR = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) ^ (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) ^ (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) ^ (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) != (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryOR = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) | (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) | (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) | (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) || (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};

		Object * applyAddition(Token * t, Object * l, Object * r) {
			if (l -> isString() || r -> isString()) {
				auto search = stringAddition.find({ l -> type, r -> type });
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
				"Binary operator '+' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}
		Object * applySubtraction(Token * t, Object * l, Object * r) {
			auto search = binarySubtraction.find({ l -> type, r -> type });
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
		Object * applyMultiplication(Token * t, Object * l, Object * r) {
			auto search = binaryMultiplication.find({ l -> type, r -> type });
			if (search != binaryMultiplication.end()) {
				auto handler = search -> second;
				return handler(l, r);
			}
			search = binaryMultiplication.find({ r -> type, l -> type });
			if (search != binaryMultiplication.end()) {
				auto handler = search -> second;
				return handler(r, l);
			}
			throw EvaluationError(
				"Binary operator '*' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}
		Object * applyDivision(Token * t, Object * l, Object * r) {
			auto search = binaryDivision.find({ l -> type, r -> type });
			if (search != binaryDivision.end()) {
				auto handler = search -> second;
				Object * o = handler(l, r);
				if (o -> isUnknown()) {
					// Check if in try catch block.
					throw EvaluationError(
						"Binary operator '/' threw division by 0 exception!", * t
					);
				}
				return o;
			}
			throw EvaluationError(
				"Binary operator '/' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}
		Object * applyModulus(Token * t, Object * l, Object * r) {
			if (l -> type == BasicType::Int64Type &&
				r -> type == BasicType::Int64Type) {
				Int64 * a = (Int64 *) l -> value;
				Int64 * b = (Int64 *) r -> value;
				if ((* b) == 0) {
					// Check if in try catch block.
					throw EvaluationError(
						"Binary operator '\%' threw division by 0 exception!", * t
					);
				}
				Int64 * c = new Int64((* a) % (* b));
				return new Object(BasicType::Int64Type, c);
			}
			if (l -> type == BasicType::ColourType &&
				r -> type == BasicType::ColourType) {
				Colour * a = (Colour *) l -> value;
				Colour * b = (Colour *) r -> value;
				Colour * c = new Colour((* a) % (* b));
				return new Object(BasicType::ColourType, c);
				
			}
			throw EvaluationError(
				"Binary operator '\%' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
		}
		Object * applyAND(Token * t, Object * l, Object * r) {
			auto search = binaryAND.find({ l -> type, r -> type });
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
		Object * applyXOR(Token * t, Object * l, Object * r) {
			auto search = binaryXOR.find({ l -> type, r -> type });
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
		Object * applyOR(Token * t, Object * l, Object * r) {
			auto search = binaryOR.find({ l -> type, r -> type });
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

		Processor() = default;
		~Processor() = default;

		Map<BasicTypes, BinaryHandler> binaryStrictEquality = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::StringType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMixedEquality = {
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> b) && (b -> a == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> a) && (b -> b == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
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
		};

		Map<BasicTypes, BinaryHandler> binaryMajor = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMajorEqual = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMinor = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMinorEqual = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};

		Object * applyEquality(Token * t, Object * l, Object * r) {
			if (l -> type == r -> type) {
				auto search = binaryStrictEquality.find({ l -> type, r -> type });
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
			auto search = binaryMixedEquality.find({ l -> type, r -> type });
			if (search != binaryMixedEquality.end()) {
				auto handler = search -> second;
				return handler(l, r);
			}
			search = binaryMixedEquality.find({ r -> type, l -> type });
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
		Object * applyInequality(Token * t, Object * l, Object * r) {
			Object * a = applyEquality(t, l, r);
			Bool * b = (Bool *) a -> value;
			* b = !(* b); return a;
		}
		Object * applyMajor(Token * t, Object * l, Object * r) {
			auto search = binaryMajor.find({ l -> type, r -> type });
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
		Object * applyMajorEqual(Token * t, Object * l, Object * r) {
			auto search = binaryMajorEqual.find({ l -> type, r -> type });
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
		Object * applyMinor(Token * t, Object * l, Object * r) {
			auto search = binaryMinor.find({ l -> type, r -> type });
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
		Object * applyMinorEqual(Token * t, Object * l, Object * r) {
			auto search = binaryMinorEqual.find({ l -> type, r -> type });
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

		public:

		Processor(const Processor &) = delete;
		Processor(Processor &&) = delete;
		Processor & operator = (const Processor &) = delete;
		Processor & operator = (Processor &&) = delete;

		static Processor * self() {
			static Processor instance;
			return & instance;
		}

		Object * applyLogicalOperator(Token * t, Object * l, Object * r) {
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
				"Logical operator '" + t -> lexeme + "' doesn't support operands of type '" +
				l -> getObjectName() + "' and '" +
				r -> getObjectName() + "'!", * t
			);
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
				case TokenType::ampersand: {
					try { return applyAND(t, l, r); }
					catch (EvaluationError & e) { throw; }
				} break;
				case TokenType::hat: {
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
		Object * applyUnaryOperator(Token * t, Object * o) {
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
						case BasicType::Int64Type:
						case BasicType::RealType:
						case BasicType::ImaginaryType:
						case BasicType::ComplexType: return o -> copy();
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
					} else {
						throw EvaluationError(
							"Unary operator '!' doesn't support any operand of type '" +
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
							"Unary operator '~' doesn't support any operand of type '" +
							o -> getObjectName() + "'!", * t
						);	
					}
				} break;
				default: break;
			}
			throw EvaluationError(
				"Unary operator '" + t -> lexeme +
				"' doesn't support any operand of type '" +
				o -> getObjectName() + "'!", * t
			);
		}
		
	};

}

#endif
