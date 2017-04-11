/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DEQUE_H
#define _DEQUE_H

#include <atomic>
#include "dprop.hpp"
#include "dwrap.hpp"
#include "dtask.hpp"

namespace sun {
struct Index {
	idx32 tag;
	idx32 idx;
};
class Deque {
private:
	Task** data;
	size_t size;
	std::atomic<Index> head;
	std::atomic<idx64> tail;
public:
	Deque(void);
	bool empty(void);
	void push(Task *task);
	Task* take_tail(void);
	Task* take_head(void);
};
};

#define CAS compare_exchange_strong

#endif
