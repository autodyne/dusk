/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "dcore.hpp"

#define MYSELF (id == coreId)

namespace sun {
thread_local cpuid coreId;
thread_local cpuid nodeId;

Core::Core(cpuid id):id(id) {
	seed = nullptr;
	core = nullptr;
}

void Core::launch(void) {
	ignite(core, id);
}

void Core::attach(void) {
	if(core) core->join();
	core = nullptr;
}

bool Core::hide(Task* task) {
	if(runq.empty() && !seed) {
		seed = task;
		return true;
	} return false;
}

void Core::push(Task* task) {
	runq.push(task);
}

sun::Task* Core::filo(void) {
	if (MYSELF && seed) {
		Task* t = seed;
		seed = nullptr;
		return t;
	} else {
		return runq.take_tail();
	}
}

sun::Task* Core::fifo(void) {
	if(MYSELF && seed) {
		Task* t = seed;
		seed = nullptr;
		return t;
	} else {
		return runq.take_head();
	}
}
};
