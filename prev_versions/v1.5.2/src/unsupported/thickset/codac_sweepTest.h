//============================================================================
//                               P Y I B E X
// File        : sweepTest.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Oct  1, 2016
//============================================================================

#ifndef EXPLOREDAREA_H
#define EXPLOREDAREA_H

#include <ibex_IntervalVector.h>
#include <ibex_LargestFirst.h>
#include <list>
#include <vector>
#include "codac_exploredAreaTools.h"
#include "codac_ThickTest.h"

// using namespace ibex;
using std::vector;
using std::list;



namespace codac{



//enum {OUT = -1, UNKNOWN = 0, IN = 1, MAY = 2 };
class SweepTest : public ThickTest
{
public:
    // SweepTest(): ThickTest(2){}
    SweepTest(const Tube &x, const Tube &y, const Tube &ux, const Tube &uy, double L);
    SweepTest(const Tube &x, const Tube &y, const Tube &ux, const Tube &uy, const Tube &L);
    ThickBoolean test(const IntervalVector &X);
    ThickBoolean fast_test(IntervalVector &X, vector<IntBound> &T, bool debug = false);
    const Tube x;
    const Tube y;
    const Tube ux;
    const Tube uy;
    const Tube L;

    Tube genV1(IntervalVector &box, int idx_deb, int idx_end);
    Tube genV2(IntervalVector &box, int idx_deb, int idx_end);
    std::tuple<Tube, Tube> genFunction(IntervalVector& box, int idx_deb, int idx_end, int dim);
    std::tuple<double,double> getBounds(int i, int dim);
    void genV1(IntervalVector &box, Tube &v1_plus, Tube &v1_moins, int idx_deb, int idx_end);
    void genV2(IntervalVector &box, Tube &v2_plus, Tube &v2_moins, int idx_deb, int idx_end);
private:

    list<Record> backtrack;
    LargestFirst lf;
    ThickBoolean test_exist_v(IntervalVector &X, vector<IntBound> &T1, bool &is_in);
    ThickBoolean test_exist_v2(IntervalVector &X, vector<IntBound> &T1, bool &is_in);
    void genV1_1(IntervalVector& box,Tube& v1_1, Tube& v1_2, int idx_deb, int idx_end);
    std::tuple<bool, bool> check_function(IntervalVector& X, int idx_deb, int idx_end);
    ThickBoolean check_function2(IntervalVector& X, int idx_deb, int idx_end, int idx_bound);

};


/******************************************************************
// Tools and function used by the inclusion test
//  inline implementation
*******************************************************************/

vector<IntBound> extract_zeros(Tube& f, vector<IntBound> &T2, int idx_deb =  -1, int idx_end = -1);

//int fast_test(IntervalVector& X,Tube& ax, Tube& ay,Tube &ux, Tube& uy,vector<Interval>& T,bool debug = false);
enum {IDLE = 0, SIGN_PLUS, SIGN_MOINS, ZERO_PLUS, ZERO_MOINS};
enum {POSITIF = 0, ZERO, NEGATIF};

inline int get_sign(const Interval& x){
    if(x.ub() <= 0) return NEGATIF;
    else if( x.lb() >= 0) return POSITIF;
    else return ZERO;
}

// Exist [t1,t2] in T, v1(t1,t2) = 0 and v2(t1,t2) <= 0
inline bool is_inside(Tube& v2, vector<IntBound>& T){
    for(int i = 0; i < T.size(); i++){
        bool is_inside = true;
        for(int j = T[i][0]; j <= T[i][1];j++){
            if  ( !(v2[j].lb() >= 0) ){
                is_inside  = false;
                break;
            }
        }
        if(is_inside == true)
            return true;
    }
    return false;
}


// For all [t1,t2] in T, v1 != 0 or v2 <= 0
inline bool is_outside(Tube& v2, vector<IntBound>& T){
    for(int i = 0; i < T.size(); i++){
        for(int j = T[i][0]; j <= T[i][1]; j++){
            if  ( (v2[j].ub() >= 0) ){
                return false;
            }
        }
        // std::cerr << "\n";
    }
    return true;
}


inline bool keep_constant_sign(Tube& f, bool use_upper = true){
    bool is_neg = (use_upper) ? ( f[0].ub() < 0 ) : (f[0].lb() < 0);
    for(int j = 0; j < f.size();j++){
        bool tmp = (use_upper) ? ( f[j].ub() < 0 ) : (f[j].lb() < 0);
        if (tmp != is_neg || f[j].contains(0))
            return false;
    }
    return true;
}

inline bool is_always_negatif(Tube &f){
    for (int i = 0; i< f.size(); i++){
        if (f[i].ub() >= 0){
            return false;
        }
    }
    return true;
}

inline bool exist_t(Tube &v1_plus, Tube & v1_moins, Tube& v2){
    for(int j = 0; j < v1_plus.size();j++){
        if(v1_moins[j].ub()<=0 && v1_plus[j].lb()>= 0 && v2[j].lb() >= 0)
            return true;
    }
    return false;
}
}; //namespace ibex
#endif // EXPLOREDAREA_H
