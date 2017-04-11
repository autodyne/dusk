/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "../internal/dehcs.hpp"

namespace sun {
void launch(void (*pad)(void)) {
	using funct = param(*)(param);
	root((funct)pad, (void*)NULL);
}
};
