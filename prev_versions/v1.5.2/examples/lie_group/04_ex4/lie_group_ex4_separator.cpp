
//
// Created by julien-damers on 08/01/2020.
//

#include "lie_group_ex4_separator.h"

using namespace codac;
using namespace ibex;
using namespace std;

lie_group_ex4_separator::lie_group_ex4_separator(TubeVector* a, IntervalVector* constraint) : Sep(5)
{

    reference = a;
    constraintVector = constraint;


    fy = new ibex::Function("x1","x2","x3","z1","z2","z3","w1","w2","w3",
                         "(x1 + cos(x3-z3)*(w1-z1) - sin(x3-z3)*(w2-z2); \
                              x2 + sin(x3-z3)*(w1-z1) + cos(x3-z3)*(w2-z2); \
                              x3 + w3 - z3)");


    sepPhi = new ibex::SepFwdBwd(*fy,*constraintVector);

}



void lie_group_ex4_separator::separate(ibex::IntervalVector &Xin, ibex::IntervalVector &Xout)
{

    Interval x4 = Xin[3];

    IntervalVector Xinit(Xin);
    IntervalVector z(4);
    z.init(Interval(NEG_INFINITY,POS_INFINITY));
    IntervalVector w(4);
    w.init(Interval(NEG_INFINITY,POS_INFINITY));


    // Forward
    if (x4.is_empty())
    {
        Xout[0].set_empty();
        Xout[1].set_empty();
        Xout[2].set_empty();
        Xout[3].set_empty();
        return;
    }
    z = z & (*reference)(-x4);
    if (z[0].is_empty()||z[1].is_empty()||z[2].is_empty()||z[3].is_empty())
    {
        Xout[0].set_empty();
        Xout[1].set_empty();
        Xout[2].set_empty();
        Xout[3].set_empty();
        return;
    }

    w = w & (*reference)(0);
    if (w[0].is_empty()||w[1].is_empty()||w[2].is_empty()||w[3].is_empty())
    {
        Xout[0].set_empty();
        Xout[1].set_empty();
        Xout[2].set_empty();
        Xout[3].set_empty();
        return;
    }


    IntervalVector fullBoxIn(9);
    fullBoxIn[0] = Xin[0];
    fullBoxIn[1] = Xin[1];
    fullBoxIn[2] = Xin[2];
    fullBoxIn[3] = z[0];
    fullBoxIn[4] = z[1];
    fullBoxIn[5] = z[2];
    fullBoxIn[6] = w[0];
    fullBoxIn[7] = w[1];
    fullBoxIn[8] = w[2];


    IntervalVector fullBoxOut(fullBoxIn);
    IntervalVector zout(z);
    IntervalVector wout(w);
    Interval x4out(x4);


    sepPhi->separate(fullBoxIn,fullBoxOut);


    Xin[0] = fullBoxIn[0];
    Xin[1] = fullBoxIn[1];
    Xin[2] = fullBoxIn[2];
    z[0] = fullBoxIn[3];
    z[1] = fullBoxIn[4];
    z[2] = fullBoxIn[5];
    w[0] = fullBoxIn[6];
    w[1] = fullBoxIn[7];
    w[2] = fullBoxIn[8];


    Xout[0] = fullBoxOut[0];
    Xout[1] = fullBoxOut[1];
    Xout[2] = fullBoxOut[2];
    zout[0] = fullBoxOut[3];
    zout[1] = fullBoxOut[4];
    zout[2] = fullBoxOut[5];
    wout[0] = fullBoxOut[6];
    wout[1] = fullBoxOut[7];
    wout[2] = fullBoxOut[8];


    x4 = x4 & -(reference->invert(z,-x4));
    x4out = x4out & -(reference->invert(zout,-x4out));


    assert((x4|x4out) == Xinit[3]);


    if(x4.is_empty())
    {
        Xin[0].set_empty();
        Xin[1].set_empty();
        Xin[2].set_empty();
        Xin[3].set_empty();
        Xout[3] = x4out;
        return;
    }

    if(x4out.is_empty())
    {
        Xout[0].set_empty();
        Xout[1].set_empty();
        Xout[2].set_empty();
        Xout[3].set_empty();
        Xin[3] = x4;
        return;
    }

    Xin[3] = x4;
    Xout[3] = x4out;

}



lie_group_ex4_separator::~lie_group_ex4_separator()
{
    delete sepPhi;
    delete fy;
}
