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
#include <random>
#include <dusk.hpp>

using namespace sun;
using namespace std;
using namespace std::chrono;
using ns = nanoseconds;

const size_t ITEM = dget("ITEM", 64);
const size_t GRAN = dget("GRAN", 32);
const size_t TIME = dget("TIME", 10);

constexpr size_t RANK = 8;
constexpr size_t MIXT = 8;

double *X;
double N[MIXT];
double W[MIXT];
double M[MIXT][RANK];
double S[MIXT][RANK];

#define SIZE (RANK * ITEM)
#define X(n,d) X[RANK*(n)+(d)]

random_device rd;
mt19937 mt(rd());
uniform_real_distribution<> rnd(0, 1);

void repeatExpectationMaximization(int numStep);

inline double gauss(size_t trainId, int classId) {
	constexpr auto den = 1 / pow(2*M_PI, RANK*0.5);
	double pow = 0.0;
	double det = 1.0;
	for(unsigned d=0; d<RANK; d += 2) {
		const double m1 = M[classId][d+0];
		const double m2 = M[classId][d+1];
		const double s1 = S[classId][d+0];
		const double s2 = S[classId][d+1];
		const double x1 = X(trainId, d+0);
		const double x2 = X(trainId, d+1);
		pow += (x1 - m1) * (x1 - m1) / s1;
		pow += (x2 - m2) * (x2 - m2) / s2;
		det *= s1 * s2;
	}
	return den / sqrt(det) * exp(-0.5 * pow);
}

static inline double grand(int classId, int dim) {
	double r1 = sqrt(-2 * log(rnd(mt)));
	double r2 = sin(2 * M_PI * rnd(mt));
	double m = M[classId][dim];
	double s = S[classId][dim];
	return m + s * r1 * r2;
}

static void makeGaussianMixtureTrainingRecords() {
	size_t seg[MIXT + 1] = {};
	size_t i = MIXT;
	while(i--) seg[i+1] += W[i+0] * ITEM;
	while(++i < MIXT) seg[i+1] += seg[i];
	seg[MIXT] = ITEM;
	for(int k=0; k<MIXT; k++) {
		const size_t nmin = seg[k+0];
		const size_t nmax = seg[k+1];
		size_t n = nmin;
		while(n++ < nmax) for(int d=0; d<RANK; d++) {
			X(n-1, d) = grand(k, d);
		}
	}
}

static void makeGaussianMixtureModelParameters() {
	double sumW = 0.0;
	for(int k=0;k<MIXT;k++) sumW += W[k]= rnd(mt);
	for(int k=0;k<MIXT;k++) W[k] /= sumW;
	for(int k=0;k<MIXT;k++) {
	for(int d=0;d<RANK;d++) {
		M[k][d] = 10 * rnd(mt);
		S[k][d] = 10 * rnd(mt);
	}
	}
}

static void showGaussianMixtureModelParameters() {
	for(unsigned k = 0; k < MIXT; k++) {
		printf("# %fN((", W[k]);
		for(unsigned d = 1; d <= RANK; d++) {
			if(d  < RANK) printf("%+e,", M[k][d-1]);
			if(d == RANK) printf("%+e)", M[k][d-1]);
		}
		printf(",(");
		for(unsigned d = 1; d <= RANK; d++) {
			if(d  < RANK) printf("%+e,", S[k][d-1]);
			if(d == RANK) printf("%+e)", S[k][d-1]);
		}
		printf(")\n");
	}
}

void doExpectationMaximizationOnce(void);

int main(void) {
	anchoring(0);
	X = (double*) malloc(sizeof(double) * SIZE);
	printf("#generating %zu mixts\n", MIXT);
	makeGaussianMixtureModelParameters();
	showGaussianMixtureModelParameters();
	printf("#generating %zu items\n", ITEM);
	makeGaussianMixtureTrainingRecords();
	makeGaussianMixtureModelParameters();
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
	showGaussianMixtureModelParameters();
	printf("%10f secs ",  secs);
	printf("%10f MRPS\n", mrps);
}
