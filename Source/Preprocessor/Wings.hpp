
#include "../Common/Header.hpp"

#ifndef SPIN_WINGS_HPP
#define SPIN_WINGS_HPP

#include <vector>

#include  "../Compiler/Program.hpp"

namespace Spin {

	class Wings {
		private:
		static void replace(CodeUnit * code, Token::Type type, String lexeme, Token::Type newType);
		static Boolean isKnownLibrary(String l);
		static String parentFolder(String f);
		static String complete(CodeUnit * code, SizeType & i);
		static Array<String> classify(CodeUnit * code, Array<String> * libs);
		static Boolean isSpread(String wing, Array<CodeUnit *> * wings);
		static void prepareWing(CodeUnit * code);
		static void spreadWing(CodeUnit * code, Array<CodeUnit *> * resolved, Array<String> * libs);
		static void prototype(CodeUnit * code);
		public:
		Wings() = delete;
		static SourceCode * spread(String path);
	};

}

#endif
