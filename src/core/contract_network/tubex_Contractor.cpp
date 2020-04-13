/** 
 *  Contractor class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Contractor.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Contractor::Contractor(ContractorType type)
    : m_type(type)
  {
    assert(type == ContractorType::COMPONENT || type == ContractorType::EQUALITY);
  }

  Contractor::Contractor(ibex::Ctc& ctc)
    : m_type(ContractorType::IBEX), m_ibex_ctc(ctc)
  {

  }

  Contractor::Contractor(tubex::Ctc& ctc) 
    : m_type(ContractorType::TUBEX), m_tubex_ctc(ctc)
  {
    m_tubex_ctc.get().preserve_slicing(true);
  }

  Contractor::Contractor(const Contractor& ac)
    : m_type(ac.m_type), m_domains(ac.m_domains)
  {
    switch(ac.m_type)
    {
      case ContractorType::IBEX:
        m_ibex_ctc = ac.m_ibex_ctc;
        break;

      case ContractorType::TUBEX:
        m_tubex_ctc = ac.m_tubex_ctc;
        break;

      default:
        assert(false && "unhandled case");
    }
  }
  
  Contractor::~Contractor()
  {

  }

  ContractorType Contractor::type() const
  {
    return m_type;
  }

  bool Contractor::is_active() const
  {
    return m_active;
  }

  void Contractor::set_active(bool active)
  {
    m_active = active;
  }

  vector<Domain*>& Contractor::domains()
  {
    return m_domains;
  }

  bool Contractor::operator==(const Contractor& x) const
  {
    if(m_type != x.m_type)
      return false;

    switch(m_type)
    {
      case ContractorType::IBEX:
        if(&m_ibex_ctc.get() != &x.m_ibex_ctc.get())
          return false;

      case ContractorType::TUBEX:
        if(&m_tubex_ctc.get() != &x.m_tubex_ctc.get())
          return false;

      case ContractorType::COMPONENT:
      case ContractorType::EQUALITY:
        // Nothing to compare
        break;

      default:
        assert(false && "unhandled case");
        return false;
    }

    if(m_domains.size() != x.m_domains.size())
      return false;

    for(size_t i = 0 ; i < m_domains.size() ; i++)
      if(*m_domains[i] != *x.m_domains[i])
        return false;

    return true;
  }

  bool Contractor::comes_from(const Contractor& x) const
  {
    if(m_type != x.m_type)
      return false;

    switch(m_type)
    {
      case ContractorType::IBEX:
        return &m_ibex_ctc.get() == &x.m_ibex_ctc.get();

      case ContractorType::TUBEX:
        return &m_tubex_ctc.get() == &x.m_tubex_ctc.get();
          return false;

      default:
        return false;
    }
  }

  void Contractor::contract()
  {
    assert(!m_domains.empty());

    if(m_type == ContractorType::IBEX)
    {
      if(m_domains.size() == 1 && m_domains[0]->type() == Domain::Type::INTERVAL_VECTOR)
      {
        m_ibex_ctc.get().contract(m_domains[0]->interval_vector());
      }

      else if(m_domains[0]->type() == Domain::Type::INTERVAL) // set of scalar values
      {
        IntervalVector box(m_domains.size());
        for(size_t i = 0 ; i < m_domains.size() ; i++)
          box[i] = m_domains[i]->interval();

        m_ibex_ctc.get().contract(box);

        for(size_t i = 0 ; i < m_domains.size() ; i++)
          m_domains[i]->interval() &= box[i];
      }

      else if(m_domains[0]->type() == Domain::Type::INTERVAL_VECTOR) // set of vector values
      {
        for(int k = 0 ; k < m_domains[0]->interval_vector().size() ; k++)
        {
          IntervalVector box(m_domains.size());
          for(size_t i = 0 ; i < m_domains.size() ; i++)
            box[i] = m_domains[i]->interval_vector()[k];

          m_ibex_ctc.get().contract(box);

          for(size_t i = 0 ; i < m_domains.size() ; i++)
            m_domains[i]->interval_vector()[k] &= box[i];
        }
      }
    }

    else if(m_type == ContractorType::TUBEX)
    {
      m_tubex_ctc.get().contract(m_domains);
    }

    else if(m_type == ContractorType::COMPONENT)
    {
      // Symbolic
    }

    else if(m_type == ContractorType::EQUALITY)
    {
      assert(m_domains.size() == 2);
      assert(m_domains[0]->type() == m_domains[1]->type());

      switch(m_domains[0]->type())
      {
        case Domain::Type::INTERVAL:
        {
          Interval inter = m_domains[0]->interval() & m_domains[1]->interval();
          m_domains[0]->interval() = inter;
          m_domains[1]->interval() = inter;
        }
        break;
      
        case Domain::Type::INTERVAL_VECTOR:
        {
          IntervalVector inter = m_domains[0]->interval_vector() & m_domains[1]->interval_vector();
          m_domains[0]->interval_vector() = inter;
          m_domains[1]->interval_vector() = inter;
        }
        break;
    
        case Domain::Type::TUBE:
        {
          Tube inter = m_domains[0]->tube() & m_domains[1]->tube();
          m_domains[0]->tube() = inter;
          m_domains[1]->tube() = inter;
        }
        break;
        
        case Domain::Type::TUBE_VECTOR:
        {
          TubeVector inter = m_domains[0]->tube_vector() & m_domains[1]->tube_vector();
          m_domains[0]->tube_vector() = inter;
          m_domains[1]->tube_vector() = inter;
        }
        break;

        default:
          assert(false && "unhandled case");
      }
    }

    else
      assert(false && "unhandled case");
  }
  
  const string Contractor::name()
  {
    if(m_name.empty())
    {
      switch(type())
      {
        case ContractorType::COMPONENT:
          return "i";

        case ContractorType::EQUALITY:
          return "=";

        case ContractorType::IBEX:
        case ContractorType::TUBEX:
        default:
          return "?";
      }
    }

    return m_name;
  }
  
  void Contractor::set_name(const string& name)
  {
    m_name = name;
  }
}