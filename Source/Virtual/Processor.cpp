
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

namespace Spin {

	Processor::Crash::Crash(SizeType a, ByteCode b) {
		address = a; instruction = b;
	}
	ByteCode Processor::Crash::getInstruction() const {
		return instruction;
	}
	SizeType Processor::Crash::getAddress() const {
		return address;
	}

	const Real Processor::infinity = std::numeric_limits<double>::infinity();
	const Real Processor::undefined = std::numeric_limits<double>::quiet_NaN();

	Array<Pair<Pointer, Type>> Processor::objects;

	Value Processor::evaluate(Program * program) {
		if (!program) return { .integer = 0 };
		// Main:
		Value a, b, c;
		SizeType base = 0, ip = 0;
		const SizeType count = program -> instructions.size();
		while (ip < count) {
			const ByteCode data = program -> instructions[ip];
			switch (data.code) {
				case OPCode::RST: break;
				case OPCode::PSH: stack.push(data.as.value); break;
				case OPCode::STR:
					stack.push({
						.pointer = new String(program -> strings.at(
							data.as.index
						))
					});
				break;
				case OPCode::GET: stack.push(stack.at(data.as.index)); break;
				case OPCode::SET: stack.edit(data.as.index, stack.top()); break;
				case OPCode::SSF: frame.push(base); base = stack.size() - data.as.index; break;
				case OPCode::GLF: stack.push(stack.at(base + data.as.index)); break;
				case OPCode::SLF: stack.edit(base + data.as.index, stack.top()); break;
				case OPCode::CTP: c = stack.pop(); break;
				case OPCode::LTP: stack.push(c); break;
				case OPCode::SWP:
					b = stack.pop();
					a = stack.pop();
					stack.push(stack.at((SizeType)a.integer));
					stack.push(stack.at((SizeType)b.integer));
					stack.edit((SizeType)a.integer, stack.pop());
					stack.edit((SizeType)b.integer, stack.pop());
				break;
				case OPCode::ADD:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .integer = (Int64)((Int64)(a.byte) + (Int64)(b.byte)) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .integer = (Int64)((Int64)(a.byte) + b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .integer = (Int64)(a.integer + (Int64)(b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .integer = a.integer + b.integer });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .real = a.integer + b.real });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .real = a.real + b.integer });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .real = a.real + b.real });
						break;
						// Basic Objects:
						case compose(Type::IntegerType, Type::ImaginaryType): {
							Complex * complex = new Complex((Real)a.integer, b.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::IntegerType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								(complex -> a) + (Real)a.integer,
								(complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::RealType, Type::ImaginaryType): {
							Complex * complex = new Complex(a.real, b.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::RealType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								(complex -> a) + a.real,
								(complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::IntegerType): {
							Complex * complex = new Complex((Real)b.integer, a.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::RealType): {
							Complex * complex = new Complex(b.real, a.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								(complex -> a),
								(complex -> b) + a.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::IntegerType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) + (Real)b.integer,
								(complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::RealType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) + b.real,
								(complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ImaginaryType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a),
								(complex -> b) + b.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ComplexType): {
							Complex * complex = new Complex(
								*((Complex *)a.pointer) +
								*((Complex *)b.pointer)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::StringType, Type::CharacterType): {
							String * string = new String(*((String *)(a.pointer)));
							string -> push_back((Character)b.byte);
							objects.push_back({ string, Type::StringType });
							stack.push({ .pointer = string });
						} break;
						case compose(Type::CharacterType, Type::StringType): {
							String * string = new String(
								((Character)a.byte) +
								(*((String *)(b.pointer)))
							);
							objects.push_back({ string, Type::StringType });
							stack.push({ .pointer = string });
						} break;
						case compose(Type::StringType, Type::StringType): {
							String * string = new String(
								(*((String *)(a.pointer))) +
								(*((String *)(b.pointer)))
							);
							objects.push_back({ string, Type::StringType });
							stack.push({ .pointer = string });
						} break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::SUB:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .integer = (Int64)((Int64)(a.byte) - (Int64)(b.byte)) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .integer = (Int64)((Int64)(a.byte) - b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .integer = (Int64)(a.integer - (Int64)(b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .integer = a.integer - b.integer });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .real = a.integer - b.real });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .real = a.real - b.integer });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .real = a.real - b.real });
						break;
						// Basic Objects:
						case compose(Type::IntegerType, Type::ImaginaryType): {
							Complex * complex = new Complex((Real)a.integer, - b.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::IntegerType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								(Real)a.integer - (complex -> a),
								- (complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::RealType, Type::ImaginaryType): {
							Complex * complex = new Complex(a.real, - b.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::RealType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								a.real - (complex -> a),
								- (complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::IntegerType): {
							Complex * complex = new Complex(-((Real)b.integer), a.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::RealType): {
							Complex * complex = new Complex(- b.real, a.real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								- (complex -> a),
								a.real - (complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::IntegerType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) - (Real)b.integer,
								(complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::RealType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) - b.real,
								(complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ImaginaryType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a),
								(complex -> b) - b.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ComplexType): {
							Complex * complex = new Complex(
								*((Complex *)a.pointer) -
								*((Complex *)b.pointer)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::MUL:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .integer = (Int64)((Int64)(a.byte) * (Int64)(b.byte)) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .integer = (Int64)((Int64)(a.byte) * b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .integer = (Int64)(a.integer * (Int64)(b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .integer = a.integer * b.integer });
						break;
						case compose(Type::IntegerType, Type::RealType):
						case compose(Type::IntegerType, Type::ImaginaryType):
							stack.push({ .real = a.integer * b.real });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .real = a.real * b.integer });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::RealType, Type::ImaginaryType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .real = a.real * b.real });
						break;
						case compose(Type::ImaginaryType, Type::IntegerType):
							stack.push({ .real = a.real * b.integer });
						break;
						case compose(Type::ImaginaryType, Type::RealType):
							stack.push({ .real = a.real * b.real });
						break;
						// Basic Objects:
						case compose(Type::IntegerType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								(complex -> a) * (Real)a.integer,
								(complex -> b) * (Real)a.integer
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::RealType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								(complex -> a) * a.real,
								(complex -> b) * a.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							complex = new Complex(
								- ((complex -> b) * a.real),
								(complex -> a) * a.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::IntegerType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) * (Real)b.integer,
								(complex -> b) * (Real)b.integer
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::RealType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) * b.real,
								(complex -> b) * b.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ImaginaryType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								- ((complex -> b) * b.real),
								(complex -> a) * b.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ComplexType): {
							Complex * complex = new Complex(
								*((Complex *)a.pointer) *
								*((Complex *)b.pointer)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::DIV:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) / (Int64)(b.byte)) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							if (!b.integer) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) / b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)(a.integer / (Int64)(b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							if (!b.integer) throw Crash(ip, data);
							stack.push({ .integer = a.integer / b.integer });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .real = (Real)(a.integer) / b.real });
						break;
						case compose(Type::IntegerType, Type::ImaginaryType):
							stack.push({ .real = (Real)(a.integer) / b.real });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .real = a.real / (Real)(b.integer) });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::RealType, Type::ImaginaryType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
						case compose(Type::ImaginaryType, Type::RealType):
							stack.push({ .real = a.real / b.real });
						break;
						case compose(Type::ImaginaryType, Type::IntegerType):
							stack.push({ .real = a.real / (Real)(b.integer) });
						break;
						// Basic Objects:
						case compose(Type::IntegerType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							a.real = ((Real)a.integer) / (complex -> getNormalised());
							complex = new Complex(
								a.real * (complex -> a),
								a.real * (- (complex -> b))
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::RealType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							a.real /= (complex -> getNormalised());
							complex = new Complex(
								a.real * (complex -> a),
								a.real * (- (complex -> b))
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::ComplexType): {
							Complex * complex = (Complex *)b.pointer;
							a.real /= (complex -> getNormalised());
							complex = new Complex(
								(complex -> b) * a.real,
								(complex -> a) * a.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::IntegerType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) / (Real)b.integer,
								(complex -> b) / (Real)b.integer
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::RealType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> a) / b.real,
								(complex -> b) / b.real
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ImaginaryType): {
							Complex * complex = (Complex *)a.pointer;
							complex = new Complex(
								(complex -> b) / b.real,
								- ((complex -> a) / b.real)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::ComplexType): {
							Complex * complex = new Complex(
								*((Complex *)a.pointer) /
								*((Complex *)b.pointer)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::MOD:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						case compose(Type::CharacterType, Type::CharacterType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) % (Int64)(b.byte)) });
						break;
						case compose(Type::CharacterType, Type::ByteType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) % (Int64)(b.byte)) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
							if (!b.integer) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) % b.integer) });
						break;
						case compose(Type::ByteType, Type::CharacterType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) % (Int64)(b.byte)) });
						break;
						case compose(Type::ByteType, Type::ByteType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) % (Int64)(b.byte)) });
						break;
						case compose(Type::ByteType, Type::IntegerType):
							if (!b.integer) throw Crash(ip, data);
							stack.push({ .integer = (Int64)((Int64)(a.byte) % b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)(a.integer % (Int64)(b.byte)) });
						break;
						case compose(Type::IntegerType, Type::ByteType):
							if (!b.byte) throw Crash(ip, data);
							stack.push({ .integer = (Int64)(a.integer % (Int64)(b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							if (!b.integer) throw Crash(ip, data);
							stack.push({ .integer = a.integer % b.integer });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::BSL:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.type) {
						case Type::CharacterType:
						case      Type::ByteType:
							stack.push({ .byte = (Byte)(a.byte << (SizeType)b.integer) });
						break;
						case   Type::IntegerType:
							stack.push({ .integer = (a.integer << (SizeType)b.integer) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::BSR:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.type) {
						case Type::CharacterType:
						case      Type::ByteType:
							stack.push({ .byte = (Byte)(a.byte >> (SizeType)b.integer) });
						break;
						case   Type::IntegerType:
							stack.push({ .integer = (a.integer >> (SizeType)b.integer) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::BRL:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.type) {
						case Type::CharacterType:
						case      Type::ByteType:
							stack.push({ .byte = (Byte)(
								a.byte << (SizeType)b.integer |
								a.byte >> (8 - (SizeType)b.integer)
							) });
						break;
						case   Type::IntegerType:
							stack.push({ .integer = (
								a.integer << (SizeType)b.integer |
								a.integer >> (64 - (SizeType)b.integer)
							) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::BRR:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.type) {
						case Type::CharacterType:
						case      Type::ByteType:
							stack.push({ .byte = (Byte)(
								a.byte >> (SizeType)b.integer |
								a.byte << (8 - (SizeType)b.integer)
							) });
						break;
						case   Type::IntegerType:
							stack.push({ .integer = (
								a.integer >> (SizeType)b.integer |
								a.integer << (64 - (SizeType)b.integer)
							) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::NEG:
					switch (data.as.type) {
						case Type::CharacterType:
						case      Type::ByteType: stack.push({ .integer = - stack.pop().byte }); break;
						case   Type::IntegerType: stack.push({ .integer = - stack.pop().integer }); break;
						case      Type::RealType:
						case Type::ImaginaryType: stack.push({ .real = - stack.pop().real }); break;
						case   Type::ComplexType: {
							Complex * complex = (Complex *)stack.pop().pointer;
							complex = new Complex(
								- (complex -> a),
								- (complex -> b)
							);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::INV:
					switch (data.as.type) {
						case Type::IntegerType: stack.push({ .integer = ~(stack.pop().integer) });
						case    Type::ByteType: stack.push({ .byte = (Byte)(~(stack.pop().byte)) });
						default: return { .integer = 0 };
					}
				break;
				case OPCode::SSC:
					b = stack.pop();
					a = stack.pop();
					if (b.integer < 0 ||
						b.integer > (((String *)a.pointer) -> size()) - 1) throw Crash(ip, data);
					stack.push({
						.byte = (Byte)((String *)a.pointer) -> at(b.integer)
					});
				break;
				case OPCode::CCJ:
					stack.push({ .pointer = new Complex(((Complex *)stack.pop().pointer) -> getConjugate()) });
				break;
				case OPCode::VCJ: break;
				case OPCode::MCJ: break;
				case OPCode::PST: stack.push({ .boolean = true }); break;
				case OPCode::PSF: stack.push({ .boolean = false }); break;
				case OPCode::PSI: stack.push({ .real = infinity }); break;
				case OPCode::PSU: stack.push({ .real = undefined }); break;
				case OPCode::PEC: stack.push({ .pointer = new Complex() }); break;
				case OPCode::PES: stack.push({ .pointer = new String() }); break;
				case OPCode::POP: stack.decrease(); break;
				case OPCode::DSK: stack.decrease(data.as.index); break;
				case OPCode::JMP: ip += data.as.index; continue;
				case OPCode::JMB: ip -= data.as.index; continue;
				case OPCode::JIF: if (!stack.pop().boolean) { ip += data.as.index; continue; } break;
				case OPCode::JAF: if (!stack.top().boolean) { ip += data.as.index; continue; } break;
				case OPCode::JIT: if  (stack.pop().boolean) { ip += data.as.index; continue; } break;
				case OPCode::JAT: if  (stack.top().boolean) { ip += data.as.index; continue; } break;
				case OPCode::EQL:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::BooleanType, Type::BooleanType):
							stack.push({ .boolean = (a.boolean == b.boolean) });
						break;
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .boolean = (a.byte == b.byte) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .boolean = (((Int64)a.byte) == b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .boolean = (a.integer == ((Int64)b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .boolean = (a.integer == b.integer) });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .boolean = (((Real)a.integer) == b.real) });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .boolean = (a.real == ((Real)b.integer)) });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .boolean = (a.real == b.real) });
						break;
						// Basic Objects:
						case compose(Type::StringType, Type::StringType):
							stack.push({ .boolean = ((*((String *)a.pointer)) == (*(String *)b.pointer)) });
						break;
						case compose(Type::ComplexType, Type::ComplexType):
							stack.push({ .boolean = ((*((Complex *)a.pointer)) == (*(Complex *)b.pointer)) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::NEQ:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::BooleanType, Type::BooleanType):
							stack.push({ .boolean = (a.boolean != b.boolean) });
						break;
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .boolean = (a.byte != b.byte) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .boolean = (((Int64)a.byte) != b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .boolean = (a.integer != ((Int64)b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .boolean = (a.integer != b.integer) });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .boolean = (((Real)a.integer) != b.real) });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .boolean = (a.real != ((Real)b.integer)) });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .boolean = (a.real != b.real) });
						break;
						// Basic Objects:
						case compose(Type::StringType, Type::StringType):
							stack.push({ .boolean = ((*((String *)a.pointer)) != (*(String *)b.pointer)) });
						break;
						case compose(Type::ComplexType, Type::ComplexType):
							stack.push({ .boolean = ((*((Complex *)a.pointer)) != (*(Complex *)b.pointer)) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::GRT:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .boolean = (a.byte > b.byte) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .boolean = (((Int64)a.byte) > b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .boolean = (a.integer > ((Int64)b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .boolean = (a.integer > b.integer) });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .boolean = (((Real)a.integer) > b.real) });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .boolean = (a.real > ((Real)b.integer)) });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .boolean = (a.real > b.real) });
						break;
						// Basic Objects:
						case compose(Type::StringType, Type::StringType):
							stack.push({ .boolean = ((*((String *)a.pointer)) > (*(String *)b.pointer)) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::GEQ:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .boolean = (a.byte >= b.byte) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .boolean = (((Int64)a.byte) >= b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .boolean = (a.integer >= ((Int64)b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .boolean = (a.integer >= b.integer) });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .boolean = (((Real)a.integer) >= b.real) });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .boolean = (a.real >= ((Real)b.integer)) });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .boolean = (a.real >= b.real) });
						break;
						// Basic Objects:
						case compose(Type::StringType, Type::StringType):
							stack.push({ .boolean = ((*((String *)a.pointer)) >= (*(String *)b.pointer)) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::LSS:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .boolean = (a.byte < b.byte) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .boolean = (((Int64)a.byte) < b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .boolean = (a.integer < ((Int64)b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .boolean = (a.integer < b.integer) });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .boolean = (((Real)a.integer) < b.real) });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .boolean = (a.real < ((Real)b.integer)) });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .boolean = (a.real < b.real) });
						break;
						// Basic Objects:
						case compose(Type::StringType, Type::StringType):
							stack.push({ .boolean = ((*((String *)a.pointer)) < (*(String *)b.pointer)) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::LEQ:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::CharacterType):
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType):
						case compose(Type::ByteType, Type::ByteType):
							stack.push({ .boolean = (a.byte <= b.byte) });
						break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .boolean = (((Int64)a.byte) <= b.integer) });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .boolean = (a.integer <= ((Int64)b.byte)) });
						break;
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .boolean = (a.integer <= b.integer) });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .boolean = (((Real)a.integer) <= b.real) });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .boolean = (a.real <= ((Real)b.integer)) });
						break;
						case compose(Type::RealType, Type::RealType):
						case compose(Type::ImaginaryType, Type::ImaginaryType):
							stack.push({ .boolean = (a.real <= b.real) });
						break;
						// Basic Objects:
						case compose(Type::StringType, Type::StringType):
							stack.push({ .boolean = ((*((String *)a.pointer)) <= (*(String *)b.pointer)) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::NOT: stack.push({ .boolean = !(stack.pop().boolean) }); break;
				case OPCode::BWA:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .integer = a.integer & b.integer });
						break;
						case compose(Type::ByteType, Type::ByteType):
						case compose(Type::CharacterType, Type::CharacterType):
							stack.push({ .byte = (Byte)(a.byte & b.byte) });
						break;
						case compose(Type::BooleanType, Type::BooleanType):
							stack.push({ .boolean = a.boolean && b.boolean });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::BWO:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .integer = a.integer | b.integer });
						break;
						case compose(Type::ByteType, Type::ByteType):
						case compose(Type::CharacterType, Type::CharacterType):
							stack.push({ .byte = (Byte)(a.byte | b.byte) });
						break;
						case compose(Type::BooleanType, Type::BooleanType):
							stack.push({ .boolean = a.boolean || b.boolean });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::BWX:
					b = stack.pop();
					a = stack.pop();
					switch (data.as.types) {
						case compose(Type::IntegerType, Type::IntegerType):
							stack.push({ .integer = a.integer ^ b.integer });
						break;
						case compose(Type::ByteType, Type::ByteType):
						case compose(Type::CharacterType, Type::CharacterType):
							stack.push({ .byte = (Byte)(a.byte ^ b.byte) });
						break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::CAL: call.push(ip); ip = data.as.index; continue;
				case OPCode::RET: base = frame.pop(); ip = call.pop(); break;
				case OPCode::CST:
					// Attention! Has to be read from l to r: ((r)l).
					//            It will always return type of r.
					switch (data.as.types) {
						// Basic Types:
						case compose(Type::CharacterType, Type::ByteType):
						case compose(Type::ByteType, Type::CharacterType): break;
						case compose(Type::CharacterType, Type::IntegerType):
						case compose(Type::ByteType, Type::IntegerType):
							stack.push({ .integer = (Int64)stack.pop().byte });
						break;
						case compose(Type::IntegerType, Type::CharacterType):
						case compose(Type::IntegerType, Type::ByteType):
							stack.push({ .byte = (Byte)stack.pop().integer });
						break;
						case compose(Type::IntegerType, Type::RealType):
							stack.push({ .real = (Real)stack.pop().integer });
						break;
						case compose(Type::RealType, Type::IntegerType):
							stack.push({ .integer = (Int64)stack.pop().real });
						break;
						// Basic Objects:
						case compose(Type::IntegerType, Type::ComplexType): {
							Complex * complex = new Complex((Real)stack.pop().integer, 0.0);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::RealType, Type::ComplexType): {
							Complex * complex = new Complex(stack.pop().real, 0.0);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ImaginaryType, Type::ComplexType): {
							Complex * complex = new Complex(0.0, stack.pop().real);
							objects.push_back({ complex, Type::ComplexType });
							stack.push({ .pointer = complex });
						} break;
						case compose(Type::ComplexType, Type::IntegerType): {
							stack.push({ .integer = (Int64)(((Complex *)stack.pop().pointer) -> a) });
						} break;
						case compose(Type::ComplexType, Type::RealType): {
							stack.push({ .real = (((Complex *)stack.pop().pointer) -> a) });
						} break;
						case compose(Type::ComplexType, Type::ImaginaryType): {
							stack.push({ .real = (((Complex *)stack.pop().pointer) -> b) });
						} break;
						case compose(Type::CharacterType, Type::StringType): {
							String * string = new String(1, (Character)stack.pop().byte);
							objects.push_back({ string, Type::StringType });
							stack.push({ .pointer = string });
						} break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::PRT:
					switch (data.as.type) {
						// Basic Types:
						case   Type::BooleanType: OStream << (stack.pop().boolean ? "true" : "false"); break;
						case Type::CharacterType: OStream << (Character)stack.pop().byte; break;
						case      Type::ByteType: OStream << hexadecimal << (Int64)stack.pop().byte << decimal; break;
						case   Type::IntegerType: OStream << stack.pop().integer; break;
						case      Type::RealType: OStream << Converter::realToString(stack.pop().real); break;
						case Type::ImaginaryType: OStream << Converter::imaginaryToString(stack.pop().real); break;
						// Basic Objects:
						case   Type::ComplexType: OStream << ((Complex *)stack.pop().pointer) -> toString(); break;
						case    Type::StringType: OStream << (*((String *)stack.pop().pointer)); break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::PRL:
					switch (data.as.type) {
						// Basic Types:
						case   Type::BooleanType: OStream << (stack.pop().boolean ? "true" : "false") << endLine; break;
						case Type::CharacterType: OStream << (Character)stack.pop().byte << endLine; break;
						case      Type::ByteType: OStream << hexadecimal << (Int64)stack.pop().byte << decimal << endLine; break;
						case   Type::IntegerType: OStream << stack.pop().integer << endLine; break;
						case      Type::RealType: OStream << Converter::realToString(stack.pop().real) << endLine; break;
						case Type::ImaginaryType: OStream << Converter::imaginaryToString(stack.pop().real) << endLine; break;
						// Basic Objects:
						case   Type::ComplexType: OStream << ((Complex *)stack.pop().pointer) -> toString() << endLine; break;
						case    Type::StringType: OStream << (*((String *)stack.pop().pointer)) << endLine; break;
						default: return { .integer = 0 };
					}
				break;
				case OPCode::NLN: OStream << endLine; break;
				case OPCode::HLT:
					// Free:
					stack.clear();
					freeObjects();
					return { .integer = 0 };
				break;
				default: break; // TODO: Exception.
			}
			ip += 1;
		}
		if (stack.isEmpty()) return { .integer = 0 };
		return stack.pop();
	}

	void Processor::run(Program * program) {
		try { evaluate(program); }
		catch (Processor::Crash & c) { throw; }
		stack.clear();
		freeObjects();
	}

	Value Processor::fold(Array<ByteCode> code) {
		Program * program = new Program();
		program -> instructions = code;
		try { return evaluate(program); }
		catch (Processor::Crash & c) { throw; }
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

#endif
