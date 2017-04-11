/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DWRAP_H
#define _DWRAP_H

#include <thread>
#include <pthread.h>
#include "dprop.hpp"

namespace sun {
using param = void *;
using cpuid = size_t;
using idx32 = uint32_t;
using idx64 = uint64_t;
using vproc = std::thread;
using funct = param(*)(param);
};

namespace sun {
void ignite(vproc*&, cpuid);
void sortnodes(cpuid*,cpuid);
cpuid nodeof(cpuid) noexcept;
cpuid numcpu(cpuid) noexcept;
void routine(cpuid) noexcept;
};

#define CORES(c) int c=0;c<totalCores;c++
#define NODES(n) int n=0;n<totalNodes;n++

#endif
