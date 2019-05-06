
/*!
 *
 *    + --------------------------------------- +
 *    |  StrongList.hpp                         |
 *    |                                         |
 *    |               Strong List               |
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

#ifndef STRONGLIST
#define STRONGLIST

#include <iostream>

#include "Exceptions.hpp"

using UInt32 = std::uint32_t;

/* MARK: - Strong List - */

/*!
 *   @brief Namespace Collection.
 *   Contains classes designed for
 *   handling collections of elements.
 *   @author Cristian A.
 */
namespace Collection {

	/*!
	 *   @brief Strong List.
	 *   Generally used for collecting elements.
	 *   @note This list stores two pointers
	 *   in memory and it's meant to be really
	 *   fast, having complexity O(1) for most of
	 *   the operations and O(n / 2) in the
	 *   worse case when searching for an element
	 *   inside the list. On the other hand it
	 *   requires more memory to store a node.
	 *   @warning This class must be used within
	 *   a try catch block. It raises exceptions.
	 *   @author Cristian A.
	 */
	template <typename Type>
	class StrongList {

	private:

		/*! @brief Size of the stack. */
		UInt32 size = 0;

		/*! @brief Stack element. */
		struct Node {
			Type value;
			Node * prev;
			Node * next;
		};

		/*! @brief Root element. */
		Node * root = NULL;
		/*! @brief Leaf element. */
		Node * leaf = NULL;

		/*! @brief Prepares the first element. */
		void seed(Type & val) {
			Node * node = new Node;
			node -> value = val;
			node -> prev = NULL;
			node -> next = NULL;
			root = node; leaf = node;
		}

	public:

		/* MARK: - Size Related */

		/*!
		 *   @brief Gets the number of
		 *   elements inside the list.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The elements' count.
		 */
		UInt32 count() const { return size; }

		/*!
		 *   @brief Checks if the list is empty.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns True if the list is empty,
		 *   False if it contains at least an element.
		 */
		bool isEmpty() const { return size == 0; }

		/* MARK: - Subscript Operator */

		/*!
		 *   @brief Subscription.
		 *
		 *   @b Complexity: O(n), n < size / 2.
		 *
		 *   @b Complexity: O(size - n), n >= size / 2.
		 *
		 *   Where @b n is the index
		 *   of the requested node.
		 *
		 *   @param pos Index.
		 *   @returns The node at the
		 *   specified index.
		 */
		Type & operator [] (UInt32 pos) {
			if (size == 0) throw EmptyListException();
			if (pos >= size) throw InvalidIndexException();
			if (pos < size / 2) {
				Node * pointer = root;
				for (UInt32 i = 0; i < pos; i++) {
					pointer = pointer -> next;
				}
				return pointer -> value;
			} else {
				Node * pointer = leaf;
				for (UInt32 i = size - 1; i > pos; i--) {
					pointer = pointer -> prev;
				}
				return pointer -> value;
			}
		}

		/* MARK: - Node Operations */

		/*!
		 *   @brief Gets the node in the
		 *   specified position.
		 *
		 *   @b Complexity: O(n), n < size / 2.
		 *
		 *   @b Complexity: O(size - n), n >= size / 2.
		 *
		 *   Where @b n is the index
		 *   of the node to get.
		 *
		 *   @param pos Index.
		 *   @returns The node at the
		 *   specified index.
		 */
		Type getNode(UInt32 pos) {
			if (size == 0) throw EmptyListException();
			if (pos >= size) throw InvalidIndexException();
			if (pos < size / 2) {
				Node * pointer = root;
				for (UInt32 i = 0; i < pos; i++) {
					pointer = pointer -> next;
				}
				return pointer -> value;
			} else {
				Node * pointer = leaf;
				for (UInt32 i = size - 1; i > pos; i--) {
					pointer = pointer -> prev;
				}
				return pointer -> value;
			}
		}

		/*!
		 *   @brief Sets the node in the
		 *   specified position.
		 *
		 *   @b Complexity: O(n), n < size / 2.
		 *
		 *   @b Complexity: O(size - n), n >= size / 2.
		 *
		 *   Where @b n is the index
		 *   of the node to set.
		 *
		 *   @param pos Index.
		 *   @param val Value to set.
		 */
		void setNode(UInt32 pos, Type & val) {
			if (size == 0) throw EmptyListException();
			if (pos >= size) throw InvalidIndexException();
			if (pos < size / 2) {
				Node * pointer = root;
				for (UInt32 i = 0; i < pos; i++) {
					pointer = pointer -> next;
				}
				pointer -> value = val;
			} else {
				Node * pointer = leaf;
				for (UInt32 i = size - 1; i > pos; i--) {
					pointer = pointer -> prev;
				}
				pointer -> value = val;
			}
		}

		/* MARK: - Linking Operations */

		/*!
		 *   @brief Appends a node to the
		 *   end of the list.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to link.
		 */
		void link(Type & val) {
			if (size == 0) seed(val);
			else {
				Node * node = new Node;
				node -> value = val;
				node -> prev = leaf;
				node -> next = NULL;
				leaf = node;
				node -> prev -> next = node;
			}
			size++;
		}

		/*!
		 *   @brief Links a node before
		 *   the specified position.
		 *
		 *   @b Complexity: O(n), n < size / 2.
		 *
		 *   @b Complexity: O(size - n), n >= size / 2.
		 *
		 *   Where @b n is the index
		 *   of the node to link.
		 *
		 *   @param pos Index.
		 *   @param val Value to link.
		 */
		void linkBefore(UInt32 pos, Type & val) {
			if (size == 0) throw EmptyListException();
			if (pos >= size) throw InvalidIndexException();
			Node * node = new Node;
			node -> value = val;
			node -> prev = NULL;
			node -> next = NULL;
			if (pos < size / 2) {
				Node * pointer = root;
				for (UInt32 i = 0; i < pos; i++) {
					pointer = pointer -> next;
				}
				node -> prev = pointer -> prev;
				node -> next = pointer;
				node -> prev -> next = node;
				node -> next -> prev = node;
			} else {
				Node * pointer = leaf;
				for (UInt32 i = size - 1; i > pos; i--) {
					pointer = pointer -> prev;
				}
				node -> prev = pointer -> prev;
				node -> next = pointer;
				node -> prev -> next = node;
				node -> next -> prev = node;
			}
			size++;
		}

		/*!
		 *   @brief Links a node after
		 *   the specified position.
		 *
		 *   @b Complexity: O(n), n < size / 2.
		 *
		 *   @b Complexity: O(size - n), n >= size / 2.
		 *
		 *   Where @b n is the index
		 *   of the node to link.
		 *
		 *   @param pos Index.
		 *   @param val Value to link.
		 */
		void linkAfter(UInt32 pos, Type & val) {
			if (size == 0) throw EmptyListException();
			if (pos >= size - 1) throw InvalidIndexException();
			Node * node = new Node;
			node -> value = val;
			node -> prev = NULL;
			node -> next = NULL;
			if (pos < size / 2) {
				Node * pointer = root;
				for (UInt32 i = 0; i < pos; i++) {
					pointer = pointer -> next;
				}
				node -> prev = pointer;
				node -> next = pointer -> next;
				node -> prev -> next = node;
				node -> next -> prev = node;
			} else {
				Node * pointer = leaf;
				for (UInt32 i = size - 1; i > pos; i--) {
					pointer = pointer -> prev;
				}
				node -> prev = pointer;
				node -> next = pointer -> next;
				node -> prev -> next = node;
				node -> next -> prev = node;
			}
			size++;
		}

		/*!
		 *   @brief Hangs a node at
		 *   the top of the list.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to hang.
		 */
		void hang(Type & val) {
			if (size == 0) seed(val);
			else {
				Node * node = new Node;
				node -> value = val;
				node -> prev = NULL;
				node -> next = root;
				root = node;
				node -> next -> prev = node;
			}
			size++;
		}

		/*!
		 *   @brief Unlinks a node at
		 *   the specified position.
		 *
		 *   @b Complexity: O(n), n < size / 2.
		 *
		 *   @b Complexity: O(size - n), n >= size / 2.
		 *
		 *   Where @b n is the index
		 *   of the node to remove.
		 *
		 *   @param pos Index.
		 */
		void unlink(UInt32 pos) {
			if (size == 0) throw EmptyListException();
			if (pos >= size) throw InvalidIndexException();
			if (pos == 0) unlinkFirst();
			else if (pos == size - 1) unlinkLast();
			else {
				if (pos < size / 2) {
					Node * pointer = root;
					for (UInt32 i = 0; i < pos; i++) {
						pointer = pointer -> next;
					}
					Node * p = pointer -> prev;
					Node * c = pointer;
					Node * n = pointer -> next;
					p -> next = n;
					n -> prev = p;
					delete c;
					size--;
				} else {
					Node * pointer = leaf;
					for (UInt32 i = size - 1; i > pos; i--) {
						pointer = pointer -> prev;
					}
					Node * p = pointer -> prev;
					Node * c = pointer;
					Node * n = pointer -> next;
					p -> next = n;
					n -> prev = p;
					delete c;
					size--;
				}
			}
		}

		/*!
		 *   @brief Unlinks the first
		 *   node of the list.
		 *
		 *   @b Complexity: O(1).
		 */
		void unlinkFirst() {
			if (size == 0) throw EmptyListException();
			Node * pointer;
			pointer = root -> next;
			delete root;
			root = pointer;
			size--;
		}

		/*!
		 *   @brief Unlinks the last
		 *   node of the list.
		 *
		 *   @b Complexity: O(1).
		 */
		void unlinkLast() {
			if (size == 0) throw EmptyListException();
			Node * pointer;
			pointer = leaf -> prev;
			delete leaf;
			leaf = pointer;
			size--;
		}

		/* MARK: - Reduce */

		/*!
		 *   @brief Erases n elements
		 *   from the start of the list.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the number
		 *   of elements to remove.
		 */
		void reduceRoot(UInt32 n) {
			if (n > size) throw ElementCountException();
			for (UInt32 i = 0; i < n; i++) {
				Node * pointer;
				pointer = root -> next;
				delete root;
				root = pointer;
				size--;
			}
		}

		/*!
		 *   @brief Erases n elements
		 *   from the end of the list.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the number
		 *   of elements to remove.
		 */
		void reduceLeaf(UInt32 n) {
			if (n > size) throw ElementCountException();
			for (UInt32 i = 0; i < n; i++) {
				Node * pointer;
				pointer = leaf -> prev;
				delete leaf;
				leaf = pointer;
				size--;
			}
		}

		/* MARK: - Cleaning */

		/*!
		 *   @brief Erases every element
		 *   inside the list.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the list.
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
