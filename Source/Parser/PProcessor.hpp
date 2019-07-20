
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

		private:

		StrongList<Token> * tokens = nullptr;

		String * inputFile = nullptr;

		String fileName = "";

		void processBras() {
			
		}
		void processKets() {
			
		}
		void processBraKets() {
			
		}

		void processVectors() {
			
		}

		void processCombinedOperators() {
			
		}

		public:

		PreProcessor() { }

		StrongList<Token> * process(StrongList<Token> * t,
								    String * i = nullptr,
									String f = "Unknown File") {
			tokens = t; inputFile = i; fileName = f;
			processVectors();
			processCombinedOperators();
		}


	};

}

#endif
