
/*!
 *
 *    + --------------------------------------- +
 *    |  main.cpp                               |
 *    |                                         |
 *    |                  Main                   |
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

 #include <iostream>

#include "Collection/Collection.hpp"

#include "Lexer/Lexer.hpp"

using namespace std;

using namespace Collection;
using namespace Stack;

using UInt32 = std::uint32_t;

int main(int argc, const char * argv[]) {

	Lexer lexer = Lexer("fdghsjdhgf   345 2746.564 'f' \"dfjgd\" false");

	StrongList<Token> tokens = lexer.tokenize();

	for (UInt32 i = 0; i < tokens.count(); i++) {
		cout << tokens[i].type << ":    " << tokens[i].value << endl;
	}

	int x = 0;
	cin >> x;

	return EXIT_SUCCESS;
}
