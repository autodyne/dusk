/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "gmm.main.hpp"

struct Chunk {
	size_t from = 0;
	size_t last = ITEM;
	double N[MIXT] = {};
	double M[MIXT][RANK] = {};
	double S[MIXT][RANK] = {};
};

void doExpectationMaximizationLeaf(Chunk *mine) {
	const size_t from = mine->from;
	const size_t last = mine->last;
	for(size_t n=from; n<last; n++) {
		double norm = 0;
		double Pr[MIXT];
		for(int k=0; k<MIXT; k++) {
			norm += Pr[k] = W[k] * gauss(n, k);
		}
		for(int k=0; k<MIXT; k++) {
			mine->N[k] += Pr[k] /= norm;
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			const double x = X(n, d);
			mine->M[k][d] += Pr[k] * x;
			mine->S[k][d] += Pr[k] * x * x;
		}
		}
	}
}

void doExpectationMaximizationTree(Chunk *mine) {
	const size_t from = mine->from;
	const size_t last = mine->last;
	const size_t half = (from + last) / 2;
	if(last - from > GRAN) {
		Chunk c1, c2;
		c1.from = from; 
		c1.last = half;
		c2.from = half;
		c2.last = last;
		salvo(doExpectationMaximizationTree,&c1,&c2);
		for(int k=0; k<MIXT; k++) {
			mine->N[k] = c1.N[k] + c2.N[k];
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			mine->M[k][d] = c1.M[k][d] + c2.M[k][d];
			mine->S[k][d] = c1.S[k][d] + c2.S[k][d];
		}
		}
	} else doExpectationMaximizationLeaf(mine);
}

inline void doExpectationMaximizationOnce(void) {
	Chunk ch;
	doExpectationMaximizationTree(&ch);
	for(int k=0; k<MIXT; k++) N[k]=ch.N[k];
	for(int k=0; k<MIXT; k++) W[k]=N[k] / ITEM;
	for(int k=0; k<MIXT; k++) {
	for(int d=0; d<RANK; d++) {
		M[k][d] = ch.M[k][d] / N[k];
		S[k][d] = ch.S[k][d] / N[k];
		S[k][d]-= M[k][d] * M[k][d];
	}
	}
}
