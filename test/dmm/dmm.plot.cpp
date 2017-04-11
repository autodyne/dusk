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

const size_t RANK = dget("RANK", 8192);
const size_t GRAN = dget("GRAN", 128);
const size_t TIME = dget("TIME", 10);
const size_t SKIP = dget("SKIP", 4);
const size_t nCPU = sun::totalCores;

void plot(string tact, string bin);

int main(void) {
	time_t timer;
	time(&timer);
	printf("%s", ctime(&timer));
	printf("matrix: %zu ", RANK);
	printf("grain: %zu\n", GRAN);
	dset("RANK", RANK);
	dset("GRAN", GRAN);
	dset("TIME", TIME);
	fflush(stdout);
	plot("QUEUE", "./dmm.grid");
	plot("PDRWS", "./dmm.tree");
	plot("ADRWS", "./dmm.tree");
	time(&timer);
	printf("%s", ctime(&timer));
}

void plot(string tact, string bin) {
	const char* ctac = tact.c_str();
	const char* cbin = bin.c_str();
	setenv("DUSK_TACTICS", ctac, 1);
	printf("(%s) %s\n", ctac, cbin);
	size_t cpu = 1;
PLOT_LOOP:
	dset("DUSK_WORKER_NUM", cpu);
	printf("*%3zu cores: ", cpu);
	const char *cmd = bin.c_str();
	FILE *stream = popen(cmd,"r");
	const size_t SIZE = 1024;
	char line[SIZE];
	while(fgets(line, SIZE, stream)) {
		if(line[0]!='#') printf(line);
	}
	pclose(stream);
	fflush(stdout);
	cpu += (cpu < SKIP)? cpu: SKIP;
	if(cpu <= nCPU) goto PLOT_LOOP;
}
