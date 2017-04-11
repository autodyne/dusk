/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DFIFO_H
#define _DFIFO_H

namespace sun {
class Queue: public Sched {
	param start(Task*) const;
	Task* spawn(Task*) const;
	Task* fetch(Task*) const;
};
};

#endif
