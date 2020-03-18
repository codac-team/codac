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

  int ContractorNetwork::ctc_nb() const
  {
    return m_v_ctc.size();
  }

  double ContractorNetwork::domains_volume() const // todo: avoid redundant additions
  {
    double volume = 0.;
    
    for(const auto dom : m_v_domains)
      volume += dom->volume();

    return volume;
  }

  void ContractorNetwork::contract(float r)
  {
    int k = 0;
    clock_t t_start = clock();
    cout << "Contractor network has " << ctc_nb()
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

          for(int i = 0 ; i < ctc->m_domains.size() ; i++) // for each domain related to this contractor
          {
            double current_volume = ctc->m_domains[i].ad->volume();

            if(current_volume != ctc->m_domains[i].volume) // if the domain has "changed"
            {
              // We keep in mind that at least one contractor is still active:
              no_active_ctc = false; 

              // And we activate each contractor related to these domains, according to graph orientation
              for(auto& dom_ctc : ctc->m_domains[i].ad->m_v_ctc) 
                if(dom_ctc != ctc)
                  dom_ctc->set_active(true);
            }

            ctc->m_domains[i].volume = current_volume; // updated old volume
          }
        }
      }

    } while(!no_active_ctc);

    cout << endl
         << "  computation time: " << (double)(clock() - t_start)/CLOCKS_PER_SEC << "s" << endl;

    for(const auto& ctc : m_v_ctc)
      for(const auto& dom : ctc->m_domains)
        if(dom.ad->is_empty())
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

  void ContractorNetwork::add_domain(AbstractDomain *ad, AbstractContractor *ac, DomainRelation rel)
  {
    if(ad->is_empty())
    {
      cout << "warning: adding empty domain" << endl;
      exit(1);
    }

    // Looking if this domain is not already added
    for(int i = 0 ; i < m_v_domains.size() ; i++)
    {
      if(*m_v_domains[i] == *ad) // found
      {
        DomainParams dom_params;
        dom_params.ad = m_v_domains[i];
        dom_params.rel = rel;
        ac->m_domains.push_back(dom_params);
        m_v_domains[i]->m_v_ctc.push_back(ac);
        delete ad;
        return;
      }
    }
    
    // Else: add this new domain

      // If this domain is a tube vector, each component is added.
      // This allows dependencies between scalar tubes and slices
      if(ad->type() == DomainType::TUBE_VECTOR)
      {
        for(int i = 0 ; i < ad->m_tv.size() ; i++)
        {
          AbstractDomain *temp = new AbstractDomain(ad->m_tv[i]);

          bool found = false;
          for(int i = 0 ; i < m_v_domains.size() ; i++)
          {
            if(*m_v_domains[i] == *temp) // found
              found = true;
          }

          if(found)
            delete temp;

          else
          {
            //cout << "added tube" << endl;
            m_v_domains.push_back(temp);
          }
        }
      }


      // Then the domain is added
      DomainParams dom_params;
      dom_params.ad = ad;
      dom_params.rel = rel;
      ac->m_domains.push_back(dom_params);
      ad->m_v_ctc.push_back(ac);

      // Looking if this domain is not an item of some already known vector
      /*for(int i = 0 ; i < m_v_domains.size() ; i++)
      {
        if(ad->is_component_of(m_v_domains[i]))
        {
          // Adding relation between the domains


          //AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
          //add_domain(new AbstractDomain(i1), abstract_ctc);
          //add_domain(new AbstractDomain(i2), abstract_ctc);
          //m_v_ctc.push_back(abstract_ctc);
          //cout << "SUCCESS" << endl;
          //exit(1);
        }

        else if(ad->is_slice_of(m_v_domains[i]))
        {
          //cout << "slice of" << endl;
          // Adding relation between the domains

          AbstractContractor *ac_link_slices = new AbstractContractor();
          ac_link_slices->m_domains.push_back(make_pair(ad,DomainRelation::IN | DomainRelation::OUT));
          ac_link_slices->m_domains.push_back(make_pair(m_v_domains[i],DomainRelation::IN | DomainRelation::OUT));

          ad->m_v_ctc.push_back(ac_link_slices);
          m_v_domains[i]->m_v_ctc.push_back(ac_link_slices);

          m_v_ctc.push_back(ac_link_slices);
          //break;

          //AbstractContractor *abstract_ctc = new AbstractContractor(ctc);
          //add_domain(new AbstractDomain(i1), abstract_ctc);
          //add_domain(new AbstractDomain(i2), abstract_ctc);
          //m_v_ctc.push_back(abstract_ctc);

          //cout << "SUCCESS" << endl;
          //exit(1);
        }

        else if(ad->is_prev_slice_of(m_v_domains[i]))
        {
          cout << "prev" << endl;
        }

        else if(ad->is_next_slice_of(m_v_domains[i]))
        {
          AbstractContractor *ac_link_slices = new AbstractContractor();
          ac_link_slices->m_domains.push_back(make_pair(ad,DomainRelation::IN | DomainRelation::OUT));
          ac_link_slices->m_domains.push_back(make_pair(m_v_domains[i],DomainRelation::IN | DomainRelation::OUT));

          ad->m_v_ctc.push_back(ac_link_slices);
          m_v_domains[i]->m_v_ctc.push_back(ac_link_slices);

          m_v_ctc.push_back(ac_link_slices);
          //break;
        }
      }*/

      m_v_domains.push_back(ad);
  }
}