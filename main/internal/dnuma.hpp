/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DNUMA_H
#define _DNUMA_H

#include "dprop.hpp"
#include "dnode.hpp"

namespace sun {
Node* NEAR(cpuid node, int rank);
};

#define ZERO sun::CORE(0)
#define SELF sun::CORE(sun::coreId)

#endif
