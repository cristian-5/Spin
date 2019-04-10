
/*!
 *
 *    + --------------------------------------- +
 *    |  HeapExceptions.h                       |
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

using namespace std;

/* MARK: - Heap Exceptions - */

/*!
 *   @brief Namespace HeapCollection.
 *   Contains classes designed for
 *   handling collections of elements.
 *   @author Cristian A.
 */
namespace HeapCollection {
	
	/*!
	 *   @brief Invalid Index Exception.
	 *   Raised when the index overflows
	 *   or underflows the list bounds.
	 *   @author Cristian A.
	 */
	class InvalidIndexException: public exception {
	public:
		InvalidIndexException(): exception() { }
	};
	
	/*!
	 *   @brief Invalid Element Count Exception.
	 *   Raised when the number of elements
	 *   to remove is invalid.
	 *   @author Cristian A.
	 */
	class ElementCountException: public exception {
	public:
		ElementCountException(): exception() { }
	};
	
	/*!
	 *   @brief Element Not Found Exception.
	 *   Raised when the element can't be found.
	 *   @author Cristian A.
	 */
	class ElementNotFoundException: public exception {
	public:
		ElementNotFoundException(): exception() { }
	};
	
	/*!
	 *   @brief Empty List Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty list.
	 *   @author Cristian A.
	 */
	class EmptyListException: public exception {
	public:
		EmptyListException(): exception() { }
	};
	
	/*!
	 *   @brief Empty Loop Exception.
	 *   Raised when trying to perform
	 *   an operation on an empty loop.
	 *   @author Cristian A.
	 */
	class EmptyLoopException: public exception {
	public:
		EmptyLoopException(): exception() { }
	};
	
	/*!
	 *   @brief Hanging In Loop Exception.
	 *   Raised when trying to perform
	 *   a linkBefore operation with a
	 *   value of 0 on a loop.
	 *   @author Cristian A.
	 */
	class HangingInLoopException: public exception {
	public:
		HangingInLoopException(): exception() { }
	};
	
	/*!
	 *   @brief Empty Stack Exception.
	 *   Raised when trying to perform a
	 *   pop operation on an empty stack.
	 *   @author Cristian A.
	 */
	class EmptyStackException: public exception {
	public:
		EmptyStackException(): exception() { }
	};

}

#endif
