
#include "../Aliases.hpp"

#ifndef SPIN_MANAGER_HPP
#define SPIN_MANAGER_HPP

#include <fstream>
#include <sstream>

#include "Exception.hpp"

using StringStream = std::stringstream;
using IFStream = std::ifstream;
using OFStream = std::ofstream;

namespace Spin {

	class Manager {
		public:
		class BadFileException: public Exception {
			private:
			const String path;
			public:
			BadFileException(const String & path);
			const String & getPath() const;
		};
		class BadAccessException: public Exception {
			private:
			const String path;
			public:
			BadAccessException(const String & path);
			const String & getPath() const;
		};
		Manager() = delete;
		static UInt64 getLine(String * input, SizeType cursor);
		static String * stringFromFile(String path);
		static void createNewFile(String path, String content = String());
	};

}

#endif
