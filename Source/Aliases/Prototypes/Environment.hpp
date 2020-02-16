
#include "../Aliases.hpp"

#ifndef SPIN_ENVIRONMENT_HPP
#define SPIN_ENVIRONMENT_HPP

#include <vector>
#include <unordered_map>

#include "Exception.hpp"

namespace Spin {

	class Object;

	class Environment {
		private:
		Environment * enclosing = nullptr;
		Dictionary<String, Object *> values = Dictionary<String, Object *>();
		Array<Object *> lostAndFound = Array<Object *>();
		public:
		class VariableRedefinitionException: public Exception {
			private:
			const String type;
			public:
			VariableRedefinitionException(String type);
			const String & getType() const;
		};
		class VariableNotFoundException: public Exception {
			public: VariableNotFoundException() = default;
		};
		Environment() = default;
		Environment(Environment * enclosing);
		~Environment();
		void define(String name, Object * value);
		void forget(String name);
		void lose(Object * something);
		void collect();
		void unbind(String name);
		Object * getReference(String name);
		Object * getValue(String name);
	};

}

#endif
