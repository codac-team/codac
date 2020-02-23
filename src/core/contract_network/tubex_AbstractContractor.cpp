/** 
 *  AbstractContractor class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_AbstractContractor.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  AbstractContractor::AbstractContractor(ibex::Ctc *ctc) : m_ibex_ctc(ctc)
  {

  }

  AbstractContractor::AbstractContractor(tubex::Ctc *ctc) : m_tubex_ctc(ctc)
  {
    
  }

  void AbstractContractor::contract()
  {
    if(m_ibex_ctc != NULL)
    {
      if(v_domains.size() == 1 && v_domains[0].m_iv != NULL)
        m_ibex_ctc->contract(*v_domains[0].m_iv);

      else if(v_domains[0].m_i != NULL) // set of scalar values
      {
        IntervalVector box(v_domains.size());
        for(int i = 0 ; i < v_domains.size() ; i++)
          box[i] = *v_domains[i].m_i;

          m_ibex_ctc->contract(box);

        for(int i = 0 ; i < v_domains.size() ; i++)
          *v_domains[i].m_i &= box[i];
      }

      else if(v_domains[0].m_iv != NULL) // set of vector values
      {
        for(int k = 0 ; k < v_domains[0].m_iv->size() ; k++)
        {
          IntervalVector box(v_domains.size());
          for(int i = 0 ; i < v_domains.size() ; i++)
            box[i] = (*v_domains[i].m_iv)[k];

            m_ibex_ctc->contract(box);

          for(int i = 0 ; i < v_domains.size() ; i++)
            (*v_domains[i].m_iv)[k] &= box[i];
        }
      }
    }

    else if(m_tubex_ctc != NULL)
    {
      m_tubex_ctc->contract(v_domains);
    }

    else
      assert(false && "unhandled case");
  }

  double AbstractContractor::domains_volume() const
  {
    double volume = 0.;
    for(int i = 0 ; i < v_domains.size() ; i++)
      volume += v_domains[i].volume();
    return volume;
  }
}