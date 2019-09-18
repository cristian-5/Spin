
/*!
 *
 *    + --------------------------------------- +
 *    |  Linker.hpp                             |
 *    |                                         |
 *    |               Stack Linker              |
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

#ifndef STACKLINKER
#define STACKLINKER

namespace Stack {

	FilePosition Linker::getPosition(String * input, UInt32 cursor) {
		FilePosition result = { 1, 1 };
		if (!input || input -> length() == 0) return { 0, 0 };
		if (cursor == 0 && input -> length() > 0) return result;
		if (cursor >= input -> length()) {
			return getPosition(input, input -> length() - 1);
		}
		for (UInt32 i = 0; i < cursor; i++) {
			if (input -> at(i) == '\n') {
				result.row++;
				result.col = 1;
			} else result.col++;
		}
		return result;
	}
	ArrayList<String> Linker::linesFromFile(String & path) {
		ArrayList<String> set = ArrayList<String>();
		IFStream file(path.stringValue());
		if (file.good()) {
			String line = "";
			while (getline(file, line)) {
				set.push(line);
			}
		} else throw BadFileException(path);
		return set;
	}
	String Linker::stringFromFile(String path) {
		String line = "";
		IFStream file(path.stringValue());
		if (file.good()) {
			String tmp = "";
			while (getline(file, tmp)) {
				line += "\n" + tmp;
			}
		} else throw BadFileException(path);
		return line;
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
