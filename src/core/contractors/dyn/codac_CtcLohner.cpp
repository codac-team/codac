/** 
 *  CtcLohner class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Auguste Bourgois
 *  \copyright  Copyright 2020 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcLohner.h"

#include <codac_CtcLohner.h>
#include <Eigen/QR>
#include <ibex.h>
#include <codac_Eigen.h>
#include <codac_DomainsTypeException.h>


using namespace ibex;
using namespace std;

namespace codac {

// todo: to be placed in header file

/**
 * \class GlobalEnclosureError
 *
 * \brief Encapsulates runtime error for global enclosure estimation failure
 */
struct GlobalEnclosureError : public std::runtime_error {
  GlobalEnclosureError() : std::runtime_error(
      "Exceeded loop maximum range while looking for a global enclosure for the system.") {}
};

/**
 * \class LohnerAlgorithm
 *
 * \brief Simple first order Lohner algorithm to perform guaranteed integration of a system
 * \f$\dot{\mathbf{x}}=\mathbf{f}(\mathbf{x})\f$
 */
class LohnerAlgorithm {
public:
  constexpr static const double FWD = 1., BWD = -1.;

  /**
   * \brief Creates a Lohner algorithm object
   *
   * \param f function defining the system \f$\dot{\mathbf{x}}=\mathbf{f}(\mathbf{x})\f$
   * \param h time step of the integration method
   * \param direction forward or backaward integration
   * \param u0 initial condition of the system
   * \param contractions number of contractions of the global enclosure by the estimated local enclosure
   * \param eps inflation parameter for the global enclosure
   */
  LohnerAlgorithm(const Function *f,
                  double h,
                  bool forward,
                  const IntervalVector &u0 = IntervalVector::empty(1),
                  int contractions = 1,
                  double eps = 0.1);
  /**
   * \brief integrate the system over a given number of steps
   *
   * \param steps number of steps to integrate
   * \param H parameter to overwrite the integration time step
   * \return enclosure of the system's state
   */
  const IntervalVector &integrate(unsigned int steps, double H = -1);

  /**
   * \brief contract the global & local enclosure of the previous integration step
   *
   * \param x estimation of the global enclosure
   */
  void contractStep(const IntervalVector &x);

  /**
   * \brief Returns the current global enclosure, i.e. the box enclosing the trajectories between times \f$k-1\f$ and
   * \f$k\f$
   *
   * \return global enclosure
   */
  const IntervalVector &getLocalEnclosure() const;

  /**
   * \brief Returns the current local enclosure, i.e. the box enclosing the trajectories at time \f$k\f$
   *
   * \return local enclosure \f$[\mathbf{x}_{k}]\f$
   */
  const IntervalVector &getGlobalEnclosure() const;
private:
  /**
   * \brief Computes an estimation of the global enclosure of the system
   *
   * \param initialGuess initial guess for the global enclosure
   * \param dir forward or backward in time
   * \return estimation of the global enclosure
   */
  IntervalVector globalEnclosure(const IntervalVector &initialGuess, double dir);

  unsigned int dim; //!< dimension of the system's state
  double h; //!< integration time step
  double direction; //!< forward or backward integration
  double eps; //!< inflation parameter for the global enclosure
  int contractions; //!< number of contractions of the global enclosure by the estimated local enclosure
  IntervalVector u; //!< local enclosure
  IntervalVector z; //!< Taylor-Lagrange remainder (order 2)
  IntervalVector r; //!< enclosure of uncertainties in the frame given by the matrix B
  IntervalVector u_tilde; //!< global enclosure
  Matrix B, Binv;
  Vector u_hat; //!< center of the box u
  const Function *f; //!< litteral function of the system \f$\dot{\mathbf{x}}=\mathbf{f}(\mathbf{x})\f$
};

// --

LohnerAlgorithm::LohnerAlgorithm(const Function *f,
                                 double h,
                                 bool forward,
                                 const IntervalVector &u0,
                                 int contractions,
                                 double eps)
    : dim(f->nb_var()),
      h(h),
      direction((forward) ? FWD : BWD),
      eps(eps),
      contractions(contractions),
      u(u0),
      z(u0 - u0.mid()),
      r(z),
      B(Matrix::eye(dim)),
      Binv(Matrix::eye(dim)),
      u_hat(u0.mid()),
      f(f) {}

const IntervalVector &LohnerAlgorithm::integrate(unsigned int steps, double H) {
  if (H > 0) h = H;
  for (unsigned int i = 0; i < steps; ++i) {
    auto z1 = z, r1 = r, u1 = u;
    auto B1 = B, B1inv = Binv;
    auto u_hat1 = u_hat;
    IntervalVector u_t = globalEnclosure(u, FWD);
    for (int j = 0; j < contractions; ++j) {
      z1 = 0.5 * h * h * f->jacobian(u_t) * f->eval_vector(u_t);
      Vector m1 = z1.mid();
      IntervalMatrix A = Matrix::eye(dim) + h * direction * f->jacobian(u);
      Eigen::HouseholderQR<Eigen::MatrixXd> qr(EigenHelpers::i2e((A * B).mid()));
      B1 = EigenHelpers::e2i(qr.householderQ());
      B1inv = ibex::real_inverse(B1);
      r1 = (B1inv * A * B) * r + B1inv * (z1 - m1);
      u_hat1 = u_hat + h * direction * f->eval_vector(u_hat).mid() + m1;
      u1 = u_hat1 + B1 * r1;
      if (j < contractions - 1) {
        u_t = u_t & globalEnclosure(u1, BWD);
      }
    }
    z = z1, r = r1, u = u1, B = B1, Binv = B1inv, u_hat = u_hat1, u_tilde = u_t;
  }
  return u;
}

IntervalVector LohnerAlgorithm::globalEnclosure(const IntervalVector &initialGuess, double dir) {
  IntervalVector u_0 = initialGuess;
  for (unsigned int i = 0; i < 30; ++i) {
    IntervalVector u_1 = initialGuess + dir * direction * Interval(0, h) * f->eval_vector(u_0);
    if (u_0.is_superset(u_1)) {
      return u_0;
    } else {
      u_0 = (1 + eps) * u_1 - eps * u_1;
    }
  }
  throw GlobalEnclosureError();
}

void LohnerAlgorithm::contractStep(const IntervalVector &x) {
  u = x & u;
  u_hat = u.mid();
  r = r & (Binv * (u - u_hat));
}

const IntervalVector &LohnerAlgorithm::getLocalEnclosure() const {
  return u;
}

const IntervalVector &LohnerAlgorithm::getGlobalEnclosure() const {
  return u_tilde;
}

CtcLohner::CtcLohner(const Function &f, int contractions, double eps)
    : DynCtc(),
      m_f(f),
      contractions(contractions),
      dim(f.nb_var()),
      eps(eps) {}

void CtcLohner::contract(codac::TubeVector &tube, TimePropag t_propa) {
  assert((!tube.is_empty()) && (tube.size() == dim));
  IntervalVector input_gate(dim, Interval(0)), output_gate(dim, Interval(0)), slice(dim, Interval(0));
  double h;
  if (t_propa & TimePropag::FORWARD) {
    for (int j = 0; j < dim; ++j) {
      input_gate[j] = tube[j].slice(0)->input_gate();
    }
    LohnerAlgorithm lo(&m_f, 0.1, true, input_gate, contractions, eps);
    // Forward loop
    for (int i = 0; i < tube.nb_slices(); ++i) {
      h = tube[0].slice(i)->tdomain().diam();
      for (int j = 0; j < dim; ++j) {
        output_gate[j] = tube[j].slice(i)->output_gate();
        slice[j] = tube[j].slice(i)->codomain();
      }
      lo.integrate(1, h);
      lo.contractStep(output_gate);
      for (int j = 0; j < dim; ++j)
        tube[j].slice(i)->set_envelope(slice[j] & lo.getGlobalEnclosure()[j]);
    }
    tube.set(output_gate & lo.getLocalEnclosure(), tube.tdomain().ub());
  }
  if (t_propa & TimePropag::BACKWARD) {
    for (int j = 0; j < dim; ++j) {
      input_gate[j] = tube[j].last_slice()->output_gate();
    }
    LohnerAlgorithm lo2(&m_f, 0.1, false, input_gate, contractions, eps);
    // Backward loop
    for (int i = tube.nb_slices() - 1; i >= 0; --i) {
      h = tube[0].slice(i)->tdomain().diam();
      for (int j = 0; j < dim; ++j) {
        output_gate[j] = tube[j].slice(i)->input_gate();
        slice[j] = tube[j].slice(i)->codomain();
      }
      lo2.integrate(1, h);
      lo2.contractStep(output_gate);
      for (int j = 0; j < dim; ++j)
        tube[j].slice(i)->set_envelope(slice[j] & lo2.getGlobalEnclosure()[j]);
    }
    tube.set(output_gate & lo2.getLocalEnclosure(), tube.tdomain().lb());
  }
}

void CtcLohner::contract(codac::Tube &tube, TimePropag t_propa) {
  assert(!tube.is_empty());
  codac::TubeVector tubeVector(1, tube);
  contract(tubeVector, t_propa);
  tube = tubeVector[0];
}

// Static members for contractor signature (mainly used for CN Exceptions)
const string CtcLohner::m_ctc_name = "CtcLohner";
vector<string> CtcLohner::m_str_expected_doms(
    {
        "Tube",
        "TubeVector",
    });

void CtcLohner::contract(vector<Domain *> &v_domains) {

  // todo: manage the contractor on a slice level

  if (v_domains.size() != 1)
    throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);

  if (v_domains[0]->type() == Domain::Type::T_TUBE)
    contract(v_domains[0]->tube(), TimePropag::FORWARD | TimePropag::BACKWARD);

  else if (v_domains[0]->type() == Domain::Type::T_TUBE_VECTOR)
    contract(v_domains[0]->tube_vector(), TimePropag::FORWARD | TimePropag::BACKWARD);

  else
    throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
}

} // namespace codac