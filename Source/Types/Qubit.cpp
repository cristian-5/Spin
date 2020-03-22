
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
#include "../Aliases/Prototypes/Chaos.hpp"

#include <ctime>

namespace Spin {

	Boolean Qubit::isValid() {
		Real sum = 0;
		const SizeType size = data -> getSize();
		for (SizeType i = 0; i < size; i += 1) {
			sum += (data -> at(i)).getNormalised();
		}
		return std::fabs(sum) > 1 - Qubit::error &&
			   std::fabs(sum) < 1 + Qubit::error;
	}

	Boolean Qubit::measure(Int64 qubit) {
		if (!isValid) throw InavlidStateException();
		Chaos<UInt64>::seed = time(nullptr);
		Real rand = Chaos<UInt64>::next();
		for (UInt64 i = 0; i < data -> getSize(); i += 1) {
			rand -= (data -> at(i).getNormalised()) * 0xFFFFFFFFFFFFFFFF;
			if (rand <= 0) {
				collapse(i);
				return ;
			}
		}
		return false;
	}

	Int64 Qubit::collapse(Int64 i) {
		for (UInt64 j = 0; j < data -> getSize(); j += 1) {
			data -> at(j) = 0;
		}
		data -> at(i) = 1;
	}

}

#endif
