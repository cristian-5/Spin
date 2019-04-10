
/*!
 *
 *    + --------------------------------------- +
 *    |  FileHandler.h                          |
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
#include <string.h>
#include <fstream>

#include "HeapCollection.h"

using namespace std;

using namespace HeapCollection;

class FileHandler {
	
public:
	
	/*!
	 *   @brief Bad File Exception.
	 *   Raised when the input file
	 *   is corrupted or the path is invalid.
	 *   @author Cristian A.
	 */
	class BadFileException: public exception {

	private:

		const string * _path;

	public:

		BadFileException(string & path): exception() { _path = & path; }
		
		const string getPath() const { return * _path; }

	};
	
	static StrongList<string> linesFromFile(string & path) {
		StrongList<string> set = StrongList<string>();
		ifstream file(path.c_str());
		if (file.good()) {
			string line = "";
			while (getline(file, line)) {
				set.link(line);
			}
		} else throw BadFileException(path);
		return set;
	}
	
	static string stringFromFile(string path) {
		string line = "";
		ifstream file(path.c_str());
		if (file.good()) {
			string tmp = "";
			while (getline(file, tmp)) {
				line += "\n" + tmp;
			}
		} else throw BadFileException(path);
		return line;
	}
	
	static bool createNewFile(string path = "", string content = "") {
		
		return true;
	}
	
};


#endif
