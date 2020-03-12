
/*!
 *
 *    + --------------------------------------- +
 *    |  Qubit.cpp                              |
 *    |                                         |
 *    |               Quantum Bit               |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
!*/

#include "../Aliases/Prototypes/Qubit.hpp"

#ifndef SPIN_QUBIT
#define SPIN_QUBIT

#include "../Aliases/Prototypes/Complex.hpp"
#include "../Aliases/Prototypes/Vector.hpp"
#include "../Aliases/Prototypes/Congruential.hpp"

namespace Spin {

	Bool Qubit::isValid() {
		Real sum = 0;
		const SizeType size = data -> getSize();
		for (SizeType i = 0; i < size; i += 1) {
			sum += (data -> at(i)).getNormalised();
		}
		return std::fabs(sum) > 1 - Qubit::error &&
			   std::fabs(sum) < 1 + Qubit::error;
	}

	Bool Qubit::measure(Int64 qubit) {
		if (!isValid) throw InavlidStateException();
		
		return false;
	}

}

#endif
