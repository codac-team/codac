/** 
 *  tubex-lib - Examples
 *  Robot evolution equations: causal chain
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Guaranteed Computation of Robot Trajectories"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>
#include <tubex-rob.h>
#include "tubex-ode.h"





using namespace std;
using namespace ibex;
using namespace tubex;
using namespace vibes;


int main()
{
    // ----- Generate reference tube thanks to ODE integration -----

    Interval domain(0.,17.);
    tubex::Function fa("x1","x2","x3","x4","(cos(x3);sin(x3);sin(0.4*x4);1)"); //function to be integrated
    IntervalVector a0(4,Interval(0.,0.)); // inintial condition for reference tube
    double timestep = 0.1;
    TubeVector a = TubeVectorODE(domain,fa,a0,timestep,CAPD_MODE);
    a.resize(3); // removing last component of the tube identical to the domain

    // ----- Generate derivative of [a](.) -----
    tubex::Function fv("x1","x2","x3","(cos(x3);sin(x3);sin(0.4*t))"); // function for the derivative tube
    TubeVector va= fv.eval_vector(a);

    // ----- Initialise estimate -----
    IntervalVector map(a.size());
    map.init(Interval(-100.,100.)); // space we consider
    TubeVector x(a.domain(),map);
    x.sample(a); // make sure that our reference and our estimate have the same slices
    IntervalVector x0(3,Interval(-0.1,0.1)); // Initial condition for our estimate

    // ----- Initialise derivative of the estimate -----
    TubeVector vx = fv.eval_vector(x);
    assert(a.same_slicing(a,vx));

    // ----- Initial conditions -----



    // ----- Contractors initialisation -----
    ibex::Function phi("x1","x2","x3","z1","z2","z3",
                       "(x1 + cos(x3-z3)*(-z1) - sin(x3-z3)*(-z2); \
                          x2 + sin(x3-z3)*(-z1) + cos(x3-z3)*(-z2); \
                          x3  - z3)");
    ibex::CtcFwdBwd ctc_phi(phi,x0); // lie symmetries transformation contractor
    tubex::CtcDeriv ctc_deriv;
    tubex::CtcEval ctc_eval;


    // ----- Graphics initialisation -----
    beginDrawing();
    VIBesFigMap fig("Map");
    tubex::rgb red = make_rgb(255,0,0,50);
    tubex::rgb green = make_rgb(0,255,0,50);
    ColorMap estimateColorMap(InterpolMode::RGB);
    estimateColorMap.add_color_point(red,0);
    estimateColorMap.add_color_point(green,1);
    fig.set_properties(100,100,800,800);
    fig.set_tube_max_disp_slices(20000);
    fig.smooth_tube_drawing(true);
    fig.add_tube(&x, "estimate_hand", 0,1);
    fig.set_tube_color(&x,estimateColorMap);
    fig.add_tube(&a, "reference_hand",0,1);

    // ----- Create contractor network and contract -----

    ContractorNetwork cn;
    cn.set_fixedpoint_ratio(0.);
    cn.add(ctc_deriv,{a,va});
    cn.add(ctc_deriv,{x,vx});

    for(double i=0.; i<domain.ub();i = i+0.1)
    {
        Interval &t = cn.create_var(Interval(i,i));
        IntervalVector &at = cn.create_var(IntervalVector(a(t)));
        IntervalVector &xt = cn.create_var(IntervalVector(x(t)));
        cn.add(ctc_phi,{xt[0],xt[1],xt[2],at[0],at[1],at[2]});
        cn.add(ctc_eval,{t,xt,x,vx});
    }

    cn.contract();
    fig.show();

    return EXIT_SUCCESS;

}
