
/*!
 *
 *    + --------------------------------------- +
 *    |  Object.hpp                             |
 *    |                                         |
 *    |               Stack Object              |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
 */

#ifndef STACKOBJECT
#define STACKOBJECT

#include "../Aliases/Aliases.hpp"

namespace Stack {

	enum BasicType {

		BooleanType,
		CharacterType,

		Int8Type,
		Int16Type,
		Int32Type,
		Int64Type,
		UInt8Type,
		UInt16Type,
		UInt32Type,
		UInt64Type,

		ColourType,

		FloatType,
		DoubleType,
		RealType,

		ImaginaryType,
		ComplexType,

		StringType,

		ArrayListType,

		VectorType,

		ClassType,
		StructureType,
		ExceptionType,

		UnknownType

	};

	class Object {

		public:

		void * value = nullptr;

		BasicType type = BasicType::UnknownType;

		Object() { }

		Object(BasicType t, void * v) {
			type = t; value = v;
		}

		~Object() { delete value; }

	};

}

#endif
