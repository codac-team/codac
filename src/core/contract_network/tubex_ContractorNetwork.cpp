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
    int k = 0;
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
      AbstractContractor *ctc = m_deque.front();
      m_deque.pop_front();

      ctc->contract();
      ctc->m_active = false;

      for(auto& ctc_dom : ctc->m_domains) // for each domain related to this contractor
      {
        double current_volume = ctc_dom->volume(); // new volume after contraction

        // If the domain has "changed" after the contraction
        if(current_volume/ctc_dom->m_volume < 1.-m_fixedpoint_ratio)
        {
          // We activate each contractor related to these domains, according to graph orientation

          // Local deque, for specific order related to this domain
          deque<AbstractContractor*> ctc_deque;

          for(auto& ctc_of_dom : ctc_dom->m_v_ctc) 
            if(ctc_of_dom != ctc && !ctc_of_dom->m_active)
            {
              ctc_of_dom->m_active = true;

              if(ctc_of_dom->type() == ContractorType::NONE)
                ctc_deque.push_back(ctc_of_dom);

              else
                ctc_deque.push_front(ctc_of_dom); // priority
            }

          // Merging this local deque in the CN one
          for(auto& c : ctc_deque)
            m_deque.push_front(c);
        }
        
        ctc_dom->m_volume = current_volume; // updated old volume
      }
    }

    if(verbose)
      cout << endl
           << "  computation time: " << (double)(clock() - t_start)/CLOCKS_PER_SEC << "s" << endl;

    // Emptiness test
    for(const auto& ctc : m_v_ctc)
      for(const auto& dom : ctc->m_domains)
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
    return add_domain(new AbstractDomain(*new Interval(i_)))->m_i;
  }

  IntervalVector& ContractorNetwork::create_var(const IntervalVector& i_)
  {
    // todo: manage delete
    return add_domain(new AbstractDomain(*new IntervalVector(i_)))->m_iv;
  }

  void ContractorNetwork::add(ibex::Ctc& ctc, initializer_list<AbstractDomain> list)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    for(const auto& dom : list)
      add_domain(new AbstractDomain(dom), abstract_ctc);
    add_contractor(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, initializer_list<AbstractDomain> list)
  {
    // If adding CtcDeriv with two scalar tubes
    if(typeid(ctc) == typeid(CtcDeriv) && list.begin()->type() == DomainType::TUBE)
    {
      assert(list.size() == 2);
      const AbstractDomain *dom_tube1 = list.begin();
      const AbstractDomain *dom_tube2;

      // Add tube domains for dependencies
      for(auto& dom : list)
      {
        assert(dom.type() == DomainType::TUBE
          && dom.m_t.nb_slices() == dom_tube1->m_t.nb_slices());
        add_domain(new AbstractDomain(dom));
        dom_tube2 = &dom;
      }

      // Add dependencies between slices
      Slice *s_x = dom_tube1->m_t.first_slice(), *s_v = dom_tube2->m_t.first_slice();
      while(s_x != NULL)
      {
        add(ctc, {*s_x,*s_v});
        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }
    }

    else
    {
      AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
      for(const auto& dom : list)
        add_domain(new AbstractDomain(dom), abstract_ctc);
      add_contractor(abstract_ctc);
    }
  }

  AbstractDomain* ContractorNetwork::add_domain(AbstractDomain *ad)
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
        AbstractContractor *ac_link = new AbstractContractor();
        add_domain(new AbstractDomain(ad->m_tv), ac_link); // adding vector
        for(int i = 0 ; i < ad->m_tv.size() ; i++)
          add_domain(new AbstractDomain(ad->m_tv[i]), ac_link); // adding its components
        add_contractor(ac_link);
      }

      else if(ad->type() == DomainType::INTERVAL_VECTOR)
      {
        AbstractContractor *ac_link = new AbstractContractor();
        add_domain(new AbstractDomain(ad->m_iv), ac_link); // adding vector
        for(int i = 0 ; i < ad->m_iv.size() ; i++)
          add_domain(new AbstractDomain(ad->m_iv[i]), ac_link); // adding its components
        add_contractor(ac_link);
      }

      else if(ad->type() == DomainType::TUBE)
      {
        // Dependencies tube <-> slice
        AbstractContractor *ac_link = new AbstractContractor();
        add_domain(new AbstractDomain(ad->m_t), ac_link); // adding tube
        for(Slice *s = ad->m_t.first_slice() ; s != NULL ; s = s->next_slice())
          add_domain(new AbstractDomain(*s), ac_link); // adding one of its slices
        add_contractor(ac_link);

        // Dependencies slice <-> slice
        for(Slice *s = ad->m_t.first_slice() ; s->next_slice() != NULL ; s = s->next_slice())
        {
          AbstractContractor *ac_link_slices = new AbstractContractor();
          add_domain(new AbstractDomain(*s), ac_link_slices);
          add_domain(new AbstractDomain(*(s->next_slice())), ac_link_slices);
          add_contractor(ac_link_slices);
        }
      }

    return ad;
  }

  void ContractorNetwork::add_domain(AbstractDomain *ad, AbstractContractor *ac)
  {
    // The domain ad may be already in the graph, we are looking for it,
    // or we use the current pointer. The result is pointed by ad_.
    AbstractDomain *ad_ = add_domain(ad);

    ac->m_domains.push_back(ad_);
    ad_->m_v_ctc.push_back(ac);
  }

  void ContractorNetwork::add_contractor(AbstractContractor *&ac)
  {
    // Looking if this contractor is not already part of the graph
    for(auto& ctc : m_v_ctc)
      if(*ctc == *ac) // found
      {
        // todo: delete ac
        return;
      }
    
    m_v_ctc.push_back(ac);

    if(ac->type() == ContractorType::NONE)
      m_deque.push_back(ac);

    else
      m_deque.push_front(ac); // priority
  }
}