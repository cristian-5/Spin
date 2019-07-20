
/*!
 *
 *    + --------------------------------------- +
 *    |  PProcessor.hpp                         |
 *    |                                         |
 *    |               PreProcessor              |
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

#ifndef STACKPREPROCESSOR
#define STACKPREPROCESSOR

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"

#include "ASTree.hpp"
#include "Exceptions.hpp"

using namespace Collection;

namespace Stack {

	class PreProcessor {

		public:

		PreProcessor() { }

		void processIncludes() { }

		void processBras() { }
		void processKets() { }
		void processBraKets() { }

		void processVectors() { }

		void processCombinedOperators() { }


	};

}

#endif
