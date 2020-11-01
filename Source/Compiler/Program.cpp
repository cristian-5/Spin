
/*!
 *
 *    + --------------------------------------- +
 *    |  Program.cpp                            |
 *    |                                         |
 *    |                 Program                 |
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

#include "Program.hpp"

#ifndef SPIN_PROGRAM_CPP
#define SPIN_PROGRAM_CPP

#include "../Utility/Serialiser.hpp"
#include "../Manager/Manager.hpp"

namespace Spin {

	CodeUnit::CodeUnit(Array<Token> * tokens, String * name, String * contents) {
		this -> tokens = tokens;
		this -> name = name;
		this -> contents = contents;
	}
	CodeUnit::~CodeUnit() {
		if (tokens) delete tokens;
		if (name) delete name;
		if (contents) delete contents;
	}

	Program::Error::Error(CodeUnit * c, String m, Token t, ErrorCode e) {
		file = * c -> name;
		message = m;
		line = Manager::getLine(c -> contents, t.position);
		positionStart = t.position;
		positionEnd = t.position + t.lexeme.length();
		error = e;
	}
	String Program::Error::getFile() const {
		return file;
	}
	String Program::Error::getMessage() const {
		return message;
	}
	UInt32 Program::Error::getLine() const {
		return line;
	}
	UInt32 Program::Error::getPositionStart() const {
		return positionStart;
	}
	UInt32 Program::Error::getPositionEnd() const {
		return positionEnd;
	}
	ErrorCode Program::Error::getErrorValue() const {
		return error;
	}
	String Program::Error::getErrorCode() const {
		switch (error) {
			case ErrorCode::flm: return "FLM";
			case ErrorCode::lxr: return "LXR";
			case ErrorCode::ppr: return "PPR";
			case ErrorCode::syx: return "SYX";
			case ErrorCode::typ: return "TYP";
			case ErrorCode::lgc: return "LGC";
			case ErrorCode::evl: return "EVL";
			default: return "UNK";
		}
	}
	void Program::serialise(String path) const {
		Buffer * buffer = new Buffer();
		Serialiser::write<UInt64>(buffer, strings.size());
		for (String s : strings) {
			Serialiser::write<String>(buffer, s);
		}
		Serialiser::write<UInt64>(buffer, instructions.size());
		for (ByteCode byte : instructions) {
			Serialiser::write<Byte>(buffer, byte.code);
			switch (byte.code) {
				case OPCode::PSH: case OPCode::STR:
				case OPCode::GET: case OPCode::SET:
				case OPCode::SSF: case OPCode::GLF:
				case OPCode::SLF: case OPCode::CAL:
				case OPCode::DSK: case OPCode::JMP:
				case OPCode::JIF: case OPCode::JAF:
				case OPCode::JIT: case OPCode::JAT:
				case OPCode::SGS: case OPCode::AGS:
				case OPCode::SSS: case OPCode::ASS:
				case OPCode::PSA:
					// 8 Bytes arguments:
					Serialiser::write<UInt64>(buffer, byte.as.index);
				break;
				case OPCode::ADD: case OPCode::SUB:
				case OPCode::MUL: case OPCode::DIV:
				case OPCode::MOD: case OPCode::EQL:
				case OPCode::NEQ: case OPCode::GRT:
				case OPCode::LSS: case OPCode::GEQ:
				case OPCode::LEQ: case OPCode::BWA:
				case OPCode::BWO: case OPCode::BWX:
				case OPCode::CST:
					// 2 Bytes arguments (types):
					Serialiser::write<UInt16>(buffer, byte.as.types);
				break;
				case OPCode::BSL: case OPCode::BSR:
				case OPCode::BRL: case OPCode::BRR:
				case OPCode::NEG: case OPCode::INV:
				case OPCode::INT: case OPCode::TYP:
					// 1 Byte argument (type):
					Serialiser::write<Byte>(buffer, byte.as.type);
				break;
				default: break;
			}
		}
		try { Manager::writeBuffer(path, buffer); }
		catch (Manager::BadFileException & b) {
			delete buffer;
			throw;
		}
		delete buffer;
	}
	Program * Program::from(String path) {
		Program * program = new Program();
		Buffer * buffer;
		try { buffer = Manager::readBuffer(path); }
		catch (Manager::BadFileException & b) {
			delete program;
			throw;
		}
		Serialiser::prepare();
		SizeType size = 0;
		try { size = Serialiser::read<UInt64>(buffer); }
		catch (Serialiser::ReadingError & e) {
			delete buffer;
			delete program;
			throw;
		}
		String s;
		while (size > 0) {
			try { s = Serialiser::read<String>(buffer); }
			catch (Serialiser::ReadingError & e) {
				delete buffer;
				delete program;
				throw;
			}
			program -> strings.push_back(s);
			size -= 1;
		}
		try { size = Serialiser::read<UInt64>(buffer); }
		catch (Serialiser::ReadingError & e) {
			delete buffer;
			delete program;
			throw;
		}
		ByteCode byte;
		while (size > 0) {
			try {
				byte.code = (OPCode)(Serialiser::read<Byte>(buffer));
			} catch (Serialiser::ReadingError & e) {
				delete buffer;
				delete program;
				throw;
			}
			switch (byte.code) {
				case OPCode::PSH: case OPCode::STR:
				case OPCode::GET: case OPCode::SET:
				case OPCode::SSF: case OPCode::GLF:
				case OPCode::SLF: case OPCode::CAL:
				case OPCode::DSK: case OPCode::JMP:
				case OPCode::JIF: case OPCode::JAF:
				case OPCode::JIT: case OPCode::JAT:
				case OPCode::SGS: case OPCode::AGS:
				case OPCode::SSS: case OPCode::ASS:
				case OPCode::PSA:
					// 8 Bytes arguments:
					try {
						byte.as.index = Serialiser::read<UInt64>(buffer);
					} catch (Serialiser::ReadingError & e) {
						delete buffer;
						delete program;
						throw;
					}
				break;
				case OPCode::ADD: case OPCode::SUB:
				case OPCode::MUL: case OPCode::DIV:
				case OPCode::MOD: case OPCode::EQL:
				case OPCode::NEQ: case OPCode::GRT:
				case OPCode::LSS: case OPCode::GEQ:
				case OPCode::LEQ: case OPCode::BWA:
				case OPCode::BWO: case OPCode::BWX:
				case OPCode::CST:
					// 2 Bytes arguments (types):
					try {
						byte.as.types = Serialiser::read<UInt16>(buffer);
					} catch (Serialiser::ReadingError & e) {
						delete buffer;
						delete program;
						throw;
					}
				break;
				case OPCode::BSL: case OPCode::BSR:
				case OPCode::BRL: case OPCode::BRR:
				case OPCode::NEG: case OPCode::INV:
				case OPCode::INT: case OPCode::TYP:
					// 1 Byte argument (type):
					try {
						byte.as.type = (Type)(Serialiser::read<Byte>(buffer));
					} catch (Serialiser::ReadingError & e) {
						delete buffer;
						delete program;
						throw;
					}
				break;
				default: break;
			}
			program -> instructions.push_back(byte);
			size -= 1;
		}
		return program;
	}

	SourceCode::SourceCode(CodeUnit * main, Array<CodeUnit *> * wings, Array<String> * libraries) {
		this -> main = main;
		this -> wings = wings;
		this -> libraries = libraries;
	}

}

#endif
