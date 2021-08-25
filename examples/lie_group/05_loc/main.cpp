/** 
 *  Codac - Examples
 *  Lie symmetries examples
 * ----------------------------------------------------------------------------
 *
 *
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Julien Damers
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>
#include <codac-rob.h>
#include <codac-capd.h>

using namespace std;
using namespace codac;
using namespace vibes;

int main()
{
    // ----- Generate reference tube thanks to ODE integration -----

    Interval tdomain(0.,17.);
    TFunction f("x1","x2","x3","(cos(x3);sin(x3);sin(0.4*t))"); //function to be integrated
    IntervalVector a0(3,Interval(0.,0.)); // initial condition for reference tube
    double timestep = 0.1;
    TubeVector a = CAPD_integrateODE(tdomain,f,a0,timestep);
    // ----- Generate derivative of [a](.) -----
    // function for the derivative tube
    TubeVector va= f.eval_vector(a);

    // ----- Initialise estimate -----
    IntervalVector map(a.size());
    map.init(Interval(-100.,100.)); // space we consider
    TubeVector x(a.tdomain(),map);
    x.sample(a); // make sure that our reference and our estimate have the same slices
    IntervalVector x0(3,Interval(-0.1,0.1)); // Initial condition for our estimate

    // ----- Initialise derivative of the estimate -----
    TubeVector vx = f.eval_vector(x);
    assert(a.same_slicing(a,vx));


    // ----- Contractors initialisation -----
    Function phi("x1","x2","x3","z1","z2","z3",
                 "(x1 + cos(x3-z3)*(-z1) - sin(x3-z3)*(-z2); \
                    x2 + sin(x3-z3)*(-z1) + cos(x3-z3)*(-z2); \
                    x3  - z3)");
    CtcFunction ctc_phi(phi,x0);
    CtcDeriv ctc_deriv;
    CtcEval ctc_eval;


    // ----- Graphics initialisation -----
    beginDrawing();
    VIBesFigMap fig("Map");
    rgb red = make_rgb(255,0,0,50);
    rgb green = make_rgb(0,255,0,50);
    ColorMap estimateColorMap(InterpolMode::RGB);
    estimateColorMap.add_color_point(red,0);
    estimateColorMap.add_color_point(green,1);
    fig.set_properties(100,100,800,800);
    fig.set_tube_max_disp_slices(20000);
    fig.smooth_tube_drawing(true);
    fig.add_tube(&x, "estimate", 0,1);
    fig.set_tube_color(&x,estimateColorMap);


    // ----- Create contractor network and contract -----

    ContractorNetwork cn;
    cn.add(ctc_deriv,{a,va});
    cn.add(ctc_deriv,{x,vx});
    cn.add(ctc_phi,{x,a});
    cn.contract();
    fig.show();

    return a.is_subset(x) ? EXIT_SUCCESS : EXIT_FAILURE;

}
