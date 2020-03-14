
#include "../../Source/Aliases/Aliases.hpp"

#ifndef SPIN_BENCHMARK_HPP
#define SPIN_BENCHMARK_HPP

namespace Spin {

	class Timer {
		private: Timer() = delete;
		public:
		static UInt64 time;
		static void reset();
		static void start();
		static void stop();
	};

}

#endif
