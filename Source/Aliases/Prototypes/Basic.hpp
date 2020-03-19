
#include "../Aliases.hpp"

#ifndef SPIN_BASIC_HPP
#define SPIN_BASIC_HPP

namespace Spin {

	class Object;

	class BasicString {
		private: BasicString() = delete;
		static inline Object * getLengthProperty(String * self);
		static inline Object * getIsEmptyMethod(String * self);
		public:
		static Object * handleGetValue(String * self, String & name);
	};

}

#endif
