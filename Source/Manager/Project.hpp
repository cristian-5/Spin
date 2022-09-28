
#include "../Common/Header.hpp"

#ifndef SPIN_PROJECT_HPP
#define SPIN_PROJECT_HPP

#include "../Compiler/Program.hpp"

namespace Spin {

	class Project {
		public:
		Project() = delete;
		static SourceCode * from(String path);
	};

}

#endif
