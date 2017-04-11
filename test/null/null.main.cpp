/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>
#include <dusk.hpp>
#include "../../main/internal/dnuma.hpp"

using namespace sun;
using namespace std;
using namespace std::chrono;
using ns = nanoseconds;

const size_t TASK = dget("TASK", 8);
const size_t nCPU = sun::totalCores;

std::atomic<size_t> sum;
void runTaskPollTest(const int tid);

int main(void) {
	auto cpus = new thread*[nCPU];
	auto worker = runTaskPollTest;
	for(size_t i=0; i<TASK; i++) {
		CORE(0)->push(nullptr);
	}
	for(size_t c=0; c<nCPU; c++) {
		cpus[c] = new thread(worker, c);
	}
	for(size_t c=0; c<nCPU; c++) {
		cpus[c]->join();
	}
	printf("%5zu nsecs\n", sum / TASK);
}

void runTaskPollTest(const int tid) {
	anchoring(tid);
	const size_t numTask = TASK / nCPU;
	const auto s = system_clock::now();
	for(size_t i=0; i<numTask; i++) {
		CORE(0)->fifo();
	}
	const auto e = system_clock::now();
	auto nsec = duration_cast<ns>(e-s);
	sum += nsec.count();
}
