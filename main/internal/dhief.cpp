/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <cassert>
#include <cmath>
#include "dehcs.hpp"

namespace sun {
Task* PdRws::spawn(Task *egg) const {
	SELF->push(egg);
	return egg;
}

Task* PdRws::fetch(Task *now) const {
	Task *own = SELF->filo();
	if(own != NULL) return own;
	return CORE(rand())->fifo();
}

param AdRws::start(Task *ini) const {
	ZERO->launch();
	// you must kill all threads!!!!
	for(CORES(c)) CORE(c)->attach();
	return ini->fruit();
}

static bool deployment(Task *force) {
	const auto numc = totalCores;
	thread_local size_t incr = 0;
	const size_t c = sun::coreId;
	size_t off = ceil(log2(c+1));
	size_t aim = (1<<off+incr)+c;
	if(aim >= numc) return false;
	auto target = sun::CORE(aim);
	assert(target->hide(force));
	target->launch();
	return incr++, true;
}

Task* AdRws::spawn(Task *egg) const {
	if(!SELF->hide(egg)) {
	if(!deployment(egg)) {
		SELF->push(egg);
	}
	}
	return egg;
}
};
