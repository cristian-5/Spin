
/*!
 *
 *    + --------------------------------------- +
 *    |  Lexer.hpp                              |
 *    |                                         |
 *    |              Language Lexer             |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
 */

#include "../Aliases/Prototypes.hpp"

#ifndef SPINLEXER
#define SPINLEXER

namespace Spin {

	void Lexer::removeComments(String & input) {
		if (input.empty()) return;
		try {
			Regex regex = Regex("\\/[\\/]+.*");
			SMatch match;
			regexSearch(input, match, regex);
			while (match.size() >= 1) {
				SizeType len = match.str(0).length();
				SizeType pos = match.position(0);
				for (SizeType i = pos; i < pos + len; i += 1) {
					input[i] = ' ';
				}
				regexSearch(input, match, regex);
			}
		} catch (RegexError & e) { }
	}

	Array<Token> * Lexer::tokenise(String * input) const {
		// Last Token Fallback:
		String data = (* input) + "\n";
		// Remove Single Line Comments:
		Lexer::removeComments(data);
		// Remove Line Endings:
		RegexTools::replaceMatches("\n", data, " ");
		Array<Token> * tokens = new Array<Token>();
		SizeType pos = 0;
		Token temp = Token("beginFile", TokenType::beginFile, 0);
		tokens -> push(temp);
		Bool previousInvalid = false;
		while (data.length() > 0) {
			Bool tokenised = false;
			for (TokenRule rule : grammar) {
				String result = RegexTools::findFirstGroup(rule.pattern, data);
				if (result.length() > 0) {
					tokenised = true;
					data = data.subString(result.length());
					temp = Token(result, rule.type, pos);
					pos += result.length();
					previousInvalid = false;
					if (rule.type == TokenType::empty) break;
					if (rule.type == TokenType::comment) break;
					tokens -> push(temp); break;
				}
			}
			if (!tokenised) {
				String s = "-"; s[0] = data[0];
				if (!previousInvalid) {
					tokens -> push({ s, TokenType::invalid, pos });
					previousInvalid = true;
				} else {
					tokens -> at(tokens -> size() - 1).lexeme += s;
				}
				pos += 1;
				data.erase(data.begin());
			}
		}
		tokens -> push({ "endFile", TokenType::endFile, 0 });
		tokens -> shrinkToFit();
		return tokens;
	}

}

#endif
