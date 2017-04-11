/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <numa.h>
#include <cmath>
#include <algorithm>
#include "dprop.hpp"

namespace sun {
const size_t totalCores = []() {
	auto NUMC = "DUSK_WORKER_NUM";
	char *val = getenv(NUMC);
	if(val) return atoi(val);
	return numa_num_task_cpus();
}();

const size_t totalNodes = []() {
	size_t numc = sun::totalCores;
	int numnodes = 0;
	for(size_t c=0; c<numc; c++) {
		using namespace std;
		int n = numa_node_of_cpu(c);
		numnodes = max(numnodes, n);
	}
	return numnodes + 1;
}();

const size_t stackSpace = []() {
	auto SIZE = "DUSK_STACK_SIZE";
	size_t len = 0x100000ull;
	char *val = getenv(SIZE);
	if(val) len = atoll(val);
	auto n = ceil(log2(len));
	return 1ul << (size_t) n;
}();

sun::Tactics tactics() noexcept {
	auto TACT = "DUSK_TACTICS";
	#define is(s) !strcmp(t,(s))
	char* t = getenv(TACT);
	if(!t) return PDRWS;
	if(is("QUEUE")) return QUEUE;
	if(is("PDRWS")) return PDRWS;
	if(is("ADRWS")) return ADRWS;
	return PDRWS;
}
};
