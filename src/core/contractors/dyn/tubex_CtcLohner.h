#ifndef TUBEX_LIB_TUBEX_CTCLOHNER_H
#define TUBEX_LIB_TUBEX_CTCLOHNER_H

#include "tubex_DynCtc.h"
#include "tubex_TFnc.h"
#include "tubex_Slice.h"

namespace tubex {

struct GlobalEnclosureError : public std::runtime_error{
GlobalEnclosureError() : std::runtime_error("Exceeded loop maximum range while looking for a global enclosure for the system.") {}
};

class LohnerAlgorithm {
public:
  constexpr static const double FWD = 1, BWD = -1;
  LohnerAlgorithm(const ibex::Function *f,
                 double h,
                 const ibex::IntervalVector &u0 = ibex::IntervalVector::empty(1),
                 int contractions = 1,
                 double eps = 0.1);

  const ibex::IntervalVector &integrate(uint steps, double H = -1);
  void contractStep(const ibex::IntervalVector &x);
  const ibex::IntervalVector &getLocalEnclosure() const;
  const ibex::IntervalVector &getGlobalEnclosure() const;
private:
  ibex::IntervalVector globalEnclosure(const ibex::IntervalVector &initialGuess, double direction);

  uint dim;
  double h, eps;
  int contractions;
  ibex::IntervalVector u, z, r, u_tilde;
  ibex::Matrix B, Binv;
  ibex::Vector u_hat;
  const ibex::Function *f;
};

class CtcLohner : public tubex::DynCtc {
public:
  explicit CtcLohner(const ibex::Function &f, int contractions = 5, double eps = 0.1);

  void contract(tubex::TubeVector &tube, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

  void contract(std::vector<tubex::Domain *> &v_domains) override;

protected:
  const ibex::ExprSymbol& _x_;
  ibex::Function m_f, m_f_1;
  int contractions, dim;
  double eps;
};

} // namespace tubex
#endif //TUBEX_LIB_TUBEX_CTCLOHNER_H
