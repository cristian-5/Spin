
/*!
 *
 *    + --------------------------------------- +
 *    |  HeapStack.hpp                          |
 *    |                                         |
 *    |               Heap Stack                |
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

#ifndef HEAPSTACK
#define HEAPSTACK

#include "../Aliases/Aliases.hpp"

#include "Exceptions.hpp"

/*!
 *   @brief Namespace Collection.
 *   Contains classes designed for
 *   handling collections of elements.
 *   @author Cristian A.
 */
namespace Collection {

	/*!
	 *   @brief Heap Stack.
	 *   Generally used for collecting elements.
	 *   @note This stack stores only one pointer
	 *   in memory so it's lightweight and meant
	 *   to be used with a big collection of
	 *   linked elements. This class has a
	 *   complexity of O(1) when pushing or popping
	 *   an element inside the stack.
	 *   @warning This class must be used within
	 *   a try catch block. It raises exceptions.
	 */
	template <typename Type>
	class HeapStack {

		private:

		/*! @brief Size of the stack. */
		UInt32 size = 0;

		/*! @brief Stack element. */
		struct Node {
			Type value;
			Node * next;
		};

		/*! @brief Root element. */
		Node * root = nullptr;

		/*! @brief Prepares the first element. */
		void seed(Type & val) {
			root = new Node;
			root -> value = val;
			root -> next = nullptr;
		}

		public:

		/*!
		 *   @brief Gets the number of
		 *   elements inside the stack.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The elements' count.
		 */
		UInt32 count() const { return size; }

		/*!
		 *   @brief Checks if the stack is empty.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns True if the stack is empty,
		 *   False if it contains at least an element.
		 */
		bool isEmpty() const { return size == 0; }

		HeapStack() { }

		~HeapStack() { clean(); }

		/*!
		 *   @brief Pushes an element onto
		 *   the stack. The element is a
		 *   copy of the original.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val The value to push.
		 */
		void push(Type & val) {
			if (size == 0) {
				seed(val);
			} else {
				Node * newRoot = new Node;
				newRoot -> value = val;
				newRoot -> next = root;
				root = newRoot;
			}
			size++;
		}

		/*!
		 *   @brief Removes (pops) the
		 *   element on top of the stack.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The popped element.
		 */
		Type pop() {
			if (size == 0) throw EmptyStackException();
			Node * pointer = root;
			Type value = pointer -> value;
			root = pointer -> next;
			delete pointer;
			size--;
			return value;
		}

		/*!
		 *   @brief Erases every element
		 *   inside the stack.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the stack.
		 */
		void clean() {
			while (size > 0) {
				Node * pointer;
				pointer = root -> next;
				delete root;
				root = pointer;
				size--;
			}
		}

	};

}

#endif
