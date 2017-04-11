/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <algorithm>
#include "dmm.main.hpp"

void calcMatrixMultiplicationTree(Chunk *node) {
	const size_t minI = get<0>(get<0>(*node));
	const size_t minJ = get<0>(get<1>(*node));
	const size_t minK = get<0>(get<2>(*node));
	const size_t maxI = get<1>(get<0>(*node));
	const size_t maxJ = get<1>(get<1>(*node));
	const size_t maxK = get<1>(get<2>(*node));
	const size_t lenI = maxI - minI;
	const size_t lenJ = maxJ - minJ;
	const size_t lenK = maxK - minK;
	const size_t most = max({lenI,lenJ,lenK});
	if(most <= GRAN) {
		calcMatrixMultiplicationLeaf(node);
	} else if(most == lenI) {
		const size_t mid = (minI + maxI) / 2;
		Chunk c1 = *node;
		Chunk c2 = *node;
		get<1>(get<0>(c1)) = mid;
		get<0>(get<0>(c2)) = mid;
		salvo(calcMatrixMultiplicationTree,&c1,&c2);
	} else if(most == lenJ) {
		const size_t mid = (minJ + maxJ) / 2;
		Chunk c1 = *node;
		Chunk c2 = *node;
		get<1>(get<1>(c1)) = mid;
		get<0>(get<1>(c2)) = mid;
		salvo(calcMatrixMultiplicationTree,&c1,&c2);
	} else {
		const size_t mid = (minK + maxK) / 2;
		Chunk c1 = *node;
		Chunk c2 = *node;
		get<1>(get<2>(c1)) = mid;
		get<0>(get<2>(c2)) = mid;
		calcMatrixMultiplicationTree(&c1);
		calcMatrixMultiplicationTree(&c2);
	}
}

inline void calcMatrixMultiplicationRoot(void) {
	Chunk root;
	get<0>(root) = make_tuple(0, RANK);
	get<1>(root) = make_tuple(0, RANK);
	get<2>(root) = make_tuple(0, RANK);
	calcMatrixMultiplicationTree(&root);
}
