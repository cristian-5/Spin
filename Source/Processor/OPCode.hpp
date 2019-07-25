
/*!
 *
 *    + --------------------------------------- +
 *    |  OPCode.hpp                             |
 *    |                                         |
 *    |              Operation Code             |
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

#ifndef STACKOPCODE
#define STACKOPCODE

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"
#include "../Interpreter/Object.hpp"

using Collection::StrongList;

#define growArray(ptr, type, oldCount, count) \
(type *) reallocate(ptr, sizeof(type) * (oldCount), sizeof(type) * (count))
#define freeArray(type, ptr, oldCount) \
reallocate(ptr, sizeof(type) * (oldCount), 0)
#define growCapacity(old) \
((old) < 8 ? 8 : (old) * 2)

namespace Stack {

	enum OPCode {
		CST,       // Constant
		RET,       // Return Instruction
	};

	class CodeChunk {

		private:

		UInt8 * code = nullptr;

		UInt32 capacity = 0;
		UInt32 count = 0;

		StrongList<Object> constants = StrongList<Object>();

		void * reallocate(void * ptr, SizeType oldSize, SizeType newSize) {
			if (newSize == 0) {
				delete ptr;
				return nullptr;
			}
			return reallocation(ptr, newSize);
		}

		String disassemblyOPCode(UInt32 & of) {
			if (of >= count) return "";
			UInt8 op = code[of];
			switch (op) {
				case OPCode::CST: {
					of++;
					UInt32 pos = 0;
					for (UInt8 i = of; i < of + 4; i++) {
						pos << 8; pos += code[i];
					}
					of += 4; return "CST " + toString(pos);
				} break;
				case OPCode::RET: {
					of++; return "RET";
				} break;
				default: return "UNK";
			}
		}

		public:

		CodeChunk() { }

		void append(UInt8 byte) {
			if (capacity < count + 1) {
				UInt32 oldCapacity = capacity;
				capacity = growCapacity(oldCapacity);
				code = growArray(code, UInt8, oldCapacity, capacity);
			}
			code[count] = byte;
			count++;
		}

		void free() {
			freeArray(UInt8, code, capacity);
			code = nullptr;
			capacity = 0;
			count = 0;
		}

		UInt32 addConstant(Object value) {
			constants.link(value);
			return constants.count() - 1;
		}

		String disassembly() {
			String result = "";
			UInt32 offset = 0;
			while (offset < count) {
				result += disassemblyOPCode(offset);
				result += '\n';
			}
			return result;
		}

		~CodeChunk() { free(); }

	};


}

#endif
