//============================================================================
//                               P Y I B E X
// File        : ThickTest.h
// Author(s)   : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  15, 2015
//============================================================================

#ifndef __THICK_TESTS_H__
#define __THICK_TESTS_H__

#include "ibex_IntervalVector.h"
#include "ThickBoolean.h"
#include "ThickBox.h"
#include "ibex_Array.h"
#include "ibex_Function.h"
using ibex::Interval;
using ibex::IntervalVector;
using ibex::ThickBoolean;


// Generic function type ( to use lambda function)
using GenericFunc = std::function<IntervalVector (const IntervalVector&)>;


class ThickTest {
public:
	ThickTest(int nb_var);
	virtual ~ThickTest();
	virtual ThickBoolean test(const IntervalVector& box)=0;
	virtual void setParams(const IntervalVector& params) {std::cerr << "WARNING setParams not implemented !!!\n";}
	const int nb_var;
};

inline ThickTest::ThickTest(int n): nb_var(n) { }
inline ThickTest::~ThickTest() { }


class ThickDisk : public ThickTest{
public:
	ThickDisk(const Interval& mx, const Interval& my, const Interval& Rmin_, const Interval& Rmax_) :
		ThickTest(2), mx(mx), my(my), Rmin(sqr(Rmin_)), Rmax(sqr(Rmax_)), bbox(2) {
			bbox[0] = mx + Interval(-Rmax.ub(), Rmax.ub());
			bbox[1] = my + Interval(-Rmax.ub(), Rmax.ub());;
		};
	ThickDisk(double& mx, double& my, double& rmin) :
		ThickTest(2), mx(mx), my(my), Rmin(Interval(0, rmin*rmin)), Rmax(Interval(0, rmin*rmin)), bbox(2) {
			bbox[0] = mx + Interval(-rmin, rmin);
			bbox[1] = my + Interval(-rmin, rmin);
		};

	ThickBoolean test(const IntervalVector& box);
	void setParams(const IntervalVector& box){ mx = box[0]; my=box[1];}
private:
	Interval mx, my, Rmin, Rmax;
	IntervalVector bbox;
};

class ThickQInter : public ThickTest{
public:
	ThickQInter(const ibex::Array<ThickTest>& L, int q):
	ThickTest(L[0].nb_var), 	list(L), q(q) {}

	ThickBoolean test(const IntervalVector& box);

private:
	ibex::Array<ThickTest> list;
	int q;
};


class ThickInter : public ThickTest{
public:
	ThickInter(const ibex::Array<ThickTest>& L):
	ThickTest(L[0].nb_var), 	list(L) {}

	ThickBoolean test(const IntervalVector& box);

private:
	ibex::Array<ThickTest> list;
};


class ThickAnd : public ThickTest{
public:
	ThickAnd(const ibex::Array<ThickTest>& L):
	ThickTest(L[0].nb_var), 	list(L) {}

	ThickBoolean test(const IntervalVector& box);

private:
	ibex::Array<ThickTest> list;
};

class ThickOr : public ThickTest{
public:
	ThickOr(const ibex::Array<ThickTest>& L):
	ThickTest(L[0].nb_var), 	list(L) {}

	ThickBoolean test(const IntervalVector& box);
	void setParams(const IntervalVector& box){
		for (int i = 0; i < list.size(); i++){
			list[i].setParams(box);
		}
	}
private:
	ibex::Array<ThickTest> list;
};


class ThinInverse : public ThickTest{
public:
	ThinInverse(ibex::Function& f, ThickTest& t):
	ThickTest(t.nb_var), 	f(f), t(t) {}

	ThickBoolean test(const IntervalVector& box){
	  return t.test(this->f.eval_vector(box));
	}
private:
	ibex::Function &f;
	ThickTest &t;

};



class ThinfIn : public ThickTest{
public:
	ThinfIn(ibex::Function& f, const IntervalVector& y):
	f(f), y(y), ThickTest(f.nb_var()) {}

	ThickBoolean test(const IntervalVector& box){
		IntervalVector X = f.eval_vector(box);
		if (X.is_subset(y)){
			return IN;
		} else if (X.is_disjoint(y)){
			return OUT;
		}
	  return UNK;
	}
private:
	ibex::Function &f;
	IntervalVector y;
};

class ThickfIn : public ThickTest{
public:
	ThickfIn(ibex::Function& f_lb, ibex::Function& f_ub, const IntervalVector& y):
	y(y), ThickTest(f_lb.nb_var()) {
		// GenericFunc a = [&f_lb](const IntervalVector& X)->IntervalVector { return f_lb.eval_vector(X);} ;
		this->f_lb = [&f_lb](const IntervalVector& X)->IntervalVector { return f_lb.eval_vector(X); };
		this->f_ub = [&f_ub](const IntervalVector& X)->IntervalVector { return f_ub.eval_vector(X); };
	}

	ThickfIn(GenericFunc f_lb, GenericFunc f_ub, const IntervalVector& y):
	f_lb(f_lb), f_ub(f_ub), y(y), ThickTest(y.size()) {}

	ThickBoolean test(const IntervalVector& box);


private:
	GenericFunc f_lb;
	GenericFunc f_ub;
	IntervalVector y;
};

class ThickTranslateInBox : public ThickTest{
public:
	ThickTranslateInBox(const IntervalVector& dX, const IntervalVector& y):
	y(y), dX(dX), ThickTest(dX.size()) {}
	ThickBoolean test(const IntervalVector& box){
		return ThickBox(box + dX.lb(), box + dX.ub()).is_inside(y);
	}
	void setParams(const IntervalVector& params){ dX = params; }
private:
	IntervalVector y, dX;
};

class ThickSector: public ThickTest {
public:
	ThickSector(const Interval&x,const Interval&y,const Interval& theta,const Interval& d);
	ThickSector(const Interval&x,const Interval&y,const Interval& theta,const Interval& d1, const Interval& d2);

	ThickBoolean test(const IntervalVector& box);
	ThickBoolean testPlan(const IntervalVector& box, const Interval& th);
	void setParams(const IntervalVector& box){ x=box[0]; y=box[1]; tDisk.setParams(box);}

private:

	Interval x, y, theta, d;
	ThickDisk tDisk;
};


#endif //__THICK_TESTS_H__
