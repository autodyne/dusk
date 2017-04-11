/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <sched.h>
#include "hpmm.main.hpp"

struct Local {
	double N[MIXT] = {};
	double L[MIXT][RANK] = {};
};

Local** locals = [&](void) {
	size_t P = sun::totalCores;
	Local** bufs = new Local*[P];
	while(P--) bufs[P] = new Local;
	return bufs;
}();

void doExpectationMaximizationItem(size_t cell) {
	Node* gran = nodes[cell];
	auto &W = gran->W;
	auto &N = locals[sched_getcpu()]->N;
	auto &L = locals[sched_getcpu()]->L;	
	const size_t numItem = gran->size();
	double sum[MIXT] = {};
	for(size_t n = 0; n < numItem; n++) {
		double norm = 0;
		double Pr[MIXT];
		Item *item = gran->samples[n];
		for(int k=0; k<MIXT; k++) {
			norm += Pr[k] = gran->pdf(n, k);
		}
		for(int k=0; k<MIXT; k++) {
			sum[k] += Pr[k] /= norm;
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			L[k][d] += Pr[k] * item->X[d];
		}
		}
	}
	for(int k=0; k<MIXT; k++) {
		W[k] = sum[k] / numItem;
		N[k] = sum[k] + N[k];
	}
}

inline void doExpectationMaximizationOnce(void) {
	const size_t numNode = nodes.size();
	const size_t nCPU = sun::totalCores;
	burst(numNode, doExpectationMaximizationItem);
	double N[MIXT] = {};
	for(int c=0; c<nCPU; c++) {
	for(int k=0; k<MIXT; k++) {
		N[k] += locals[c]->N[k];
		locals[c]->N[k] = 0;
	}
	}
	double L[MIXT][RANK] = {};
	for(int c=0; c<nCPU; c++) {
	for(int k=0; k<MIXT; k++) {
	for(int d=0; d<RANK; d++) {
		L[k][d] += locals[c]->L[k][d];
		locals[c]->L[k][d] = 0;
	}
	}
	}
	for(int k=0; k<MIXT; k++) {
	for(int d=0; d<RANK; d++) {
		M[k][d] = L[k][d] / N[k];
	}
	}
}
