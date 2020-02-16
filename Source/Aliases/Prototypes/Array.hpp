
#include "../Aliases.hpp"

#ifndef SPIN_ARRAY_HPP
#define SPIN_ARRAY_HPP

#include <vector>

namespace Spin {

	class Object;

	class ArrayList {
		public:
		Array<Object *> * elements = nullptr;
		ArrayList(Array<Object *> * e);
		ArrayList(Object * o);
		ArrayList();
		~ArrayList();
		Object * copyAt(SizeType i) const;
		Object * referenceAt(SizeType i) const;
		ArrayList * copy() const;
		String stringValue() const;
	};

}

#endif
