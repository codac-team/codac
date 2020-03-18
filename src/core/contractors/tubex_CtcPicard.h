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

#include "tubex_Ctc.h"
#include "tubex_Fnc.h"
#include "tubex_Slice.h"

namespace tubex
{
  /**
   * \brief CtcPicard class.
   *
   * \todo Restricted domain feature?
   */
  class CtcPicard : public Ctc
  {
    public:

      CtcPicard(float delta = 1.1);

      void contract(std::vector<DomainParams>& v_domains);
      
      void contract(const tubex::Fnc& f,
                    Tube& x,
                    TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(const tubex::Fnc& f,
                    TubeVector& x,
                    TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      int picard_iterations() const;

    protected:

      void contract_kth_slices(const tubex::Fnc& f,
                               TubeVector& tube,
                               int k,
                               TimePropag t_propa);
      void guess_kth_slices_envelope(const Fnc& f,
                               TubeVector& tube,
                               int k,
                               TimePropag t_propa);

      float m_delta;
      int m_picard_iterations = 0;
  };
}

#endif