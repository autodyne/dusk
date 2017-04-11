/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <chrono>
#include <cmath>
#include <cstdio>
#include <map>
#include <random>
#include <fstream>
#include <sstream>
#include <dusk.hpp>

using namespace sun;
using namespace std;
using namespace std::chrono;
using ns = nanoseconds;

const size_t GRAN = dget("GRAN", 32);
const size_t TIME = dget("TIME", 10);

size_t ITEM = 0;

constexpr int RANK = 2;
constexpr int MIXT = 4;

struct Item {size_t X[RANK];};

struct Node {
	string name;
	double W[MIXT];
	size_t size() const;
	std::vector<Item*> samples;
	Node(const string n):name(n){}
	double pdf(size_t, int) const;
};

double M[MIXT][RANK];
vector<Node*> nodes;

size_t Node::size() const {return samples.size();}

inline double poisson(double param, size_t value) {
	double fact = 1;
	for(int k=2; k<value; k++) fact *= k;
	return pow(param, value) * exp(-param) / fact;
}

double Node::pdf(size_t itemId, int mixtId) const {
	Item* sample = samples[itemId];
	double result = W[mixtId];
	for(int d = 0; d < RANK; d++) {
		double m = M[mixtId][d];
		size_t x = sample->X[d];
		result *= poisson(m, x);
	}
	return result;
}

static void makeHierarchicalPoissonMixtureModel() {
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<> rnd(0, 1);
	for(int k=0;k<MIXT;k++) for(int d=0;d<RANK;d++) {
		M[k][d] = (size_t)(100 * rnd(mt));
	}
	for(auto it=nodes.begin();it!=nodes.end();it++) {
		Node* n = *it;
		for(int k=0;k<MIXT;k++) n->W[k] = 1.0/MIXT;
	}
}

static void showHierarchicalPoissonMixtureModel() {
	for(int k=0;k<MIXT;k++) {
		printf("#mixt %d: ", k);
		for(int d=0;d<RANK;d++) printf("%e ", M[k][d]);
		printf("\n");
	}
	for(auto it=nodes.begin();it!=nodes.end();it++) {
		Node* n = *it;
		printf("#node %5s: ", n->name.c_str());
		for(int k=0;k<MIXT;k++) printf("%e ", n->W[k]);
		printf("\n");
	}
}

static void loadSampleData(const string filepath) {
	ifstream ifs(filepath);
	string line, val, name;
	map<string, Node*> map;
	while(ifs && getline(ifs, line) && ++ITEM) {
		istringstream strm(line);
		Item *newItem = new Item;
		for(int i=0; getline(strm,val,','); i++) {
			if(i) newItem->X[i-1] = stoi(val);
			else name = val;
		}
		if(map.find(name) == map.end()) {
			map[name] = new Node(name);
			nodes.push_back(map[name]);
		}
		map[name]->samples.push_back(newItem);
	}
}

void doExpectationMaximizationOnce(void);

int main(int argc, char** argv) {
	anchoring(0);
	printf("#%d dims, ", RANK);
	printf("%d mixts\n", MIXT);
	printf("#loading %s\n", argv[1]);
	loadSampleData(argv[1]);
	const size_t NODE = nodes.size();
	printf("#%zu nodes,", NODE);
	printf("%zu items\n", ITEM);
	if(!NODE) exit(1);
	makeHierarchicalPoissonMixtureModel();
	const auto s = system_clock::now();
	for(int step = 0; step < TIME; step++) {
		printf("#estimating step %d\r", step);
		fflush(stdout);
		launch(doExpectationMaximizationOnce);
	}
	const auto e = system_clock::now();
	auto nsec = duration_cast<ns>(e-s);
	auto secs = nsec.count() / 1e9 / TIME;
	auto mrps = ITEM / secs / (1 << 20);
	showHierarchicalPoissonMixtureModel();
	printf("%10f secs ",  secs);
	printf("%10f MRPS\n", mrps);
}
