
#include "../Aliases.hpp"

#ifndef SPIN_OBJECT_HPP
#define SPIN_OBJECT_HPP

namespace Spin {

	class Token;

	enum BasicType: UInt8 {

		BoolType,
		CharacterType,

		ByteType,
		
		IntegerType,

		RealType,

		ImaginaryType,
		ComplexType,

		StringType,

		ArrayType,

		VectorType,

		FunctionType,

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
		secureAccess
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
		Bool isByte() const;
		Bool isInteger() const;
		Bool isReal() const;
		Bool isComplexType() const;
		Bool isNumericType() const;
		Bool isBool() const;
		Bool isString() const;
		Bool isCharacter() const;
		String getObjectName() const;
		Object * copy() const;
		String getObjectStringValue() const;
		Bool isUnknown() const;
		Bool isFunction() const;
		Bool isCallable() const;
		Bool isArray() const;
		Bool isSubscriptable() const;
		Bool getBoolValue() const;
		static BasicType typeFromString(String & s);
		static Object * fromLiteral(Token * t);
	};

}

#endif
