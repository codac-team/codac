/** 
 *  ContractorNetwork class : solver
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <time.h>
#include "codac_ContractorNetwork.h"
#include "codac_Exception.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Public methods

    // Contraction process

    double ContractorNetwork::contract(bool verbose)
    {
      clock_t t_start = clock();

      if(verbose)
      {
        cout << "Contractor network has " << m_map_ctc.size()
             << " contractors and " << m_map_domains.size() << " domains" << endl;
        cout << "Computing, " << nb_ctc_in_stack() << " contractors currently in stack";
        if(!std::isinf(m_contraction_duration_max))
          cout << " during " << m_contraction_duration_max << "s";
        cout << endl;
      }

      while(!m_deque.empty()
        && (double)(clock() - t_start)/CLOCKS_PER_SEC < m_contraction_duration_max)
      {
        Contractor *ctc = m_deque.front();
        m_deque.pop_front();

        ctc->contract();
        ctc->set_active(false);

        for(auto& ctc_dom : ctc->domains()) // for each domain related to this contractor
        {
          // If the domain has "changed" after the contraction
          trigger_ctc_related_to_dom(ctc_dom, ctc);
        }
      }

      if(verbose)
        cout << "  Constraint propagation time: " << (double)(clock() - t_start)/CLOCKS_PER_SEC << "s" << endl;

      // Emptiness test
      // todo: test only contracted domains?
      if(verbose)
        for(const auto& dom : m_map_domains)
          if(dom.second->is_empty())
          {
            cout << "  Warning: empty set" << endl;
            break;
          }

      return (double)(clock() - t_start)/CLOCKS_PER_SEC;
    }

    double ContractorNetwork::contract_during(double dt, bool verbose)
    {
      double prev_dt = m_contraction_duration_max;
      m_contraction_duration_max = dt;
      double contraction_time = contract(verbose);
      m_contraction_duration_max = prev_dt;
      return contraction_time;
    }

    void ContractorNetwork::set_fixedpoint_ratio(float r)
    {
      assert(Interval(0.,1).contains(r) && "invalid ratio");
      m_fixedpoint_ratio = r;
    }

    void ContractorNetwork::trigger_all_contractors()
    {
      m_deque.clear();

      for(const auto& ctc : m_map_ctc)
      {
        ctc.second->set_active(true);
        add_ctc_to_queue(ctc.second, m_deque);
      }
    }

    int ContractorNetwork::nb_ctc_in_stack() const
    {
      return m_deque.size();
    }

  // Protected methods

    void ContractorNetwork::add_ctc_to_queue(Contractor *ac, deque<Contractor*>& ctc_deque)
    {
      // todo: propagate for EQUALITY contractors even in case of poor contractions?

      if(ac->type() == Contractor::Type::T_COMPONENT)
        ctc_deque.push_back(ac);

      else
        ctc_deque.push_front(ac); // priority
    }

    void ContractorNetwork::trigger_ctc_related_to_dom(Domain *dom, Contractor *ctc_to_avoid)
    {
      double current_volume = dom->compute_volume(); // new volume after contraction

      if(current_volume/dom->get_saved_volume() < 1.-m_fixedpoint_ratio)
      {
        // We activate each contractor related to these domains, according to graph orientation

        // Local deque, for specific order related to this domain
        deque<Contractor*> ctc_deque;

        for(auto& ctc_of_dom : dom->contractors()) 
          if(ctc_of_dom != ctc_to_avoid && !ctc_of_dom->is_active())
          {
            ctc_of_dom->set_active(true);
            add_ctc_to_queue(ctc_of_dom, ctc_deque);
          }

        // Merging this local deque in the CN one
        for(auto& c : ctc_deque)
          m_deque.push_front(c);
      }
      
      dom->set_volume(current_volume); // updating old volume
    }
}