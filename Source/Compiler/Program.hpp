
#include "../Common/Header.hpp"

#ifndef SPIN_PROGRAM_HPP
#define SPIN_PROGRAM_HPP

#include "../Token/Token.hpp"

#include <vector>
#include <unordered_map>

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
		flm, lxr, ppr, syx, typ, lgc, evl
	};

	// Never change the order of types
	// since it's used for type check.

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

		ClassType,
		InstanceType,

		VoidType,

	};

	enum OPCode : UInt8 {

		RST, // rest

		CNS, // constant

		GLB, // global definition
		GGB, // get global
		SGB, // set global
		GLC, // get local
		SLC, // set local

		SWP, // swap

		ADD, // addition
		SUB, // subtract
		MUL, // multiply
		DIV, // divide
		MOD, // modulus
		NEG, // negate
		INV, // bitwise inversion

		CCJ, // complex conjugate
		VCJ, // vector conjugate
		MCJ, // matrix conjugate

		PST, // push true
		PSF, // push false
		PSI, // push infinity
		PSU, // push undefined
		POP, // pop
		DSK, // decrease stack

		JMP, // jump
		JMB, // jump backwards
		JIF, // jump if false
		JAF, // jump if false, avoid pop
		JIT, // jump if true
		JAT, // jump if true, avoid pop

		EQL, // equal
		NEQ, // not equal
		GRT, // great
		LSS, // less
		GEQ, // great equal
		LEQ, // less equal

		NOT, // boolean not

		BWA, // bitwise and
		BWO, // bitwise or
		BWX, // bitwise xor

		CAL, // call
		RET, // return

		CST, // casting

		PRN, // print
		NLN, // new line

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
			SizeType index;
			Value value;
			Type type;
			Types types;
		} as;
	};

	class Program {
		public:
		class Error : Exception {
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
		Dictionary<SizeType, Program::Error> errors;
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
