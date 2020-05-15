/** 
 *  \file
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCFUNCTION_H__
#define __TUBEX_CTCFUNCTION_H__

#include <string>
#include "tubex_Ctc.h"
#include "ibex_Function.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_Ctc3BCid.h"

namespace tubex
{
  /**
   * \class CtcFunction
   * \brief Generic static \f$\mathcal{C}\f$ that contracts a box \f$[\mathbf{x}]\f$ 
   *        according to the constraint \f$\mathbf{f}(\mathbf{x})=\mathbf{0}\f$ or \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$.
   *        It stands on the CtcFwdBwd of IBEX (HC4Revise) combined with a Ctc3BCid.
   */
  class CtcFunction : public ibex::Ctc3BCid
  {
    public:

      CtcFunction(const char* x1, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* f);
      CtcFunction(const ibex::Function& f, const ibex::Domain& y);
      CtcFunction(const ibex::Function& f, const ibex::Interval& y);
      CtcFunction(const ibex::Function& f, const ibex::IntervalVector& y);

      static const std::string parse_f(const char* f);
  };
}


#endif