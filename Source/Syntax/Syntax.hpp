
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

		private: Syntax() { }

	};

}

#endif
