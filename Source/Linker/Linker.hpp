
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

#ifndef STACKLINKER
#define STACKLINKER

#include "../Aliases/Aliases.hpp"

namespace Stack {

	/*!
	 *   @brief Bad File Exception.
	 *   Raised when the input file
	 *   is corrupted or the path is invalid.
	 */
	class BadFileException: public Exception {
		private:
		const String path;
		public:
		const String & getPath() const { return path; }
		BadFileException(const String & p):
		Exception(), path(p) { }
	};

	/*!
	 *   @brief Bad Access Exception.
	 *   Raised when the output file
	 *   is corrupted or the path is invalid.
	 */
	class BadAccessException: public Exception {
		private:
		const String path;
		public:
		const String & getPath() const { return path; }
		BadAccessException(const String & p):
		Exception(), path(p) { }
	};

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

	class Linker {

		private:

		Linker() = default;

		public:

		static FilePosition getPosition(String * input, UInt32 cursor) {
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

		static ArrayList<String> linesFromFile(String & path) {
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

		static void createNewFile(String path, String content = "") {
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
