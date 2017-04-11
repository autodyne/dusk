/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DTASK_H
#define _DTASK_H

#include "dwrap.hpp"

namespace sun {
class Task {
private:
	funct fun;
	param arg;
	param ret;
	bool done;
public:
	Task(funct,param);
	bool isDone(void);
	void invoke(void);
	param fruit(void);
};
};

#endif
