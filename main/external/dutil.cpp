/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <string>
#include <cassert>
#include <cstdlib>
#include <pthread.h>
#include "dutil.hpp"

using namespace std;

namespace sun {
size_t dget(ctext name, size_t def) {
	const auto val = getenv(name);
	return val? (stoll(val)): def;
}

size_t dset(ctext name, size_t val) {
	const auto str = to_string(val);
	setenv(name, str.c_str(), true);
	return val;
}

void anchoring(const size_t coreId) {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(coreId, &cpuset);
	size_t size = sizeof(cpu_set_t);
	pthread_t self = pthread_self();
	auto P = pthread_setaffinity_np;
	assert(!P(self, size, &cpuset));
}
};
