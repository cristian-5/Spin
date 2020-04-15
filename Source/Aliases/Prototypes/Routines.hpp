
#include "../Aliases.hpp"

#ifndef SPIN_FUNCTION_HPP
#define SPIN_FUNCTION_HPP

#include <functional>
#include <vector>

#include "Exception.hpp"
#include "Environment.hpp"
#include "Object.hpp"

namespace Spin {

	class FunctionStatement;
	class ProcedureStatement;
	class Parameter;
	class Token;

	typedef Lambda<Object * (Array<Object *> a, Token * t)> NativeLambda;

	class ParameterException: public Exception {
		public: ParameterException() = default;
	};

	class CallProtocol {
		public:
		Object * self = nullptr;
		virtual ~CallProtocol() = default;
		virtual Object * call(Array<Object *> a, Token * c) = 0;
		virtual String stringValue() const = 0;
		virtual inline UInt32 arity() const = 0;
		virtual CallProtocol * copy() const = 0;
		inline void bindSelf(Environment * e) {
			if (self) e -> define("self", self);
		}
		inline void unbindSelf(Environment * e) {
			if (self) {
				e -> unbind("self");
				// Self is a wrapper to the original declaration.
				// We can't destroy the original and so we swap
				// it with something fake which easy to delete.
				// That way we only delete the wrapper leaving
				// the original declaration intact.
				self -> value = nullptr;
				delete self;
				self = nullptr;
			}
		}
		template<typename t>
		Boolean isInstanceOf() {
			return (DynamicCast<t *>(this));
		}
	};

	class Function: public CallProtocol {
		private:
		FunctionStatement * declaration = nullptr;
		public:
		Environment * closure = nullptr;
		Function() = default;
		Function(FunctionStatement * d, Environment * c);
		~Function() = default;
		Object * call(Array<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
    };

	class Procedure: public CallProtocol {
		private:
		ProcedureStatement * declaration = nullptr;
		public:
		Environment * closure = nullptr;
		Procedure() = default;
		Procedure(ProcedureStatement * d, Environment * c);
		~Procedure() = default;
		Object * call(Array<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
    };

	class NativeFunction: public CallProtocol {
		private:
		Array<Parameter *> * params = nullptr;
		NativeLambda lambda = nullptr;
		String name;
		Boolean mutableParameters = false;
		public:
		NativeFunction(NativeLambda l, Array<Parameter *> * p, String n, Boolean m = false);
		~NativeFunction() = default;
		Object * call(Array<Object *> a, Token * c) override;
		void deallocate(Array<Object *> & parameters);
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
	};

	class NativeProcedure: public CallProtocol {
		private:
		Array<Parameter *> * params = nullptr;
		NativeLambda lambda = nullptr;
		String name;
		Boolean mutableParameters = false;
		public:
		NativeProcedure(NativeLambda l, Array<Parameter *> * p, String n, Boolean m = false);
		~NativeProcedure() = default;
		Object * call(Array<Object *> a, Token * c) override;
		void deallocate(Array<Object *> & parameters);
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
	};

}

#endif
