/** 
 *  CtcPicard class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Auguste Bourgois
 *  \copyright  Copyright 2020 Tubex Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCLOHNER_H__
#define __TUBEX_CTCLOHNER_H__

#include "tubex_DynCtc.h"
#include "tubex_TFnc.h"
#include "tubex_Slice.h"

namespace tubex {

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

#endif