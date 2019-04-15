
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
	class InvalidIndexException: public std::exception {
	public:
		InvalidIndexException(): std::exception() { }
	};

	/*!
	 *   @brief Invalid Element Count Exception.
	 *   Raised when the number of elements
	 *   to remove is invalid.
	 *   @author Cristian A.
	 */
	class ElementCountException: public std::exception {
	public:
		ElementCountException(): std::exception() { }
	};

	/*!
	 *   @brief Element Not Found Exception.
	 *   Raised when the element can't be found.
	 *   @author Cristian A.
	 */
	class ElementNotFoundException: public std::exception {
	public:
		ElementNotFoundException(): std::exception() { }
	};

	/*!
	 *   @brief Empty List Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty list.
	 *   @author Cristian A.
	 */
	class EmptyListException: public std::exception {
	public:
		EmptyListException(): std::exception() { }
	};

	/*!
	 *   @brief Empty Loop Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty loop.
	 *   @author Cristian A.
	 */
	class EmptyLoopException: public std::exception {
	public:
		EmptyLoopException(): std::exception() { }
	};

	/*!
	 *   @brief Hanging In Loop Exception.
	 *   Raised when trying to perform
	 *   a linkBefore operation with a
	 *   value of 0 on a loop.
	 *   @author Cristian A.
	 */
	class HangingInLoopException: public std::exception {
	public:
		HangingInLoopException(): std::exception() { }
	};

	/*!
	 *   @brief Empty Stack Exception.
	 *   Raised when trying to perform a
	 *   pop operation on an empty stack.
	 *   @author Cristian A.
	 */
	class EmptyStackException: public std::exception {
	public:
		EmptyStackException(): std::exception() { }
	};

}

#endif
