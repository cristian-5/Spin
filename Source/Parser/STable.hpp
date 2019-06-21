
/*!
 *
 *    + --------------------------------------- +
 *    |  STable.hpp                             |
 *    |                                         |
 *    |               Symbol Table              |
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

#ifndef STACKSTABLE
#define STACKSTABLE

#include <iostream>
#include <string>

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"
#include "../Token/Token.hpp"

using namespace Collection;

namespace Stack {

	class SymbolTable {

		private:

		StrongList<StrongMap<String, ParseData> *> * table = nullptr;
		UInt32 depth = 0;

		public:

		SymbolTable() {
			table = new  StrongList<StrongMap<String, ParseData> *>();
			StrongMap<String, ParseData> * temp;
			temp = new StrongMap<String, ParseData>();
			table -> link(temp);
		}

		void enterNewScope() {
			StrongMap<String, ParseData> * temp;
			temp = new StrongMap<String, ParseData>();
			table -> link(temp);
			depth += 1;
		}

		void leaveScope() {
			if (depth == 0) return;
			table -> unlinkLast();
			depth -= 1;
		}

		void declare(String var, ParseData value) {
			if (table -> isEmpty()) return;
			StrongMap<String, ParseData> * map;
			map = table -> getNode(table -> count() - 1);
			map -> link(var, value);
		}

		void update(String var, ParseData value) {
			if (table -> isEmpty()) return;
			for (UInt32 i = table -> count() - 1; i >= 0; i--) {
				StrongMap<String, ParseData> * map;
				map = table -> getNode(i);
				Int64 pos = map -> findPosition(var);
				if (pos != -1) {
					auto temp = map -> getNode((UInt32)pos);
					temp.val = value;
					map -> setNode((UInt32)pos, temp);
					return;
				}
			}
		}
		
		Boolean isDeclaredInScope(String var) {
			if (table -> isEmpty()) return;
			StrongMap<String, ParseData> * map;
			map = table -> getNode(table -> count() - 1);
			return map -> findPosition(var) != -1;
		}

		Boolean isDeclared(String var) {
			if (table -> isEmpty()) return;
			for (UInt32 i = table -> count() - 1; i >= 0; i--) {
				StrongMap<String, ParseData> * map;
				map = table -> getNode(i);
				Int64 pos = map -> findPosition(var);
				if (pos != -1) return true;
			}
			return false;
		}

		ParseData get(std::string var) {
			if (table -> isEmpty()) return;
			for (UInt32 i = table -> count() - 1; i >= 0; i--) {
				StrongMap<String, ParseData> * map;
				map = table -> getNode(i);
				Int64 pos = map -> findPosition(var);
				if (pos != -1) {
					return map -> getNode((UInt32)pos).val;
				}
			}
			ParseData d;
			d.type = InvalidType;
			return d;
		}

	};

}

#endif
