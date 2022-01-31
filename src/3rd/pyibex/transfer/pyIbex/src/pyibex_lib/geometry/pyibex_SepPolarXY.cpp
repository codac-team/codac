//============================================================================
//                               P Y I B E X
// File        : Polar separator with projection on the XY-plan
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Jun 29, 2017
//============================================================================

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#endif
#include "pyibex_SepPolarXY.h"

namespace pyibex {


SepPolarXY::SepPolarXY(Interval rho, Interval theta) : rho(rho), theta(theta), Sep(2) {
	rho_m = Interval(0, rho.lb());
	rho_p = Interval(rho.ub(), POS_INFINITY);
	double limit = theta.mid() - M_PI;
	theta_m = Interval(limit, theta.lb());
	theta_p = Interval(theta.ub(), limit + 2*M_PI);
	cmpl = Interval(0, 2*M_PI);
}


void SepPolarXY::contractOut(IntervalVector &x_out){
	Interval th = this->theta;
	Interval r = this->rho;
	this->ctc.contract(x_out[0], x_out[1], r, th);
	if(x_out[0].is_empty() || x_out[1].is_empty())
		x_out.set_empty();
}


void SepPolarXY::contractIn(IntervalVector &x_in){
	Interval x1(x_in[0]); Interval y1(x_in[1]);
	Interval x2(x_in[0]); Interval y2(x_in[1]);
	Interval x3(x_in[0]); Interval y3(x_in[1]);
	Interval x4(x_in[0]); Interval y4(x_in[1]);

	Interval ALLREALS1 = Interval::POS_REALS;
	Interval ALLREALS2 = Interval::POS_REALS;
	Interval cmpl1(theta);
	Interval cmpl2(theta);
	Interval theta_m_tmp(theta_m);
	Interval theta_p_tmp(theta_p);
	Interval rho_m_tmp(rho_m);
	Interval rho_p_tmp(rho_p);

	this->ctc.contract(x1, y1, ALLREALS1, theta_m_tmp);
	this->ctc.contract(x2, y2, ALLREALS2, theta_p_tmp);
	this->ctc.contract(x3, y3, rho_m_tmp, cmpl1);
	this->ctc.contract(x4, y4, rho_p_tmp, cmpl2);
	x_in[0] &= (x1 | x2 | x3 | x4);
	x_in[1] &= (y1 | y2 | y3 | y4);
	if(x_in[0].is_empty() || x_in[1].is_empty())
		x_in.set_empty();
}

void SepPolarXY::separate(IntervalVector& x_in, IntervalVector& x_out){



	assert(x_out.size() == 2);
	assert(x_in.size() == 2);
	x_out &= x_in;
	x_in &= x_out;

    if (x_in.is_empty()){
        std::cerr << "EMPRT\n";
        return;
    }
    IntervalVector xinOld(x_in);
    IntervalVector xoutOld(x_out);
	contractOut(x_out);
	contractIn(x_in);

    if (x_in.is_empty() && x_out.is_empty()){
        std::cerr << "Xin and Xout are both empty\n";
        std::cerr << "config : \n"; // << std::hexfloat;
        std::cerr << "Rho = [" << rho.lb() << " , " << rho.ub() << "]\n";
        std::cerr << "Theta = [" << theta.lb() << " , " << theta.ub() << "]\n";
        std::cerr << "X[0] = [" << xinOld[0].lb() << " , " << xinOld[0].ub() << "]\n";
        std::cerr << "X[1] = [" << xinOld[1].lb() << " , " << xinOld[1].ub() << "]\n";
        // std::cerr << std::defaultfloat;
        contractOut(xoutOld);
        contractIn(xinOld);
//        assert(false);
        // Bug in CtcPolar. and maybe with atan function or modulo
    }
}

// SepPolarXYT(const Interval& rho, const Interval& phi): rho(rho), phi(phi) {}
// 		// sep(*(new SepPolarXY(rho, phi))){
// }
//
// void separate(IntervalVector& x_in, IntervalVector& x_out){
// 		assert(x_out.size() == 3);
// 		assert(x_in.size() == 3);
// 		x_out &= x_in;
// 		x_in &= x_out;
// 		Interval theta_in = x_in[2] + phi;
// 		Interval theta_out = x_out[2] + phi;
// 		SepPolarXY sep(rho, theta);
// 		sep.separate(x_in)
//
//
//
// }

}; // end namespace pyibex
