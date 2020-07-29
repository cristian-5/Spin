
#include "../Common/Header.hpp"

#ifndef SPIN_COMPILER_HPP
#define SPIN_COMPILER_HPP

#include <vector>

namespace Spin {
	
	class Library {

		private:

		static const Array<String> libraries;

		public:

		static Boolean isKnown(String name);

	};

}

#endif
