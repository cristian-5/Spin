
#include "../Aliases.hpp"

#ifndef SPIN_WINGS_HPP
#define SPIN_WINGS_HPP

#include "Exception.hpp"

#include <vector>

#include  "Program.hpp"

namespace Spin {

	class Wings {
		private:
		static void replace(CodeUnit * code, TokenType type, String lexeme, TokenType newType);
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
		static Program * spread(String path);
	};

}

#endif
