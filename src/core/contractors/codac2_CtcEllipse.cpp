/** 
 *  codac2_CtcEllipse.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Luc Jaulin, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcEllipse.h"
#include "codac2_CtcAction.h"
#include "codac2_Sep.h"
#include "codac2_template_tools.h"

using namespace std;
using namespace codac2;


CtcEllipse0::CtcEllipse0(const IntervalVector& q)
  : Ctc<CtcEllipse0,IntervalVector>(2), _q(q), _qs(OctaSym({1,3,2,6,5,4})(q))
{
  assert(q.size() == 6);
}

Interval _psi0(const IntervalVector& q) // top vertex of the ellipse
{
  assert(q.size() == 6);
  Interval a2 = 4*q[3]*q[5]-sqr(q[4]);
  Interval b2 = 4*q[3]*q[2]-2*q[1]*q[4];
  Interval c2 = 4*q[3]*q[0]-sqr(q[1]);
  Interval d2 = sqr(b2)-4*a2*c2;
  return (-b2+sqrt(d2))/(2*a2);
}

Interval _phi0(const IntervalVector& q, const Interval& x2)
{
  assert(q.size() == 6);
  Interval a1 = q[3];
  Interval b1 = q[1]+q[4]*x2;
  Interval c1 = q[0]+q[2]*x2+q[5]*sqr(x2);
  Interval d1 = max(sqr(b1)-4*a1*c1,Interval(0.));
  return (-b1+sqrt(d1))/(2*a1);
}

void CtcEllipse0::contract(IntervalVector& x) const
{
  assert(x.size() == 2);

  if(!x.is_empty())
  {
    IntervalVector a({
      {_phi0(_q,_psi0(_q)).lb(),_psi0(_qs).ub()},
      {_phi0(_qs,_psi0(_qs)).lb(),_psi0(_q).ub()}
    });

    IntervalVector b = x & a;

    x &= IntervalVector({
      {_phi0(_q,b[1].ub()).lb(),_phi0(_q,b[1].lb()).ub()},
      {_phi0(_qs,b[0].ub()).lb(),_phi0(_qs,b[0].lb()).ub()}
    });
  }
}

CtcEllipse::CtcEllipse(const IntervalVector& q)
  : Ctc<CtcEllipse,IntervalVector>(2), _q(q)
{
  assert_release(q.size() == 6);
}

void CtcEllipse::contract(IntervalVector& x) const
{
  assert_release(x.size() == 2);
  
  x = contract_ith({1, 2}, x) | contract_ith({-1,-2}, x)
    | contract_ith({1,-2}, x) | contract_ith({-1, 2}, x);
}

const IntervalVector& CtcEllipse::q() const
{
  return _q;
}

OctaSym _sign(const OctaSym& i)
{
  OctaSym s(i);
  for(auto& si : s)
    si = (si > 0) ? 1 : ((si < 0) ? -1 : 0);
  return s;
}

OctaSym _psi(const OctaSym& e)
{
  assert(e.size() == 2);
  return { 1, e[0]*2, e[1]*3, 4, e[0]*e[1]*5, 6 };
}

IntervalVector CtcEllipse::contract_ith(const OctaSym& i, const IntervalVector& x) const
{
  OctaSym se(_psi(_sign(i)));
  CtcEllipse0 ctc0(se(_q));

  OctaSym s(i);
  OctaSym a(s);

  IntervalVector _x(x);
  a(ctc0).contract(_x);
  return _x;
}