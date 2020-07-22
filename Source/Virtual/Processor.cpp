
/*!
 *
 *    + --------------------------------------- +
 *    |  Processor.cpp                          |
 *    |                                         |
 *    |            Virtual Processor            |
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

#include "Processor.hpp"

#ifndef SPIN_PROCESSOR_CPP
#define SPIN_PROCESSOR_CPP

#include <limits>

#include "../Utility/Converter.hpp"
#include "../Types/Complex.hpp"

#define DefineCastTable(A) const Dictionary<Types, Processor::Mutation> Processor::A
#define DefineBinaryTable(A) const Dictionary<Types, Processor::Process> Processor::A
#define DefineUnaryTable(A) const Dictionary<Type, Processor::Mutation> Processor::A
#define DefineImmutableTable(A) const Dictionary<Type, Processor::Immutable> Processor::A

#define binaryCase(T)                         \
	b = stack.pop();                          \
	a = stack.pop();                          \
	stack.push(                               \
		T.find(data.as.types) -> second(a, b) \
	)

#define binaryExceptionCase(T)                     \
	b = stack.pop();                               \
	a = stack.pop();                               \
	try {                                          \
		a = T.find(data.as.types) -> second(a, b); \
	} catch (Exception & e) {                      \
		auto search = program -> errors.find(ip);  \
		if (search != program -> errors.end()) {   \
			throw search -> second;                \
		} else return;                             \
	}                                              \
	stack.push(a);

#define unaryCase(T)                      \
	a = stack.pop();                      \
	stack.push(                           \
		T.find(data.as.type) -> second(a) \
	)

#define immutableCase(T)              \
	a = stack.pop();                  \
	T.find(data.as.type) -> second(a)

#define symmetric(A) compose(A, A)

#define makeBinaryFrom(L) [] (Value l, Value r) -> Value L
#define makeUnaryFrom(L) [] (Value r) -> Value L
#define makeCastFrom(L) [] (Value c) -> Value L
#define makeImmutableFrom(L) [] (Value r) L

namespace Spin {

	const Real Processor::infinity = std::numeric_limits<double>::infinity();
	const Real Processor::undefined = std::numeric_limits<double>::quiet_NaN();

	Array<Pair<Pointer, Type>> Processor::objects = { };

	// Attention! Has to be read from l to r: ((r)l).
	//            It will always return type of r.
	DefineCastTable(cast) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::ByteType),
			makeCastFrom({ return c; })
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeCastFrom({ return { .integer = (Int64)c.byte }; })
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeCastFrom({ return c; })
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeCastFrom({ return { .integer = (Int64)c.byte }; })
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeCastFrom({ return { .byte = (Byte)c.integer }; })
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeCastFrom({ return { .byte = (Byte)c.integer }; })
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeCastFrom({ return { .real = (Real)c.integer }; })
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeCastFrom({ return { .integer = (Int64)c.real }; })
		},
		// Basic Objects:
		{
			compose(Type::IntegerType, Type::ComplexType),
			makeCastFrom({
				Complex * complex = new Complex((Real)c.integer, 0.0);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::RealType, Type::ComplexType),
			makeCastFrom({
				Complex * complex = new Complex(c.real, 0.0);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ComplexType),
			makeCastFrom({
				Complex * complex = new Complex(0.0, c.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::IntegerType),
			makeCastFrom({
				return { .integer = (Int64)(((Complex *)c.pointer) -> a) };
			})
		},
		{
			compose(Type::ComplexType, Type::RealType),
			makeCastFrom({
				return { .real = (((Complex *)c.pointer) -> a) };
			})
		},
		{
			compose(Type::ComplexType, Type::ImaginaryType),
			makeCastFrom({
				return { .real = (((Complex *)c.pointer) -> b) };
			})
		},
		{
			compose(Type::CharacterType, Type::StringType),
			makeCastFrom({
				String * string = new String(1, (Character)c.byte);
				objects.push_back({ string, Type::StringType });
				return { .pointer = string };
			})
		},
	};

	DefineBinaryTable(addition) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) + (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) + (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) + r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) + (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) + (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) + r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)(l.integer + (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)(l.integer + (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = l.integer + r.integer };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.integer + r.real };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .real = l.real + r.integer };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.real + r.real };
			})
		},	
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = l.real + r.real };
			})
		},
		// Basic Objects:
		{
			compose(Type::IntegerType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = new Complex((Real)l.integer, r.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::IntegerType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					(complex -> a) + (Real)l.integer,
					(complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::RealType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = new Complex(l.real, r.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::RealType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					(complex -> a) + l.real,
					(complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::IntegerType),
			makeBinaryFrom({
				Complex * complex = new Complex((Real)r.integer, l.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::RealType),
			makeBinaryFrom({
				Complex * complex = new Complex(r.real, l.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					(complex -> a),
					(complex -> b) + l.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::IntegerType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) + (Real)r.integer,
					(complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::RealType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) + r.real,
					(complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a),
					(complex -> b) + r.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = new Complex(
					*((Complex *)l.pointer) +
					*((Complex *)r.pointer)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::StringType, Type::StringType),
			makeBinaryFrom({
				String * string = new String(
					(*((String *)(l.pointer))) +
					(*((String *)(r.pointer)))
				);
				objects.push_back({ string, Type::StringType });
				return { .pointer = string };
			})
		},
	};
	DefineBinaryTable(subtraction) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) - (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) - (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) - r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) - (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) - (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) - r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)(l.integer - (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)(l.integer - (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = l.integer - r.integer };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.integer - r.real };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .real = l.real - r.integer };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.real - r.real };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = l.real - r.real };
			})
		},
		// Basic Objects:
		{
			compose(Type::IntegerType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = new Complex((Real)l.integer, - r.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::IntegerType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					(Real)l.integer - (complex -> a),
					- (complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::RealType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = new Complex(l.real, - r.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::RealType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					l.real - (complex -> a),
					- (complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::IntegerType),
			makeBinaryFrom({
				Complex * complex = new Complex(-((Real)r.integer), l.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::RealType),
			makeBinaryFrom({
				Complex * complex = new Complex(- r.real, l.real);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					- (complex -> a),
					l.real - (complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::IntegerType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) - (Real)r.integer,
					(complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::RealType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) - r.real,
					(complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a),
					(complex -> b) - r.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = new Complex(
					*((Complex *)l.pointer) -
					*((Complex *)r.pointer)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
	};
	DefineBinaryTable(multiplication) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) * (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) * (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) * r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) * (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) * (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = (Int64)((Int64)(l.byte) * r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .integer = (Int64)(l.integer * (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return { .integer = (Int64)(l.integer * (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = l.integer * r.integer };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.integer * r.real };
			})
		},
		{
			compose(Type::IntegerType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = l.integer * r.real };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .real = l.real * r.integer };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.real * r.real };
			})
		},
		{
			compose(Type::RealType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = l.real * r.real };
			})
		},
		{
			compose(Type::ImaginaryType, Type::IntegerType),
			makeBinaryFrom({
				return { .real = l.real * r.integer };
			})
		},
		{
			compose(Type::ImaginaryType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.real * r.real };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = l.real * r.real };
			})
		},
		// Basic Objects:
		{
			compose(Type::IntegerType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					(complex -> a) * (Real)l.integer,
					(complex -> b) * (Real)l.integer
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::RealType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					(complex -> a) * l.real,
					(complex -> b) * l.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					- ((complex -> b) * l.real),
					(complex -> a) * l.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::IntegerType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) * (Real)r.integer,
					(complex -> b) * (Real)r.integer
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::RealType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) * r.real,
					(complex -> b) * r.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					- ((complex -> b) * r.real),
					(complex -> a) * r.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = new Complex(
					*((Complex *)l.pointer) *
					*((Complex *)r.pointer)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
	};
	DefineBinaryTable(division) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) / (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) / (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				if (!r.integer) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) / r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) / (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) / (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				if (!r.integer) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) / r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)(l.integer / (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)(l.integer / (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				if (!r.integer) throw Exception();
				return { .integer = l.integer / r.integer };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .real = (Real)(l.integer) / r.real };
			})
		},
		{
			compose(Type::IntegerType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = (Real)(l.integer) / r.real };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .real = l.real / (Real)(r.integer) };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.real / r.real };
			})
		},
		{
			compose(Type::RealType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = l.real / r.real };
			})
		},
		{
			compose(Type::ImaginaryType, Type::IntegerType),
			makeBinaryFrom({
				return { .real = l.real / (Real)(r.integer) };
			})
		},
		{
			compose(Type::ImaginaryType, Type::RealType),
			makeBinaryFrom({
				return { .real = l.real / r.real };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .real = l.real / r.real };
			})
		},
		// Basic Objects:
		{
			compose(Type::IntegerType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				l.real = ((Real)l.integer) / (complex -> getNormalised());
				complex = new Complex(
					l.real * (complex -> a),
					l.real * (- (complex -> b))
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::RealType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				l.real /= (complex -> getNormalised());
				complex = new Complex(
					l.real * (complex -> a),
					l.real * (- (complex -> b))
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = (Complex *)r.pointer;
				l.real /= (complex -> getNormalised());
				complex = new Complex(
					(complex -> b) * l.real,
					(complex -> a) * l.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::IntegerType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) / (Real)r.integer,
					(complex -> b) / (Real)r.integer
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::RealType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> a) / r.real,
					(complex -> b) / r.real
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ImaginaryType),
			makeBinaryFrom({
				Complex * complex = (Complex *)l.pointer;
				complex = new Complex(
					(complex -> b) / r.real,
					- ((complex -> a) / r.real)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
		{
			compose(Type::ComplexType, Type::ComplexType),
			makeBinaryFrom({
				Complex * complex = new Complex(
					*((Complex *)l.pointer) /
					*((Complex *)r.pointer)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
	};
	DefineBinaryTable(modulus) = {
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) % (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) % (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				if (!r.integer) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) % r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) % (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) % (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				if (!r.integer) throw Exception();
				return { .integer = (Int64)((Int64)(l.byte) % r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)(l.integer % (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				if (!r.byte) throw Exception();
				return { .integer = (Int64)(l.integer % (Int64)(r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				if (!r.integer) throw Exception();
				return { .integer = l.integer % r.integer };
			})
		},
	};

	DefineBinaryTable(bitwiseAND) = {
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = l.integer & r.integer };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .byte = (Byte)(l.byte & r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .byte = (Byte)(l.byte & r.byte) };
			})
		}
	};
	DefineBinaryTable(bitwiseOR) = {
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = l.integer | r.integer };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .byte = (Byte)(l.byte | r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .byte = (Byte)(l.byte | r.byte) };
			})
		}
	};
	DefineBinaryTable(bitwiseXOR) = {
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .integer = l.integer ^ r.integer };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .byte = (Byte)(l.byte ^ r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .byte = (Byte)(l.byte ^ r.byte) };
			})
		},
	};

	DefineUnaryTable(negation) = {
		{ Type::CharacterType, makeUnaryFrom({ return { .integer = - r.byte }; }) },
		{      Type::ByteType, makeUnaryFrom({ return { .integer = - r.byte }; }) },
		{   Type::IntegerType, makeUnaryFrom({ return { .integer = - r.integer }; }) },
		{      Type::RealType, makeUnaryFrom({ return { .real = - r.real }; }) },
		{ Type::ImaginaryType, makeUnaryFrom({ return { .real = - r.real }; }) },
		{
			Type::ComplexType,
			makeUnaryFrom({
				Complex * complex = (Complex *)r.pointer;
				complex = new Complex(
					- (complex -> a),
					- (complex -> b)
				);
				objects.push_back({ complex, Type::ComplexType });
				return { .pointer = complex };
			})
		},
	};
	DefineImmutableTable(print) = {
		// Basic Types:
		{   Type::BooleanType, makeImmutableFrom({ OStream << (r.boolean ? "true" : "false"); }) },
		{ Type::CharacterType, makeImmutableFrom({ OStream << (Character)r.byte; }) },
		{      Type::ByteType, makeImmutableFrom({ OStream << hexadecimal << (Int64)r.byte << decimal; }) },
		{   Type::IntegerType, makeImmutableFrom({ OStream << r.integer; }) },
		{      Type::RealType, makeImmutableFrom({ OStream << Converter::realToString(r.real); }) },
		{ Type::ImaginaryType, makeImmutableFrom({ OStream << Converter::realToString(r.real); }) },
		// Basic Objects:
		{   Type::ComplexType, makeImmutableFrom({ OStream << ((Complex *)r.pointer) -> toString(); }) },
		{    Type::StringType, makeImmutableFrom({ OStream << (*((String *)r.pointer)); }) },
	};

	DefineBinaryTable(inequality) = {
		// Basic Types:
		{
			compose(Type::BooleanType, Type::BooleanType),
			makeBinaryFrom({
				return { .boolean = (l.boolean != r.boolean) };
			})
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte != r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte != r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) != r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte != r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte != r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) != r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.integer != ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return{ .boolean = (l.integer != ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.integer != r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (((Real)l.integer) != r.real) };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.real != ((Real)r.integer)) };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (l.real != r.real) };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .boolean = (l.real != r.real) };
			})
		},
		// Basic Objects:
		{
			compose(Type::StringType, Type::StringType),
			makeBinaryFrom({
				return { .boolean = ((*((String *)l.pointer)) != (*(String *)r.pointer)) };
			})
		},
	};
	DefineBinaryTable(equality) = {
		// Basic Types:
		{
			compose(Type::BooleanType, Type::BooleanType),
			makeBinaryFrom({
				return { .boolean = (l.boolean == r.boolean) };
			})
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte == r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte == r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) == r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte == r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte == r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) == r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.integer == ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return{ .boolean = (l.integer == ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.integer == r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (((Real)l.integer) == r.real) };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.real == ((Real)r.integer)) };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (l.real == r.real) };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .boolean = (l.real == r.real) };
			})
		},
		// Basic Objects:
		{
			compose(Type::StringType, Type::StringType),
			makeBinaryFrom({
				return { .boolean = ((*((String *)l.pointer)) == (*(String *)r.pointer)) };
			})
		},
	};

	DefineBinaryTable(major) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte > r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte > r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) > r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte > r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte > r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) > r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.integer > ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return{ .boolean = (l.integer > ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.integer > r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (((Real)l.integer) > r.real) };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.real > ((Real)r.integer)) };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (l.real > r.real) };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .boolean = (l.real > r.real) };
			})
		},
	};
	DefineBinaryTable(majorEqual) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte >= r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte >= r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) >= r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte >= r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte >= r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) >= r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.integer >= ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return{ .boolean = (l.integer >= ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.integer >= r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (((Real)l.integer) >= r.real) };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.real >= ((Real)r.integer)) };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (l.real >= r.real) };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .boolean = (l.real >= r.real) };
			})
		},
	};
	DefineBinaryTable(minor) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte < r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte < r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) < r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte < r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte < r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) < r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.integer < ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return{ .boolean = (l.integer < ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.integer < r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (((Real)l.integer) < r.real) };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.real < ((Real)r.integer)) };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (l.real < r.real) };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .boolean = (l.real < r.real) };
			})
		},
	};
	DefineBinaryTable(minorEqual) = {
		// Basic Types:
		{
			compose(Type::CharacterType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte <= r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte <= r.byte) };
			})
		},
		{
			compose(Type::CharacterType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) <= r.integer) };
			})
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.byte <= r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::ByteType),
			makeBinaryFrom({
				return { .boolean = (l.byte <= r.byte) };
			})
		},
		{
			compose(Type::ByteType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (((Int64)l.byte) <= r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::CharacterType),
			makeBinaryFrom({
				return { .boolean = (l.integer <= ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::ByteType),
			makeBinaryFrom({
				return{ .boolean = (l.integer <= ((Int64)r.byte)) };
			})
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.integer <= r.integer) };
			})
		},
		{
			compose(Type::IntegerType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (((Real)l.integer) <= r.real) };
			})
		},
		{
			compose(Type::RealType, Type::IntegerType),
			makeBinaryFrom({
				return { .boolean = (l.real <= ((Real)r.integer)) };
			})
		},
		{
			compose(Type::RealType, Type::RealType),
			makeBinaryFrom({
				return { .boolean = (l.real <= r.real) };
			})
		},
		{
			compose(Type::ImaginaryType, Type::ImaginaryType),
			makeBinaryFrom({
				return { .boolean = (l.real <= r.real) };
			})
		},
	};

	void Processor::run(Program * program) {
		if (!program) return;
		instructions = program -> instructions;
		// Main:
		Value a, b;
		const SizeType count = instructions.size();
		for (SizeType ip = 0; ip < count; ip += 1) {
			const ByteCode data = instructions[ip];
			switch (data.code) {
				case OPCode::RST: continue;
				case OPCode::CNS: stack.push(data.as.value); break;
				case OPCode::GLB: globals.push_back(data.as.value); break;
				case OPCode::GGB: stack.push(globals[data.as.index]); break;
				case OPCode::SGB: globals[data.as.index] = stack.top(); break;
				case OPCode::GLC: stack.push(stack.at(data.as.index)); break;
				case OPCode::SLC: stack.edit(data.as.index, stack.top()); break;
				case OPCode::SWP:
					b = stack.pop();
					a = stack.pop();
					stack.push(stack.at((SizeType)a.integer));
					stack.push(stack.at((SizeType)b.integer));
					stack.edit((SizeType)a.integer, stack.pop());
					stack.edit((SizeType)b.integer, stack.pop());
				break;
				case OPCode::ADD: binaryCase(addition); break;
				case OPCode::SUB: binaryCase(subtraction); break;
				case OPCode::MUL: binaryCase(multiplication); break;
				case OPCode::DIV: binaryExceptionCase(division); break;
				case OPCode::MOD: binaryExceptionCase(modulus); break;
				case OPCode::NEG: unaryCase(negation); break;
				case OPCode::INV:
					if (data.as.type == Type::IntegerType) {
						a = stack.pop();
						stack.push({ .integer = ~ a.integer });
					} else {
						a = stack.pop();
						stack.push({ .byte = (Byte)(~ a.byte) });
					}
				break;
				case OPCode::PST: stack.push({ .boolean = true }); break;
				case OPCode::PSF: stack.push({ .boolean = false }); break;
				case OPCode::PSI: stack.push({ .real = infinity }); break;
				case OPCode::PSU: stack.push({ .real = undefined }); break;
				case OPCode::POP: stack.decrease(); break;
				case OPCode::DSK: stack.decrease(data.as.index); break;
				case OPCode::JMP: ip += data.as.index; break;
				case OPCode::JMB: ip -= data.as.index; break;
				case OPCode::JIF: if (!stack.pop().boolean) ip += data.as.index; break;
				case OPCode::JAF: if (!stack.top().boolean) ip += data.as.index; break;
				case OPCode::JIT: if (stack.pop().boolean) ip += data.as.index; break;
				case OPCode::JAT: if (stack.top().boolean) ip += data.as.index; break;
				case OPCode::EQL: binaryCase(equality); break;
				case OPCode::NEQ: binaryCase(inequality); break;
				case OPCode::GRT: binaryCase(major); break;
				case OPCode::GEQ: binaryCase(majorEqual); break;
				case OPCode::LSS: binaryCase(minor); break;
				case OPCode::LEQ: binaryCase(minorEqual); break;
				case OPCode::NOT: stack.push({ .boolean = !(stack.pop().boolean) }); break;
				case OPCode::BWA: binaryCase(bitwiseAND); break;
				case OPCode::BWO: binaryCase(bitwiseOR); break;
				case OPCode::BWX: binaryCase(bitwiseXOR); break;
				case OPCode::RET: break;
				case OPCode::CST: 
					a = stack.pop();
					stack.push(cast.find(data.as.types) -> second(a));
				break;
				case OPCode::PRN: immutableCase(print); break;
				case OPCode::NLN: OStream << endLine; break;
				case OPCode::HLT:
					// Free:
					stack.clear();
					freeLiterals(program);
					freeObjects();
					return;
				break;
				default: break; // TODO: Exception.
			}
		}
		// Free:
		stack.clear();
		freeLiterals(program);
		freeObjects();
	}

	void Processor::freeLiterals(Program * program) {
		for (auto & object : program -> objects) {
			switch (object.second) {
				case Type::ComplexType: delete ((String *)object.first); break;
				case  Type::StringType: delete ((Complex *)object.first); break;
				default: break;
			}
		}
		program -> objects.clear();
	}
	void Processor::freeObjects() {
		for (auto & object : objects) {
			switch (object.second) {
				case Type::ComplexType: delete ((String *)object.first); break;
				case  Type::StringType: delete ((String *)object.first); break;
				default: break;
			}
		}
		objects.clear();
	}

}

#undef DefineCastTable
#undef DefineBinaryTable
#undef DefineUnaryTable
#undef DefineImmutableTable
#undef binaryCase
#undef unaryCase
#undef immutableCase
#undef symmetric
#undef makeBinaryFrom
#undef makeUnaryFrom
#undef makeCastFrom
#undef makeImmutableFrom

#endif
