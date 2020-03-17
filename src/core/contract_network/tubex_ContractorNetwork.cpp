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
    double volume;
    clock_t t_start = clock();
    cout << "Contractor network has " << ctc_nb()
         << " contractors and " << m_v_domains.size() << " domains" << endl;

    bool no_active_ctc;

    do
    {
      k++;
      cout << "\r  contracting: iteration " << k << "...    " << flush;
      volume = domains_volume();

      no_active_ctc = true;

      for(auto& ctc : m_v_ctc)
      {
        if(ctc->active())
        {
          vector<double> v_vol(ctc->m_v_domains.size());
          for(int i = 0 ; i < ctc->m_v_domains.size() ; i++)
            v_vol[i] = ctc->m_v_domains[i]->volume();

          ctc->contract();
          ctc->set_active(false);

          for(int i = 0 ; i < ctc->m_v_domains.size() ; i++)
          {
            if(ctc->m_v_domains[i]->volume() != v_vol[i]) // fixed point condition
            {
              no_active_ctc = false; // something happened

              for(auto& dom_ctc : ctc->m_v_domains[i]->m_v_ctc)
                if(dom_ctc != ctc)
                  dom_ctc->set_active(true);
            }
          }
        }
      }

    } while(!no_active_ctc);

    cout << endl
         << "  computation time: " << (double)(clock() - t_start)/CLOCKS_PER_SEC << "s" << endl;

    for(const auto& ctc : m_v_ctc)
      for(const auto& dom : ctc->m_v_domains)
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

  void ContractorNetwork::add_domain(AbstractDomain *ad, AbstractContractor *ac)
  {
    if(ad->is_empty())
    {
      cout << "warning: adding empty domain" << endl;
      exit(1);
    }

    for(int i = 0 ; i < m_v_domains.size() ; i++)
    {
      // Looking if this domain is not already added
      if(*m_v_domains[i] == *ad) // found
      {
        ac->m_v_domains.push_back(m_v_domains[i]);
        m_v_domains[i]->m_v_ctc.push_back(ac);
        delete ad;
        return;
      }

      // Looking is this domain is not an item of some already known vector
      else if(ad->is_component_of(m_v_domains[i])) // found
      {
        cout << "SUCCESS" << endl;
        exit(1);
      }
    }
    
    // Else: add this new domain
    m_v_domains.push_back(ad);
    ac->m_v_domains.push_back(ad);
    ad->m_v_ctc.push_back(ac);
  }
}