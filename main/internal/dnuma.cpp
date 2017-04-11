/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "dnuma.hpp"

namespace sun {
static Core** makecores(void) {
	const auto tc = totalCores;
	auto cores = new Core*[tc];
	for(cpuid c=0; c<tc; c++) {
		cores[c] = new Core(c);
	}
	return cores;
}

static Node** makenodes(void) {
	const auto tn = totalNodes;
	auto nodes = new Node*[tn];
	for(cpuid n=0; n<tn; n++) {
		nodes[n] = new Node(n);
	}
	return nodes;
}

static cpuid* maketable(void) {
	const cpuid tn = totalNodes;
	auto ids = new cpuid[tn*tn];
	for(cpuid n=0; n<tn; n++) {
		sortnodes(&ids[n*tn], n);
	}
	return ids;
}

Core* CORE(cpuid id) noexcept {
	static auto C = makecores();
	return C[id % totalCores];
}

Node* NODE(cpuid id) noexcept {
	static auto N = makenodes();
	return N[id % totalNodes];
}

Node* NEAR(cpuid node, int i) {
	static auto T = maketable();
	const cpuid tn = totalNodes;
	const cpuid off = node * tn;
	return NODE(T[off + i]);
}
};
