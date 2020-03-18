
#include "../Aliases.hpp"

#ifndef SPIN_PROGRAM_HPP
#define SPIN_PROGRAM_HPP

#include <vector>

#include "Exception.hpp"
#include "Token.hpp"

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

		CodeUnit * main;
		Array<CodeUnit *> * wings;
		Array<String> * libraries;

		Program(CodeUnit * main, Array<CodeUnit *> * wings, Array<String> * libraries);

	};

}

#endif
