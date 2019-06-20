
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

#include <iostream>
#include <string>

#include "../Aliases/Aliases.hpp"

#include "SRule.hpp"

namespace Stack {

	class Syntax {

		public:

		static Grammar * importGrammar() {
			Token * im = new Token("import", importKeyword);
			Token * id = new Token("id", symbol);
			Token * dt = new Token("dot", dot);
			Token * sm = new Token("semicolon", semicolon);
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
			Token * pr = new Token("proc", procKeyword);
			Token * id = new Token("id", symbol);
			Token * ob = new Token("(", openRoundBracket);
			SRule * prR = new SRule(pr);
			SRule * idR = new SRule(id);
			SRule * obR = new SRule(ob);
			prR -> addNextRule(idR);
			idR -> addNextRule(obR);
			return prR;
		}

		static Grammar * argsGrammar() {
			Token * nm = new Token("name", symbol);
			Token * ty = new Token("type", symbol);
			


		}

		static Grammar * funcGrammar() {
			Token * fc = new Token("func", procKeyword);
			Token * id = new Token("id", symbol);
			Token * ob = new Token("(", openRoundBracket);

			Token * ty = new Token("type", symbol);
			Token * nm = new Token("name", symbol);

			Token * eq = new Token("=", equal);

			Token * bl = new Token("bool", boolLiteral);
			Token * cl = new Token("char", charLiteral);
			Token * il = new Token("int", integerLiteral);
			Token * rl = new Token("real", realLiteral);
			Token * sl = new Token("string", stringLiteral);



			SRule * fcR = new SRule(fc);
			SRule * idR = new SRule(id);
			SRule * obR = new SRule(ob);
			fcR -> addNextRule(idR);
			idR -> addNextRule(obR);
			return fcR;
		}

		static Grammar * fTypeGrammar() {
			Token * ar = new Token("-", minus);
			Token * rw = new Token(">", major);
			Token * sy = new Token("type", symbol);
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
