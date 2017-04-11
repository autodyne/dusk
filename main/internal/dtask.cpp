/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "dtask.hpp"

namespace sun {
Task::Task(funct f, param a) {
	this->fun = f;
	this->arg = a;
	done = false;
}

bool Task::isDone(void) {
	return done;
}

void Task::invoke(void) {
	ret = fun(arg);
	done = true;
}

param Task::fruit(void) {
	return ret;
}
};
