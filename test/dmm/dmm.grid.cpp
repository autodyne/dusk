/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include "dmm.main.hpp"

void calcMatrixMultiplicationGrid(size_t cell) {
	const size_t idxI = cell / (RANK / GRAN);
	const size_t idxJ = cell % (RANK / GRAN);
	const size_t minI = idxI * GRAN;
	const size_t minJ = idxJ * GRAN;
	const size_t maxI = minI + GRAN;
	const size_t maxJ = minJ + GRAN;
	Chunk ch;
	get<0>(ch) = make_tuple(minI, maxI);
	get<1>(ch) = make_tuple(minJ, maxJ);
	get<2>(ch) = make_tuple(0x00, RANK);
	calcMatrixMultiplicationLeaf(&ch);
}

inline void calcMatrixMultiplicationRoot(void) {
	const size_t N = pow(RANK / GRAN, 2);
	burst(N, calcMatrixMultiplicationGrid);
}
