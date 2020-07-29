
/*!
 *
 *    + --------------------------------------- +
 *    |  Libraries.cpp                          |
 *    |                                         |
 *    |                Libraries                |
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

#include "Libraries.hpp"

#ifndef SPIN_LIBRARIES_CPP
#define SPIN_LIBRARIES_CPP

namespace Spin {

	const Array<String> Library::libraries = {
		"Console"
	};

	Boolean Library::isKnown(String name) {
		for (String library : libraries) {
			if (library == name) return true;
		}
		return false;
	}

}

#endif
