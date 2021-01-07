/** 
 *  ContractorNetwork class : solver
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <time.h>
#include "tubex_ContractorNetwork.h"
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Contraction process

    double ContractorNetwork::contract(bool verbose)
    {
      clock_t t_start = clock();
      for(auto& dom : m_map_domains)
        dom.second->set_volume(dom.second->compute_volume());

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
          // If the domain has "changed" after the contraction
          trigger_ctc_related_to_dom(ctc_dom, ctc);
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

    double ContractorNetwork::contract_ordered_mode(bool verbose)
    {
      // todo: reset all saved domains' volumes
      clock_t t_start = clock();

      if(verbose)
      {
        cout << "Contractor network has " << m_map_ctc.size()
             << " contractors and " << m_map_domains.size() << " domains" << endl;
        cout << "Computing in ordered mode, " << nb_ctc_in_stack() << " contractors currently in stack";
        cout << endl;
      }

      map<DomainHashcode,Domain*> involved_domains;
      for(const auto& ctc : m_deque)
        for(const auto& dom : ctc->domains())
          involved_domains[DomainHashcode(*dom)] = dom;
      assert(!involved_domains.empty());

      bool fixed_point;
      m_iteration_nb = 0;

      do
      {
        m_iteration_nb++;

        // Forward: all contractors are called
        for(deque<Contractor*>::reverse_iterator it = m_deque.rbegin(); it != m_deque.rend(); ++it)
          (*it)->contract();

        // Volumes are computed before bwd
        for(auto& dom : involved_domains)
          dom.second->set_volume(dom.second->compute_volume());

        // Backward: all contractors are called in reverse order
        deque<Contractor*>::iterator rit = m_deque.begin();
        ++rit; // last fwd (now first bwd) contractor has already been called
        for( ; rit != m_deque.end(); ++rit)
          (*rit)->contract();

        // Looking for fixed point
        fixed_point = true;
        for(auto& dom : involved_domains)
        {
          double current_volume = dom.second->compute_volume();
          fixed_point &= !((current_volume/dom.second->get_saved_volume()) < 1.-m_fixedpoint_ratio);
          dom.second->set_volume(current_volume); // updating old volume
        }

      } while(!fixed_point);

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
        if(ctc.second->type() == Contractor::Type::T_IBEX
          || ctc.second->type() == Contractor::Type::T_TUBEX
          || ctc.second->type() == Contractor::Type::T_EQUALITY)
        {
          // Only "contracting" contractors are triggered
          ctc.second->set_active(true);
          add_ctc_to_queue(ctc.second, m_deque);
        }

        else
          ctc.second->set_active(false);
      }
    }

    int ContractorNetwork::nb_ctc_in_stack() const
    {
      return m_deque.size();
    }

    int ContractorNetwork::iteration_nb() const
    {
      return m_iteration_nb;
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