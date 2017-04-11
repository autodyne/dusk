/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <cassert>
#include "deque.hpp"

namespace sun {
Deque::Deque(void) {
	tail.store(0);
	Index h;
	h.idx = 0;
	h.tag = 0;
	head.store(h);
	size = sun::stackSpace;
	assert(size<INT32_MAX);
	data = new Task*[size];
}

bool Deque::empty(void) {
	auto h = head.load();
	auto t = tail.load();
	return t <= h.idx;
}

void Deque::push(Task *task) {
	auto t = tail.load();
	data[t] = task;
	tail.store(t +1);
	assert(t < size);
}

Task* Deque::take_head(void) {
	auto h = head.load();
	auto t = tail.load();
	if(t <= h.idx) return NULL;
	Task *popped = data[h.idx];
	auto h2 = h;
	h2.idx += 1;
	if(head.CAS(h, h2)) {
		return popped;
	} return NULL;
}

Task* Deque::take_tail(void) {
	auto t = tail.load();
	if(t==0) return NULL;
	tail.store(--t);
	Task *task = data[t];
	auto h = head.load();
	if(t > h.idx) return task;
	tail.store(0);
	Index h2;
	h2.idx = 0;
	h2.tag = h.tag + 1;
	if(t == h.idx) {
	if(head.CAS(h, h2)) {
		return task;
	}
	}
	head.store(h2);
	return nullptr;
}
};
