/**
 *  Codac - Examples
 *  Lie symmetries example 3
 * ----------------------------------------------------------------------------
 *
 *
 *  \date       2022
 *  \author     Julien Damers
 *  \copyright  Copyright 2022 Julien Damers
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <random>
#include "codac.h"
//#include "codac-capd.h"
#include "codac-unsupported.h"
#include "chrono"

using namespace std;
using namespace ibex;
using namespace codac;
using namespace vibes;


void example_3_continuous_article()
{
    Interval domain(0,5); // Integration time of the reference, must be > proj
    double timestep = 0.001; // Time step for the creation of the reference
    IntervalVector x0({{0.5,0.5},{0,0}}); // Initial condition for reference
    Function f("x","y","(-x^3-x*y^2+x-y; -y^3-x^2*y+x+y)"); // Evolution function to integrate

    // Lohner version
    CtcLohner ctc_lohner(f);
    TubeVector a(domain,timestep, f.image_dim()); // The reference
    a.set(x0,0.);
    ctc_lohner.contract(a);

    // CAPD version
    //TubeVector a = CAPD_integrateODE(domain,f,x0,timestep); // The reference

    cout << "Reference generated " << a << endl;

    IntervalVector x({{-2,2},{-2,2}}); // The space to explore for the set inversion

    double epsilon = 0.01; // define accuracy of paving

    Function phi("x10", "x20","t", "r1", "r2",
                 "((x10*(0.5*(sqrt(1/(3*exp(-2*t)+1))*cos(t)))-x20*(0.5*(sqrt(1/(3*exp(-2*t)+1))*sin(t))))/(0.25*sqrt(((1-(1/(3*exp(-2*t)+1)))/0.75)+(((1/(3*exp(-2*t)+1))/0.25)-((1-(1/(3*exp(-2*t)+1)))/0.75))*(x10^2+x20^2)))-r1;\
             (x20*(0.5*(sqrt(1/(3*exp(-2*t)+1))*cos(t)))+x10*(0.5*(sqrt(1/(3*exp(-2*t)+1))*sin(t))))/(0.25*sqrt(  ((1-(1/(3*exp(-2*t)+1)))/0.75)+( ((1/(3*exp(-2*t)+1))*(4+(1/0.75)))-(4/3) ) * (x10^2+x20^2)  ))-r2)");


    Function f_dom("x10", "x20","t", "r1", "r2",
                   "((1-(1/(3*exp(-2*t)+1)))/0.75)+(((1/(3*exp(-2*t)+1))/0.25)-((1-(1/(3*exp(-2*t)+1)))/0.75))*(x10^2+x20^2)");

    Function f_circle ("i1","i2","( (i1-1.5)^2 + (i2-1.5)^2)");

    CtcFwdBwd ctc_phi(phi);
    CtcFwdBwd ctc_dom(f_dom, Interval::NEG_REALS);
    CtcUnion ctc_phi_int(ctc_phi,ctc_dom);

    CtcFwdBwd ctc_initial(f_circle, Interval(0, 0.04));
    CtcNotIn ctc_not_initial(f_circle, Interval(0, 0.04));


    // Outside contractor
    ContractorNetwork cn_out;
    IntervalVectorVar box_out(3);
    IntervalVector& x_init_out = cn_out.create_interm_var(IntervalVector(2));
    cn_out.add(ctc_phi, {box_out, x_init_out});
    cn_out.add(ctc_initial, {x_init_out});
    CtcCN ctc_cn_out(cn_out, box_out);

    // Inside contractor
    ContractorNetwork cn_in;
    IntervalVectorVar box_in(3);
    IntervalVector& x_init_in = cn_in.create_interm_var(IntervalVector(2));
    cn_in.add(ctc_phi_int,{box_in,x_init_in});
    cn_in.add(ctc_not_initial,{x_init_in});
    CtcCN ctc_cn_in(cn_in,box_in);

    SepCtcPair sep(ctc_cn_in,ctc_cn_out);
    SepProj sep_proj(sep,Interval(-6,0), epsilon);


    // Visuals initialization
    beginDrawing();
    VIBesFigMap fig("Example 3 continuous");
    fig.set_properties(50,50,800,800);
    fig.axis_limits(x);

    auto start = chrono::steady_clock::now();
    SIVIA(x, sep_proj, epsilon,true, true, "Example 3 continuous",false,LIE_SET_COLOR_MAP); // Perform the set inversion algorithm
    auto stop = chrono::steady_clock::now();
    cout << "elapsed time for test-case 3 continuous version: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms" <<endl;
    drawCircle(1.5,1.5,0.2,"#00FF00A3[#00FF00A3]");
    drawCircle(1.5,1.5,0.2,"#00FF00A3[#00FF00A3]");
    fig.add_tube(&a,"reference",0,1);
    fig.set_tube_color(&a,"k[]");
    fig.show();
    fig.axis_limits(x);
    endDrawing();
    return;


}

void example_3_discrete_article()
{
    Interval domain(0,5); // Integration time of the reference, must be > proj
    double timestep = 0.001; // Time step for the creation of the reference
    IntervalVector x0({{0.5,0.5},{0,0}}); // Initial condition for reference
    Function f("x","y","(-x^3-x*y^2+x-y; -y^3-x^2*y+x+y)"); // Evolution function to integrate

    // Lohner version
    CtcLohner ctc_lohner(f);
    TubeVector a(domain,timestep, f.image_dim()); // The reference
    a.set(x0,0.);
    ctc_lohner.contract(a);

    // CAPD version
    //TubeVector a = CAPD_integrateODE(domain,f,x0,timestep); // The reference

    cout << "Reference generated " << a << endl;

    IntervalVector x({{-2,2},{-2,2}}); // The space to explore for the set inversion

    double epsilon = 0.01; // define accuracy of paving

    Function phi("x10", "x20","t", "r1", "r2",
                 "((x10*(0.5*(sqrt(1/(3*exp(-2*t)+1))*cos(t)))-x20*(0.5*(sqrt(1/(3*exp(-2*t)+1))*sin(t))))/(0.25*sqrt(((1-(1/(3*exp(-2*t)+1)))/0.75)+(((1/(3*exp(-2*t)+1))/0.25)-((1-(1/(3*exp(-2*t)+1)))/0.75))*(x10^2+x20^2)))-r1;\
             (x20*(0.5*(sqrt(1/(3*exp(-2*t)+1))*cos(t)))+x10*(0.5*(sqrt(1/(3*exp(-2*t)+1))*sin(t))))/(0.25*sqrt(  ((1-(1/(3*exp(-2*t)+1)))/0.75)+( ((1/(3*exp(-2*t)+1))*(4+(1/0.75)))-(4/3) ) * (x10^2+x20^2)  ))-r2)");


    Function f_dom("x10", "x20","t", "r1", "r2",
                   "((1-(1/(3*exp(-2*t)+1)))/0.75)+(((1/(3*exp(-2*t)+1))/0.25)-((1-(1/(3*exp(-2*t)+1)))/0.75))*(x10^2+x20^2)");

    Function f_circle ("i1","i2","( (i1-1.5)^2 + (i2-1.5)^2)");

    CtcFwdBwd ctc_phi(phi);
    CtcFwdBwd ctc_dom(f_dom, Interval::NEG_REALS);
    CtcUnion ctc_phi_int(ctc_phi,ctc_dom);

    CtcFwdBwd ctc_initial(f_circle, Interval(0, 0.04));
    CtcNotIn ctc_not_initial(f_circle, Interval(0, 0.04));


    ContractorNetwork cn_out;
    IntervalVectorVar box_out(3);
    IntervalVector& x_init_out = cn_out.create_interm_var(IntervalVector(2));
    cn_out.add(ctc_phi, {box_out, x_init_out});
    cn_out.add(ctc_initial, {x_init_out});
    CtcCN ctc_cn_out(cn_out, box_out);


    ContractorNetwork cn_in;
    IntervalVectorVar box_in(3);
    IntervalVector& x_init_in = cn_in.create_interm_var(IntervalVector(2));
    cn_in.add(ctc_phi_int,{box_in,x_init_in});
    cn_in.add(ctc_not_initial,{x_init_in});
    CtcCN ctc_cn_in(cn_in,box_in);

    SepCtcPair sep(ctc_cn_in,ctc_cn_out);

    vector<Interval*> projections{
            new Interval(0),
            new Interval(-0.1),
            new Interval(-0.25),
            new Interval(-0.5),
            new Interval(-0.75),
            new Interval(-1.),
            new Interval(-2.),
            new Interval(-3.),
            new Interval(-4.),
            new Interval(-5.),
            new Interval(-6.)};
    vector<Sep*> seps;
    for (size_t i=0;i<projections.size();i++)
    {
        SepProj *sepProj = new SepProj(sep,*(projections[i]),epsilon);
        seps.push_back(sepProj);
    }
    Array<Sep> ar_sep(seps);
    vector<IntervalVector> bbox(ar_sep.size(),IntervalVector(2,Interval::ALL_REALS));
    SepUnionBbox usep (ar_sep,bbox); // Create the union of all separators


    // Visuals initialization
    beginDrawing();
    beginDrawing();
    VIBesFigMap fig("Example 3 discrete");
    fig.set_properties(50,50,800,800);
    fig.axis_limits(x);

    auto start = chrono::steady_clock::now();
    SIVIA(x,usep,epsilon,true,true,"Example 3 discrete",false,LIE_SET_COLOR_MAP); // Perform the set inversion algorithm
    auto stop = chrono::steady_clock::now();
    cout << "elapsed time for test-case 3 discrete version: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms" <<endl;
    drawCircle(1.5,1.5,0.2,"#00FF00A3[#00FF00A3]");
    drawCircle(0,0,1,"k[]");
    fig.add_tube(&a,"reference",0,1);
    fig.set_tube_color(&a,"k[k]");
    fig.show();
    fig.axis_limits(x);
    endDrawing();
    return;


}

int main (int argc, char* argv[])
{
    example_3_continuous_article();
    example_3_discrete_article();
}