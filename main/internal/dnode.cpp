/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <algorithm>
#include "dnode.hpp"

namespace sun {
Node::Node(cpuid i): id(i) {
	cpuid numc = numcpu(id);
	cores = new Core*[numc];
	for(CORES(c)) {
		size_t n = nodeof(c);
		if(n != id) continue;
		Core *cpu = CORE(c);
		cores[size++] = cpu;
	}
}

Core* Node::core(cpuid id) {
	return cores[id%size];
}
};
