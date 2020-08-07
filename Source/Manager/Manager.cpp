
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

#include "Manager.hpp"

#ifndef SPIN_MANAGER_CPP
#define SPIN_MANAGER_CPP

#include <vector>

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
		IFStream file(path);
		StringStream buffer;
		if (file.good()) buffer << file.rdbuf();
		else throw BadFileException(path);
		file.close();
		return new String(buffer.str());
	}
	void Manager::createNewFile(String path, String content) {
		OFStream file(path);
		try { file << content; }
		catch (Exception & e) {
			throw BadAccessException(path);
		}
		file.close();
	}
	void Manager::writeBuffer(String path, Buffer * buffer) {
		if (!buffer) return;
		OFStream file(path, std::ios_base::binary);
		if (!file.good()) throw BadFileException(path);
		Character * contents = reinterpret_cast<Character *>(
			buffer -> data()
		);
		file.write(contents, buffer -> size());
		file.close();
	}
	Buffer * Manager::readBuffer(String path) {
		IFStream file(path, std::ios_base::binary | std::ios::ate);
		if (!file.good()) throw BadFileException(path);
		const std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		Buffer * buffer = new Buffer(size);
		if (!file.read((Character *)(buffer -> data()), size)) {
			throw BadAccessException(path);
		}
		file.close();
		return buffer;
	}

}

#endif
