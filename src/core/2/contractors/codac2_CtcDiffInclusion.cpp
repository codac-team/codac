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

  void CtcDiffInclusion::contract(TubeVector& x, TubeVector& u)
  {
    // Verifying that x and u share exactly the same tdomain and slicing:
    assert(&x.tdomain() == &u.tdomain());
    assert(_f.nb_var() == x.size() + u.size());
    assert(_f.image_dim() == x.size());

    for(auto& sx : x) // sx is a SliceVector of the TubeVector x
    {
      // su is a SliceVector of the TubeVector u:
      const SliceVector& su = sx.tslice().slices().at(&u);


      sx.set(su.codomain());
      cout << sx << " " << su << endl;

      // ...
    }

  }
}