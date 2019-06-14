
/*!
 *
 *    + --------------------------------------- +
 *    |  VProcessor.hpp                         |
 *    |                                         |
 *    |            Virtual Processor            |
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

#ifndef STACKVPROCESSOR
#define STACKVPROCESSOR

#include <iostream>

#include "../Aliases/Aliases.hpp"

namespace Stack {

	/*!
	 *   @brief Virtual Processor.
	 *   This class emulates a 64 bits
	 *   v processor and has its own
	 *   Instruction Set.
	 */
	class VProcessor64 {

		public:

		VProcessor64() { }

	};

	/*!
	 *   @brief Quantum Processor.
	 *   This class emulates a 8 bits
	 *   v processor and has its own
	 *   Instruction Set.
	 */
	class VProcessor08 {

		public:

		VProcessor08() {

		}

	}

}

#endif
