
/*!
 *
 *    + --------------------------------------- +
 *    |  Manager.cpp                            |
 *    |                                         |
 *    |               File Manager              |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
!*/

#include "../Aliases/Prototypes/Manager.hpp"

#ifndef SPIN_MANAGER
#define SPIN_MANAGER

namespace Spin {

	Manager::BadAccessException::BadAccessException(const String & path): path(path) { }
	const String & Manager::BadAccessException::getPath() const {
		return path;
	}

	Manager::BadFileException::BadFileException(const String & path): path(path) { }
	const String & Manager::BadFileException::getPath() const {
		return path;
	}

	UInt64 Manager::getLine(String * input, SizeType cursor) {
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
	String * Manager::stringFromFile(String path) {
		IFStream file(path.c_str());
		StringStream buffer;
		if (file.good()) buffer << file.rdbuf();
		else throw BadFileException(path);
		return new String(buffer.str());
	}
	void Manager::createNewFile(String path, String content) {
		OFStream file(path.c_str());
		try { file << content; }
		catch (Exception & e) {
			throw BadAccessException(path);
		}
	}

}

#endif
