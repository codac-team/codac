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
      void contract(const tubex::Fnc& f,
                    Tube& x,
                    TPropagation t_propa = FORWARD | BACKWARD);
      void contract(const tubex::Fnc& f,
                    TubeVector& x,
                    TPropagation t_propa = FORWARD | BACKWARD);
      int picard_iterations() const;

      /* the Picard algorithm for the kth slices of the tubeVector tubr */
      void contract_picard_slice(const tubex::Fnc& f,
                               TubeVector& tube,
                               int k,
			       TPropagation t_propa);
      void  set_picard_subslices (int nsubslices);

    protected:
      void contract_picard_tubeslice(const tubex::Fnc& f,
                               TubeVector& tube,
                               int & k,
			       TPropagation t_propa);
      
      void contract_kth_slices(const tubex::Fnc& f,
                               TubeVector& tube,
                               int k,
                               TPropagation t_propa);
      void guess_kth_slices_envelope(const Fnc& f,
                                     TubeVector& tube,
                                     int k,
                                     TPropagation t_propa);

      float m_delta;
      int m_picard_iterations = 0;
      int m_picard_subslices=5000;
      double m_picard_minimum_domainsize=1.e-20;
  };
}

#endif
