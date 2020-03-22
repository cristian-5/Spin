
#include "../Aliases.hpp"

#ifndef SPIN_OBJECT_HPP
#define SPIN_OBJECT_HPP

namespace Spin {

	class Token;

	enum BasicType: UInt8 {

		BooleanType,
		CharacterType,

		ByteType,
		
		IntegerType,

		RealType,

		ImaginaryType,
		ComplexType,

		StringType,

		ArrayType,

		VectorType,

		RoutineType,

		ClassType,
		InstanceType,
		StructureType,
		ExceptionType,

		UnknownType

	};

	using BasicTypes = UInt16;

	enum Modifier: UInt8 {
		publicAccess,
		hiddenAccess,
		secureAccess,
		immuneAccess
	};

	class Object {
		public:
		void * value = nullptr;
		BasicType type = BasicType::UnknownType;
		Object() = default;
		Object(BasicType t);
		Object(BasicType t, void * v);
		~Object();
		void safeDestroy();
		Boolean isByte() const;
		Boolean isInteger() const;
		Boolean isReal() const;
		Boolean isComplexType() const;
		Boolean isNumericType() const;
		Boolean isBoolean() const;
		Boolean isString() const;
		Boolean isCharacter() const;
		String getObjectName() const;
		Object * copy() const;
		String getObjectStringValue() const;
		Boolean isUnknown() const;
		Boolean isFunction() const;
		Boolean isCallable() const;
		Boolean isArray() const;
		Boolean isSubscriptable() const;
		Boolean getBoolValue() const;
		Object * getAttribute(String & name);
		static BasicType typeFromString(String & s);
		static Object * fromLiteral(Token * t);
	};

}

#endif
