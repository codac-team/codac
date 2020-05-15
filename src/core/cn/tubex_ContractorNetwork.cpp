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
#include "tubex_CtcEval.h"

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

    if(m_ctc_deriv != NULL)
      delete m_ctc_deriv;
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

  void ContractorNetwork::set_all_contractors_active()
  {
    m_deque.clear();

    for(auto& ctc : m_v_ctc)
    {
      ctc->set_active(true);
      add_ctc_to_queue(ctc, m_deque);
    }
  }
  
  bool ContractorNetwork::emptiness() const
  {
    for(const auto& dom : m_v_domains)
      if(dom->is_empty())
        return true;

    return false;
  }

  Interval& ContractorNetwork::create_var(const Interval& i)
  {
    return add_dom(Domain(i))->interval();
  }

  IntervalVector& ContractorNetwork::create_var(const IntervalVector& iv)
  {
    return add_dom(Domain(iv))->interval_vector();
  }

  Tube& ContractorNetwork::create_var(const Tube& t)
  {
    return add_dom(Domain(t))->tube();
  }

  TubeVector& ContractorNetwork::create_var(const TubeVector& tv)
  {
    return add_dom(Domain(tv))->tube_vector();
  }

  IntervalVector& ContractorNetwork::subvector(Vector& iv, int start_index, int end_index)
  {
    // todo: assert here on indexes

    add_dom(Domain(iv));
    Domain *subvec = add_dom(Domain(iv.subvector(start_index, end_index)));

    // Adding links to the complete vector
    for(int i = 0 ; i < subvec->interval_vector().size() ; i++)
    {
      // Getting pointers to the components
      Domain *subvec_i = add_dom(Domain(subvec->interval_vector()[i]));
      Domain *vec_i = add_dom(Domain(iv[i+start_index]));

      // Contractor to put them equal
      Contractor *ac_eq = add_ctc(Contractor(Contractor::Type::EQUALITY, {subvec_i, vec_i}));
      // todo: prevent from adding identical contractors if several calls of this method

      // Linking the component domains through this contractor of equality
      subvec_i->add_ctc(ac_eq);
      vec_i->add_ctc(ac_eq);
    }

    return subvec->interval_vector();
  }

  IntervalVector& ContractorNetwork::subvector(IntervalVector& iv, int start_index, int end_index)
  {
    // todo: assert here on indexes

    add_dom(Domain(iv));
    Domain *subvec = add_dom(Domain(iv.subvector(start_index, end_index)));

    // Adding links to the complete vector
    for(int i = 0 ; i < subvec->interval_vector().size() ; i++)
    {
      // Getting pointers to the components
      Domain *subvec_i = add_dom(Domain(subvec->interval_vector()[i]));
      Domain *vec_i = add_dom(Domain(iv[i+start_index]));

      // Contractor to put them equal
      Contractor *ac_eq = add_ctc(Contractor(Contractor::Type::EQUALITY, {subvec_i, vec_i}));
      // todo: prevent from adding identical contractors if several calls of this method

      // Linking the component domains through this contractor of equality
      subvec_i->add_ctc(ac_eq);
      vec_i->add_ctc(ac_eq);
    }

    return subvec->interval_vector();
  }

  TubeVector& ContractorNetwork::subvector(TubeVector& iv, int start_index, int end_index)
  {
//    // todo: assert here on indexes
//
//    TubeVector *subvector = new TubeVector(iv.subvector(start_index, end_index));
//    m_tubevectors_to_delete.push_front(subvector);
//
//    for(int i = start_index ; i <= end_index ; i++)
//    {
//      assert(Tube::same_slicing(iv[i], (*subvector)[i-start_index]));
//      Contractor *ac_eq = new Contractor(Contractor::Type::EQUALITY);
//      //add_dom(new Domain((*subvector)[i-start_index]), ac_eq); // adding vector
//      //add_dom(new Domain(iv[i]), ac_eq); // adding vector
//      //add_ctc(ac_eq);
//
//      // todo: add EQUALITY link to slices too? already done?
//      // todo: check non reundant addition of domains
//    }
////add_dom(new Domain(*subvector));
//    return *subvector;
    return iv;
  }

  void ContractorNetwork::add(ibex::Ctc& ibex_ctc, const vector<Domain>& v_domains)
  {
    // Static constraint: each slice (if dyn case) is dealt with in parallel,
    // so dynamic variables must share the same slicing
    assert(Domain::dyn_same_slicing(v_domains));

    int n = Domain::total_size(v_domains);
    assert((n % ibex_ctc.nb_var == 0) && "invalid total dimension of domains");

    // Adding domains to the CN
    for(auto& dom : v_domains)
      add_dom(dom);

    for(int i = 0 ; i < n/ibex_ctc.nb_var ; i++) // in case we are dealing with array data
    {
      int k = 0; // k-th slice
      int slices_nb = -1; // will be determined during the dowhile loop, if one dyn domain is present

      do
      {
        // Creating a vector of pointers to domains
        vector<Domain*> v_dom_ptr;
        for(auto& dom : v_domains)
        {
          switch(dom.type())
          {
            case Domain::Type::INTERVAL:
              assert(n/ibex_ctc.nb_var == 1); // no array configuration with scalar type
            case Domain::Type::SLICE:
              v_dom_ptr.push_back(add_dom(dom));
              break;

            case Domain::Type::INTERVAL_VECTOR:
              if(n/ibex_ctc.nb_var == 1) // heterogeneous case
              {
                // todo: ? add the vector itself, or each component as it is now:
                for(int j = 0 ; j < dom.interval_vector().size() ; j++)
                  v_dom_ptr.push_back(add_dom(Domain::vector_component(const_cast<Domain&>(dom), j)));
              }

              else // array data case
              {
                assert((dom.interval_vector().size() == n/ibex_ctc.nb_var) && "wrong vector dimension");
                v_dom_ptr.push_back(add_dom(Domain::vector_component(const_cast<Domain&>(dom), i)));
              }
              break;

            case Domain::Type::TUBE:
              assert(n/ibex_ctc.nb_var == 1); // no array configuration with scalar type
              v_dom_ptr.push_back(add_dom(Domain(const_cast<Slice&>(*dom.tube().slice(k)))));
              slices_nb = dom.tube().nb_slices();
              break;

            case Domain::Type::TUBE_VECTOR:
              if(n/ibex_ctc.nb_var == 1) // heterogeneous case
              {
                for(int j = 0 ; j < dom.tube_vector().size() ; j++)
                  v_dom_ptr.push_back(add_dom(Domain(const_cast<Slice&>(*dom.tube_vector()[j].slice(k)))));
              }

              else // array data case
              {
                assert((dom.tube_vector().size() == n/ibex_ctc.nb_var) && "wrong vector dimension");
                v_dom_ptr.push_back(add_dom(Domain(const_cast<Slice&>(*dom.tube_vector()[i].slice(k)))));
              }

              slices_nb = dom.tube_vector().nb_slices();
              break;

            default:
              assert(false && "unhandled case");
          }
        }

        assert((int)v_dom_ptr.size() == ibex_ctc.nb_var);

        // Creating what would be this new contractor (defined with domains)
        Contractor ctc(ibex_ctc, v_dom_ptr);

        // Getting the actual contractor (maybe the same if not already added)
        Contractor *ctc_ptr = add_ctc(ctc);

        // Linking to the related domains
        for(auto& dom : v_dom_ptr)
          dom->add_ctc(ctc_ptr);

        k++;
      } while(k < slices_nb);
    }
  }

  void ContractorNetwork::add(tubex::DynCtc& tubex_ctc, const vector<Domain>& v_domains)
  {
    bool breakdown_to_slice_level = false;

    if(typeid(tubex_ctc) == typeid(CtcEval))
    {
      assert(v_domains.size() == 3 || v_domains.size() == 4);

      if(v_domains.size() == 4) // with derivative information
      {
        static_cast<CtcEval&>(tubex_ctc).enable_time_propag(false);

        if(m_ctc_deriv == NULL)
          m_ctc_deriv = new CtcDeriv();
        add(*m_ctc_deriv, {v_domains[2], v_domains[3]});
      }
    }

    if(!tubex_ctc.is_intertemporal())
    {
      // todo: check that the related constraint is not intertemporal
      // otherwise, tube level only

      if(v_domains[0].type() == Domain::Type::TUBE || v_domains[0].type() == Domain::Type::TUBE_VECTOR)
        breakdown_to_slice_level = true;
      // todo: improve this ^
      // and check that all domains are either tubes/tubevectors or interval/intervalvectors
    }

    // Towards contractors on slices scale, if possible
    // (in case of non-intertemporal constraints)
    if(breakdown_to_slice_level)
    {
      vector<const Slice*> v_slices;

      // Vector initialization with first slices of each tube
      int nb_slices = -1;
      for(const auto& dom : v_domains)
      {
        add_dom(dom);

        switch(dom.type())
        {
          case Domain::Type::TUBE:
          {
            if(nb_slices == -1)
              nb_slices = dom.tube().nb_slices();

            else
              assert(nb_slices == dom.tube().nb_slices());

            v_slices.push_back(dom.tube().first_slice());
          }
          break;

          case Domain::Type::TUBE_VECTOR:
          {            
            for(int j = 0 ; j < dom.tube_vector().size() ; j++)
            {
              if(nb_slices == -1)
                nb_slices = dom.tube_vector()[j].nb_slices();

              else
                assert(nb_slices == dom.tube_vector()[j].nb_slices());

              v_slices.push_back(dom.tube_vector()[j].first_slice());
            }
          }
          break;

          default:
            assert(false && "domain is not a tube or a tube vector");
        }
      }

      // Adding each row of slices
      for(int k = 0 ; k < nb_slices ; k++)
      {
        vector<Domain> v_slices_domains;
        for(size_t i = 0 ; i < v_slices.size() ; i++)
          v_slices_domains.push_back(Domain(const_cast<Slice&>(*v_slices[i])));

        add(tubex_ctc, v_slices_domains); 

        for(auto& s : v_slices)
          s = s->next_slice();
      }
    }

    else
    {
      // Creating a vector of pointers to domains
      vector<Domain*> v_dom_ptr;
      for(const auto& dom : v_domains)
        v_dom_ptr.push_back(add_dom(dom));

      // Creating what would be this new contractors (namely defined with domains)
      Contractor ctc(tubex_ctc, v_dom_ptr);

      // Getting the actual contractor (maybe the same if not already added)
      Contractor *ctc_ptr = add_ctc(ctc);

      // Linking to the related domains
      for(auto& dom : v_dom_ptr)
        dom->add_ctc(ctc_ptr);
    }
  }
  
  void ContractorNetwork::add_data(Tube& tube, double t, const Interval& y)
  {
    Domain *ad = add_dom(Domain(tube));
    assert(ad->type() == Domain::Type::TUBE);
    ad->add_data(t, y, *this);
  }
  
  void ContractorNetwork::add_data(TubeVector& tube, double t, const IntervalVector& y)
  {
    Domain *ad = add_dom(Domain(tube));
    assert(ad->type() == Domain::Type::TUBE_VECTOR);
    ad->add_data(t, y, *this);
  }

  Domain* ContractorNetwork::add_dom(const Domain& ad)
  {
    // Looking if this domain is not already part of the graph
    for(auto& dom : m_v_domains)
      if(*dom == ad) // found
        return dom;
    
    // Else, create and add this new domain
      Domain *dom = new Domain(ad);
      m_v_domains.push_back(dom);

    // And add possible dependencies

      switch(dom->type())
      {
        case Domain::Type::INTERVAL:
          // nothing to do
          break;

        case Domain::Type::SLICE:
          // nothing to do
          break;

        case Domain::Type::TUBE_VECTOR:
        {
          vector<Domain*> v_doms;
          v_doms.push_back(dom);
          for(int i = 0 ; i < dom->tube_vector().size() ; i++)
            v_doms.push_back(add_dom(Domain(dom->tube_vector()[i])));

          Contractor *ac_component = add_ctc(Contractor(Contractor::Type::COMPONENT, v_doms));

          dom->add_ctc(ac_component); // main vector
          for(auto& dom_i : v_doms)
            dom_i->add_ctc(ac_component); // and its components
        }
        break;

        case Domain::Type::INTERVAL_VECTOR:
        {
          vector<Domain*> v_doms;
          v_doms.push_back(dom); // main vector

          // And its components
          for(int i = 0 ; i < dom->interval_vector().size() ; i++)
            v_doms.push_back(add_dom(Domain::vector_component(*dom, i)));

          Contractor *ac_component = add_ctc(Contractor(Contractor::Type::COMPONENT, v_doms));
          for(auto& dom_i : v_doms)
            dom_i->add_ctc(ac_component);
        }
        break;

        case Domain::Type::TUBE:
        {
          vector<Domain*> v_doms;
          v_doms.push_back(dom);
          for(Slice *s = dom->tube().first_slice() ; s != NULL ; s = s->next_slice())
            v_doms.push_back(add_dom(Domain(*s)));

          // Dependencies tube <-> slice
          Contractor *ac_component = add_ctc(Contractor(Contractor::Type::COMPONENT, v_doms));

          dom->add_ctc(ac_component);
          for(auto& dom_i : v_doms)
            dom_i->add_ctc(ac_component);

          // Dependencies slice <-> slice
          for(Slice *s = dom->tube().first_slice() ; s->next_slice() != NULL ; s = s->next_slice())
          {
            Domain *dom_i1 = add_dom(Domain(*s));
            Domain *dom_i2 = add_dom(Domain(*(s->next_slice())));

            Contractor *ac_component_slices = add_ctc(Contractor(Contractor::Type::COMPONENT, {dom_i1, dom_i2}));

            dom_i1->add_ctc(ac_component_slices);
            dom_i2->add_ctc(ac_component_slices);
          }
        }
        break;
      }

    return dom;
  }

  Contractor* ContractorNetwork::add_ctc(const Contractor& ac)
  {
    // Looking if this contractor is not already part of the graph
    for(auto& ctc : m_v_ctc)
      if(*ctc == ac) // found
        return ctc;

    // Else, create and add this new domain
    Contractor *ctc = new Contractor(ac);
    m_v_ctc.push_back(ctc);
    add_ctc_to_queue(ctc, m_deque);
    return ctc;
  }

  void ContractorNetwork::add_ctc_to_queue(Contractor *ac, deque<Contractor*>& ctc_deque)
  {
    // todo: propagate for EQUALITY contractors even in case of poor contractions?

    if(ac->type() == Contractor::Type::COMPONENT)
      ctc_deque.push_back(ac);

    else
      ctc_deque.push_front(ac); // priority
  }

  ostream& operator<<(ostream& str, const ContractorNetwork& x)
  {
    str << x.nb_ctc() << " contractors\n";
    str << x.nb_dom() << " domains:\n";
    for(const auto& dom : x.m_v_domains)
      str << *dom << endl;
    return str;
  }
}