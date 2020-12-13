/** 
 *  CtcPicard class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCPICARD_H__
#define __TUBEX_CTCPICARD_H__

#include "tubex_DynCtc.h"
#include "tubex_TFnc.h"
#include "tubex_Slice.h"

namespace tubex
{
  /**
   * \brief CtcPicard class.
   *
   * \todo Restricted tdomain feature?
   */
  class CtcPicard : public DynCtc
  {
    public:

      CtcPicard(ibex::Function& f, float delta = 1.1);
      CtcPicard(TFnc& f, float delta = 1.1);
      ~CtcPicard();

      void contract(std::vector<Domain*>& v_domains);
      void contract(Tube& x, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(TubeVector& x, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      int picard_iterations() const;

    protected:

      void contract_kth_slices(TubeVector& x, int k, TimePropag t_propa);
      void guess_kth_slices_envelope(TubeVector& x, int k, TimePropag t_propa);

      const TFunction* m_f_ptr = NULL;
      const TFnc& m_f;
      const float m_delta;
      int m_picard_iterations = 0;

      static const std::string m_ctc_name; //!< class name (mainly used for CN Exceptions)
      static std::vector<std::string> m_str_expected_doms; //!< allowed domains signatures (mainly used for CN Exceptions)
      friend class ContractorNetwork;
  };
}

#endif