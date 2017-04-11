/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <vector>
#include <sched.h>
#include "gmm.main.hpp"

struct Local {
	double N[MIXT] = {};
	double M[MIXT][RANK] = {};
	double S[MIXT][RANK] = {};
};

Local** locals = [&]() {
	size_t P = sun::totalCores;
	Local** bufs = new Local*[P];
	while(P--) bufs[P]=new Local;
	return bufs;
}();

void doExpectationMaximizationGrid(size_t cell) {
	const size_t from = GRAN * (cell + 0);
	const size_t last = GRAN * (cell + 1);
	Local* local = locals[sched_getcpu()];
	for(size_t n = from; n < last; n++) {
		double norm = 0;
		double Pr[MIXT];
		for(int k=0; k<MIXT; k++) {
			norm += Pr[k] = W[k] * gauss(n, k);
		}
		for(int k=0; k<MIXT; k++) {
			local->N[k] += Pr[k] /= norm;
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			const double x = X(n, d);
			local->M[k][d] += Pr[k] * x;
			local->S[k][d] += Pr[k] * x * x;
		}
		}
	}
}

inline void doExpectationMaximizationOnce(void) {
	const size_t numCell = ITEM / GRAN;
	burst(numCell, doExpectationMaximizationGrid);	
	for(int k=0; k<MIXT; k++) N[k] = 0;
	for(int p=0; p<totalCores; p++) {
	for(int k=0; k<MIXT; k++) {
		Local* local = locals[p];
		N[k] += local->N[k];
		local->N[k] = 0;
		for(int d=0; d<RANK; d++) {
			M[k][d] += local->M[k][d];
			S[k][d] += local->S[k][d];
			local->M[k][d] = 0;
			local->S[k][d] = 0;
		}
	}
	}
	for(int k=0; k<MIXT; k++) {
		W[k] = N[k] / ITEM;
	}
	for(int k=0; k<MIXT; k++) {
	for(int d=0; d<RANK; d++) {
		M[k][d] /= N[k];
		S[k][d] /= N[k];
		S[k][d] -= M[k][d] * M[k][d];
	}
	}
}
