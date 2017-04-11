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
#include <cstring>
#include <string>
#include <dusk.hpp>

using namespace sun;
using namespace std;

const size_t ITEM = dget("ITEM", 1048576);
const size_t GRAN = dget("GRAN", 16);
const size_t TIME = dget("TIME", 10);
const size_t SKIP = dget("SKIP", 4);
const size_t nCPU = sun::totalCores;

const size_t ILIM = ITEM * nCPU;

size_t maxItem = 1;

void plot1(string tact, string bin);
void plot2(string tact, string bin);

int main() {
	while(maxItem < ILIM) maxItem *= 2;
	time_t timer;
	time(&timer);
	printf("%s", ctime(&timer));
	dset("GRAN", GRAN);
	dset("TIME", TIME);
	printf("[strong scaling]\n");
	plot1("QUEUE", "./gmm.grid");
	plot1("PDRWS", "./gmm.tree");
	plot1("ADRWS", "./gmm.tree");
	printf("[weak scaling]\n");
	plot2("QUEUE", "./gmm.grid");
	plot2("PDRWS", "./gmm.tree");
	plot2("ADRWS", "./gmm.tree");
	time(&timer);
	printf("%s", ctime(&timer));
}

void test(string bin, size_t item) {
	dset("ITEM", item);
	const char *cmd = bin.c_str();
	FILE *stream = popen(cmd,"r");
	const size_t SIZE = 1024;
	char line[SIZE];
	while(fgets(line,SIZE,stream)) {
		if(line[0]=='#') continue;
		printf(strtok(line,"\n"));
	}
	pclose(stream);
}

void plot1(string tact, string bin) {
	const char *ctac = tact.c_str();
	const char *cbin = bin.c_str();
	setenv("DUSK_TACTICS", ctac, 1);
	printf("(%s) %s: ", ctac, cbin);
	printf("%10zu items ", maxItem);
	printf("(grain: %2zu)\n", GRAN);
	fflush(stdout);
	size_t cpu = 1;
PLOT_LOOP:
	dset("DUSK_WORKER_NUM", cpu);
	printf("*%3zu cores: ", cpu);
	test(bin, maxItem);
	printf("\n");
	fflush(stdout);
	cpu += (cpu < SKIP)? cpu: SKIP;
	if(cpu <= nCPU) goto PLOT_LOOP;
}

void plot2(string tact, string bin) {
	const char* ctac = tact.c_str();
	const char* cbin = bin.c_str();
	setenv("DUSK_TACTICS", ctac, 1);
	printf("(%s) %s: ", ctac, cbin);
	printf("%zu items/core ", ITEM);
	printf("(grain: %2zu)\n", GRAN);
	fflush(stdout);
	size_t cpu = 1;
PLOT_LOOP:
	dset("DUSK_WORKER_NUM", cpu);
	printf("*%3zu cores: ", cpu);
	test(bin, ITEM * cpu);
	printf("\n");
	fflush(stdout);
	cpu += (cpu < SKIP)? cpu: SKIP;
	if(cpu <= nCPU) goto PLOT_LOOP;
}
