
/*!
 *
 *    + --------------------------------------- +
 *    |  Decompiler.cpp                         |
 *    |                                         |
 *    |               Decompiler                |
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

#include "Decompiler.hpp"

#ifndef SPIN_DECOMPILER_CPP
#define SPIN_DECOMPILER_CPP

#include <vector>

#define reset "\x1B[0m"

namespace Spin {

	const String Decompiler::colours[] = {
		   [Decompiler::Colour::red] = { "\x1B[38;5;160m" },
		[Decompiler::Colour::yellow] = { "\x1B[38;5;220m" },
		 [Decompiler::Colour::white] = { "\x1B[38;5;15m" },
		 [Decompiler::Colour::black] = { "\x1B[38;5;0m" },
		[Decompiler::Colour::orange] = { "\x1B[38;5;202m" },
		  [Decompiler::Colour::blue] = { "\x1B[38;5;33m" },
		 [Decompiler::Colour::green] = { "\x1B[38;5;34m" },
		[Decompiler::Colour::purple] = { "\x1B[38;5;164m" },
		  [Decompiler::Colour::pink] = { "\x1B[38;5;207m" },
		  [Decompiler::Colour::grey] = { "\x1B[38;5;250m" },
		   [Decompiler::Colour::sky] = { "\x1B[38;5;14m" },
		 [Decompiler::Colour::acqua] = { "\x1B[38;5;30m" },
		 [Decompiler::Colour::peach] = { "\x1B[38;5;211m" },
		  [Decompiler::Colour::dark] = { "\x1B[38;5;242m" },
	};

	void Decompiler::tableOP(String o, String s) {
		if (s.length() > 16) {
			s = "\"" + s.substr(0, 15) + "...\"";
		} else {
			s = "\"" + s + "\"";
		}
		if (na) {
			OStream << "    " << o << "    " << s << endLine;
			return;
		}
		OStream << "    " << colours[Colour::green] << o << reset
				<< "    " << colours[Colour::acqua] << s << reset << endLine;
	}
	void Decompiler::aloneOP(String o, Colour c, String h) {
		if (na) {
			OStream << "    " << o
					<< "                        "
					<< "! " << h << endLine;
			return;
		}
		OStream << "    " << colours[c] << o << reset
				<< "                        " << colours[Colour::grey]
				<< "! " << h << reset << endLine;
	}
	void Decompiler::constOP(String o, Int64 i, Colour c) {
		if (na) {
			OStream << "    " << o << "    "
					<< upperCase << hexadecimal
					<< i << endLine;
			return;
		}
		OStream << "    " << colours[c] << o << reset
				<< "    " << colours[Colour::acqua]
				<< upperCase << hexadecimal << i << reset << endLine;
	}
	void Decompiler::typesOP(String o, Types x, Colour c, String h) {
		Type a = (Type)((x & 0xFF00) >> 8);
		Type b = (Type)(x & 0x00FF);
		if (na) {
			OStream << "    " << o
				<< "    " << resolve(a) << ", "
				<< resolve(b) << "            "
				<< "! " << h << endLine;
			return;
		}
		Colour w = (a <= Type::ImaginaryType ? Colour::orange : Colour::pink);
		Colour k = (b <= Type::ImaginaryType ? Colour::orange : Colour::pink);
		OStream << "    " << colours[c] << o << reset
				<< "    " << colours[w] << resolve(a)
				<< reset << ", " << colours[k]
				<< resolve(b) << reset
				<< "            " << colours[Colour::grey]
				<< "! " << h << reset << endLine;
	}
	void Decompiler::unaryOP(String o, Type x, Colour c, String h) {
		Colour a = (x <= Type::ImaginaryType ? Colour::orange : Colour::pink);
		if (na) {
			OStream << "    " << o << "    "
				<< resolve(x) << "                 "
				<< "! " << h << endLine;
			return;
		}
		OStream << "    " << colours[c] << o << reset
				<< "    " << colours[a]
				<< resolve(x) << reset
				<< "                 " << colours[Colour::grey]
				<< "! " << h << reset << endLine;
	}
	void Decompiler::jmptoOP(String o, SizeType x, String h) {
		if (na) {
			OStream << "    " << o << "    "
				<< upperCase << hexadecimal
				<< padding(16) << x
				<< "    ! " << h << endLine;
			return;
		}
		OStream << "    " << colours[Colour::red] << o << reset
				<< "    " << colours[Colour::acqua]
				<< upperCase << hexadecimal
				<< padding(16) << x << reset
				<< "    " << colours[Colour::grey]
				<< "! " << h << reset << endLine;
	}
	void Decompiler::smallOP(String o, Byte b, Colour c, String h) {
		if (na) {
			OStream << "    " << o << "    "
					<< upperCase << hexadecimal << padding(2)
					<< (UInt64)b << "                  !" << h << endLine;
			return;
		}
		OStream << "    " << colours[c] << o << reset
				<< "    " << colours[Colour::acqua]
				<< upperCase << hexadecimal  << padding(2)
				<< (UInt64)b << reset << colours[Colour::grey]
				<< "                  !" << h << endLine;
	}

	void Decompiler::rest_OP() {
		if (na) {
			OStream << "    "
					<< "RST    --------------------------------------------"
					<< endLine;
			return;
		}
		OStream << "    " << colours[Colour::grey]
				<< "RST    --------------------------------------------"
				<< reset << endLine;
	}
	void Decompiler::type_OP(String o, Type t, Colour c) {
		if (na) {
			OStream << "    " << o << "    " << resolve(t)
					<< "                   ! type info" << endLine;
			return;
		}
		OStream << "    " << colours[c] << o << reset
				<< "    " << colours[Colour::acqua] << resolve(t)
				<< colours[Colour::grey] << "                   ! type info"
				<< reset << endLine;
	}

	String Decompiler::resolve(Type type) {
		switch (type) {
			case Type::BooleanType: return "BLN";
			case Type::CharacterType: return "CHR";
			case Type::ByteType: return "BYT";
			case Type::IntegerType: return "INT";
			case Type::RealType: return "REA";
			case Type::ImaginaryType: return "IMG";
			case Type::ComplexType: return "CPX";
			case Type::StringType: return "STR";
			case Type::ArrayType: return "ARR";
			case Type::EmptyArray: return "ARR";
			case Type::VoidType: return "VOD";
			default: return "UNK";
		}
	}

	void Decompiler::decompile(Program * program, SizeType index) {
		const ByteCode byte = program -> instructions.at(index);
		if (na) {
			OStream << upperCase
					<< hexadecimal << padding(8) << index;
		} else {
			OStream << colours[Colour::dark] << upperCase
					<< hexadecimal << padding(8) << index;
		}
		switch (byte.code) {
			case OPCode::RST: rest_OP(); break;
			case OPCode::PSH: constOP("PSH", byte.as.value.integer, Colour::green); break;
			case OPCode::STR: tableOP("STR", program -> strings.at(byte.as.index)); break;
			case OPCode::TYP: type_OP("TYP", byte.as.type, Colour::green); break;
			case OPCode::LLA: aloneOP("LLA", Colour::red, "load lamda address"); break;
			case OPCode::ULA: aloneOP("ULA", Colour::red, "unload lamda address"); break;
			case OPCode::LAM: aloneOP("LAM", Colour::red, "lamda call"); break;
			case OPCode::GET: constOP("GET", byte.as.index, Colour::purple); break;
			case OPCode::SET: constOP("SET", byte.as.index, Colour::purple); break;
			case OPCode::SSF: constOP("SSF", byte.as.index, Colour::purple); break;
			case OPCode::GLF: constOP("GLF", byte.as.index, Colour::purple); break;
			case OPCode::SLF: constOP("SLF", byte.as.index, Colour::purple); break;
			case OPCode::CTP: aloneOP("CTP", Colour::blue, "copy temporary"); break;
			case OPCode::LTP: aloneOP("LTP", Colour::blue, "load temporary"); break;
			case OPCode::SWP: aloneOP("SWP", Colour::blue, "swap"); break;
			case OPCode::ADD: typesOP("ADD", byte.as.types, Colour::blue, "addition"); break;
			case OPCode::SUB: typesOP("SUB", byte.as.types, Colour::blue, "subtraction"); break;
			case OPCode::MUL: typesOP("MUL", byte.as.types, Colour::blue, "multiplication"); break;
			case OPCode::DIV: typesOP("DIV", byte.as.types, Colour::blue, "division"); break;
			case OPCode::MOD: typesOP("MOD", byte.as.types, Colour::blue, "modulus"); break;
			case OPCode::BSL: unaryOP("BSL", byte.as.type, Colour::purple, "shift left"); break;
			case OPCode::BSR: unaryOP("BSR", byte.as.type, Colour::purple, "shift right"); break;
			case OPCode::BRL: unaryOP("BRL", byte.as.type, Colour::purple, "rotate left"); break;
			case OPCode::BRR: unaryOP("BRR", byte.as.type, Colour::purple, "rotate right"); break;
			case OPCode::NEG: unaryOP("NEG", byte.as.type, Colour::purple, "negation"); break;
			case OPCode::INV: unaryOP("INV", byte.as.type, Colour::purple, "inversion"); break;
			case OPCode::SSC: aloneOP("SSC", Colour::purple, "string subscription"); break;
			case OPCode::ASC: aloneOP("ASC", Colour::purple, "array subscription"); break;
			case OPCode::CCJ: aloneOP("CCJ", Colour::purple, "complex conjugate"); break;
			case OPCode::VCJ: aloneOP("VCJ", Colour::purple, "vector conjugate"); break;
			case OPCode::MCJ: aloneOP("VCJ", Colour::purple, "matrix conjugate"); break;
			case OPCode::PST: aloneOP("PST", Colour::green, "push true"); break;
			case OPCode::PSF: aloneOP("PSF", Colour::green, "push false"); break;
			case OPCode::PSI: aloneOP("PSI", Colour::green, "push infinity"); break;
			case OPCode::PSU: aloneOP("PSU", Colour::green, "push undefined"); break;
			case OPCode::PEC: aloneOP("PEC", Colour::green, "push empty complex"); break;
			case OPCode::PES: aloneOP("PES", Colour::green, "push empty string"); break;
			case OPCode::PSA: constOP("PSA", byte.as.value.integer, Colour::green); break;
			case OPCode::PEA: aloneOP("PEA", Colour::green, "push empty array"); break;
			case OPCode::POP: aloneOP("POP", Colour::yellow, "pop"); break;
			case OPCode::DSK: constOP("DSK", byte.as.value.integer, Colour::yellow); break;
			case OPCode::JMP: jmptoOP("JMP", byte.as.index, "jump"); break;
			case OPCode::JIF: jmptoOP("JIF", byte.as.index, "jump if false"); break;
			case OPCode::JAF: jmptoOP("JAF", byte.as.index, "jump if false, avoid pop"); break;
			case OPCode::JIT: jmptoOP("JIT", byte.as.index, "jump if true"); break;
			case OPCode::JAT: jmptoOP("JAT", byte.as.index, "jump if true, avoid pop"); break;
			case OPCode::EQL: typesOP("EQL", byte.as.types, Colour::orange, "equal"); break;
			case OPCode::NEQ: typesOP("NEQ", byte.as.types, Colour::orange, "not equal"); break;
			case OPCode::GRT: typesOP("GRT", byte.as.types, Colour::orange, "great"); break;
			case OPCode::GEQ: typesOP("GEQ", byte.as.types, Colour::orange, "great equal"); break;
			case OPCode::LSS: typesOP("LSS", byte.as.types, Colour::orange, "less"); break;
			case OPCode::LEQ: typesOP("LEQ", byte.as.types, Colour::orange, "less equal"); break;
			case OPCode::NOT: aloneOP("NOT", Colour::yellow, "logic not"); break;
			case OPCode::BWA: typesOP("BWA", byte.as.types, Colour::blue, "bitwise and"); break;
			case OPCode::BWO: typesOP("BWO", byte.as.types, Colour::blue, "bitwise or"); break;
			case OPCode::BWX: typesOP("BWX", byte.as.types, Colour::blue, "bitwise xor"); break;
			case OPCode::CAL: jmptoOP("CAL", byte.as.index, "call"); break;
			case OPCode::RET: aloneOP("RET", Colour::red, "return"); break;
			case OPCode::CST: typesOP("CST", byte.as.types, Colour::orange, "cast"); break;
			case OPCode::INT: smallOP("INT", byte.as.type, Colour::peach, "interrupt"); break;
			case OPCode::HLT: aloneOP("HLT", Colour::red, "halt"); break;
			default: break;
		}
		OStream << decimal;
	}
	void Decompiler::decompile(Program * program, Boolean noAnsi) {
		OStream << endLine;
		na = noAnsi;
		for (SizeType i = 0; i < program -> instructions.size(); i += 1) {
			decompile(program, i);
		}
		OStream << endLine;
	}

}

#undef reset

#endif
