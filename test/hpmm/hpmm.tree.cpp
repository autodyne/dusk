/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "hpmm.main.hpp"

struct Chunk {
	Node* node;
	size_t from = 0, last = 0;
	double N[MIXT] = {};
	double L[MIXT][RANK] = {};
};

void doExpectationMaximizationItem(Chunk* mine) {
	const size_t from = mine->from;
	const size_t last = mine->last;
	const size_t half = (from + last) / 2;
	Node* node = mine->node;
	if(last - from > GRAN) {
		Chunk c1, c2;
		c1.node = node;
		c2.node = node;
		c1.from = from;
		c1.last = half;
		c2.from = half;
		c2.last = last;
		salvo(doExpectationMaximizationItem,&c1,&c2);
		for(int k=0; k<MIXT; k++) {
			mine->N[k] = c1.N[k] + c2.N[k];
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			mine->L[k][d] = c1.L[k][d] + c2.L[k][d];
		}
		}
	} else for(size_t n = from; n < last; n++) {
		double norm = 0;
		double Pr[MIXT];
		const Item* item = node->samples[n];
		for(int k=0; k<MIXT; k++) {
			norm += Pr[k] = node->pdf(n, k);
		}
		for(int k=0; k<MIXT; k++) {
			mine->N[k] += Pr[k] /= norm;
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			mine->L[k][d] += Pr[k] * item->X[d];
		}
		}
	}
}

void doExpectationMaximizationNode(Chunk* mine) {
	const size_t from = mine->from;
	const size_t last = mine->last;
	const size_t half = (from + last) / 2;
	if(last - from > 1) {
		Chunk c1, c2;
		c1.from = from;
		c1.last = half;
		c2.from = half;
		c2.last = last;
		salvo(doExpectationMaximizationNode,&c1,&c2);
		for(int k=0; k<MIXT; k++) {
			mine->N[k] = c1.N[k] + c2.N[k];
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			mine->L[k][d] = c1.L[k][d] + c2.L[k][d];
		}
		}
	} else {
		Node* node = nodes[from];
		const size_t size = node->size();
		Chunk child;
		child.node = node;
		child.last = size;
		doExpectationMaximizationItem(&child);
		for(int k=0; k<MIXT; k++) {
			mine->N[k] = child.N[k];
			node->W[k] = child.N[k] / size;
		}
		for(int k=0; k<MIXT; k++) {
		for(int d=0; d<RANK; d++) {
			mine->L[k][d] = child.L[k][d];
		}
		}
	}
}

inline void doExpectationMaximizationOnce(void) {
	Chunk root;
	root.last = nodes.size();
	doExpectationMaximizationNode(&root);
	for(int k=0; k<MIXT; k++) {
	for(int d=0; d<RANK; d++) {
		M[k][d] = root.L[k][d] / root.N[k];
	}
	}
}
