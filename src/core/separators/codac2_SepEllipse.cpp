/** 
 *  codac2_SepEllipse.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepEllipse.h"

using namespace std;
using namespace codac2;

SepEllipse::SepEllipse(const Vector& q)
  : Sep<SepEllipse>(2), _ctc(CtcEllipse(q))
{
  assert_release(q.size() == 6);
}

bool test_ellipse(const IntervalVector& x, const IntervalVector& q)
{
  // todo: Interval evaluation:
  return (q[0]
        + q[1]*x[0]
        + q[2]*x[1]
        + q[3]*pow(x[0],2)
        + q[4]*x[0]*x[1]
        + q[5]*pow(x[1],2)).ub() < 0.;
}

BoxPair SepEllipse::separate(const IntervalVector& x) const
{
  assert_release(x.size() == this->size());

  IntervalVector p(x), x_in(x), x_out(x);
  _ctc.contract(p);

  if(p.is_empty())
  {
    if(test_ellipse(x.mid(),_ctc.q()))
      return { IntervalVector::empty(2), x_out };
    
    else
      return { x_in, IntervalVector::empty(2) };
  }

  Index i = Vector(x_out.rad()-p.rad()).max_coeff_index();

  double e1 = p[i].lb() - x_out[i].lb();
  double e2 = x_out[i].ub() - p[i].ub();

  if(e1+e2 <= 0.)
    return { x_in, x_out };

  IntervalVector q(x);
  IntervalVector r(x);

  if(e2 > e1)
  {
    q[i] = Interval(p[i].ub(), x[i].ub());
    r[i] = Interval(x[i].lb(), p[i].ub());
  }

  else
  {
    q[i] = Interval(x[i].lb(), p[i].lb());
    r[i] = Interval(p[i].lb(), x[i].ub());
  }

  if(test_ellipse(q.mid(),_ctc.q()))
    return { r, x_out };

  else
    return { x_in, r };
}