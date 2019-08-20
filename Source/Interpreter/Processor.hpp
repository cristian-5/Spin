
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
			},
			{
				BasicType::CharacterType,
				[] (Object * o) {
					Character * c = (Character *) o -> value;
					Int16 * i = new Int16(-(* c));
					return new Object(BasicType::Int16Type, i);
				}
			},
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
			},
			{
				BasicType::ColourType,
				[] (Object * o) {
					Colour * c = (Colour *) o -> value;
					c = new Colour(~(* c));
					return new Object(o -> type, c);
				}
			}
		};

		Map<BasicTypes, BinaryHandler> binaryIntIntAddition = {
			{
				{ BasicType::Int32Type, BasicType::Int32Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					Int32 * b = (Int32 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int32Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int32Type, BasicType::Int16Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					Int16 * b = (Int16 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int32Type, BasicType::Int8Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					Int8 * b = (Int8 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::Int32Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int32 * b = (Int32 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
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
				{ BasicType::Int64Type, BasicType::Int16Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int16 * b = (Int16 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::Int8Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int8 * b = (Int8 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::Int32Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					Int32 * b = (Int32 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::Int16Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					Int16 * b = (Int16 *) r -> value;
					Int32 * c = new Int32((Int32)(* a) + (Int32)(* b));
					return new Object(BasicType::Int32Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::Int8Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					Int8 * b = (Int8 *) r -> value;
					Int16 * c = new Int16((Int16)(* a) + (Int16)(* b));
					return new Object(BasicType::Int16Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::Int32Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					Int32 * b = (Int32 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::Int16Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					Int16 * b = (Int16 *) r -> value;
					Int32 * c = new Int32((Int32)(* a) + (Int32)(* b));
					return new Object(BasicType::Int32Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::Int8Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					Int8 * b = (Int8 *) r -> value;
					Int32 * c = new Int32((Int32)(* a) + (Int32)(* b));
					return new Object(BasicType::Int32Type, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryUIntUIntAddition = {
			{
				{ BasicType::UInt32Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					UInt32 * a = (UInt32 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (UInt64)(* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt32Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					UInt32 * a = (UInt32 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt32Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					UInt32 * a = (UInt32 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (UInt64)(* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt32Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					UInt32 * a = (UInt32 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (UInt64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::UInt64Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					UInt64 * a = (UInt64 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					UInt64 * c = new UInt64((* a) + (UInt64)(* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt64Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					UInt64 * a = (UInt64 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					UInt64 * c = new UInt64((* a) + (* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt64Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					UInt64 * a = (UInt64 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					UInt64 * c = new UInt64((* a) + (UInt64)(* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt64Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					UInt64 * a = (UInt64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt64 * c = new UInt64((* a) + (UInt64)(* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt8Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (UInt64)(* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt8Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt8Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					UInt32 * c = new UInt32((UInt32)(* a) + (UInt32)(* b));
					return new Object(BasicType::UInt32Type, c);
				}
			},
			{
				{ BasicType::UInt8Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt16 * c = new UInt16((UInt16)(* a) + (UInt16)(* b));
					return new Object(BasicType::UInt16Type, c);
				}
			},
			{
				{ BasicType::UInt16Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					UInt16 * a = (UInt16 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (UInt64)(* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt16Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					UInt16 * a = (UInt16 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					UInt64 * c = new UInt64((UInt64)(* a) + (* b));
					return new Object(BasicType::UInt64Type, c);
				}
			},
			{
				{ BasicType::UInt16Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					UInt16 * a = (UInt16 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					UInt32 * c = new UInt32((UInt32)(* a) + (UInt32)(* b));
					return new Object(BasicType::UInt32Type, c);
				}
			},
			{
				{ BasicType::UInt16Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					UInt16 * a = (UInt16 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt32 * c = new UInt32((UInt32)(* a) + (UInt32)(* b));
					return new Object(BasicType::UInt32Type, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryIntUIntAddition = {
			{
				{ BasicType::Int32Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int32Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int32Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int32Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					Int32 * a = (Int32 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					Int32 * c = new Int32((Int32)(* a) + (Int32)(* b));
					return new Object(BasicType::Int32Type, c);
				}
			},
			{
				{ BasicType::Int8Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					Int8 * a = (Int8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int16 * c = new Int16((Int16)(* a) + (Int16)(* b));
					return new Object(BasicType::Int16Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::UInt32Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					UInt32 * b = (UInt32 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::UInt64Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					UInt64 * b = (UInt64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::UInt16Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					UInt16 * b = (UInt16 *) r -> value;
					Int32 * c = new Int32((Int32)(* a) + (Int32)(* b));
					return new Object(BasicType::Int32Type, c);
				}
			},
			{
				{ BasicType::Int16Type, BasicType::UInt8Type },
				[] (Object * l, Object * r) {
					Int16 * a = (Int16 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int32 * c = new Int32((Int32)(* a) + (Int32)(* b));
					return new Object(BasicType::Int32Type, c);
				}
			}
		};

		Map<BasicTypes, BinaryHandler> binaryMixedAddition = {
			{
				{ BasicType::RealType, BasicType::Int32Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int32 * b = (Int32 *) r -> value;
					Real * c = new Real((* a) + (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) + (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int8Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int8 * b = (Int8 *) r -> value;
					Real * c = new Real((* a) + (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int16Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int16 * b = (Int16 *) r -> value;
					Real * c = new Real((* a) + (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
		};

		Object * applyAddition(Token * t, Object * l, Object * r) {
			Boolean lInteger = l -> isIntegerType();
			Boolean rInteger = r -> isIntegerType();
			if (lInteger && rInteger) {
				lInteger = l -> isSignedType();
				rInteger = r -> isSignedType();
				if (lInteger && rInteger) {
					auto search = binaryIntIntAddition.find({ l -> type, r -> type});
					if (search != binaryIntIntAddition.end()) {
						auto handler = search -> second;
						return handler(l, r);
					}
					throw EvaluationError(
						"Binary operator '+' doesn't match operands of type '" +
						l -> getObjectName() + "' and '" +
						r -> getObjectName() + "'!", * t
					);
				} else if (lInteger && !rInteger) {
					auto search = binaryIntUIntAddition.find({ l -> type, r -> type});
					if (search != binaryIntUIntAddition.end()) {
						auto handler = search -> second;
						return handler(l, r);
					}
					throw EvaluationError(
						"Binary operator '+' doesn't match operands of type '" +
						l -> getObjectName() + "' and '" +
						r -> getObjectName() + "'!", * t
					);
				} else if (!lInteger && rInteger) {
					auto search = binaryIntUIntAddition.find({ r -> type, l -> type});
					if (search != binaryIntUIntAddition.end()) {
						auto handler = search -> second;
						return handler(l, r);
					}
					throw EvaluationError(
						"Binary operator '+' doesn't match operands of type '" +
						l -> getObjectName() + "' and '" +
						r -> getObjectName() + "'!", * t
					);
				} else {
					auto search = binaryUIntUIntAddition.find({ l -> type, r -> type});
					if (search != binaryUIntUIntAddition.end()) {
						auto handler = search -> second;
						return handler(l, r);
					}
					throw EvaluationError(
						"Binary operator '+' doesn't match operands of type '" +
						l -> getObjectName() + "' and '" +
						r -> getObjectName() + "'!", * t
					);
				}
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
