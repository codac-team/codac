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
#include "tubex_CtcEval.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcDistance.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  int Contractor::ctc_counter = 0;

  Contractor::Contractor(Type type, const vector<Domain*>& v_domains)
    : m_type(type), m_v_domains(v_domains)
  {
    assert(!v_domains.empty());
    assert(v_domains.size() != 8);

    ctc_counter++;
    m_ctc_id = ctc_counter;
  }

  Contractor::Contractor(ibex::Ctc& ctc, const vector<Domain*>& v_domains)
    : Contractor(Type::IBEX, v_domains)
  {
    assert(!v_domains.empty());
    assert(v_domains.size() != 8);

    m_ibex_ctc = reference_wrapper<ibex::Ctc>(ctc);
  }

  Contractor::Contractor(tubex::Ctc& ctc, const vector<Domain*>& v_domains) 
    : Contractor(Type::TUBEX, v_domains)
  {
    assert(!v_domains.empty());
    assert(v_domains.size() != 8);

    m_tubex_ctc = reference_wrapper<tubex::Ctc>(ctc);
    m_tubex_ctc.get().preserve_slicing(true);
  }

  Contractor::Contractor(const Contractor& ac)
    : Contractor(ac.m_type, ac.m_v_domains)
  {
    assert(!ac.m_v_domains.empty());
    assert(ac.m_v_domains.size() != 8);

    m_name = ac.m_name;
    m_ctc_id = ac.m_ctc_id;

    switch(ac.m_type)
    {
      case Type::EQUALITY:
      case Type::COMPONENT:
        // Nothing to do
        break;
        
      case Type::IBEX:
        m_ibex_ctc = reference_wrapper<ibex::Ctc>(ac.m_ibex_ctc);
        break;

      case Type::TUBEX:
        m_tubex_ctc = reference_wrapper<tubex::Ctc>(ac.m_tubex_ctc);
        break;

      default:
        assert(false && "unhandled case");
    }
  }
  
  Contractor::~Contractor()
  {

  }

  int Contractor::id() const
  {
    return m_ctc_id;
  }

  Contractor::Type Contractor::type() const
  {
    return m_type;
  }

  ibex::Ctc& Contractor::ibex_ctc()
  {
    assert(m_type == Type::IBEX);
    return m_ibex_ctc.get();
  }

  tubex::Ctc& Contractor::tubex_ctc()
  {
    assert(m_type == Type::TUBEX);
    return m_tubex_ctc.get();
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
    return const_cast<vector<Domain*>&>(static_cast<const Contractor&>(*this).domains());
  }

  const vector<Domain*>& Contractor::domains() const
  {
    return m_v_domains;
  }

  bool Contractor::operator==(const Contractor& x) const
  {
    assert(!m_v_domains.empty() && !x.m_v_domains.empty());

    if(m_type != x.m_type)
      return false;

    switch(m_type)
    {
      case Type::IBEX:
        if(&m_ibex_ctc.get() != &x.m_ibex_ctc.get())
          return false;

        if(typeid(m_ibex_ctc.get()) != typeid(x.m_ibex_ctc.get()))
          return false;

      case Type::TUBEX:
        if(typeid(m_tubex_ctc.get()) != typeid(x.m_tubex_ctc.get()))
          return false;

        if(&m_tubex_ctc.get() != &x.m_tubex_ctc.get() &&
          (typeid(m_tubex_ctc.get()) != typeid(CtcEval) && 
           typeid(m_tubex_ctc.get()) != typeid(CtcDeriv) && 
           typeid(m_tubex_ctc.get()) != typeid(CtcDistance)))
          return false;

      case Type::COMPONENT:
      case Type::EQUALITY:
        // Nothing to compare
        break;

      default:
        assert(false && "unhandled case");
        return false;
    }
    
    if(m_v_domains.size() != x.m_v_domains.size())
      return false;

    for(size_t i = 0 ; i < m_v_domains.size() ; i++)
    {
      bool found = false;
      for(size_t j = 0 ; j < x.m_v_domains.size() ; j++)
        if(*m_v_domains[i] == *x.m_v_domains[j])
        {
          found = true;
          break;
        }

      if(!found)
        return false;
    }

    return true;
  }

  void Contractor::contract()
  {
    assert(!m_v_domains.empty());

    if(m_type == Type::IBEX)
    {
      if(m_v_domains.size() == 1 && m_v_domains[0]->type() == Domain::Type::INTERVAL_VECTOR)
      {
        m_ibex_ctc.get().contract(m_v_domains[0]->interval_vector());
      }

      else if(m_v_domains[0]->type() == Domain::Type::INTERVAL) // set of scalar values
      {
        IntervalVector box(m_v_domains.size());
        for(size_t i = 0 ; i < m_v_domains.size() ; i++)
          box[i] = m_v_domains[i]->interval();

        m_ibex_ctc.get().contract(box);

        for(size_t i = 0 ; i < m_v_domains.size() ; i++)
          m_v_domains[i]->interval() &= box[i];
      }

      else if(m_v_domains[0]->type() == Domain::Type::INTERVAL_VECTOR) // set of vector values
      {
        for(int k = 0 ; k < m_v_domains[0]->interval_vector().size() ; k++)
        {
          IntervalVector box(m_v_domains.size());
          for(size_t i = 0 ; i < m_v_domains.size() ; i++)
            box[i] = m_v_domains[i]->interval_vector()[k];

          m_ibex_ctc.get().contract(box);

          for(size_t i = 0 ; i < m_v_domains.size() ; i++)
            m_v_domains[i]->interval_vector()[k] &= box[i];
        }
      }
    }

    else if(m_type == Type::TUBEX)
    {
      m_tubex_ctc.get().contract(m_v_domains);
    }

    else if(m_type == Type::COMPONENT)
    {
      // Symbolic
    }

    else if(m_type == Type::EQUALITY)
    {
      assert(m_v_domains.size() == 2);
      assert(m_v_domains[0]->type() == m_v_domains[1]->type());

      switch(m_v_domains[0]->type())
      {
        case Domain::Type::INTERVAL:
        {
          Interval inter = m_v_domains[0]->interval() & m_v_domains[1]->interval();
          m_v_domains[0]->interval() = inter;
          m_v_domains[1]->interval() = inter;
        }
        break;
      
        case Domain::Type::INTERVAL_VECTOR:
        {
          IntervalVector inter = m_v_domains[0]->interval_vector() & m_v_domains[1]->interval_vector();
          m_v_domains[0]->interval_vector() = inter;
          m_v_domains[1]->interval_vector() = inter;
        }
        break;
    
        case Domain::Type::TUBE:
        {
          Tube inter = m_v_domains[0]->tube() & m_v_domains[1]->tube();
          m_v_domains[0]->tube() = inter;
          m_v_domains[1]->tube() = inter;
        }
        break;
        
        case Domain::Type::TUBE_VECTOR:
        {
          TubeVector inter = m_v_domains[0]->tube_vector() & m_v_domains[1]->tube_vector();
          m_v_domains[0]->tube_vector() = inter;
          m_v_domains[1]->tube_vector() = inter;
        }
        break;

        default:
          assert(false && "unhandled case");
      }
    }

    else
      assert(false && "unhandled case");
  }
  
  const string Contractor::name() const
  {
    switch(type())
    {
      case Type::COMPONENT:
        return "";

      case Type::EQUALITY:
        return "=";

      case Type::TUBEX:
        if(m_name.empty())
        {
          if(typeid(m_tubex_ctc.get()) == typeid(CtcEval))
            return "\\mathcal{C}_{\\textrm{eval}}";

          if(typeid(m_tubex_ctc.get()) == typeid(CtcDeriv))
            return "\\mathcal{C}_{\\frac{d}{dt}}";
        }
        return "\\mathcal{C}_{" + m_name + "}";

      case Type::IBEX:
      default:
        return "\\mathcal{C}_{" + m_name + "}";
    }
  }
  
  void Contractor::set_name(const string& name)
  {
    m_name = name;
  }
  
  ostream& operator<<(ostream& str, const Contractor& x)
  {
    str << "Contractor " << x.name() << " (" << x.m_v_domains.size() << " doms)" << flush;
    return str;
  }
}