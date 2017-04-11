/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DPROP_H
#define _DPROP_H

#include <cstddef>

namespace sun {
enum Tactics {
	QUEUE,
	PDRWS,
	ADRWS
};
Tactics tactics() noexcept;
extern const size_t totalCores;
extern const size_t totalNodes;
extern const size_t stackSpace;
};

#endif
