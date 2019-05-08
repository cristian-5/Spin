
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

#include "../Collection/Collection.hpp"

using namespace Collection;

using String = std::string;
using Exception = std::exception;
using IFStream = std::ifstream;

#define Boolean bool
#define stringValue c_str

/*! @brief Namespace Stack */
namespace Stack {

	/*!
	 *   @brief Bad File Exception.
	 *   Raised when the input file
	 *   is corrupted or the path is invalid.
	 *   @author Cristian A.
	 */
	class BadFileException: public Exception {

		private:

		const String * p;

		public:

		BadFileException(String & path): Exception() { p = & path; }

		const String getPath() const { return * p; }

	};

	class FileHandler {

		public:

		static StrongList<String> linesFromFile(String & path) {
			StrongList<String> set = StrongList<String>();
			IFStream file(path.stringValue());
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
			IFStream file(path.stringValue());
			if (file.good()) {
				String tmp = "";
				while (getline(file, tmp)) {
					line += "\n" + tmp;
				}
			} else throw BadFileException(path);
			return line;
		}

		static Boolean createNewFile(String path = "", String content = "") {

			return true;
		}

	};

}

#endif