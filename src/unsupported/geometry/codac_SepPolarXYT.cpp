//============================================================================
//                               P Y I B E X
// File        : Polar separator with projection on the XYTheta-plan
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Jun 29, 2017
//============================================================================

#include "codac_SepPolarXYT.h"


#include <cmath>

namespace pyibex {



SepPolarXYT::SepPolarXYT(Interval rho, Interval theta, double mx, double my) : rho(rho), theta(theta), mx(mx), my(my), Sep(3) {
	rho_m = Interval(0, rho.lb());
	rho_p = Interval(rho.ub(), POS_INFINITY);
	double limit = theta.mid() - M_PI;
	theta_m = Interval(limit, theta.lb());
	theta_p = Interval(theta.ub(), limit + 2*M_PI);
	cmpl = Interval(0, 2*M_PI);
}

void SepPolarXYT::contract(Interval &x, Interval &y, Interval &theta, Interval &rho, Interval &phi){

	Interval dx = this->mx - x;
	Interval dy = this->my - y;

	dx &= rho*cos(theta + phi);
	dy &= rho*sin(theta + phi);

	Interval a1 = sqr(dx);
	Interval a2 = sqr(dy);
	Interval a3 = a1 + a2;
	Interval a4 = sqr(rho);
	a4 &= a3;
	a3 &= a4;
	bwd_sqr(a4, rho);
	bwd_add(a3, a2, a1);
	bwd_sqr(a1, dx);
	bwd_sqr(a2, dy);

	a1 = atan2(dy, dx);
	a2 = a1 - theta;
	bwd_imod(phi, a2, 2*M_PI);

	a1 &= a2 + theta;
	theta &= a1 - a2;
	// bwd_sub(a2, a1, theta);
	bwd_atan2(a1, dy, dx);

 	x &= mx - dx;
 	y &= my - dy;

	if (x.is_empty() || y.is_empty() || theta.is_empty()){
		x.set_empty(); y.set_empty(); theta.set_empty();
	}
}

void SepPolarXYT::contractOut(IntervalVector &x_out){
	Interval th = this->theta;
	Interval r = this->rho;
	this->contract(x_out[0], x_out[1], x_out[2], r, th);
	if(x_out[0].is_empty() || x_out[1].is_empty() || x_out[2].is_empty())
		x_out.set_empty();
}


void SepPolarXYT::contractIn(IntervalVector &x_in){
	Interval x1(x_in[0]); Interval y1(x_in[1]); Interval theta1(x_in[2]);
	Interval x2(x_in[0]); Interval y2(x_in[1]); Interval theta2(x_in[2]);
	Interval x3(x_in[0]); Interval y3(x_in[1]); Interval theta3(x_in[2]);
	Interval x4(x_in[0]); Interval y4(x_in[1]); Interval theta4(x_in[2]);

	Interval ALLREALS1 = Interval::POS_REALS;
	Interval ALLREALS2 = Interval::POS_REALS;
	Interval cmpl1(cmpl);
	Interval cmpl2(cmpl);
	Interval theta_m_tmp(theta_m);
	Interval theta_p_tmp(theta_p);
	Interval rho_m_tmp(rho_m);
	Interval rho_p_tmp(rho_p);

	this->contract(x1, y1, theta1, ALLREALS1, theta_m_tmp);
	this->contract(x2, y2, theta2, ALLREALS2, theta_p_tmp);
	this->contract(x3, y3, theta3, rho_m_tmp, cmpl1);
	this->contract(x4, y4, theta4, rho_p_tmp, cmpl2);
	x_in[0] &= (x1 | x2 | x3 | x4);
	x_in[1] &= (y1 | y2 | y3 | y4);
	x_in[2] = (theta1 | theta2 | theta3 | theta4);
	if(x_in[0].is_empty() || x_in[1].is_empty() || x_in[2].is_empty())
		x_in.set_empty();
}

void SepPolarXYT::separate(IntervalVector& x_in, IntervalVector& x_out){

	assert(x_out.size() == 3);
	assert(x_in.size() == 3);
	x_out &= x_in;
	x_in &= x_out;

	contractOut(x_out);
	contractIn(x_in);
}

};
