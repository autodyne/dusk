/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "dehcs.hpp"

namespace sun {
Task *initial = nullptr;
thread_local Task *task;

const auto sched = []()->Sched* {
	switch(sun::tactics()) {
	case QUEUE: return new Queue;
	case PDRWS: return new PdRws;
	case ADRWS: return new AdRws;
	}
	return nullptr;
}();

static void spinwait(Task *target) {
	while(!target->isDone()) {
		auto loot = sched->fetch(task);
		if(loot) (task=loot)->invoke();
	}
}

void routine(cpuid cpuId) noexcept {
	sun::nodeId = nodeof(cpuId);
	sun::coreId = cpuId;
	task = nullptr;
	sun::spinwait(initial);
}

param dusk(Task* waiting) noexcept {
	spinwait(waiting);
	auto results = waiting->fruit();
	delete waiting;
	return results;
}

Task* dawn(funct func, param argm) {
	Task *egg = new Task(func, argm);
	return sched->spawn(egg);
}

param root(funct func, param argm) {
	initial = new Task(func,argm);
	ZERO->hide(initial);
	sched->start(initial);
	auto res = initial->fruit();
	delete initial;
	// did you kill all threads?
	return initial = nullptr, res;
}
};
