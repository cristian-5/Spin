
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

using namespace std;

//using namespace StackCompiler;
using namespace Collection;

int main(int argc, const char * argv[]) {

	LinkedList<int> c;
	c.link(39);
	cout << c[0] << endl;

	int cd;
	cin >> cd;

	return EXIT_SUCCESS;
}
