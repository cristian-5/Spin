
#include "../Common/Interface.hpp"

#ifndef SPIN_PROCESSOR_HPP
#define SPIN_PROCESSOR_HPP

#include <unordered_map>
#include <vector>

#include "../Utility/Stack.hpp"
#include "../Compiler/Program.hpp"

namespace Spin {


	class Processor {

		private:

		static Array<Pair<Pointer, Type>> objects;
		Array<ByteCode> instructions;

		Stack<Value> stack;
		Stack<SizeType> call;

		Processor() = default;
		~Processor() = default;

		typedef Value (* Process)(Value, Value);
		typedef Value (* Mutation)(Value);
		typedef void (* Immutable)(Value);

		static const Dictionary<Types, Mutation> cast;

		static const Dictionary<Types, Process> addition;
		static const Dictionary<Types, Process> subtraction;
		static const Dictionary<Types, Process> multiplication;
		static const Dictionary<Types, Process> division;
		static const Dictionary<Types, Process> modulus;

		static const Dictionary<Type, Mutation> negation;
		static const Dictionary<Type, Immutable> print;

		static const Dictionary<Types, Process> bitwiseAND;
		static const Dictionary<Types, Process> bitwiseOR;
		static const Dictionary<Types, Process> bitwiseXOR;

		static const Dictionary<Types, Process> inequality;
		static const Dictionary<Types, Process> equality;

		static const Dictionary<Types, Process> major;
		static const Dictionary<Types, Process> majorEqual;
		static const Dictionary<Types, Process> minor;
		static const Dictionary<Types, Process> minorEqual;

		static consteval Types compose(Type a, Type b) {
			return (Types)(((Types) a << 8) | b);
		}

		static const Real infinity;
		static const Real undefined;

		void freeObjects();

		public:

		Processor(const Processor &) = delete;
		Processor(Processor &&) = delete;
		Processor & operator = (const Processor &) = delete;
		Processor & operator = (Processor &&) = delete;
		static Processor * self() {
			static Processor instance;
			return & instance;
		}

		void run(Program * program);

	};

}

#endif
