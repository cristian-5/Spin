
/*!
 *
 *    + --------------------------------------- +
 *    |  Lexer.hpp                              |
 *    |                                         |
 *    |                  Lexer                  |
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

#ifndef STACKLEXER
#define STACKLEXER

namespace Stack {

	String Lexer::handleComments(String input) const {
		if (input.length() == 0) return input;
		try {
			Regex regex("\\/[\\/]+.*");
			SMatch match;
			regexSearch(input, match, regex);
			while (match.size() >= 1) {
				UInt32 len = match.str(0).length();
				UInt32 pos = match.position(0);
				for (UInt32 i = pos; i < pos + len; i++) {
					input[i] = ' ';
				}
				regexSearch(input, match, regex);
			}
		} catch (RegexError & e) { }
		return input;
	}

	Array<Token> * Lexer::tokenise(String * input, String fileName) const {
		// Handle Last Token:
		String data = (* input) + "\n";
		// Handle Single Line Comments:
		data = handleComments(data);
		// Handle EndLines:
		data = RegexTools::replaceMatches("\n", data, " ");
		Array<Token> * tokens = new Array<Token>();
		UInt32 pos = 0;
		Token temp = Token("beginFile", TokenType::beginFile, 0);
		tokens -> push(temp);
		Bool previousInvalid = false;
		while (data.length() > 0) {
			Bool tokenised = false;
			for (TokenRule rule : grammar) {
				String result = RegexTools::matchCloseStart(rule.pattern, data);
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
