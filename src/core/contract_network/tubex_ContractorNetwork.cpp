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
    for(int i = 0 ; i < v_interm_var.size() ; i++)
    {
      if(v_interm_var[i].m_i != NULL)
        delete v_interm_var[i].m_i;
      if(v_interm_var[i].m_iv != NULL)
        delete v_interm_var[i].m_iv;
    }
  }

  int ContractorNetwork::ctc_nb() const
  {
    return v_ctc.size();
  }

  double ContractorNetwork::domains_volume() const // todo: avoid redundant additions
  {
    double volume = 0.;
    for(int i = 0 ; i < v_ctc.size() ; i++)
      volume += v_ctc[i].domains_volume();
    return volume;
  }

  void ContractorNetwork::contract(float r)
  {
    int k = 0;
    double volume;

    do
    {
      k++;
      cout << "\r  contracting: iteration " << k << "...    " << flush;
      volume = domains_volume();

      for(int i = 0 ; i < v_ctc.size() ; i++)
        v_ctc[i].contract();

    } while(domains_volume()/volume < 1.-r);

    cout << endl;
  }

  Interval* ContractorNetwork::interm_var(const Interval& i_)
  {
    Interval *i = new Interval(i_);
    v_interm_var.push_back(AbstractDomain(i));
    return i;
  }

  IntervalVector* ContractorNetwork::interm_var(const IntervalVector& i_)
  {
    IntervalVector *i = new IntervalVector(i_);
    v_interm_var.push_back(AbstractDomain(i));
    return i;
  }

  void ContractorNetwork::add(ibex::Ctc* ctc, IntervalVector *iv)
  {
    AbstractContractor abstract_ctc(ctc);
    abstract_ctc.v_domains.push_back(AbstractDomain(iv));
    v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(ibex::Ctc* ctc, Interval *i1, Interval *i2, Interval *i3)
  {
    AbstractContractor abstract_ctc(ctc);
    abstract_ctc.v_domains.push_back(AbstractDomain(i1));
    abstract_ctc.v_domains.push_back(AbstractDomain(i2));
    abstract_ctc.v_domains.push_back(AbstractDomain(i3));
    v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(ibex::Ctc* ctc, IntervalVector *i1, IntervalVector *i2, IntervalVector *i3)
  {
    AbstractContractor abstract_ctc(ctc);
    abstract_ctc.v_domains.push_back(AbstractDomain(i1));
    abstract_ctc.v_domains.push_back(AbstractDomain(i2));
    abstract_ctc.v_domains.push_back(AbstractDomain(i3));
    v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(ibex::Ctc* ctc, Interval *i1, Interval *i2, Interval *i3, Interval *i4)
  {
    AbstractContractor abstract_ctc(ctc);
    abstract_ctc.v_domains.push_back(AbstractDomain(i1));
    abstract_ctc.v_domains.push_back(AbstractDomain(i2));
    abstract_ctc.v_domains.push_back(AbstractDomain(i3));
    abstract_ctc.v_domains.push_back(AbstractDomain(i4));
    v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc* ctc, Interval *i1, IntervalVector *i2, TubeVector *i3, TubeVector *i4)
  {
    AbstractContractor abstract_ctc(ctc);
    abstract_ctc.v_domains.push_back(AbstractDomain(i1));
    abstract_ctc.v_domains.push_back(AbstractDomain(i2));
    abstract_ctc.v_domains.push_back(AbstractDomain(i3));
    abstract_ctc.v_domains.push_back(AbstractDomain(i4));
    v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc* ctc, Tube *i1, Tube *i2)
  {
    AbstractContractor abstract_ctc(ctc);
    abstract_ctc.v_domains.push_back(AbstractDomain(i1));
    abstract_ctc.v_domains.push_back(AbstractDomain(i2));
    v_ctc.push_back(abstract_ctc);
  }

  void ContractorNetwork::add(tubex::Ctc* ctc, TubeVector *i1, TubeVector *i2)
  {
    AbstractContractor abstract_ctc(ctc);
    abstract_ctc.v_domains.push_back(AbstractDomain(i1));
    abstract_ctc.v_domains.push_back(AbstractDomain(i2));
    v_ctc.push_back(abstract_ctc);
  }
}