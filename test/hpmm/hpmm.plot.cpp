/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <fstream>
#include <sstream>
#include <dusk.hpp>

using namespace std;
using namespace sun;

const size_t ITEM = dget("ITEM", 1048576);
const size_t NODE = dget("NODE", 4);
const size_t GRAN = dget("GRAN", 16);
const size_t TIME = dget("TIME", 10);
const size_t SKIP = dget("SKIP", 4);
const size_t RANK = dget("RANK", 2);
const size_t nCPU = sun::totalCores;

using iifunc = int(*)(int);

string prefix;
string binary;
iifunc weight;

const size_t NLIM = NODE * nCPU;
const size_t ILIM = ITEM * nCPU;

size_t maxNode = 1;
size_t maxItem = 1;

void make1(string type, bool equal);
void make2(string type, bool fixed);
void plot1(string tact, string bin);
void plot2(string tact, string bin);

#define MODE1 "dist.equal"
#define MODE2 "dist.slope"
#define MODE3 "node.fixed"
#define MODE4 "node.scale"

int main() {
	while(maxNode < NLIM) maxNode *= 2;
	while(maxItem < ILIM) maxItem *= 2;
	time_t timer;
	time(&timer);
	printf("%s", ctime(&timer));
	dset("GRAN", GRAN);
	dset("TIME", TIME);
	fflush(stdout);
	make1(MODE1, 1);
	make1(MODE2, 0);
	make2(MODE3, 1);
	make2(MODE4, 0);
	printf("[strong scaling]\n");
	plot1("QUEUE", "hpmm.grid");
	plot1("PDRWS", "hpmm.tree");
	plot1("ADRWS", "hpmm.tree");
	printf("[weak scaling]\n");
	plot2("QUEUE", "hpmm.grid");
	plot2("PDRWS", "hpmm.tree");
	plot2("ADRWS", "hpmm.tree");
	time(&timer);
	printf("%s", ctime(&timer));
}

random_device rd;
mt19937 mt(rd());
poisson_distribution<> mixt1(10.0);
poisson_distribution<> mixt2(20.0);
poisson_distribution<> mixt3(30.0);
poisson_distribution<> mixt4(40.0);
uniform_int_distribution<> W(0, 9);

inline int poisson_mixture(int dim) {
	static int w = 0;
	switch(dim? w: w = W(mt)) {
		case 0: return mixt1(mt);
		case 1: return mixt2(mt);
		case 2: return mixt2(mt);
		case 3: return mixt3(mt);
		case 4: return mixt3(mt);
		case 5: return mixt3(mt);
		case 6: return mixt4(mt);
		case 7: return mixt4(mt);
		case 8: return mixt4(mt);
		case 9: return mixt4(mt);
	}
}

void make(size_t node, size_t item) {
	ostringstream name;
	name << ".";
	name << prefix;
	name << ".node" << node;
	name << ".item" << item;
	name << ".csv";
	ofstream ofs(name.str());
	size_t N[node];
	double denom = 0;
	size_t total = 0;
	for(size_t n=0; n<node; n++) {
		denom += N[n] = weight(n);
	}
	double diameter = item / denom;
	for(size_t n=0; n<node; n++) {
		total += N[n] *= diameter;
	}
	N[0] += item - total;
	for(size_t n=0; n<node; n++) {
	for(size_t i=0; i<N[n]; i++) {
		ofs << "n" << n << ",";
		for(int d=0; d<RANK; d++) {
			ofs << poisson_mixture(d);
			if(d < RANK-1) ofs << ",";
			else ofs << endl;
		}
	}
	}
}

const auto w1 = [](int n){return 1;};
const auto wn = [](int n){return n;};
const iifunc wfuncs[2] = {wn, w1};

void make1(string type, bool equal) {
	prefix = type;
	weight = wfuncs[equal];
	make(maxNode, maxItem);
}

void make2(string type, bool fixed) {
	prefix = type;
	weight = w1;
	size_t cpu = 1;
MAKE_LOOP:
	const size_t node = NODE * cpu;
	const size_t item = ITEM * cpu;
	make(fixed?maxNode:node, item);
	cpu += (cpu < SKIP)? cpu: SKIP;
	if(cpu <= nCPU) goto MAKE_LOOP;
}

void test(size_t node, size_t item) {
	printf(" (%s: ", prefix.c_str());
	printf("%4zu nodes ", node);
	ostringstream cmd1;
	cmd1 << "./" << binary;
	cmd1 << " ." << prefix;
	cmd1 << ".node" << node;
	cmd1 << ".item" << item;
	cmd1 << ".csv";
	const string cmd2 = cmd1.str();
	const auto cmd3 = cmd2.c_str();
	FILE *stream = popen(cmd3,"r");
	const size_t SIZE = 1024;
	char line[SIZE];
	while(fgets(line,SIZE,stream)) {
		if(line[0]=='#') continue;
		printf(strtok(line,"\n"));
	}
	pclose(stream);
	printf(")");
}

void plot1(string tact, string bin) {
	const char* ctac = tact.c_str();
	const char* cbin = bin.c_str();
	setenv("DUSK_TACTICS", ctac, 1);
	printf("(%s) %s: ", ctac, cbin);
	printf("%10zu items ", maxItem);
	printf("(grain: %2zu)\n", GRAN);
	binary = bin;
	fflush(stdout);
	size_t cpu = 1;
PLOT_LOOP:
	dset("DUSK_WORKER_NUM", cpu);
	printf("*%3zu cores: ", cpu);
	prefix = MODE1;
	test(maxNode, maxItem);
	prefix = MODE2;
	test(maxNode, maxItem);
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
	binary = bin;
	fflush(stdout);
	size_t cpu = 1;
PLOT_LOOP:
	size_t numNode = NODE * cpu;
	size_t numItem = ITEM * cpu;
	dset("DUSK_WORKER_NUM", cpu);
	printf("*%3zu cores: ", cpu);
	prefix = MODE3;
	test(maxNode, numItem);
	prefix = MODE4;
	test(numNode, numItem);
	printf("\n");
	fflush(stdout);
	cpu += (cpu < SKIP)? cpu: SKIP;
	if(cpu <= nCPU) goto PLOT_LOOP;
}
