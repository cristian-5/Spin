
#include "../Common/Header.hpp"

#ifndef SPIN_PROGRAM_HPP
#define SPIN_PROGRAM_HPP

#include "../Common/Exception.hpp"

#include "../Token/Token.hpp"

#include <vector>

namespace Spin {

	class CodeUnit {
		public:
		Array<Token> * tokens;
		String * name;
		String * contents;
		CodeUnit(Array<Token> * tokens,
				 String * name,
				 String * contents);
		~CodeUnit();
	};

	enum ErrorCode : UInt8 {
		flm, lxr, ppr, syx, evl
	};

	enum Type : UInt8 {

		BooleanType,
		CharacterType,
		ByteType,
		
		IntegerType,
		RealType,
		ImaginaryType,

		ComplexType,
		StringType,
		ArrayType,
		VectorType,
		RoutineType,
		ClassType,
		InstanceType,

		VoidType,

	};

	enum OPCode : UInt8 {
		RST, // rest
		CNS, // constant
		ADD, // addition
		SUB, // subtract
		MUL, // multiply
		DIV, // divide
		MOD, // modulus
		NEG, // negate
		INV, // bitwise inversion
		PST, // push true
		PSF, // push false
		PSI, // push infinity
		PSU, // push undefined
		EQL, // equal
		NEQ, // not equal
		GRT, // great
		LSS, // less
		GEQ, // great equal
		LEQ, // less equal
		NOT, // boolean not
		AND, // boolean and
		ORR, // boolean or
		BWA, // bitwise and
		BWO, // bitwise or
		BWX, // bitwise xor
		RET, // return
		HLT, // halt
	};

	using Types = UInt16;

	union Value {
		Integer integer;
		Pointer pointer;
		Real real;
		Byte byte;
		Boolean boolean;
	};

	struct ByteCode {
		OPCode code = OPCode::RST;
		union {
			Value value;
			Type type;
			Types types;
		} as;
	};

	class Program {
		public:
		class Error: Exception {
			private:
			String file;
			String message;
			UInt32 line;
			UInt32 positionStart;
			UInt32 positionEnd;
			ErrorCode error;
			public:
			Error(CodeUnit * c, String m, Token t, ErrorCode e);
			String getFile() const;
			String getMessage() const;
			UInt32 getLine() const;
			UInt32 getPositionStart() const;
			UInt32 getPositionEnd() const;
			ErrorCode getErrorValue() const;
			String getErrorCode() const;
		};
		Program() = default;
		Array<ByteCode> instructions;
		Array<Pair<Pointer, Type>> objects;
	};

	class SourceCode {
		public:
		CodeUnit * main;
		Array<CodeUnit *> * wings;
		Array<String> * libraries;
		SourceCode(CodeUnit * main,
				   Array<CodeUnit *> * wings,
				   Array<String> * libraries);
	};

}

#endif
