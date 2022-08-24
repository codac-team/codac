/** 
 *  Codac - Examples
 *  Lie symmetries example 1
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
#include <cstdlib>
#include <random>
#include "codac.h"
//#include "codac-capd.h"
#include "codac-rob.h"
#include "codac-unsupported.h"

#include "chrono"

using namespace std;
using namespace ibex;
using namespace codac;
using namespace vibes;


void example_1_continous()
{

    Interval domain(0,5); // Integration time of the reference, must be > proj
    double timestep = 0.001; // Time step for the creation of the reference
    IntervalVector x0({{0,0},{1,1}});
    Function f("x1","x2","(1; -x2)");

    // Lohner version
    CtcLohner ctc_lohner(f);
    TubeVector a(domain,timestep, f.image_dim()); // The reference
    a.set(x0,0.);
    ctc_lohner.contract(a);

    // CAPD Version
    // TubeVector a = CAPD_integrateODE(domain,f,x0,timestep);
    a.enable_synthesis(codac::SynthesisMode::BINARY_TREE);
    cout << "Reference generated " << a << endl;

    IntervalVector X0({{0,1},{2,3}}); // The uncertain initial condition

    IntervalVector x({{-0.1,6.5},{-0.2,3.5}}); // The space to explore for the set inversion

    double epsilon = timestep*10.; // define accuracy of paving

    // Generate the separator for the forward reach set
    ibex::Function phi("x1","x2","t","(x1+t;x2/exp(t))"); // define transformation function
    SepFwdBwd fullSep(phi,X0); // Create the separator on Phi with X0 as constraint
    IntervalVector proj(1);
    proj[0] = Interval(-5,0); // Define the interval of time on which we want to integrate
    SepProj sepProj(fullSep,proj,epsilon); // Create the separator object


    // Visuals initialization
    beginDrawing();
    VIBesFigMap fig_map("Example 1 continuous");
    fig_map.set_properties(50,50,800,464);
    fig_map.axis_limits(x);
    auto start = chrono::steady_clock::now();
    SIVIA(x, sepProj, epsilon,true, true, "Example 1 continuous",false,LIE_SET_COLOR_MAP); // Perform the set inversion algorithm
    auto stop = chrono::steady_clock::now();
    cout << "elapsed time test-case 1 continuous: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms" <<endl;
    drawBox(X0[0].lb(),X0[0].ub(),X0[1].lb(),X0[1].ub(),"#00FF00A3[#00FF00A3]");
    fig_map.add_tube(&a,"reference",0,1);
    fig_map.set_tube_color(&a,"k[k]");
    fig_map.show();
    fig_map.axis_limits(x);
    endDrawing();
    return;
}





void example_1_discrete()
{

    // Generate reference
    Interval domain(0,5); // Integration time of the reference, must be > proj
    double timestep = 0.001; // Time step for the creation of the reference
    IntervalVector x0({{0,0},{1,1}});
    Function f("x1","x2","(1; -x2)");

    // Lohner version
    CtcLohner ctc_lohner(f);
    TubeVector a(domain,timestep, f.image_dim()); // The reference
    a.set(x0,0.);
    ctc_lohner.contract(a);

    // CAPD Version
    // TubeVector a = CAPD_integrateODE(domain,f,x0,timestep);
    a.enable_synthesis(codac::SynthesisMode::BINARY_TREE);
    cout << "Reference generated " << a << endl;


    IntervalVector X0({{0,1},{2,3}});
    IntervalVector x({{-0.1,6.5},{-0.2,3.5}});
    double epsilon = timestep*10.;

    // Generate the separator for the forward reach set
    ibex::Function phi("x1","x2","t","(x1+t;x2/exp(t))");
    SepFwdBwd* fullSep;
    fullSep = new SepFwdBwd(phi,X0);

    // Define the different times on which we want to integrate
    vector<Interval*> projections{
            new Interval(0.),
            new Interval(-1.),
            new Interval(-2.),
            new Interval(-3.),
            new Interval(-4.),
            new Interval(-5.)};


    vector<Sep*> seps;
    for (size_t i=0;i<projections.size();i++) // Generate the separator for each individual time
    {
        SepProj *sepProj = new SepProj(*fullSep,*(projections[i]),epsilon);
        seps.push_back(sepProj);
    }
    Array<Sep> ar_sep(seps);
    vector<IntervalVector> bbox(ar_sep.size(),IntervalVector(2,Interval::ALL_REALS));
    SepUnionBbox usep (ar_sep,bbox); // Create the union of all separators



    // Visuals initialization
    beginDrawing();
    VIBesFigMap fig_map("Example 1 discrete");
    fig_map.set_properties(50,50+464,800,464);
    fig_map.axis_limits(x);

    auto start = chrono::steady_clock::now();
    SIVIA(x, usep, epsilon,true, true, "Example 1 discrete",false,LIE_SET_COLOR_MAP); // Perform the set inversion algorithm
    auto stop = chrono::steady_clock::now();
    cout << "elapsed time test-case 1 discrete: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms" <<endl;

    drawBox(X0[0].lb(),X0[0].ub(),X0[1].lb(),X0[1].ub(),"#00FF00A3[#00FF00A3]");
    fig_map.add_tube(&a,"reference",0,1);
    fig_map.set_tube_color(&a,"k[k]");
    fig_map.show();
    fig_map.axis_limits(x);
    endDrawing();

    int n = seps.size();
    for (int i =0; i<n; i++)
    {
        delete(projections[i]);
        delete(seps[i]);
    }

    return;
}


int main (int argc, char* argv[])
{
    example_1_continous();
    example_1_discrete();




}
