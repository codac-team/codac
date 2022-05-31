/** 
 *  CtcDiffInclusion class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcDiffInclusion.h"

using namespace std;
using namespace ibex;

namespace codac2
{
  CtcDiffInclusion::CtcDiffInclusion(const TFunction& f)
    : _f(f)
  {

  }

  const TFunction& CtcDiffInclusion::f() const
  {
    return _f;
  }

  void CtcDiffInclusion::contract(TubeVector& x, const TubeVector& u)
  {
    // Verifying that x and u share exactly the same tdomain and slicing:
    assert(&x.tdomain() == &u.tdomain());
    // Verifying that the provided tubes are consistent with the function
    assert((size_t)_f.nb_var() == 2);
    assert((size_t)_f.image_dim() == x.size());

    for(auto& sx : x) // sx is a SliceVector of the TubeVector x
    {
      if(sx.is_gate()) // the slace may be on a degenerated temporal domain, i.e. a gate
        continue;

      // su is a SliceVector of the TubeVector u:
      const SliceVector& su = sx.tslice().slices().at(&u);
      const double dt = sx.t0_tf().diam();


      // sx.set(su.codomain());
      // cout << sx << " " << su << endl;

      // ...
    }
  }

  void CtcDiffInclusion::contract(SliceVector& x, const SliceVector& u)
  {
    // Verifying that x and u share exactly the same tdomain
    assert(&x.tslice() == &u.tslice());
    // Verifying that the provided slices are consistent with the function
    assert((size_t)_f.nb_var() == 2);
    assert((size_t)_f.image_dim() == x.size());

    const double dt = x.t0_tf().diam();

    // ...
  }
}