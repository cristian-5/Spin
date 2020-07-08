
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

#ifndef SPIN_PROCESSOR
#define SPIN_PROCESSOR

#define DefineBinaryTable(A) const Dictionary<Types, Processor::Process> Processor::A
#define DefineUnaryTable(A) const Dictionary<Type, Processor::Mutation> Processor::A

#define binaryCase(T)                       \
	b = stack.pop();                        \
	a = stack.pop();                        \
	stack.push(                             \
		T.find(ip.as.types) -> second(a, b) \
	)                                       \

#define unaryCase(T)                        \
	a = stack.pop();                        \
	stack.push(                             \
		T.find(ip.as.type) -> second(a)     \
	)                                       \

#define symmetric(A) compose(A, A)

namespace Spin {

	DefineBinaryTable(addition) = {
		{
			compose(Type::CharacterType, Type::CharacterType),
			[] (Value l, Value r) -> Value {
				return { .byte = (Character)(l.byte + r.byte) };
			}
		},
		{
			compose(Type::ByteType, Type::CharacterType),
			[] (Value l, Value r) -> Value {
				return { .byte = (Byte)(l.byte + r.byte) };
			}
		},
		{
			compose(Type::CharacterType, Type::ByteType),
			[] (Value l, Value r) -> Value {
				return { .byte = (Byte)(l.byte + r.byte) };
			}
		},
		{
			compose(Type::IntegerType, Type::IntegerType),
			[] (Value l, Value r) -> Value {
				return { .integer = l.integer + r.integer };
			}
		},
		{
			compose(Type::RealType, Type::RealType),
			[] (Value l, Value r) -> Value {
				return { .real = l.real + r.real };
			}
		},
		
		{
			compose(Type::IntegerType, Type::RealType),
			[] (Value l, Value r) -> Value {
				return { .real = l.integer + r.real };
			}
		}
	};

	DefineBinaryTable(subtraction) = { };
	DefineBinaryTable(multiplication) = { };
	DefineBinaryTable(division) = { };
	DefineBinaryTable(modulus) = { };

	DefineBinaryTable(smartAND) = {
		{
			compose(Type::IntegerType, Type::IntegerType),
			[] (Value l, Value r) -> Value {
				return { .integer = l.integer & r.integer };
			}
		},
		{
			compose(Type::ByteType, Type::ByteType),
			[] (Value l, Value r) -> Value {
				return { .byte = l.byte & r.byte };
			}
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			[] (Value l, Value r) -> Value {
				return { .byte = l.byte & r.byte };
			}
		},
		{
			compose(Type::BooleanType, Type::BooleanType),
			[] (Value l, Value r) -> Value {
				return { .boolean = l.boolean && r.boolean };
			}
		}
	};
	DefineBinaryTable(smartOR) = {
		{
			compose(Type::IntegerType, Type::IntegerType),
			[] (Value l, Value r) -> Value {
				return { .integer = l.integer | r.integer };
			}
		},
		{
			compose(Type::ByteType, Type::ByteType),
			[] (Value l, Value r) -> Value {
				return { .byte = l.byte | r.byte };
			}
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			[] (Value l, Value r) -> Value {
				return { .byte = l.byte | r.byte };
			}
		},
		{
			compose(Type::BooleanType, Type::BooleanType),
			[] (Value l, Value r) -> Value {
				return { .boolean = l.boolean || r.boolean };
			}
		}
	};
	DefineBinaryTable(smartXOR) = {
		{
			compose(Type::IntegerType, Type::IntegerType),
			[] (Value l, Value r) -> Value {
				return { .integer = l.integer ^ r.integer };
			}
		},
		{
			compose(Type::ByteType, Type::ByteType),
			[] (Value l, Value r) -> Value {
				return { .byte = l.byte ^ r.byte };
			}
		},
		{
			compose(Type::CharacterType, Type::CharacterType),
			[] (Value l, Value r) -> Value {
				return { .byte = l.byte ^ r.byte };
			}
		},
		{
			compose(Type::BooleanType, Type::BooleanType),
			[] (Value l, Value r) -> Value {
				return { .boolean = l.boolean != r.boolean };
			}
		}
	};

	DefineUnaryTable(negation) = {
		{ Type::IntegerType, [] (Value a) -> Value { return { .integer = - a.integer }; } },
		{ Type::RealType, [] (Value a) -> Value { return { .real = - a.real }; } },
		{ Type::ImaginaryType, [] (Value a) -> Value { return { .real = - a.real }; } },
		// TODO: Complex, Vector.
	};

	void Processor::run(Program * program) {
		if (!program) return;
		instructions = program -> instructions;
		literals = program -> literals;
		// Main:
		Value a, b;
		for (ByteCode ip : instructions) {
			switch (ip.code) {
				case OPCode::RST: break;
				case OPCode::CNS: stack.push(literals[ip.as.index]); break;
				case OPCode::ADD: binaryCase(addition); break;
				case OPCode::SUB: binaryCase(subtraction); break;
				case OPCode::MUL: binaryCase(multiplication); break;
				case OPCode::DIV: binaryCase(division); break;
				case OPCode::MOD: binaryCase(modulus); break;
				case OPCode::NEG: unaryCase(negation); break;
				case OPCode::INV:
					if (ip.as.type == Type::IntegerType) {
						a = stack.pop();
						stack.push({ .integer = ~ a.integer });
					} else {
						a = stack.pop();
						stack.push({ .byte = (Byte)(~(Integer)a.byte) });
					}
				break;
				case OPCode::PST: stack.push({ .boolean = true }); break;
				case OPCode::PSF: stack.push({ .boolean = false }); break;
				case OPCode::NOT:
					if (ip.as.type == Type::BooleanType) {
						stack.push({ .boolean = !(stack.pop().boolean) });
					} else stack.push({ .boolean = !(stack.pop().integer) });
				break;
				case OPCode::AND: binaryCase(smartAND); break;
				case OPCode::ORR: binaryCase(smartOR); break;
				case OPCode::XOR: binaryCase(smartXOR); break;
				case OPCode::RET: break;
				default: break; // TODO: Exception.
			}
		}
		std::cout << (int)stack.top().integer << std::endl;
		// Free:
		stack.clear();
	}

}

#endif
