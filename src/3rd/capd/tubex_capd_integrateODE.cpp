/**
 *  ODE integration tools based on CAPD
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iomanip>
#include "tubex_capd_integrateODE.h"
#include "tubex_Exception.h"
#include "tubex_TFunction.h"
#include "ibex_Expr2Minibex.h"
#include <capd/capdlib.h>

using namespace std;
using namespace ibex;
using namespace tubex;

namespace tubex
{
  string capd_str_function(const Function& f)
  {
    string capd_string = "time:t;";

    capd_string += "var:";
    for(int i = 0 ; i < f.nb_var() ; i++)
      if(f.arg_name(i) != string("t")) // reserved var name
        capd_string += f.arg_name(i) + string(i == f.nb_var() - 1 ? "" : ",");

    capd_string += ";fun:" + to_string(f) + ";";
    return capd_string;
  }

  TubeVector CAPD_integrateODE(const Interval& tdomain, capd::IMap& vector_field, const IntervalVector& x0, double tube_dt, int capd_order, double capd_dt)
  {
    assert(DynamicalItem::valid_tdomain(tdomain));
    assert(capd_order > 0);
    assert(capd_dt >= 0.); // if 0, auto setting of CAPD algorithm
    assert(tube_dt > 0.);

    // Initialization

    capd::IOdeSolver solver(vector_field, capd_order);
    if(capd_dt != 0.)
      solver.setStep(capd_dt);
    capd::ITimeMap time_map(solver);

    // Initial condition

    capd::IVector capd_x0(x0.size());
    for(int i = 0 ; i < x0.size() ; i++)
      capd_x0[i] = capd::interval(x0[i].lb(), x0[i].ub()); // todo: use helpers here
    capd::C0Rect2Set set(capd_x0, tdomain.lb()); // x0 at t0

    // Integration

    capd::ITimeMap::SolutionCurve solution(tdomain.lb()); // define functional object
    time_map(tdomain.ub()+capd_dt, set, solution); // and integrate
    assert(Interval(solution.getLeftDomain(), solution.getRightDomain()).is_superset(tdomain)); // todo: use helpers here

    // Building the tube

    TubeVector tube(tdomain, tube_dt, vector_field.imageDimension());

    Slice **v_s = new Slice*[tube.size()];
    for(int i = 0 ; i < tube.size() ; i++)
      v_s[i] = tube[i].first_slice();

    while(v_s[0] != NULL)
    {
      capd::IVector y = solution(capd::interval(v_s[0]->tdomain().lb(), v_s[0]->tdomain().ub()));

      for(int i = 0 ; i < tube.size() ; i++)
      {
        v_s[i]->set(Interval(y[i].left().leftBound(), y[i].right().rightBound())); // todo: use helpers here
        v_s[i] = v_s[i]->next_slice();
      }
    }

    delete v_s;
    return tube;
  }

  // For autonomous systems
  TubeVector CAPD_integrateODE(const Interval& tdomain, const Function& f, const IntervalVector& x0, double tube_dt, int capd_order, double capd_dt)
  {
    assert(f.nb_var() == f.image_dim());
    assert(f.nb_var() == x0.size());
    capd::IMap vector_field(capd_str_function(f));
    return CAPD_integrateODE(tdomain, vector_field, x0, tube_dt, capd_order, capd_dt);
  }

  // For non-autonomous systems
  TubeVector CAPD_integrateODE(const Interval& tdomain, const TFunction& f, const IntervalVector& x0, double tube_dt, int capd_order, double capd_dt)
  {
    assert(f.nb_var() == f.image_dim());
    assert(f.nb_var() == x0.size());
    capd::IMap vector_field(capd_str_function(f.getFunction()));
    return CAPD_integrateODE(tdomain, vector_field, x0, tube_dt, capd_order, capd_dt);
  }
}