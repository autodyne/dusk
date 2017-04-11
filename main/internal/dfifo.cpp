/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "dehcs.hpp"

namespace sun {
param Queue::start(Task *ini) const {
	for(CORES(c)) CORE(c)->launch();
	for(CORES(c)) CORE(c)->attach();
	return ini->fruit();
}

Task* Queue::spawn(Task *egg) const {
	ZERO->push(egg);
	return egg;
}

Task* Queue::fetch(Task *now) const {
	return ZERO->fifo();
}
};
