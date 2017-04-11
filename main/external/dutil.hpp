/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DUTIL_H
#define _DUTIL_H

#include <cstddef>

namespace sun {
using ctext = const char*;
size_t dget(ctext name, size_t def);
size_t dset(ctext name, size_t val);
void anchoring(const size_t coreId);
};

#endif
