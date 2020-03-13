
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
		static Bool isKnownLibrary(String l);
		static String complete(CodeUnit * code, SizeType & i);
		static Array<String> classify(CodeUnit * code, Array<Hash> * libs);
		static Bool isSpread(String wing, Array<CodeUnit *> * wings);
		static void prepareWing(CodeUnit * code);
		static void spreadWing(CodeUnit * code, Array<CodeUnit *> * resolved, Array<Hash> * libs);
		static void prototype(CodeUnit * code);

		public:

		static Program * spread(String path);

	};

}

#endif
