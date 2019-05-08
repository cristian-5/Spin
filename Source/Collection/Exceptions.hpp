
/*!
 *
 *    + --------------------------------------- +
 *    |  Exceptions.hpp                         |
 *    |                                         |
 *    |             Heap Exceptions             |
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

#ifndef HEAPEXCEPTIONS
#define HEAPEXCEPTIONS

#include <iostream>

using Exception = std::exception;

/* MARK: - Heap Exceptions - */

/*!
 *   @brief Namespace Collection.
 *   Contains classes designed for
 *   handling collections of elements.
 *   @author Cristian A.
 */
namespace Collection {

	/*!
	 *   @brief Invalid Index Exception.
	 *   Raised when the index overflows
	 *   or underflows the list bounds.
	 *   @author Cristian A.
	 */
	class InvalidIndexException: public Exception {
		public: InvalidIndexException(): Exception() { }
	};

	/*!
	 *   @brief Invalid Element Count Exception.
	 *   Raised when the number of elements
	 *   to remove is invalid.
	 *   @author Cristian A.
	 */
	class ElementCountException: public Exception {
		public: ElementCountException(): Exception() { }
	};

	/*!
	 *   @brief Element Not Found Exception.
	 *   Raised when the element can't be found.
	 *   @author Cristian A.
	 */
	class ElementNotFoundException: public Exception {
		public: ElementNotFoundException(): Exception() { }
	};

	/*!
	 *   @brief Empty List Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty list.
	 *   @author Cristian A.
	 */
	class EmptyListException: public Exception {
		public: EmptyListException(): Exception() { }
	};

	/*!
	 *   @brief Empty Loop Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty loop.
	 *   @author Cristian A.
	 */
	class EmptyLoopException: public Exception {
		public: EmptyLoopException(): Exception() { }
	};

	/*!
	 *   @brief Hanging In Loop Exception.
	 *   Raised when trying to perform
	 *   a linkBefore operation with a
	 *   value of 0 on a loop.
	 *   @author Cristian A.
	 */
	class HangingInLoopException: public Exception {
		public: HangingInLoopException(): Exception() { }
	};

	/*!
	 *   @brief Empty Stack Exception.
	 *   Raised when trying to perform a
	 *   pop operation on an empty stack.
	 *   @author Cristian A.
	 */
	class EmptyStackException: public Exception {
		public: EmptyStackException(): Exception() { }
	};

}

#endif
