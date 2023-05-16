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

void example_2_continuous()
{

    // Generate reference
    Interval domain(0, 8);
    double timestep = 0.01;
    IntervalVector x0({{0., 0.},
                       {0., 0.}});
    Function f("x", "y", "(1;sin(x))");

    // Lohner version
    CtcLohner ctc_lohner(f);
    TubeVector a(domain,timestep, f.image_dim()); // The reference
    a.set(x0,0.);
    ctc_lohner.contract(a);

    // CAPD Version
    //TubeVector a = CAPD_integrateODE(domain,f,x0,timestep);

    a.enable_synthesis(codac::SynthesisMode::BINARY_TREE);
    cout << "Reference generated " << a << endl;

    double epsilon = timestep;

    IntervalVector X0({{0,1},{0,1}});
    IntervalVector x({{-1,10},{-1,3.2}});
    ibex::Function phi("x1","x2","t","(x1+t;x2+cos(x1)-cos(x1+t) )");
    SepFwdBwd fullSep(phi,X0);
    IntervalVector proj(1);
    proj[0] = Interval(-8,0);
    SepProj sepProj(fullSep,proj,epsilon);

    // Visuals initialization
    beginDrawing();
    VIBesFigMap fig_map("Example 2 continuous");
    fig_map.set_properties(50,50,800,368);
    fig_map.axis_limits(x);

    auto start = chrono::steady_clock::now();
    SIVIA(x, sepProj, epsilon,true, true, "Example 2 continuous",false,LIE_SET_COLOR_MAP); // Perform the set inversion algorithm
    auto stop = chrono::steady_clock::now();

    cout << "elapsed time test-case 2 continuous: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms" <<endl;
    drawBox(X0[0].lb(),X0[0].ub(),X0[1].lb(),X0[1].ub(),"#00FF00A3[#00FF00A3]");
    fig_map.add_tube(&a,"reference",0,1);
    fig_map.set_tube_color(&a,"k[k]");
    fig_map.show();
    fig_map.axis_limits(x);
    endDrawing();

    return;
}


void example_2_discrete()
{
    // Generate reference
    Interval domain(0, 8);
    double timestep = 0.01;
    IntervalVector x0({{0., 0.},
                       {0., 0.}});
    Function f("x", "y", "(1;sin(x))");

    // Lohner version
    CtcLohner ctc_lohner(f);
    TubeVector a(domain,timestep, f.image_dim()); // The reference
    a.set(x0,0.);
    ctc_lohner.contract(a);

    // CAPD Version
    //TubeVector a = CAPD_integrateODE(domain,f,x0,timestep);

    a.enable_synthesis(codac::SynthesisMode::BINARY_TREE);
    cout << "Reference generated " << a << endl;

    double epsilon = timestep; // define accuracy of paving

    IntervalVector X0({{0,1},{0,1}});
    IntervalVector x({{-1,10},{-1,3.2}});
    ibex::Function phi("x1","x2","t","(x1+t;x2+cos(x1)-cos(x1+t) )");

    SepFwdBwd* fullSep;
    fullSep = new SepFwdBwd(phi,X0);
    vector<Interval*> projections{
            new Interval(0),
            new Interval(-2),
            new Interval(-4),
            new Interval(-6),
            new Interval(-8)};

    vector<Sep*> seps;
    for (size_t i=0;i<projections.size();i++)
    {
        SepProj *sepProj = new SepProj(*fullSep,*(projections[i]),epsilon);
        seps.push_back(sepProj);

    }
    Array<Sep> ar_sep(seps);
    vector<IntervalVector> bbox(ar_sep.size(),IntervalVector(2,Interval::ALL_REALS));
    SepUnionBbox usep (ar_sep,bbox); // Create the union of all separators



    beginDrawing();
    VIBesFigMap fig_map("Example 2 discrete");
    fig_map.set_properties(50,50+368,800,368);
    fig_map.axis_limits(x);
    auto start = chrono::steady_clock::now();
    SIVIA(x, usep, epsilon,true, true, "Example 2 discrete",false,LIE_SET_COLOR_MAP); // Perform the set inversion algorithm
    auto stop = chrono::steady_clock::now();
    cout << "elapsed time test-case 2 discrete: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms" <<endl;
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
    example_2_continuous();
    example_2_discrete();
    return(0);

}
