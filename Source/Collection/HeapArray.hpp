
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
	 *   @brief Heap Array.
	 *   Generally used for collecting elements.
	 *   @note This loop stores values in dynamic
	 *   arrays so it's very fast but grows with a
	 *   factor of 2 every time there's an overflow.
	 *   @warning This class must be used within
	 *   a try catch block. It raises exceptions.
	 *   This array doesn't shrink.
	 */
	template <typename Type>
	class HeapArray {

		private:

		/*! @brief Size of the stack. */
		UInt32 size = 0;

		/*! @brief Current Capacity. */
		UInt32 capacity = 0;

		/*! @brief Current Array Pointer. */
		Type * data = nullptr;

		/*! @brief Grow Factor. */
		Real factor = 2;

		/*! @brief Prepares the Array. */
		void grow() {
			UInt32 newCapacity = capacity * factor;
			Type * newData = new Type[newCapacity];
			for (UInt32 i = 0; i < size; i++) {
				newData[i] = data[i];
			}
			delete [] data;
			data = newData;
		}

		public:

		/*!
		 *   @brief Gets the number of
		 *   elements inside the loop.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The elements' count.
		 */
		UInt32 count() const { return size; }

		/*!
		 *   @brief Gets the current capacity.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The current capacity.
		 */
		UInt32 capacity() const { return capacity; }

		/*!
		 *   @brief Gets the current grow factor.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The grow factor.
		 */
		UInt32 factor() const { return factor; }

		/*!
		 *   @brief Checks if the array is empty.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns True if the array is empty,
		 *   False if it contains at least an element.
		 */
		bool isEmpty() const { return size == 0; }

		HeapArray(UInt32 capacity = 100) {
			if (capacity == 0) throw BadAllocationException();
			this -> capacity = capacity;
			data = new Type[capacity];
			if (data == nullptr) throw BadAllocationException();
		}

		HeapArray(UInt32 capacity, Real factor) {
			if (capacity == 0) throw BadAllocationException();
			if (factor <= 1.0) throw InvalidFactorException();
			this -> capacity = capacity;
			this -> factor = factor;
			data = new Type[capacity];
			if (data == nullptr) throw BadAllocationException();
		}

		~HeapArray() { clean(); }

		/*!
		 *   @brief Subscription.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param pos Index.
		 *   @returns The node at the
		 *   specified index.
		 */
		Type & operator [] (UInt32 pos) {
			if (pos >= size) throw InvalidIndexException();
			return & data[pos];
		}

		/*!
		 *   @brief Gets the node in the
		 *   specified position.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param pos Index.
		 *   @returns The node at the
		 *   specified index.
		 */
		Type getNode(UInt32 pos) {
			if (pos >= size) throw InvalidIndexException();
			return data[pos];
		}

		/*!
		 *   @brief Sets the node in the
		 *   specified position.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param pos Index.
		 *   @param val Value to set.
		 */
		void setNode(UInt32 pos, Type val) {
			if (pos >= size) throw InvalidIndexException();
			data[pos] = val;
		}

		/*!
		 *   @brief Pushes an element.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to push.
		 */
		void push(Type val) {
			if (size == capacity) grow();
			data[size - 1] = val;
			size++;
		}

		/*!
		 *   @brief Pops an element.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns the popped value.
		 */
		Type pop() {
			if (size == 0) throw EmptyArrayException();
			size--;
			return data[size];
		}

		/*!
		 *   @brief Gives back last pointer.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns the pointer.
		 */
		Type * lastPointer() {
			if (size == 0) return nullptr;
			return & data[size];
		}

		/*!
		 *   @brief Gives back first pointer.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns the pointer.
		 */
		Type * firstPointer() {
			if (size == 0) return nullptr;
			return & data[0];
		}

		/*!
		 *   @brief Appends a node to the
		 *   end of the array.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to link.
		 */
		inline void link(Type val) {
			try {
				push(val);
			} catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Unlinks the node
		 *   at the end of the array.
		 *
		 *   @b Complexity: O(1).
		 *
		 */
		void unlinkLast() {
			if (size == 0) return;
			size--;
		}

		/*!
		 *   @brief Deallocates the array.
		 *
		 *   @b Complexity: O(1).
		 *
		 */
		void clean() { delete [] data; }

	};

}

#endif
