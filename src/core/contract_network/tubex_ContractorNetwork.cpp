/** 
 *  ContractorNetwork class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_ContractorNetwork.h"
#include "tubex_CtcDeriv.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  ContractorNetwork::ContractorNetwork()
  {

  }

  ContractorNetwork::~ContractorNetwork()
  {
    for(auto& dom : m_v_domains)
      delete dom;
    for(auto& ctc : m_v_ctc)
      delete ctc;
  }

  int ContractorNetwork::nb_ctc() const
  {
    return m_v_ctc.size();
  }

  int ContractorNetwork::nb_dom() const
  {
    return m_v_domains.size();
  }

  int ContractorNetwork::nb_ctc_in_stack() const
  {
    return m_deque.size();
  }

  void ContractorNetwork::set_fixedpoint_ratio(float r)
  {
    assert(Interval(0.,1).contains(r));
    m_fixedpoint_ratio = r;
  }

  double ContractorNetwork::contract(bool verbose)
  {
    clock_t t_start = clock();

    if(verbose)
    {
      cout << "Contractor network has " << m_v_ctc.size()
           << " contractors and " << m_v_domains.size() << " domains" << endl;
      cout << "Computing, " << nb_ctc_in_stack() << " contractors in m_deque";
      if(!isinf(m_contraction_duration_max))
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
        double current_volume = ctc_dom->compute_volume(); // new volume after contraction

        // If the domain has "changed" after the contraction
        if(current_volume/ctc_dom->get_saved_volume() < 1.-m_fixedpoint_ratio)
        {
          // We activate each contractor related to these domains, according to graph orientation

          // Local deque, for specific order related to this domain
          deque<Contractor*> ctc_deque;

          for(auto& ctc_of_dom : ctc_dom->contractors()) 
            if(ctc_of_dom != ctc && !ctc_of_dom->is_active())
            {
              ctc_of_dom->set_active(true);

              if(ctc_of_dom->type() == ContractorType::COMPONENT)
                ctc_deque.push_back(ctc_of_dom);

              else
                ctc_deque.push_front(ctc_of_dom); // priority
            }

          // Merging this local deque in the CN one
          for(auto& c : ctc_deque)
            m_deque.push_front(c);
        }
        
        ctc_dom->set_volume(current_volume); // updating old volume
      }
    }

    if(verbose)
      cout << endl
           << "  computation time: " << (double)(clock() - t_start)/CLOCKS_PER_SEC << "s" << endl;

    // Emptiness test
    for(const auto& ctc : m_v_ctc)
      for(const auto& dom : ctc->domains())
        if(dom->is_empty())
        {
          cout << "  warning: empty set" << endl;
          exit(1);
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

  Interval& ContractorNetwork::create_var(const Interval& i_)
  {
    // todo: manage delete
    return add_domain(new Domain(*new Interval(i_)))->interval();
  }

  IntervalVector& ContractorNetwork::create_var(const IntervalVector& iv_)
  {
    // todo: manage delete
    return add_domain(new Domain(*new IntervalVector(iv_)))->interval_vector();
  }

  void ContractorNetwork::add(ibex::Ctc& ctc, initializer_list<Domain> list)
  {
    Contractor *abstract_ctc = new Contractor(ctc);
    for(const auto& dom : list)
      add_domain(new Domain(dom), abstract_ctc);
    add_contractor(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, initializer_list<Domain> list)
  {
    // If adding CtcDeriv with two scalar tubes
    if(typeid(ctc) == typeid(CtcDeriv) && list.begin()->type() == DomainType::TUBE)
    {
      assert(list.size() == 2);
      const Domain *dom_tube1 = list.begin();
      const Domain *dom_tube2;

      // Add tube domains for dependencies
      for(auto& dom : list)
      {
        assert(dom.type() == DomainType::TUBE
          && dom.tube().nb_slices() == dom_tube1->tube().nb_slices());
        add_domain(new Domain(dom));
        dom_tube2 = &dom;
      }

      // Add dependencies between slices
      Slice *s_x = const_cast<Slice*>(dom_tube1->tube().first_slice());
      Slice *s_v = const_cast<Slice*>(dom_tube2->tube().first_slice());
      while(s_x != NULL)
      {
        add(ctc, {*s_x,*s_v});
        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }
    }

    else
    {
      Contractor *abstract_ctc = new Contractor(ctc);
      for(const auto& dom : list)
        add_domain(new Domain(dom), abstract_ctc);
      add_contractor(abstract_ctc);
    }
  }

  Domain* ContractorNetwork::add_domain(Domain *ad)
  {
    if(ad->is_empty())
    {
      cout << "warning: adding empty domain" << endl;
      exit(1);
    }

    // Looking if this domain is not already part of the graph
    for(auto& dom : m_v_domains)
      if(*dom == *ad) // found
      {
        delete ad;
        return dom;
      }
    
    // Else, add this new domain
    
      m_v_domains.push_back(ad);

    // And add possible dependencies

      if(ad->type() == DomainType::TUBE_VECTOR)
      {
        Contractor *ac_link = new Contractor();
        add_domain(new Domain(ad->tube_vector()), ac_link); // adding vector
        for(int i = 0 ; i < ad->tube_vector().size() ; i++)
          add_domain(new Domain(ad->tube_vector()[i]), ac_link); // adding its components
        add_contractor(ac_link);
      }

      else if(ad->type() == DomainType::INTERVAL_VECTOR)
      {
        Contractor *ac_link = new Contractor();
        add_domain(new Domain(ad->interval_vector()), ac_link); // adding vector
        for(int i = 0 ; i < ad->interval_vector().size() ; i++)
          add_domain(new Domain(ad->interval_vector()[i]), ac_link); // adding its components
        add_contractor(ac_link);
      }

      else if(ad->type() == DomainType::TUBE)
      {
        // Dependencies tube <-> slice
        Contractor *ac_link = new Contractor();
        add_domain(new Domain(ad->tube()), ac_link); // adding tube
        for(Slice *s = ad->tube().first_slice() ; s != NULL ; s = s->next_slice())
          add_domain(new Domain(*s), ac_link); // adding one of its slices
        add_contractor(ac_link);

        // Dependencies slice <-> slice
        for(Slice *s = ad->tube().first_slice() ; s->next_slice() != NULL ; s = s->next_slice())
        {
          Contractor *ac_link_slices = new Contractor();
          add_domain(new Domain(*s), ac_link_slices);
          add_domain(new Domain(*(s->next_slice())), ac_link_slices);
          add_contractor(ac_link_slices);
        }
      }

    return ad;
  }

  void ContractorNetwork::add_domain(Domain *ad, Contractor *ac)
  {
    // The domain ad may be already in the graph, we are looking for it,
    // or we use the current pointer. The result is pointed by ad_.
    Domain *ad_ = add_domain(ad);

    ac->domains().push_back(ad_);
    ad_->contractors().push_back(ac);
  }

  void ContractorNetwork::add_contractor(Contractor *&ac)
  {
    // Looking if this contractor is not already part of the graph
    for(auto& ctc : m_v_ctc)
      if(*ctc == *ac) // found
      {
        // todo: delete ac
        return;
      }
    
    m_v_ctc.push_back(ac);

    if(ac->type() == ContractorType::COMPONENT)
      m_deque.push_back(ac);

    else
      m_deque.push_front(ac); // priority
  }
}