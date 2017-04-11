/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DEHCS_H
#define _DEHCS_H

#include "dnuma.hpp"

namespace sun {
struct Sched {
	virtual param start(Task*) const = 0;
	virtual Task* spawn(Task*) const = 0;
	virtual Task* fetch(Task*) const = 0;
};
extern thread_local Task *task;
param root(funct f, param arg);
Task* dawn(funct f, param arg);
param dusk(Task* task)noexcept;
};

#include "dfifo.hpp"
#include "dhief.hpp"

#endif
