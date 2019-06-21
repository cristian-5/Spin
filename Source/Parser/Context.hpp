
/*!
 *
 *    + --------------------------------------- +
 *    |  Context.hpp                            |
 *    |                                         |
 *    |                 Context                 |
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

#ifndef STACKCONTEXT
#define STACKCONTEXT

#include <iostream>
#include <string>

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"

using namespace Collection;

namespace Stack {

	class Context {

        private:

        StrongList<Context> contexts = StrongList<Context>();

	};

    class FileUnitCTX: public Context {
        public:


    };

    class LibraryCTX: public Context {

        public:

        void hello() {

        }

    };

}

#endif
