/** 
 *  codac2_CtcLohner.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Auguste Bourgois, Maël Godard
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcLohner.h"
#include "codac2_Slice.h"

using namespace std;
using namespace codac2;

LohnerAlgorithm::LohnerAlgorithm(const AnalyticFunction<VectorType> *f,
                                 double h,
                                 bool forward,
                                 const IntervalVector &u0,
                                 int contractions,
                                 double eps)
    : _dim(f->input_size()),
      _h(h),
      _direction((forward) ? FWD : BWD),
      _eps(eps),
      _contractions(contractions),
      _u(u0),
      _z(u0 - u0.mid()),
      _r(_z),
      _B(Matrix::eye(_dim,_dim)),
      _Binv(Matrix::eye(_dim,_dim)),
      _u_hat(u0.mid()),
      _f(f) {}

const IntervalVector &LohnerAlgorithm::integrate(unsigned int steps, double H) {
  if (H > 0) _h = H;
  for (unsigned int i = 0; i < steps; ++i) {
    IntervalVector z1 = _z, r1 = _r, u1 = _u;
    Matrix B1 = _B, B1inv = _Binv;
    Vector u_hat1 = _u_hat;
    IntervalVector u_t = globalEnclosure(_u, FWD);
    for (int j = 0; j < _contractions; ++j) {
      z1 = 0.5 * _h * _h * _f->diff(u_t) * _f->eval(u_t);
      Vector m1 = z1.mid();
      IntervalMatrix A = Matrix::eye(_dim,_dim) + _h * _direction * _f->diff(_u);
      Eigen::HouseholderQR<Eigen::MatrixXd> qr((A * _B.template cast<Interval>()).mid());
      B1 = qr.householderQ();
      B1inv = B1.inverse();
      r1 = (B1inv.template cast<Interval>() * A * _B.template cast<Interval>()) * _r + B1inv.template cast<Interval>() * (z1 - m1);
      IntervalVector inter = _f->eval(_u_hat);
      u_hat1 = _u_hat + _h * _direction * inter.mid() + m1;
      u1 = u_hat1 + B1 * r1;
      if (j < _contractions - 1) {
        u_t = u_t & globalEnclosure(u1, BWD);
      }
    }
    _z = z1, _r = r1, _u = u1, _B = B1, _Binv = B1inv, _u_hat = u_hat1, _u_tilde = u_t;
  }
  return _u;
}

IntervalVector LohnerAlgorithm::globalEnclosure(const IntervalVector &initialGuess, double dir) {
  IntervalVector u_0 = initialGuess;
  for (unsigned int i = 0; i < 30; ++i) {
    IntervalVector u_1 = initialGuess + dir * _direction * Interval(0, _h) * _f->eval(u_0);
    if (u_0.is_superset(u_1)) {
      return u_0;
    } else {
      u_0 = (1 + _eps) * u_1 - _eps * u_1;
    }
  }
  throw GlobalEnclosureError();
}

void LohnerAlgorithm::contractStep(const IntervalVector &x) {
  _u = x & _u;
  _u_hat = _u.mid();
  _r = _r & (_Binv * (_u - _u_hat));
}

const IntervalVector &LohnerAlgorithm::getLocalEnclosure() const {
  return _u;
}

const IntervalVector &LohnerAlgorithm::getGlobalEnclosure() const {
  return _u_tilde;
}

CtcLohner::CtcLohner(const AnalyticFunction<VectorType>& f, int contractions, double eps)
  :   _f(f),
      _contractions(contractions),
      _dim(f.input_size()),
      _eps(eps) {}

void CtcLohner::contract(codac2::SlicedTube<IntervalVector> &tube, TimePropag t_propa) const
{
  assert((!tube.is_empty()) && (tube.size() == _dim));
  IntervalVector input_gate = IntervalVector::zero(_dim);
  IntervalVector output_gate = IntervalVector::zero(_dim);
  IntervalVector slice = IntervalVector::zero(_dim);
  double h;
  if((t_propa & TimePropag::FWD) == TimePropag::FWD)
  {
    input_gate = tube.first_slice()->input_gate();
    LohnerAlgorithm lo(&_f, 0.1, true, input_gate, _contractions, _eps);
    // Forward loop
    for (auto it = tube[0].tdomain()->begin(); it != tube[0].tdomain()->end(); ++it) 
    {
      if (!tube.slice(it)->is_gate())
      {
        h = (*it).diam();
        output_gate = tube.slice(it)->output_gate();
        slice = tube.slice(it)->codomain();
        lo.integrate(1, h);
        lo.contractStep(output_gate);
        tube.slice(it)->set(slice & lo.getGlobalEnclosure());
      }
    }
    tube.set(output_gate & lo.getLocalEnclosure(), tube.tdomain()->t0_tf().ub());
  }
  if((t_propa & TimePropag::BWD) == TimePropag::BWD) 
  {
    input_gate = tube.last_slice()->output_gate();
    LohnerAlgorithm lo2(&_f, 0.1, false, input_gate, _contractions, _eps);
    // Backward loop
    for (auto it = tube[0].tdomain()->rbegin(); it != tube[0].tdomain()->rend(); ++it) 
    {
      if (!tube.slice(it)->is_gate())
      {
        h = (*it).diam();
        output_gate = tube.slice(it)->input_gate();
        slice = tube.slice(it)->codomain();
        lo2.integrate(1, h);
        lo2.contractStep(output_gate);
        tube.slice(it)->set(slice & lo2.getGlobalEnclosure());
      }
    }
    tube.set(output_gate & lo2.getLocalEnclosure(), tube.tdomain()->t0_tf().lb());
  }
}