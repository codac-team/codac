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
#include "tubex_CtcEval.h"
#include "tubex_CtcDeriv.h" // todo: remove this
#include "ibex_CtcEmpty.h" // todo: remove this

using namespace std;
using namespace ibex;

namespace tubex
{
  AbstractContractor::AbstractContractor()
    : m_type(ContractorType::NONE), m_ibex_ctc(*new CtcEmpty(1)), m_tubex_ctc(*new CtcDeriv)
  {

  }

  AbstractContractor::AbstractContractor(const AbstractContractor& ac)
    : m_type(ac.m_type), m_ibex_ctc(ac.m_ibex_ctc), m_tubex_ctc(ac.m_tubex_ctc), m_domains(ac.m_domains)
  {

  }

  AbstractContractor::AbstractContractor(ibex::Ctc& ctc)
    : m_type(ContractorType::IBEX), m_ibex_ctc(ctc), m_tubex_ctc(*new CtcDeriv)
  {

  }

  AbstractContractor::AbstractContractor(tubex::Ctc& ctc) 
    : m_type(ContractorType::TUBEX), m_ibex_ctc(*new CtcEmpty(1)), m_tubex_ctc(ctc)
  {
    if(typeid(ctc) == typeid(CtcEval))
    {
      static_cast<CtcEval&>(ctc).enable_temporal_propagation(false);
      // todo: automatically add a CtcDeriv to the network
    }
  }

  AbstractContractor::~AbstractContractor()
  {

  }

  ContractorType AbstractContractor::type() const
  {
    return m_type;
  }

  bool AbstractContractor::operator==(const AbstractContractor& x) const
  {
    if(m_type != x.m_type || m_domains.size() != x.m_domains.size())
      return false;

    for(size_t i = 0 ; i < m_domains.size() ; i++)
      if(*m_domains[i] != *x.m_domains[i])
        return false;

    return true;
  }

  void AbstractContractor::contract()
  {
    assert(!m_domains.empty());

    if(m_type == ContractorType::IBEX)
    {
      if(m_domains.size() == 1 && m_domains[0]->type() == DomainType::INTERVAL_VECTOR)
      {
        m_ibex_ctc.contract(m_domains[0]->interval_vector());
      }

      else if(m_domains[0]->type() == DomainType::INTERVAL) // set of scalar values
      {
        IntervalVector box(m_domains.size());
        for(size_t i = 0 ; i < m_domains.size() ; i++)
          box[i] = m_domains[i]->interval();

        m_ibex_ctc.contract(box);

        for(size_t i = 0 ; i < m_domains.size() ; i++)
          m_domains[i]->interval() &= box[i];
      }

      else if(m_domains[0]->type() == DomainType::INTERVAL_VECTOR) // set of vector values
      {
        for(int k = 0 ; k < m_domains[0]->interval_vector().size() ; k++)
        {
          IntervalVector box(m_domains.size());
          for(size_t i = 0 ; i < m_domains.size() ; i++)
            box[i] = m_domains[i]->interval_vector()[k];

          m_ibex_ctc.contract(box);

          for(size_t i = 0 ; i < m_domains.size() ; i++)
            m_domains[i]->interval_vector()[k] &= box[i];
        }
      }
    }

    else if(m_type == ContractorType::TUBEX)
    {
      m_tubex_ctc.contract(m_domains);
    }

    else if(m_type == ContractorType::NONE)
    {
      // Symbolic
    }

    else
      assert(false && "unhandled case");
  }
}