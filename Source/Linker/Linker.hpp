
/*!
 *
 *    + --------------------------------------- +
 *    |  Linker.hpp                             |
 *    |                                         |
 *    |               File Linker               |
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

#include "../Aliases/Prototypes.hpp"

#ifndef SPINLINKER
#define SPINLINKER

namespace Spin {

	UInt64 Linker::getLine(String * input, SizeType cursor) {
		if (!input || input -> empty()) return 0;
		if (cursor == 0) return 1;
		const SizeType length = input -> length();
		if (cursor >= length) {
			return getLine(input, length - 1);
		}
		UInt64 line = 1;
		for (SizeType i = 0; i < cursor; i += 1) {
			if (input -> at(i) == '\n') line += 1;
		}
		return line;
	}
	String * Linker::stringFromFile(String path) {
		IFStream file(path.stringValue());
		StringStream buffer = StringStream();
		if (file.good()) {
			buffer << file.rdbuf();
		} else throw BadFileException(path);
		return new String(buffer.str());
	}
	void Linker::createNewFile(String path, String content) {
		OFStream file(path.stringValue());
		try {
			file << content;
		} catch (Exception & e) {
			throw BadAccessException(path);
		}
	}

}

#endif
