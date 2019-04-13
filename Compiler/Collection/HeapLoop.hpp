
/*!
 *
 *    + --------------------------------------- +
 *    |  HeapLoop.hpp                           |
 *    |                                         |
 *    |                Heap Loop                |
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

#ifndef HEAPLOOP
#define HEAPLOOP

#include <iostream>

#include "HeapExceptions.hpp"

using namespace std;

/* MARK: - Heap Loop - */

/*!
 *   @brief Namespace Collection.
 *   Contains classes designed for
 *   handling collections of elements.
 *   @author Cristian A.
 */
namespace Collection {

	/*!
	 *   @brief Heap Loop.
	 *   Generally used for collecting elements.
	 *   The Heap Loop wraps around itself.
	 *   @note This loop stores only one pointer
	 *   in memory so it's lightweight and meant
	 *   to be used with a big collection of
	 *   linked elements. On the other hand it
	 *   requires a longer time of execution when
	 *   requesting or linking a new node.
	 *   @warning This class must be used within
	 *   a try catch block. It raises exceptions.
	 *   @author Cristian A.
	 */
	template <typename Type>
	class HeapLoop {

	private:

		/*! @brief Size of the stack. */
		int size = 0;

		/*! @brief Stack element. */
		struct Node {
			Type value;
			Node * next;
		};

		/*! @brief Root element. */
		Node * root = NULL;

		/*! @brief Prepares the first element. */
		void seed(Type val) {
			root = new Node;
			root -> value = val;
			root -> next = root;
		}

	public:

		/* MARK: - Size Related */

		/*!
		 *   @brief Gets the number of
		 *   elements inside the loop.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The elements' count.
		 */
		int count() const { return size; }

		/*!
		 *   @brief Checks if the loop is empty.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns True if the loop is empty,
		 *   False if it contains at least an element.
		 */
		bool isEmpty() const { return size == 0; }

		/* MARK: - Subscript Operator */

		/*!
		 *   @brief Subscription.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the index
		 *   of the requested node.
		 *
		 *   @param pos Index.
		 *   @returns The node at the
		 *   specified index.
		 */
		Type & operator [] (int pos) {
			if (size == 0) throw EmptyLoopException();
			if (pos < 0) pos = size - (abs(pos) % size);
			else if (pos > size) pos = pos % size;
			Node * pointer = root;
			for (int i = 0; i < pos; i++) {
				pointer = pointer -> next;
			}
			return pointer -> value;
		}

		/* MARK: - Node Operations */

		/*!
		 *   @brief Gets the node in the
		 *   specified position.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the index
		 *   of the requested node.
		 *
		 *   @param pos Index.
		 *   @returns The node at the
		 *   specified index.
		 */
		Type getNode(int pos) {
			if (size == 0) throw EmptyLoopException();
			if (pos < 0) pos = size - (abs(pos) % size);
			else if (pos > size) pos = pos % size;
			Node * pointer = root;
			for (int i = 0; i < pos; i++) {
				pointer = pointer -> next;
			}
			return pointer -> value;
		}

		/*!
		 *   @brief Sets the node in the
		 *   specified position.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the index
		 *   of the requested node.
		 *
		 *   @param pos Index.
		 *   @param val Value to set.
		 */
		void setNode(int pos, Type val) {
			if (size == 0) throw EmptyLoopException();
			if (pos < 0) pos = size - (abs(pos) % size);
			else if (pos > size) pos = pos % size;
			Node * pointer = root;
			for (int i = 0; i < pos; i++) {
				pointer = pointer -> next;
			}
			pointer -> value = val;
		}

		/* MARK: - Linking Operations */

		/*!
		 *   @brief Appends a node to the
		 *   end of the loop.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the loop.
		 *
		 *   @param val Value to link.
		 */
		void link(Type val) {
			if (size == 0) seed(val);
			else {
				Node * node = new Node;
				node -> value = val;
				node -> next = NULL;
				Node * pointer = root;
				while (pointer -> next != root) {
					pointer = pointer -> next;
				}
				pointer -> next = node;
				node -> next = root;
			}
			size++;
		}

		/*!
		 *   @brief Links a node before
		 *   the specified position.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the index
		 *   of the new node.
		 *
		 *   @param pos Index.
		 *   @param val Value to link.
		 */
		void linkBefore(int pos, Type val) {
			if (size == 0) throw EmptyLoopException();
			if (pos < 0) pos = size - (abs(pos) % size);
			else if (pos > size) pos = pos % size;
			if (pos == 0) throw HangingInLoopException();
			Node * pointer = root;
			for (int i = 0; i < pos - 1; i++) {
				pointer = pointer -> next;
			}
			Node * node = new Node;
			node -> value = val;
			node -> next = pointer -> next;
			pointer -> next = node;
			size++;
		}

		/*!
		 *   @brief Links a node after
		 *   the specified position.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the index
		 *   of the new node.
		 *
		 *   @param pos Index.
		 *   @param val Value to link.
		 */
		void linkAfter(int pos, Type val) {
			if (size == 0) throw EmptyLoopException();
			if (pos < 0) pos = size - (abs(pos) % size);
			else if (pos > size) pos = pos % size;
			Node * pointer = root;
			for (int i = 0; i < pos; i++) {
				pointer = pointer -> next;
			}
			Node * node = new Node;
			node -> value = val;
			node -> next = pointer -> next;
			pointer -> next = node;
			size++;
		}

		/*!
		 *   @brief Unlinks a node at
		 *   the specified position.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the index
		 *   of the node to remove.
		 *
		 *   @param pos Index.
		 */
		void unlink(int pos) {
			if (size == 0) throw EmptyLoopException();
			if (pos < 0) pos = size - (abs(pos) % size);
			else if (pos > size) pos = pos % size;
			Node * pointer = root;
			for (int i = 0; i < pos - 1; i++) {
				pointer = pointer -> next;
			}
			Node * p = pointer;
			Node * c = pointer -> next;
			Node * n = c -> next;
			p -> next = n;
			delete c;
			size--;
		}

		/*!
		 *   @brief Unlinks the root
		 *   node of the loop.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the loop.
		 */
		void unlinkFirst() {
			if (size == 0) throw EmptyLoopException();
			Node * pointer = root;
			for (int i = 0; i < size - 1; i++) {
				pointer = pointer -> next;
			}
			pointer -> next = root -> next;
			delete root;
			root = pointer -> next;
			size--;
		}

		/*!
		 *   @brief Unlinks the node
		 *   at the end of the loop.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the loop.
		 */
		void unlinkLast() {
			if (size == 0) throw EmptyLoopException();
			Node * pointer = root;
			for (int i = 0; i < size - 2; i++) {
				pointer = pointer -> next;
			}
			Node * last = pointer -> next;
			delete last;
			pointer -> next = root;
			size--;
		}

		/* MARK: - Cleaning */

		/*!
		 *   @brief Erases every element
		 *   inside the loop.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the loop.
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
