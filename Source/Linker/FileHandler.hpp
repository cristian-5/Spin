
/*!
 *
 *    + --------------------------------------- +
 *    |  FileHandler.hpp                        |
 *    |                                         |
 *    |               File Handler              |
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

#ifndef FILEHANDLER
#define FILEHANDLER

#include <iostream>
#include <fstream>
#include <string>

#include "../Collection/Collection.h"

using namespace Collection;

using String = std::string;

#define IFStream ifstream
#define Bool bool

/*! @brief Namespace Stack */
namespace Stack {

	/*!
	 *   @brief Bad File Exception.
	 *   Raised when the input file
	 *   is corrupted or the path is invalid.
	 *   @author Cristian A.
	 */
	class BadFileException: public std::exception {

	private:

		const String * _path;

	public:

		BadFileException(String & path): std::exception() { _path = & path; }

		const String getPath() const { return * _path; }

	};

	class FileHandler {

	public:

		static StrongList<string> linesFromFile(String & path) {
			StrongList<String> set = StrongList<String>();
			IFStream file(path.c_str());
			if (file.good()) {
				String line = "";
				while (getline(file, line)) {
					set.link(line);
				}
			} else throw BadFileException(path);
			return set;
		}

		static String stringFromFile(String path) {
			String line = "";
			IFStream file(path.c_str());
			if (file.good()) {
				String tmp = "";
				while (getline(file, tmp)) {
					line += "\n" + tmp;
				}
			} else throw BadFileException(path);
			return line;
		}

		static Bool createNewFile(String path = "", String content = "") {

			return true;
		}

	};

}

#endif
