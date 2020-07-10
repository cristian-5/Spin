
/*!
 *
 *    + --------------------------------------- +
 *    |  Benchmark.cpp                          |
 *    |                                         |
 *    |                Benchmark                |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
!*/

#include "Benchmark.hpp"

#ifndef BENCHMARK_CPP
#define BENCHMARK_CPP

#include <chrono>

UInt64 Timer::time = 0;

void Timer::reset() {
	time = 0;
}

void Timer::start() {
	time = std::chrono::duration_cast
			<std::chrono::milliseconds>
			(std::chrono::system_clock::now()
			.time_since_epoch()).count();
}

void Timer::stop() {
	time = std::chrono::duration_cast
			<std::chrono::milliseconds>
			(std::chrono::system_clock::now()
			.time_since_epoch()).count() - time;
}

#endif
