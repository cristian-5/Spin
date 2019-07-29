
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

#include "../Aliases/Aliases.hpp"

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
	 */
	class InvalidIndexException: public Exception {
		public: InvalidIndexException(): Exception() { }
	};

	/*!
	 *   @brief Invalid Factor Exception.
	 *   Raised when the grow factor is
	 *   not valid.
	 */
	class InvalidFactorException: public Exception {
		public: InvalidFactorException(): Exception() { }
	};

	/*!
	 *   @brief Invalid Element Count Exception.
	 *   Raised when the number of elements
	 *   to remove is invalid.
	 */
	class ElementCountException: public Exception {
		public: ElementCountException(): Exception() { }
	};

	/*!
	 *   @brief Element Not Found Exception.
	 *   Raised when the element can't be found.
	 */
	class ElementNotFoundException: public Exception {
		public: ElementNotFoundException(): Exception() { }
	};

	/*!
	 *   @brief Empty List Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty list.
	 */
	class EmptyListException: public Exception {
		public: EmptyListException(): Exception() { }
	};

	/*!
	 *   @brief Empty Array Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty array.
	 */
	class EmptyArrayException: public Exception {
		public: EmptyArrayException(): Exception() { }
	};

	/*!
	 *   @brief Bad Allocation Exception.
	 *   Raised when failing to perform
	 *   a memory allocation operation.
	 */
	class BadAllocationException: public Exception {
		public: BadAllocationException(): Exception() { }
	};

	/*!
	 *   @brief Empty Loop Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty loop.
	 */
	class EmptyLoopException: public Exception {
		public: EmptyLoopException(): Exception() { }
	};

	/*!
	 *   @brief Hanging In Loop Exception.
	 *   Raised when trying to perform
	 *   a linkBefore operation with a
	 *   value of 0 on a loop.
	 */
	class HangingInLoopException: public Exception {
		public: HangingInLoopException(): Exception() { }
	};

	/*!
	 *   @brief Empty Stack Exception.
	 *   Raised when trying to perform a
	 *   pop operation on an empty stack.
	 */
	class EmptyStackException: public Exception {
		public: EmptyStackException(): Exception() { }
	};

}

#endif
