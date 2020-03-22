
#include "../Aliases.hpp"

#ifndef SPIN_BASIC_HPP
#define SPIN_BASIC_HPP

namespace Spin {

	class Object;

	class BasicBoolean {
		private:
		static inline Object * getToggleMethod(Boolean * self);
		static inline Object * getToStringMethod(Boolean * self);
		public:
		BasicBoolean() = delete;
		static Object * handleGetValue(Boolean * self, String & name);
	};

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
