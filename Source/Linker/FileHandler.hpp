
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

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"

using namespace Collection;

namespace Stack {

	/*!
	 *   @brief File Position Structure.
	 *   Used to determine a specific
	 *   line and column position of a
	 *   character.
	 */
	struct FilePosition {
		UInt32 col = 0;
		UInt32 row = 0;
	};

	static FilePosition getPosition(String * input, UInt32 cursor) {
		FilePosition result = { 0, 0 };
		if (cursor == 0 ||
			input -> length() == 0 ||
			cursor > input -> length()) return result;
		for (UInt32 i = 0; i < cursor; i++) {
			if (input -> at(i) == '\n') {
				result.row++;
				result.col = 0;
			} else result.col++;
		}
		result.col++;
		return result;
	}

	/*!
	 *   @brief Bad File Exception.
	 *   Raised when the input file
	 *   is corrupted or the path is invalid.
	 */
	class BadFileException: public Exception {
		private:
		const String * p;
		public:
		BadFileException(String & path): Exception() { p = & path; }
		const String getPath() const { return * p; }
	};

	/*!
	 *   @brief Bad Access Exception.
	 *   Raised when the output file
	 *   is corrupted or the path is invalid.
	 */
	class BadAccessException: public Exception {
		private:
		const String * p;
		public:
		BadAccessException(String & path): Exception() { p = & path; }
		const String getPath() const { return * p; }
	};

	/*! @brief File Handler Class. */
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

		static void createNewFile(String path = "", String content = "") {
			OFStream file(path.stringValue());
			try {
				file << content;
			} catch (Exception & e) {
				throw BadAccessException(path);
			}
		}

	};

}

#endif
