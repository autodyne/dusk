/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DHIEF_H
#define _DHIEF_H

namespace sun {
class PdRws: public Queue {
	Task* spawn(Task*) const;
	Task* fetch(Task*) const;
};
class AdRws: public PdRws {
	param start(Task*) const;
	Task* spawn(Task*) const;
};
};

#endif
