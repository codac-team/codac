/** 
 *  Contractor class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Contractor.h"
#include "codac_CtcEval.h"
#include "codac_CtcDeriv.h"
#include "codac_CtcDist.h"
#include "codac_Exception.h"
#include "codac_ContractorNetwork.h"

using namespace std;
using namespace ibex;

namespace codac
{
  int Contractor::ctc_counter = 0;

  Contractor::Contractor(Type type, const vector<Domain*>& v_domains)
    : m_type(type), m_v_domains(v_domains)
  {
    ctc_counter++;
    m_ctc_id = ctc_counter;
  }

  Contractor::Contractor(Ctc& ctc, const vector<Domain*>& v_domains)
    : Contractor(Type::T_IBEX, v_domains)
  {
    assert(!v_domains.empty());
    m_static_ctc = reference_wrapper<Ctc>(ctc);
  }

  Contractor::Contractor(DynCtc& ctc, const vector<Domain*>& v_domains) 
    : Contractor(Type::T_CODAC, v_domains)
  {
    assert(!v_domains.empty());
    m_dyn_ctc = reference_wrapper<DynCtc>(ctc);
    m_dyn_ctc.get().preserve_slicing(true);
  }

  Contractor::Contractor(ContractorNetwork& cn)
    : Contractor(Type::T_CN, vector<Domain*>())
  {
    m_cn_ctc = reference_wrapper<ContractorNetwork>(cn);
  }

  Contractor::Contractor(const Contractor& ac)
    : Contractor(ac.m_type, ac.m_v_domains)
  {
    m_name = ac.m_name;
    m_ctc_id = ac.m_ctc_id;

    switch(ac.m_type)
    {
      case Type::T_EQUALITY:
      case Type::T_COMPONENT:
        // Nothing to do
        break;
        
      case Type::T_IBEX:
        m_static_ctc = reference_wrapper<Ctc>(ac.m_static_ctc);
        break;

      case Type::T_CODAC:
        m_dyn_ctc = reference_wrapper<DynCtc>(ac.m_dyn_ctc);
        break;

      case Type::T_CN:
        m_cn_ctc = reference_wrapper<ContractorNetwork>(ac.m_cn_ctc);
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

  Ctc& Contractor::ibex_ctc()
  {
    assert(m_type == Type::T_IBEX);
    return m_static_ctc.get();
  }

  DynCtc& Contractor::codac_ctc()
  {
    assert(m_type == Type::T_CODAC);
    return m_dyn_ctc.get();
  }

  ContractorNetwork& Contractor::cn_ctc()
  {
    assert(m_type == Type::T_CN);
    return m_cn_ctc.get();
  }

  bool Contractor::is_active() const
  {
    return m_active;
  }

  void Contractor::set_active(bool active)
  {
    m_active = active;
    if(m_type == Type::T_CN)
      m_cn_ctc.get().trigger_all_contractors();
  }

  //vector<Domain*> Contractor::domains()
  //{
  //  return const_cast<vector<Domain*> >(static_cast<const Contractor&>(*this).domains());
  //}

  const vector<Domain*> Contractor::domains() const
  {
    if(m_type == Type::T_CN)
    {
      vector<Domain*> v_dom; // building a vector of domains from the CN map
      for(const auto& dom : m_cn_ctc.get().m_map_domains)
        v_dom.push_back(dom.second);
      return v_dom;
    }

    else
      return m_v_domains;
  }

  bool Contractor::operator==(const Contractor& x) const
  {
    assert(!m_v_domains.empty() && !x.m_v_domains.empty());

    if(m_type != x.m_type)
      return false;

    switch(m_type)
    {
      case Type::T_IBEX:
        if(&m_static_ctc.get() != &x.m_static_ctc.get())
          return false;

        if(typeid(m_static_ctc.get()) != typeid(x.m_static_ctc.get()))
          return false;

      case Type::T_CODAC:
        if(typeid(m_dyn_ctc.get()) != typeid(x.m_dyn_ctc.get()))
          return false;

        if(&m_dyn_ctc.get() != &x.m_dyn_ctc.get() &&
          (typeid(m_dyn_ctc.get()) != typeid(CtcEval) && 
           typeid(m_dyn_ctc.get()) != typeid(CtcDeriv) && 
           typeid(m_dyn_ctc.get()) != typeid(CtcDist)))
          return false;

      case Type::T_CN:
        if(typeid(m_cn_ctc.get()) != typeid(x.m_cn_ctc.get()))
          return false;

      case Type::T_COMPONENT:
      case Type::T_EQUALITY:
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
    assert(!m_v_domains.empty() || m_type == Type::T_CN);

    if(m_type == Type::T_IBEX)
    {
      // Data may be presented in two ways:
      // - all components in one vector box
      // - a list of heterogeneous components

      // Case: all components in one vector box
      if(m_v_domains.size() == 1 && m_v_domains[0]->type() == Domain::Type::T_INTERVAL_VECTOR)
      {
        m_static_ctc.get().contract(m_v_domains[0]->interval_vector());
      }

      // Case: list of heterogeneous components
      else
      {
        for(int j = 0 ; j < 3 ; j++) // to possibly deal with 3 subdomains of a Slice (gates + envelope)
        {
          bool at_least_one_slice = false;
          // if this ^ stays false, then the for loop will break after the first iteration

          // Building a temporary box for the contraction
          
            IntervalVector box(m_static_ctc.get().nb_var);

            int i = 0;
            for(auto& dom : m_v_domains)
            {
              switch(dom->type())
              {
                case Domain::Type::T_INTERVAL:
                  box[i] = dom->interval();
                  i++;
                  break;

                case Domain::Type::T_INTERVAL_VECTOR:
                  assert(false && "interval vectors should not be handled here");
                  box.put(i, dom->interval_vector());
                  i+=dom->interval_vector().size();
                  break;

                case Domain::Type::T_SLICE:
                  switch(j)
                  {
                    case 0: // we start from the envelope
                      box[i] = dom->slice().codomain();
                      break;
                    
                    // Then the gates
                    case 1:
                      box[i] = dom->slice().input_gate();
                      break;
                      
                    case 2:
                      box[i] = dom->slice().output_gate();
                      break;

                    default:
                      assert(false && "Slice domain already treated");
                  }
                  i++;
                  at_least_one_slice = true;
                  break;

                case Domain::Type::T_TUBE:
                case Domain::Type::T_TUBE_VECTOR:
                  assert(false && "dynamic domains should not be handled here");
                  break;

                default:
                  assert(false && "unhandled case");
              }
            }

            assert(i == m_static_ctc.get().nb_var);

          // Contracting

            m_static_ctc.get().contract(box);
            
          // Updating the domains (reverse operation)

            i = 0;
            for(auto& dom : m_v_domains)
            {
              switch(dom->type())
              {
                case Domain::Type::T_INTERVAL:
                {
                  dom->interval() = box[i];
                  i++;
                }
                break;

                case Domain::Type::T_INTERVAL_VECTOR:
                {
                  int vector_size = dom->interval_vector().size();
                  dom->interval_vector() = box.subvector(i, i+vector_size);
                  i+=vector_size;
                }
                break;

                case Domain::Type::T_SLICE:
                {
                  switch(j)
                  {
                    case 0:
                      dom->slice().set_envelope(box[i]);
                      break;

                    case 1:
                      dom->slice().set_input_gate(box[i]);
                      break;

                    case 2:
                      dom->slice().set_output_gate(box[i]);
                      break;

                    default:
                      assert(false && "Slice domain already treated");
                  }
                  i++;
                }
                break;

                default:
                  assert(false && "unhandled case");
              }
            }

            assert(i == m_static_ctc.get().nb_var);

          if(!at_least_one_slice)
            break;
        }
      }
    }

    else if(m_type == Type::T_CODAC)
    {
      m_dyn_ctc.get().contract(m_v_domains);
    }

    else if(m_type == Type::T_CN)
    {
      m_cn_ctc.get().contract();
    }

    else if(m_type == Type::T_COMPONENT)
    {
      // Symbolic
    }

    else if(m_type == Type::T_EQUALITY)
    {
      assert(m_v_domains.size() == 2);
      assert(m_v_domains[0]->type() == m_v_domains[1]->type());

      switch(m_v_domains[0]->type())
      {
        case Domain::Type::T_INTERVAL:
        {
          Interval inter = m_v_domains[0]->interval() & m_v_domains[1]->interval();
          m_v_domains[0]->interval() = inter;
          m_v_domains[1]->interval() = inter;
        }
        break;
      
        case Domain::Type::T_INTERVAL_VECTOR:
        {
          IntervalVector inter = m_v_domains[0]->interval_vector() & m_v_domains[1]->interval_vector();
          m_v_domains[0]->interval_vector() = inter;
          m_v_domains[1]->interval_vector() = inter;
        }
        break;
    
        case Domain::Type::T_TUBE:
        {
          Tube inter = m_v_domains[0]->tube() & m_v_domains[1]->tube();
          m_v_domains[0]->tube() = inter;
          m_v_domains[1]->tube() = inter;
        }
        break;
        
        case Domain::Type::T_TUBE_VECTOR:
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
      case Type::T_COMPONENT:
        return "";

      case Type::T_EQUALITY:
        return "=";

      case Type::T_CODAC:
        if(m_name.empty())
        {
          if(typeid(m_dyn_ctc.get()) == typeid(CtcEval))
            return "\\mathcal{C}_{\\textrm{eval}}";

          if(typeid(m_dyn_ctc.get()) == typeid(CtcDeriv))
            return "\\mathcal{C}_{\\frac{d}{dt}}";
        }
        return "\\mathcal{C}_{" + m_name + "}";

      case Type::T_CN:
        return "CN";

      case Type::T_IBEX:
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