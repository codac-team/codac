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

using namespace std;
using namespace ibex;

namespace tubex
{
  ContractorNetwork::ContractorNetwork()
  {

  }

  ContractorNetwork::~ContractorNetwork()
  {
    for(int i = 0 ; i < m_v_domains.size() ; i++)
      delete m_v_domains[i];
  }

  void ContractorNetwork::contract(float r)
  {
    complete_graph();

    int k = 0;
    clock_t t_start = clock();
    cout << "Contractor network has " << m_v_ctc.size()
         << " contractors and " << m_v_domains.size() << " domains" << endl;

    bool no_active_ctc;

    do
    {
      k++;
      cout << "\r  contracting: iteration " << k << "...    " << flush;

      no_active_ctc = true;

      for(auto& ctc : m_v_ctc)
      {
        if(ctc->active())
        {
          ctc->contract();
          ctc->set_active(false);

          for(auto& ctc_dom : ctc->m_domains) // for each domain related to this contractor
          {
            double current_volume = ctc_dom->volume(); // new volume after contraction

            //if(current_volume != ctc_dom->m_volume) // if the domain has "changed" after the contraction
            if(current_volume/ctc_dom->m_volume < 1.-r)
            {
              // We keep in mind that at least one contractor is still active:
              no_active_ctc = false; 

              // And we activate each contractor related to these domains, according to graph orientation
              for(auto& ctc_of_dom : ctc_dom->m_v_ctc) 
                if(ctc_of_dom != ctc)
                  ctc_of_dom->set_active(true);
            }

            ctc_dom->m_volume = current_volume; // updated old volume
          }
        }
      }

    } while(!no_active_ctc);

    cout << endl
         << "  computation time: " << (double)(clock() - t_start)/CLOCKS_PER_SEC << "s" << endl;

    for(const auto& ctc : m_v_ctc)
      for(const auto& dom : ctc->m_domains)
        if(dom->is_empty())
        {
          cout << "  warning: empty set" << endl;
          exit(1);
          return;
        }
  }

  Interval& ContractorNetwork::create_var(const Interval& i_)
  {
    Interval *i = new Interval(i_);
    m_v_domains.push_back(new AbstractDomain(*i));
    return *i; // todo: allow to delete this
  }

  IntervalVector& ContractorNetwork::create_var(const IntervalVector& i_)
  {
    IntervalVector *i = new IntervalVector(i_);
    m_v_domains.push_back(new AbstractDomain(*i));
    return *i; // todo: allow to delete this
  }

  void ContractorNetwork::add(ibex::Ctc& ctc, IntervalVector& i1)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(ibex::Ctc& ctc, Interval& i1, Interval& i2, Interval& i3)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(ibex::Ctc& ctc, IntervalVector& i1, IntervalVector& i2, IntervalVector& i3)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(ibex::Ctc& ctc, Interval& i1, Interval& i2, Interval& i3, Interval& i4)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    add_domain(new AbstractDomain(i4), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(ibex::Ctc& ctc, Interval& i1, Interval& i2, Interval& i3, Interval& i4, Interval& i5)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    add_domain(new AbstractDomain(i4), abstract_ctc);
    add_domain(new AbstractDomain(i5), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, Interval& i1, Interval& i2, Tube& i3, Tube& i4)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    add_domain(new AbstractDomain(i4), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, Interval& i1, IntervalVector& i2, TubeVector& i3, TubeVector& i4)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    add_domain(new AbstractDomain(i4), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, Interval& i1, Interval& i2, Tube& i3)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, Interval& i1, IntervalVector& i2, TubeVector& i3)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    add_domain(new AbstractDomain(i3), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, Tube& i1, Tube& i2)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, TubeVector& i1, TubeVector& i2)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc& ctc, Slice& i1, Slice& i2)
  {
    AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
    add_domain(new AbstractDomain(i1), abstract_ctc);
    add_domain(new AbstractDomain(i2), abstract_ctc);
    m_v_ctc.push_back(abstract_ctc);
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

  void ContractorNetwork::complete_graph()
  {
    // Vector components (dependencies between vector items)

      for(auto& dom : m_v_domains)
      {
        if(dom->type() == DomainType::TUBE_VECTOR)
        {
          AbstractContractor *ac_link = new AbstractContractor();
          add_domain(new AbstractDomain(dom->m_tv), ac_link); // adding vector
          for(int i = 0 ; i < dom->m_tv.size() ; i++)
            add_domain(new AbstractDomain(dom->m_tv[i]), ac_link); // adding its components
          m_v_ctc.push_back(ac_link);
        }

        else if(dom->type() == DomainType::INTERVAL_VECTOR)
        {
          AbstractContractor *ac_link = new AbstractContractor();
          add_domain(new AbstractDomain(dom->m_iv), ac_link); // adding vector
          for(int i = 0 ; i < dom->m_iv.size() ; i++)
            add_domain(new AbstractDomain(dom->m_iv[i]), ac_link); // adding its components
          m_v_ctc.push_back(ac_link);
        }
      }

    // Tube components (dependencies between slices and their tube)
    // (to be done once all tubes have been added)

      for(auto& dom : m_v_domains)
      {
        if(dom->type() == DomainType::TUBE)
        {
          // Dependencies tube <-> slice
          AbstractContractor *ac_link = new AbstractContractor();
          add_domain(new AbstractDomain(dom->m_t), ac_link); // adding tube
          for(Slice *s = dom->m_t.first_slice() ; s != NULL ; s = s->next_slice())
            add_domain(new AbstractDomain(*s), ac_link); // adding one of its slices
          m_v_ctc.push_back(ac_link);

          // Dependencies slice <-> slice
          for(Slice *s = dom->m_t.first_slice() ; s->next_slice() != NULL ; s = s->next_slice())
          {
            AbstractContractor *ac_link_slices = new AbstractContractor();
            add_domain(new AbstractDomain(*s), ac_link_slices);
            add_domain(new AbstractDomain(*(s->next_slice())), ac_link_slices);
            m_v_ctc.push_back(ac_link_slices);
          }
        }
      }
  }
}