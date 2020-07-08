
#include "../Common/Header.hpp"

#ifndef SPIN_PROCESSOR_HPP
#define SPIN_PROCESSOR_HPP

#include <unordered_map>
#include <vector>

#include "../Common/Stack.hpp"
#include "../Compiler/Program.hpp"

namespace Spin {


	class Processor {

		private:

		Array<Value> literals;
		Array<ByteCode> instructions;

		Stack<Value> stack;

		Processor() = default;
		~Processor() = default;

		typedef Value (* Process)(Value &, Value &);
		typedef Value (* Mutation)(Value &);

		static const Dictionary<Types, Process> addition;
		static const Dictionary<Types, Process> subtraction;
		static const Dictionary<Types, Process> multiplication;
		static const Dictionary<Types, Process> division;
		static const Dictionary<Types, Process> modulus;

		static const Dictionary<Type, Mutation> negation;

		static const Dictionary<Types, Process> smartAND;
		static const Dictionary<Types, Process> smartOR;
		static const Dictionary<Types, Process> smartXOR;

		static const Dictionary<Types, Process> inequality;
		static const Dictionary<Types, Process> equality;

		static const Dictionary<Types, Process> major;
		static const Dictionary<Types, Process> majorEqual;
		static const Dictionary<Types, Process> minor;
		static const Dictionary<Types, Process> minorEqual;

		static consteval Types compose(Type a, Type b) {
			return (Types)(((Types) a << 8) | b);
		}

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
