/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DCORE_H
#define _DCORE_H

#include "dwrap.hpp"
#include "deque.hpp"

namespace sun {
class Core {
private:
	vproc *core;
	Deque runq;
	Task *seed;
public:
	const cpuid id;
	Core(cpuid id);
	bool hide(Task*);
	void push(Task*);
	Task* filo(void);
	Task* fifo(void);
	void launch(void);
	void attach(void);
};
Core* CORE(cpuid index) noexcept;
extern thread_local cpuid coreId;
};

#endif
