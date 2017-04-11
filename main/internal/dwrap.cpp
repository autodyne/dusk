/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <numa.h>
#include <iterator>
#include <algorithm>
#include "dwrap.hpp"

namespace sun {
void ignite(vproc *&proc, cpuid coreId) {
	proc = new vproc(routine, coreId);
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(coreId, &set);
	const auto size = sizeof(cpu_set_t);
	auto native = proc->native_handle();
	auto affin = pthread_setaffinity_np;
	affin(native, size, &set);
}

void sortnodes(cpuid nodes[], cpuid id) {
	const size_t num = totalNodes;
	for(int n=0; n<num; n++) nodes[n] = n;
	cpuid *to = &nodes[num];
	std::sort(nodes, to, [&](int a,int b) {
		int dist1 = numa_distance(id, a);
		int dist2 = numa_distance(id, b);
		return dist1 < dist2;
	});
}

cpuid nodeof(const cpuid core) noexcept {
	return numa_node_of_cpu(core);
}

cpuid numcpu(const cpuid node) noexcept {
	size_t numcores = 0;
	for(size_t c=0; c<totalCores; c++) {
		int n = numa_node_of_cpu(c);
		if(n == node) numcores += 1;
	}
	return numcores;
}
};
