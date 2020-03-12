
#include "../Aliases.hpp"

#ifndef SPIN_PROCESSOR_HPP
#define SPIN_PROCESSOR_HPP

#include <unordered_map>

#include "Token.hpp"
#include "Object.hpp"

namespace Spin {

	class CodeUnit;

	class Processor {
		private:
		Processor();
		~Processor() = default;
		typedef Lambda<Object * (Object *)> UnaryHandler;
		typedef Lambda<Object * (Object *, Object *)> BinaryHandler;
		typedef Lambda<void (Object *, Object *)> AssignmentHandler;
		Dictionary<BasicType, UnaryHandler> unaryNegation;
		Dictionary<BasicType, UnaryHandler> unaryInversion;
		Dictionary<BasicTypes, BinaryHandler> binaryAddition;
		Dictionary<BasicTypes, BinaryHandler> stringAddition;
		Dictionary<BasicTypes, BinaryHandler> binarySubtraction;
		Dictionary<BasicTypes, BinaryHandler> binaryMultiplication;
		Dictionary<BasicTypes, BinaryHandler> binaryDivision;
		Dictionary<BasicTypes, BinaryHandler> binaryAND;
		Dictionary<BasicTypes, BinaryHandler> binaryXOR;
		Dictionary<BasicTypes, BinaryHandler> binaryOR;
		void applyAdditionAssignment(Token * t, Object * l, Object * r);
		void applySubtractionAssignment(Token * t, Object * l, Object * r);
		void applyMultiplicationAssignment(Token * t, Object * l, Object * r);
		void applyDivisionAssignment(Token * t, Object * l, Object * r);
		void applyModulusAssignment(Token * t, Object * l, Object * r);
		void applyORAssignment(Token * t, Object * l, Object * r);
		void applyANDAssignment(Token * t, Object * l, Object * r);
		void applyXORAssignment(Token * t, Object * l, Object * r);
		Object * applyAddition(Token * t, Object * l, Object * r);
		Object * applySubtraction(Token * t, Object * l, Object * r);
		Object * applyMultiplication(Token * t, Object * l, Object * r);
		Object * applyDivision(Token * t, Object * l, Object * r);
		Object * applyModulus(Token * t, Object * l, Object * r);
		Object * applyAND(Token * t, Object * l, Object * r);
		Object * applyXOR(Token * t, Object * l, Object * r);
		Object * applyOR(Token * t, Object * l, Object * r);
		Dictionary<BasicTypes, BinaryHandler> binaryStrictEquality;
		Dictionary<BasicTypes, BinaryHandler> binaryMixedEquality;
		Dictionary<BasicTypes, BinaryHandler> binaryMajor;
		Dictionary<BasicTypes, BinaryHandler> binaryMajorEqual;
		Dictionary<BasicTypes, BinaryHandler> binaryMinor;
		Dictionary<BasicTypes, BinaryHandler> binaryMinorEqual;
		Object * applyEquality(Token * t, Object * l, Object * r);
		Object * applyInequality(Token * t, Object * l, Object * r);
		Object * applyMajor(Token * t, Object * l, Object * r);
		Object * applyMajorEqual(Token * t, Object * l, Object * r);
		Object * applyMinor(Token * t, Object * l, Object * r);
		Object * applyMinorEqual(Token * t, Object * l, Object * r);
		Dictionary<BasicType, AssignmentHandler> pureAssignment;
		Dictionary<BasicTypes, AssignmentHandler> mixedAssignment;
		BasicTypes compose(BasicType a, BasicType b);
		public:
		CodeUnit * currentUnit = nullptr;
		Processor(const Processor &) = delete;
		Processor(Processor &&) = delete;
		Processor & operator = (const Processor &) = delete;
		Processor & operator = (Processor &&) = delete;
		static Processor * self() {
			static Processor instance;
			return & instance;
		}
		Object * applyComparisonOperator(Token * t, Object * l, Object * r);
		Object * applyBinaryOperator(Token * t, Object * l, Object * r);
		Object * applySubscriptOperator(Token * t, Object * l, Object * r);
		Object * applyUnaryOperator(Token * t, Object * o);
		void applyAssignment(Token * t, Object * l, Object * r);
		void applyVectorAssignment(Token * t, Object * l, Object * r);
		void applyMutableAssignment(Token * t, Object * l, Object * r);
		Object * applyInnerProduct(Token * t, Object * l, Object * r);
		Object * applyOuterProduct(Token * t, Object * l, Object * r);
	};

}

#endif
