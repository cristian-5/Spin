
#include "../Aliases.hpp"

#ifndef SPIN_CLASS_HPP
#define SPIN_CLASS_HPP

#include <vector>
#include <unordered_map>

#include "Routines.hpp"
#include "Token.hpp"
#include "Object.hpp"

namespace Spin {

	class AttributeStatement;
	class Interpreter;

	class Class: public CallProtocol {
		public:
		String name;
		Dictionary<String, Pair<Modifier, Object *>> * staticAttributes = nullptr;
		Array<AttributeStatement *> * dynamicAttributes = nullptr;
		Class(String n, Array<AttributeStatement *> * d =
			  new Array<AttributeStatement *>(),
			  Dictionary<String, Pair<Modifier, Object *>> * s =
			  new Dictionary<String, Pair<Modifier, Object *>>());
		void defineStatic(String name, Modifier access, Object * value);
		Object * call(Interpreter * i, Array<Object *> a, Token * c) override;
		Object * getInnerReference(String & name);
		Object * getInnerValue(String & name);
		Object * getReference(String & name);
		Object * getValue(String & name);
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
		void destroy();
	};

	class Instance {
		private:
		Dictionary<String, Pair<Modifier, Object *>> * attributes = nullptr;
		public:
		Class * type = nullptr;
		Instance(Class * t, Interpreter * i);
		Instance(Class * t, Dictionary<String, Pair<Modifier, Object *>> * a);
		void attributesInitialisation(Interpreter * i);
		void defineDynamic(String name, Modifier access, Object * value);
		Object * getInnerReference(String & name);
		Object * getInnerValue(String & name);
		Object * getReference(String & name);
		Object * getValue(String & name);
		String stringValue() const;
		Instance * copyByValue() const;
		Instance * copy() const;
		void destroy();
	};

}

#endif
