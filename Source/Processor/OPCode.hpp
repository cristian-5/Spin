
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
#include "../Types/Object.hpp"

using Collection::StrongList;
using Collection::HeapArray;

namespace Stack {

	enum OPCode {
		CST,       // Constant
		RET,       // Return Instruction
	};

	class CodeChunk {

		private:

		HeapArray<UInt8> code = HeapArray<UInt8>(8);

		StrongList<Object> constants = StrongList<Object>();

		String disassemblyOPCode(UInt32 & of) {
			if (of >= code.count()) return "";
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
			code.push(byte);
		}

		UInt32 addConstant(Object value) {
			constants.link(value);
			return constants.count() - 1;
		}

		String disassembly() {
			String result = "";
			UInt32 offset = 0;
			while (offset < code.count()) {
				result += disassemblyOPCode(offset);
				result += '\n';
			}
			return result;
		}

	};

}

#endif
