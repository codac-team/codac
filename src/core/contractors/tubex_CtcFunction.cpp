/** 
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcFunction.h"
#include "tubex_Domain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcFunction::CtcFunction(const char* x1, const char* f)
    : Ctc(false), m_ibex_fnc(new ibex::Function(x1, CtcFunction::parse_f(f).c_str())), m_ibex_ctc(new ibex::CtcFwdBwd(*m_ibex_fnc))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* f)
    : Ctc(false), m_ibex_fnc(new ibex::Function(x1, x2, CtcFunction::parse_f(f).c_str())), m_ibex_ctc(new ibex::CtcFwdBwd(*m_ibex_fnc))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* f)
    : Ctc(false), m_ibex_fnc(new ibex::Function(x1, x2, x3, CtcFunction::parse_f(f).c_str())), m_ibex_ctc(new ibex::CtcFwdBwd(*m_ibex_fnc))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* f)
    : Ctc(false), m_ibex_fnc(new ibex::Function(x1, x2, x3, x4, CtcFunction::parse_f(f).c_str())), m_ibex_ctc(new ibex::CtcFwdBwd(*m_ibex_fnc))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* f)
    : Ctc(false), m_ibex_fnc(new ibex::Function(x1, x2, x3, x4, x5, CtcFunction::parse_f(f).c_str())), m_ibex_ctc(new ibex::CtcFwdBwd(*m_ibex_fnc))
  {

  }

  CtcFunction::CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* f)
    : Ctc(false), m_ibex_fnc(new ibex::Function(x1, x2, x3, x4, x5, x6, CtcFunction::parse_f(f).c_str())), m_ibex_ctc(new ibex::CtcFwdBwd(*m_ibex_fnc))
  {

  }

  CtcFunction::~CtcFunction()
  {
    delete m_ibex_fnc;
    delete m_ibex_ctc;
  }
  
  void CtcFunction::contract(vector<Domain*>& v_domains)
  {
    assert(!v_domains.empty());
    // todo: check that the constraint is not temporal/intertemporal

    // Dynamical case (tubes/tubevectors)
    if(v_domains[0]->type() == Domain::Type::SLICE)
    {
      for(size_t i = 0 ; i < v_domains.size() ; i++)
      {
        assert(v_domains[i]->type() == Domain::Type::SLICE);
        if(i != 0)
          assert(v_domains[i]->slice().tdomain() == v_domains[i-1]->slice().tdomain());
      }

      // If these slices should not be impacted by the contractor
      if(!v_domains[0]->slice().tdomain().intersects(m_restricted_tdomain))
        return;

      int n = v_domains.size();

      IntervalVector envelope(n);
      IntervalVector ingate(n);
      IntervalVector outgate(n);

      for(int i = 0 ; i < n ; i++)
      {
        envelope[i] = v_domains[i]->slice().codomain();
        ingate[i] = v_domains[i]->slice().input_gate();
        outgate[i] = v_domains[i]->slice().output_gate();
      }

      m_ibex_ctc->contract(envelope);
      m_ibex_ctc->contract(ingate);
      m_ibex_ctc->contract(outgate);

      for(int i = 0 ; i < n ; i++)
      {
        v_domains[i]->slice().set_envelope(envelope[i]);
        v_domains[i]->slice().set_input_gate(ingate[i]);
        v_domains[i]->slice().set_output_gate(outgate[i]);
      }
    }

    // Static case (intervals/boxes)
    else if(v_domains[0]->type() == Domain::Type::INTERVAL || v_domains[0]->type() == Domain::Type::INTERVAL_VECTOR)
    {
      // Two implicit ways of using this contractor:
      // 1. with one constraint applied on n scalar variables over m dimensions
      // 2. with one constraint applied only once on a set of n variables (possibly heterogeneous)

      int n = m_ibex_fnc->nb_var();

      int p = 0;
      for(const auto& dom : v_domains)
      {
        switch(dom->type())
        {
          case Domain::Type::INTERVAL:
            p++;
            break;

          case Domain::Type::INTERVAL_VECTOR:
            p += dom->interval_vector().size();
            break;

          default:
            assert(false && "invalid static domain (not an interval or a box)");
        }
      }

      if(p == n) // (case 2) or (case 1 with m=1)
      {
        IntervalVector box(n);
        
        int i = 0;
        for(const auto& dom : v_domains)
        {
          switch(dom->type())
          {
            case Domain::Type::INTERVAL:
              box[i] = dom->interval();
              i++;
              break;

            case Domain::Type::INTERVAL_VECTOR:
              box.put(i, dom->interval_vector());
              i += dom->interval_vector().size();
              break;

            default:
              assert(false && "invalid static domain (not an interval or a box)");
          }
        }

        m_ibex_ctc->contract(box);

        i = 0;
        for(auto& dom : v_domains)
        {
          switch(dom->type())
          {
            case Domain::Type::INTERVAL:
            {
              dom->interval() = box[i];
              i++;
            }
            break;

            case Domain::Type::INTERVAL_VECTOR:
            {
              int n_ = dom->interval_vector().size();
              for(int j = 0 ; j < n_ ; j++)
                dom->interval_vector()[j] = box[i+j];
              i += n_;
            }
            break;

            default:
              assert(false && "invalid static domain (not an interval or a box)");
          }
        }
      }

      else if(p>n && p%n == 0) // case 1 with m!=1
      {
        int m = p/n;
        for(int i = 0 ; i < m ; i++)
        {
          IntervalVector box(n);

          for(size_t j = 0 ; j < v_domains.size() ; j++)
          {
            assert(v_domains[j]->type() == Domain::Type::INTERVAL_VECTOR);
            assert(v_domains[j]->interval_vector().size() == m);

            box[j] = v_domains[j]->interval_vector()[i];
          }

          m_ibex_ctc->contract(box);

          for(size_t j = 0 ; j < v_domains.size() ; j++)
            v_domains[j]->interval_vector()[i] = box[j];
        }
      }

      else
      {
        cout << "Input dim: " << p << ", expected dim: " << n << endl;
        assert(false && "unhandled case, probably a wrong dimension of inputs");
      }
    }

    else
    {
      assert(false && "unhandled case (wrong domain type)");
    }
  }

  const string CtcFunction::parse_f(const char* f)
  {
    string str_f(f);
    string str_eq("=");

    if(str_f.find(str_eq) != string::npos)
    {
      str_f.replace(str_f.find(str_eq), str_eq.length(), "-(");
      str_f += ")";
    }

    assert(str_f.find(str_eq) == string::npos && "only one '=' allowed");
    assert(str_f.find(">") == string::npos
        && str_f.find("<") == string::npos
        && "inequalities not supported by this contractor");

    return str_f;
  }
}