/*********************************************************************
 * dusk - lightweight workstealing scheduler
 * 
 * Copyright (C) 2015 http://pafelog.net.
 * All rights reserved.
 * 
 * See LICENSE for details.
*********************************************************************/

#include <chrono>
#include <cstdio>
#include <random>
#include <tuple>
#include <dusk.hpp>
#include <immintrin.h>

using namespace sun;
using namespace std;
using namespace std::chrono;
using ns = nanoseconds;

const size_t RANK = dget("RANK", 64);
const size_t GRAN = dget("GRAN", 32);
const size_t TIME = dget("TIME", 10);

double *A;
double *B;
double *C;

const size_t PADDED = RANK + 0x20;
const size_t SIZE = PADDED * RANK;
#define A(i,j) A[PADDED*(i)+(j)]
#define B(i,j) B[PADDED*(i)+(j)]
#define C(i,j) C[PADDED*(i)+(j)]

using Range = tuple<size_t,size_t>;
using Chunk = tuple<Range,Range,Range>;

void calcMatrixMultiplicationRoot(void);

inline void calcMatrixMultiplicationLeaf(Chunk *ch) {
	const size_t minI = get<0>(get<0>(*ch));
	const size_t minJ = get<0>(get<1>(*ch));
	const size_t minK = get<0>(get<2>(*ch));
	const size_t maxI = get<1>(get<0>(*ch));
	const size_t maxJ = get<1>(get<1>(*ch));
	const size_t maxK = get<1>(get<2>(*ch));
	for(unsigned i=minI; i<maxI; i++) {
	for(unsigned j=minJ; j<maxJ; j++) {
		__m256d u = {0};
		for(unsigned k=minK; k<maxK; k+=4) {
			auto w = _mm256_load_pd(&A(i,k));
			auto x = _mm256_load_pd(&B(j,k));
			w = _mm256_mul_pd(w, x);
			u = _mm256_add_pd(u, w);
		}
		double t[4] = {0};
		_mm256_store_pd(t, u); 
	 	for(int k=0; k<4; k++) C(i, j) += t[k];
	}
	}
}

int main(void) {
	anchoring(0);
	size_t BYTES = sizeof(double) * SIZE;
	A = (double*) _mm_malloc(BYTES, 32);
	B = (double*) _mm_malloc(BYTES, 32);
	C = (double*) _mm_malloc(BYTES, 32);
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<> rnd(0, 1);
	for (size_t i = 0; i < RANK; i++) {
	for (size_t j = 0; j < RANK; j++) {
		A(i, j) = rnd(mt);
		B(i, j) = rnd(mt);
	}
	}
	const auto s = system_clock::now();
	for(int trial=0; trial<TIME; trial++) {
		launch(calcMatrixMultiplicationRoot);
	}
	const auto e = system_clock::now();
	auto nsec = duration_cast<ns>(e-s);
	auto secs = nsec.count() / 1e9 / TIME;
	auto flps = 2e-9 * pow(RANK,3) / secs;
	printf("%10f secs ", secs);
	printf("%10f GFLOPS\n", flps); 
}
