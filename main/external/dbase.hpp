/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#ifndef _DBASE_H
#define _DBASE_H

namespace sun {
class Task;

void launch(void(*pad)(void));

template<typename Arg> void salvo(void (*f)(Arg), Arg a, Arg b) {
	using funct = void*(*)(void*);
	extern Task* dawn(funct fn, void* a);
	extern void* dusk(Task* waitingtask);
	auto t1 = dawn((funct) f, (void*) a);
	auto t2 = dawn((funct) f, (void*) b);
	dusk(t2); // don't call like f(b)!
	dusk(t1); // everytime fork tasks!
}

template<typename Idx> void burst(Idx round, void (*body)(Idx)) {
	using funct = void*(*)(void*);
	extern Task* dawn(funct fn, void* a);
	extern void* dusk(Task* waitingtask);
	Task** th = new Task*[round];
	for(Idx i=0;i<round;i++) th[i] = dawn((funct)body, (void*)i);
	for(Idx i=0;i<round;i++) dusk(th[i]);
	delete th;
}
};

#endif
