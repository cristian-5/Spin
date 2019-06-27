
/*!
 *
 *    + --------------------------------------- +
 *    |  Syntax.hpp                             |
 *    |                                         |
 *    |                  Syntax                 |
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

#ifndef STACKSYNTAX
#define STACKSYNTAX

#include "../Aliases/Aliases.hpp"

#include "SRule.hpp"

namespace Stack {

	class Syntax {

		public:

		static Grammar * importGrammar() {
			Token * im = new Token("import", TokenType::importKeyword);
			Token * id = new Token("id", TokenType::symbol);
			Token * dt = new Token("dot", TokenType::dot);
			Token * sm = new Token("semicolon", TokenType::semicolon);
			SRule * imR = new SRule(im);
			SRule * idR = new SRule(id);
			SRule * dtR = new SRule(dt);
			SRule * smR = new SRule(sm);
			imR -> addNextRule(idR);
			idR -> addNextRule(smR);
			idR -> addNextRule(dtR);
			dtR -> addNextRule(idR);
			return imR;
		}

		static Grammar * procGrammar() {
			Token * pr = new Token("proc", TokenType::procKeyword);
			Token * id = new Token("id", TokenType::symbol);
			Token * ob = new Token("(", TokenType::openRoundBracket);
			SRule * prR = new SRule(pr);
			SRule * idR = new SRule(id);
			SRule * obR = new SRule(ob);
			prR -> addNextRule(idR);
			idR -> addNextRule(obR);
			return prR;
		}

		static Grammar * argsGrammar() {
			Token * nm = new Token("name", TokenType::symbol);
			Token * ty = new Token("type", TokenType::symbol);
			return nullptr;
		}

		static Grammar * funcGrammar() {
			Token * fc = new Token("func", TokenType::procKeyword);
			Token * id = new Token("id", TokenType::symbol);
			Token * ob = new Token("(", TokenType::openRoundBracket);

			Token * ty = new Token("type", TokenType::symbol);
			Token * nm = new Token("name", TokenType::symbol);

			Token * eq = new Token("=", TokenType::equal);

			Token * bl = new Token("bool", TokenType::boolLiteral);
			Token * cl = new Token("char", TokenType::charLiteral);
			Token * il = new Token("int", TokenType::uIntLiteral);
			Token * rl = new Token("real", TokenType::realLiteral);
			Token * sl = new Token("string", TokenType::stringLiteral);



			SRule * fcR = new SRule(fc);
			SRule * idR = new SRule(id);
			SRule * obR = new SRule(ob);
			fcR -> addNextRule(idR);
			idR -> addNextRule(obR);
			return fcR;
		}

		static Grammar * fTypeGrammar() {
			Token * ar = new Token("-", TokenType::minus);
			Token * rw = new Token(">", TokenType::major);
			Token * sy = new Token("type", TokenType::symbol);
			SRule * arR = new SRule(ar);
			SRule * rwR = new SRule(rw);
			SRule * syR = new SRule(sy);
			arR -> addNextRule(rwR);
			rwR -> addNextRule(syR);
			return arR;
		}

		private: Syntax() { }

	};

}

#endif
