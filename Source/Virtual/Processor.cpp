
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

#define DefineCastTable(A) const Dictionary<Types, Processor::Mutation> Processor::A
#define DefineBinaryTable(A) const Dictionary<Types, Processor::Process> Processor::A
#define DefineUnaryTable(A) const Dictionary<Type, Processor::Mutation> Processor::A
#define DefineImmutableTable(A) const Dictionary<Type, Processor::Immutable> Processor::A

#define binaryCase(T)                       \
	b = stack.pop();                        \
	a = stack.pop();                        \
	stack.push(                             \
		T.find(data.as.types) -> second(a, b) \
	)

#define unaryCase(T)                        \
	a = stack.pop();                        \
	stack.push(                             \
		T.find(data.as.type) -> second(a)     \
	)
#define immutableCase(T)                    \
	a = stack.pop();                        \
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
	};
	DefineBinaryTable(multiplication) = {
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
	};
	DefineBinaryTable(division) = {
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
	};
	DefineImmutableTable(print) = {
		// Basic Types:
		{   Type::BooleanType, makeImmutableFrom({ OStream << (r.boolean ? "true" : "false"); }) },
		{ Type::CharacterType, makeImmutableFrom({ OStream << (Character)r.byte; }) },
		{      Type::ByteType, makeImmutableFrom({ OStream << hexadecimal << (Int64)r.byte << decimal; }) },
		{   Type::IntegerType, makeImmutableFrom({ OStream << r.integer; }) },
		{      Type::RealType, makeImmutableFrom({ OStream << r.real; }) },
		{ Type::ImaginaryType, makeImmutableFrom({ OStream << r.real; }) },
		// Basic Objects:
		{    Type::StringType, makeImmutableFrom({ OStream << (*((String *)r.pointer)); }) },
	};

	DefineBinaryTable(inequality) = { };
	DefineBinaryTable(equality) = { };

	DefineBinaryTable(major) = { };
	DefineBinaryTable(majorEqual) = { };
	DefineBinaryTable(minor) = { };
	DefineBinaryTable(minorEqual) = { };

	void Processor::run(Program * program) {
		if (!program) return;
		instructions = program -> instructions;
		// Main:
		Value a, b;
		const SizeType count = instructions.size();
		for (SizeType ip = 0; ip < count; ip += 1) {
			const ByteCode data = instructions[ip];
			switch (data.code) {
				case OPCode::RST: break;
				case OPCode::CNS: stack.push(data.as.value); break;
				case OPCode::GLB: globals.push_back(data.as.value); break;
				case OPCode::GGB: stack.push(globals[data.as.index]); break;
				case OPCode::SGB: globals[data.as.index] = stack.top(); break;
				case OPCode::GLC: stack.push(stack.at(data.as.index)); break;
				case OPCode::SLC: stack.edit(data.as.index, stack.top()); break;
				case OPCode::ADD: binaryCase(addition); break;
				case OPCode::SUB: binaryCase(subtraction); break;
				case OPCode::MUL: binaryCase(multiplication); break;
				case OPCode::DIV: break;
				case OPCode::MOD: break;
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
				case OPCode::JMP: ip += data.as.index; break;
				case OPCode::JIF: if (!stack.top().boolean) ip += data.as.index; break;
				case OPCode::EQL: binaryCase(equality); break;
				case OPCode::NEQ: binaryCase(inequality); break;
				case OPCode::GRT: binaryCase(major); break;
				case OPCode::GEQ: binaryCase(majorEqual); break;
				case OPCode::LSS: binaryCase(minor); break;
				case OPCode::LEQ: binaryCase(minorEqual); break;
				case OPCode::NOT: stack.push({ .boolean = !(stack.pop().boolean) }); break;
				case OPCode::AND:
					b = stack.pop();
					a = stack.pop();
					stack.push({ .boolean = a.boolean && b.boolean });
				break;
				case OPCode::ORR:
					b = stack.pop();
					a = stack.pop();
					stack.push({ .boolean = a.boolean || b.boolean });
				break;
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
				case OPCode::HLT: break; // TODO: set 'return;'.
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
				case Type::StringType: delete ((String *)object.first); break;
				default: break;
			}
		}
		program -> objects.clear();
	}
	void Processor::freeObjects() {
		for (auto & object : objects) {
			switch (object.second) {
				case Type::StringType: delete ((String *)object.first); break;
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
