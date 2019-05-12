
/*!
 *
 *    + --------------------------------------- +
 *    |  StrongMap.hpp                          |
 *    |                                         |
 *    |                Strong Map               |
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

#ifndef STRONGMAP
#define STRONGMAP

#include <iostream>

#include "../Aliases/Aliases.hpp"

#include "Exceptions.hpp"
#include "StrongList.hpp"

/*!
 *   @brief Namespace Collection.
 *   Contains classes designed for
 *   handling collections of elements.
 *   @author Cristian A.
 */
namespace Collection {

	/*!
	 *   @brief Strong Map.
	 *   Generally used for mapping elements.
	 *   @note This map stores two pointers
	 *   in memory and it's meant to be really
	 *   fast, having complexity O(1) for most of
	 *   the operations and O(n / 2) in the
	 *   worse case when searching for an element
	 *   inside the map. On the other hand it
	 *   requires more memory to store a node.
	 *   @warning This class must be used within
	 *   a try catch block. It raises exceptions.
	 */
	template <typename KeyType, typename ValType>
	class StrongMap {

		public:

		/*! @brief Map element. */
		struct Element {
			KeyType key;
			ValType val;
		};

		private:

		/*! @brief Map. */
		StrongList<Element> map;

		public:

		/*!
		 *   @brief Gets the number of
		 *   elements inside the map.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns The elements' count.
		 */
		UInt32 count() const { return map.count(); }

		/*!
		 *   @brief Checks if the map is empty.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @returns True if the loop is empty,
		 *   False if it contains at least an element.
		 */
		bool isEmpty() const { return map.isEmpty(); }

		StrongMap() { }

		~StrongMap() { clean(); }

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
		Element & operator [] (UInt32 pos) {
			try { return map[pos]; }
			catch (Exception & e) { throw; }
		}

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
		Element getNode(UInt32 pos) {
			try { map.getNode(pos); }
			catch (Exception & e) { throw; }
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
		void setNode(UInt32 pos, Element & val) {
			try { map.setNode(pos, val); }
			catch (Exception & e) { throw; }
		}

		/* MARK: - Linking Operations */

		/*!
		 *   @brief Appends a node to the
		 *   end of the map.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to link.
		 */
		void link(Element & val) {
			try { map.link(val); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Appends a node to the
		 *   end of the map.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to link.
		 */
		void link(KeyType & key, ValType & val) {
			try {
				Element node = { key, val };
				map.link(node);
			} catch (Exception & e) { throw; }
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
		void linkBefore(UInt32 pos, Element & val) {
			try { map.linkBefore(pos, val); }
			catch (Exception & e) { throw; }
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
		void linkAfter(UInt32 pos, Element & val) {
			try { map.linkAfter(pos, val); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Hangs a node at
		 *   the top of the map.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to hang.
		 */
		void hang(Element & val) {
			try { map.hang(val); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Hangs a node at
		 *   the top of the map.
		 *
		 *   @b Complexity: O(1).
		 *
		 *   @param val Value to hang.
		 */
		void hang(KeyType & key, ValType & val) {
			try {
				Element node = { key, val };
				map.hang(node);
			} catch (Exception & e) { throw; }
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
			try { map.unlink(pos); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Unlinks the first
		 *   node of the map.
		 *
		 *   @b Complexity: O(1).
		 */
		void unlinkFirst() {
			try { map.unlinkFirst(); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Unlinks the last
		 *   node of the map.
		 *
		 *   @b Complexity: O(1).
		 */
		void unlinkLast() {
			try { map.unlinkLast(); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Erases n elements
		 *   from the start of the map.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the number
		 *   of elements to remove.
		 */
		void reduceRoot(UInt32 n) {
			try { map.reduceRoot(n); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Erases n elements
		 *   from the end of the map.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the number
		 *   of elements to remove.
		 */
		void reduceLeaf(UInt32 n) {
			try { map.reduceLeaf(n); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Finds the position
		 *   of the first found key.
		 *   Returns -1 if not found.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map, in the worse case.
		 */
		Int32 findPosition(KeyType & key) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i].key == key) return (UInt32)i;
			} return -1;
		}

		/*!
		 *   @brief Finds the position
		 *   of the first found val.
		 *   Returns -1 if not found.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map, in the worse case.
		 */
		Int32 findPosition(ValType & val) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i].val == val) return (UInt32)i;
			} return -1;
		}

		/*!
		 *   @brief Finds the position
		 *   of the first found element.
		 *   Returns -1 if not found.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map, in the worse case.
		 */
		Int32 findPosition(KeyType & key, ValType & val) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i] == Element(key, val)) return (UInt32)i;
			} return -1;
		}

		/*!
		 *   @brief Finds the position
		 *   of the first found key.
		 *   Returns -1 if not found.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map, in the worse case.
		 */
		Int32 findPosition(Element & element) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i] == element) return (Int32)i;
			} return -1;
		}

		/*!
		 *   @brief Finds the value
		 *   of the first found key.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map, in the worse case.
		 */
		ValType findValue(KeyType & key) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i].key == key) return map[i].val;
			}
			throw ElementNotFoundException();
		}

		/*!
		 *   @brief Finds the value
		 *   of the first found key.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map, in the worse case.
		 */
		KeyType findKey(ValType & val) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i].val == val) return map[i].key;
			}
			throw ElementNotFoundException();
		}

		/*!
		 *   @brief Erases every element
		 *   inside the map.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map.
		 */
		void clean() {
			try { map.clean(); }
			catch (Exception & e) { throw; }
		}

		/*!
		 *   @brief Erases every element
		 *   with the specified key.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map.
		 */
		void removeKeys(KeyType & key) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i].key == key) {
					map.unlink(i);
					i--;
				}
			}
		}

		/*!
		 *   @brief Erases every element
		 *   with the specified value.
		 *
		 *   @b Complexity: O(n).
		 *
		 *   Where @b n is the size
		 *   of the map.
		 */
		void removeValues(ValType & val) {
			for (UInt32 i = 0; i < map.count(); i++) {
				if (map[i].val == val) {
					map.unlink(i);
					i--;
				}
			}
		}

	};

}

#define Dictionary StrongMap

#endif
