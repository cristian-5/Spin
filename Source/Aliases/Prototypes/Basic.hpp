
#include "../Aliases.hpp"

#ifndef SPIN_BASIC_HPP
#define SPIN_BASIC_HPP

namespace Spin {

	class Object;

	class BasicString {
		private:
		static inline Object * getLengthProperty(String * self);
		static inline Object * getIsEmptyMethod(String * self);
		public:
		BasicString() = delete;
		static Object * handleGetValue(String * self, String & name);
	};

}

#endif
