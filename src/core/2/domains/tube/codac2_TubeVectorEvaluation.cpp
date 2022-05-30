/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_TubeVectorEvaluation.h"
#include "codac2_TubeVector.h"
#include "codac2_SliceVector.h"
#include "ibex_Interval.h"

using namespace std;

namespace codac2
{
  TubeVectorEvaluation::TubeVectorEvaluation(TubeVector& tubevector, double t) :
    _t(Interval(t)), _tubevector(tubevector)
  {

  }

  TubeVectorEvaluation::TubeVectorEvaluation(TubeVector& tubevector, const Interval& t) :
    _t(t), _tubevector(tubevector)
  {

  }
  
  TubeVectorEvaluation::operator IntervalVector() const
  {
    return _tubevector.eval(_t);
  }

  const TubeVectorEvaluation& TubeVectorEvaluation::operator=(const IntervalVector& x)
  {
    // Sampling the tube only if affectation is performed
    // (i.e. this is not done in the constructor)
    list<TSlice>::iterator it_lb = _tubevector._tdomain.sample(_t.lb(), false);
    list<TSlice>::iterator it_ub = _tubevector._tdomain.sample(_t.ub(), _t.is_degenerated());

    do
    {
      it_lb->_slices.at(&_tubevector).set(x);
      it_lb++;
    } while(it_lb != it_ub);

    return *this;
  }

  ostream& operator<<(ostream& os, const TubeVectorEvaluation& x)
  {
    os << x._tubevector.eval(x._t) << flush;
    return os;
  }

} // namespace codac