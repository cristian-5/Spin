
#include "../Common/Interface.hpp"

#ifndef SPIN_PROCESSOR_HPP
#define SPIN_PROCESSOR_HPP

#include <vector>

#include "../Utility/Stack.hpp"
#include "../Compiler/Program.hpp"

namespace Spin {


	class Processor {

		private:

		static Array<Pair<Pointer, Type>> objects;

		Stack<Value> stack;
		Stack<SizeType> call;
		Stack<SizeType> frame;

		Processor() = default;
		~Processor() = default;

		static consteval Types compose(Type a, Type b) {
			return (Types)(((Types) a << 8) | b);
		}

		static const Real infinity;
		static const Real undefined;

		void freeObjects();

		Value evaluate(Program * program);

		public:

		class Crash: Exception {
			private:
			SizeType address = 0;
			ByteCode instruction;
			public:
			Crash(SizeType a, ByteCode b);
			ByteCode getInstruction() const;
			SizeType getAddress() const;
		};

		Processor(const Processor &) = delete;
		Processor(Processor &&) = delete;
		Processor & operator = (const Processor &) = delete;
		Processor & operator = (Processor &&) = delete;
		static Processor * self() {
			static Processor instance;
			return & instance;
		}

		void run(Program * program);

		Value fold(Array<ByteCode> code);

	};

}

#endif
