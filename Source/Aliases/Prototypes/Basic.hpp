
#include "../Aliases.hpp"

#ifndef SPIN_BASIC_HPP
#define SPIN_BASIC_HPP

#include <unordered_map>

#include "Object.hpp"
#include "Environment.hpp"

namespace Spin {

	class BasicBoolean {
		private:
		typedef Lambda<Object * (Boolean *)> BooleanHandler;
		static const Dictionary<String, BooleanHandler> attributes;
		public:
		BasicBoolean() = delete;
		static inline Object * handleGetValue(Boolean * self, String & name) {
			auto search = attributes.find(name);
			if (search != attributes.end()) {
				return (search -> second)(self);
			}
			throw Environment::VariableNotFoundException();
		}
	};

	class BasicString {
		private:
		typedef Lambda<Object * (String *)> StringHandler;
		static const Dictionary<String, StringHandler> attributes;
		public:
		BasicString() = delete;
		static inline Object * handleGetValue(String * self, String & name) {
			auto search = attributes.find(name);
			if (search != attributes.end()) {
				return (search -> second)(self);
			}
			throw Environment::VariableNotFoundException();
		}
	};

}

#endif
