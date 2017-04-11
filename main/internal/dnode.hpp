/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DNODE_H
#define _DNODE_H

#include <vector>
#include "dprop.hpp"
#include "dwrap.hpp"
#include "dcore.hpp"

namespace sun {
class Node {
private:
	Core **cores;
	cpuid size = 0;
public:
	const cpuid id;
	Node(cpuid id);
	Core* core(cpuid);
};
Node* NODE(cpuid index) noexcept;
extern thread_local cpuid nodeId;
};

#endif
