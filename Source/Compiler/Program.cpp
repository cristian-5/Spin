
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
#define SPIN_PROGRAM_CPPxw

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

	SourceCode::SourceCode(CodeUnit * main, Array<CodeUnit *> * wings, Array<String> * libraries) {
		this -> main = main;
		this -> wings = wings;
		this -> libraries = libraries;
	}

}

#endif
