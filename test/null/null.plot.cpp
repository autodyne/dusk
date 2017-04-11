/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <dusk.hpp>

using namespace sun;
using namespace std;

const size_t TASK = dget("TASK", stackSpace);
const size_t SKIP = dget("SKIP", 1);
const size_t nCPU = sun::totalCores;

void plot(string tact, string bin);

int main(void) {
	time_t timer;
	time(&timer);
	printf("%s", ctime(&timer));
	printf("%zu tasks\n", TASK);
	dset("TASK", TASK);
	fflush(stdout);
	size_t cpu = 1;
PLOT_LOOP:
	dset("DUSK_WORKER_NUM", cpu);
	printf("*%3zu cores: ", cpu);
	const char *cmd = "./null.main";
	FILE *stream = popen(cmd, "r");
	const size_t SIZE = 1024;
	char line[SIZE];
	while(fgets(line, SIZE, stream)) {
		if(line[0]!='#') printf(line);
	}
	pclose(stream);
	fflush(stdout);
	cpu += (cpu < SKIP)? cpu: SKIP;
	if(cpu <= nCPU) goto PLOT_LOOP;
	time(&timer);
	printf("%s", ctime(&timer));
}
