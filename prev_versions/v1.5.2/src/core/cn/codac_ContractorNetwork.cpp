/** 
 *  ContractorNetwork class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_ContractorNetwork.h"
#include "codac_CtcEval.h"
#include "codac_Exception.h"
#include "codac_DomainsTypeException.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Public methods

    // Definition

    ContractorNetwork::ContractorNetwork()
    {

    }

    ContractorNetwork::~ContractorNetwork()
    {
      for(auto& dom : m_map_domains)
        delete dom.second;
      for(auto& ctc : m_map_ctc)
        delete ctc.second;

      if(m_ctc_deriv)
        delete m_ctc_deriv;
    }

    int ContractorNetwork::nb_ctc() const
    {
      return m_map_ctc.size();
    }

    int ContractorNetwork::nb_dom() const
    {
      return m_map_domains.size();
    }
    
    bool ContractorNetwork::emptiness() const
    {
      for(auto& dom : m_map_domains)
        if(dom.second->is_empty())
          return true;

      return false;
    }

    // Building the network (domains, contractors)

    Interval& ContractorNetwork::create_dom(const Interval& i)
    {
      cout << "create_dom(): deprecated. Use create_interm_var() instead." << endl;
      return create_interm_var(i);
    }

    Interval& ContractorNetwork::create_interm_var(const Interval& i)
    {
      return add_dom(Domain(i, true))->interval();
    }

    IntervalVector& ContractorNetwork::create_dom(const IntervalVector& iv)
    {
      cout << "create_dom(): deprecated. Use create_interm_var() instead." << endl;
      return create_interm_var(iv);
    }

    IntervalVector& ContractorNetwork::create_interm_var(const IntervalVector& iv)
    {
      return add_dom(Domain(iv, true))->interval_vector();
    }

    Tube& ContractorNetwork::create_dom(const Tube& t)
    {
      cout << "create_dom(): deprecated. Use create_interm_var() instead." << endl;
      return create_interm_var(t);
    }

    Tube& ContractorNetwork::create_interm_var(const Tube& t)
    {
      return add_dom(Domain(t, true))->tube();
    }

    TubeVector& ContractorNetwork::create_dom(const TubeVector& tv)
    {
      cout << "create_dom(): deprecated. Use create_interm_var() instead." << endl;
      return create_interm_var(tv);
    }

    TubeVector& ContractorNetwork::create_interm_var(const TubeVector& tv)
    {
      return add_dom(Domain(tv, true))->tube_vector();
    }

    IntervalVector& ContractorNetwork::subvector(Vector& v, int start_index, int end_index)
    {
      assert(start_index >= 0);
      assert(end_index < v.size());
      assert(start_index <= end_index);

      add_dom(Domain(v));
      Domain *subvec = add_dom(Domain(v.subvector(start_index, end_index)));

      // Adding links to the complete vector
      for(int i = 0 ; i < subvec->interval_vector().size() ; i++)
      {
        // Getting pointers to the components
        Domain *subvec_i = add_dom(Domain(subvec->interval_vector()[i]));
        Domain *vec_i = add_dom(Domain(v[i+start_index]));

        // Contractor to put them equal
        Contractor *ac_eq = add_ctc(Contractor(Contractor::Type::T_EQUALITY, {subvec_i, vec_i}));
        // todo: prevent from adding identical contractors if several calls of this method

        // Linking the component domains through this contractor of equality
        subvec_i->add_ctc(ac_eq);
        vec_i->add_ctc(ac_eq);
      }

      return subvec->interval_vector();
    }

    IntervalVector& ContractorNetwork::subvector(IntervalVector& iv, int start_index, int end_index)
    {
      assert(start_index >= 0);
      assert(end_index < iv.size());
      assert(start_index <= end_index);

      add_dom(Domain(iv));
      Domain *subvec = add_dom(Domain(iv.subvector(start_index, end_index)));

      // Adding links to the complete vector
      for(int i = 0 ; i < subvec->interval_vector().size() ; i++)
      {
        // Getting pointers to the components
        Domain *subvec_i = add_dom(Domain(subvec->interval_vector()[i]));
        Domain *vec_i = add_dom(Domain(iv[i+start_index]));

        // Contractor to put them equal
        Contractor *ac_eq = add_ctc(Contractor(Contractor::Type::T_EQUALITY, {subvec_i, vec_i}));
        // todo: prevent from adding identical contractors if several calls of this method

        // Linking the component domains through this contractor of equality
        subvec_i->add_ctc(ac_eq);
        vec_i->add_ctc(ac_eq);
      }

      return subvec->interval_vector();
    }

    // not yet suported: TubeVector& ContractorNetwork::subvector(TubeVector& tv, int start_index, int end_index)
    // not yet suported: {
    // not yet suported:   assert(start_index >= 0);
    // not yet suported:   assert(end_index < tv.size());
    // not yet suported:   assert(start_index <= end_index);
    // not yet suported:
    // not yet suported:   return iv;
    // not yet suported: }

    void ContractorNetwork::add(Ctc& static_ctc, const vector<Domain>& v_domains)
    {
      if(v_domains.empty())
        throw Exception(__func__, "cannot add a contractor without domains");

      // Static constraint: each slice (if dyn case) is dealt with in parallel,
      // so dynamic variables must share the same slicing
      if(!Domain::dyn_same_slicing(v_domains))
        throw Exception(__func__, "domains do not have same slicing");

      int n = Domain::total_size(v_domains);
      if(n % static_ctc.nb_var != 0)
      {
        cout << "n=" << n << ", static_ctc.nb_var=" << static_ctc.nb_var << endl;
        throw Exception(__func__, "invalid total dimension of domains");
      }

      // Adding domains to the CN
      for(auto& dom : v_domains)
        add_dom(dom);

      for(int i = 0 ; i < n/static_ctc.nb_var ; i++) // in case we are dealing with array data
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
              case Domain::Type::T_INTERVAL:
                assert(n/static_ctc.nb_var == 1); // no array configuration with scalar type
              case Domain::Type::T_SLICE:
                v_dom_ptr.push_back(add_dom(dom));
                break;

              case Domain::Type::T_INTERVAL_VECTOR:
                if(n/static_ctc.nb_var == 1) // heterogeneous case
                {
                  // todo: ? add the vector itself, or each component as it is now:
                  for(int j = 0 ; j < dom.interval_vector().size() ; j++)
                    v_dom_ptr.push_back(add_dom(Domain::vector_component(const_cast<Domain&>(dom), j)));
                }

                else // array data case
                {
                  if(dom.interval_vector().size() != n/static_ctc.nb_var)
                    throw Exception(__func__, "wrong vector dimension");
                  v_dom_ptr.push_back(add_dom(Domain::vector_component(const_cast<Domain&>(dom), i)));
                }
                break;

              case Domain::Type::T_TUBE:
                assert(n/static_ctc.nb_var == 1); // no array configuration with scalar type
                v_dom_ptr.push_back(add_dom(Domain(const_cast<Slice&>(*dom.tube().slice(k)))));
                slices_nb = dom.tube().nb_slices();
                break;

              case Domain::Type::T_TUBE_VECTOR:
                if(n/static_ctc.nb_var == 1) // heterogeneous case
                {
                  for(int j = 0 ; j < dom.tube_vector().size() ; j++)
                    v_dom_ptr.push_back(add_dom(Domain(const_cast<Slice&>(*dom.tube_vector()[j].slice(k)))));
                }

                else // array data case
                {
                  if(dom.tube_vector().size() != n/static_ctc.nb_var)
                    throw Exception(__func__, "wrong vector dimension");
                  v_dom_ptr.push_back(add_dom(Domain(const_cast<Slice&>(*dom.tube_vector()[i].slice(k)))));
                }

                slices_nb = dom.tube_vector().nb_slices();
                break;

              default:
                assert(false && "unhandled case");
            }
          }

          assert((int)v_dom_ptr.size() == static_ctc.nb_var);

          // Creating what would be this new contractor (defined with domains)
          Contractor ctc(static_ctc, v_dom_ptr);

          // Getting the actual contractor (maybe the same if not already added)
          Contractor *ctc_ptr = add_ctc(ctc);

          // Linking to the related domains
          for(auto& dom : v_dom_ptr)
            dom->add_ctc(ctc_ptr);

          k++;
        } while(k < slices_nb);
      }
    }

    void ContractorNetwork::add(DynCtc& dyn_ctc, const vector<Domain>& v_domains)
    {
      if(v_domains.empty())
        throw Exception(__func__, "cannot add a contractor without domains");

      // If the CtcEval contractor is involved, its contracting impact is set
      // to the minimal and we add the CtcDeriv contractor on top of that, in 
      // order to reach thin propagations of contractions.
      // todo: add CtcDeriv only if not already added?
      // todo: prevent from several CtcDeriv on same couple of slices?
      if(typeid(dyn_ctc) == typeid(CtcEval))
      {
        if(v_domains.size() != 3 && v_domains.size() != 4)
          throw DomainsTypeException(CtcEval::m_ctc_name, v_domains, CtcEval::m_str_expected_doms);

        if(v_domains.size() == 4) // with derivative information
        {
          static_cast<CtcEval&>(dyn_ctc).enable_time_propag(false);

          if(m_ctc_deriv == nullptr)
            m_ctc_deriv = new CtcDeriv();
          add(*m_ctc_deriv, {v_domains[2], v_domains[3]});
        }
      }

      if(typeid(dyn_ctc) == typeid(CtcDeriv)
        && !Domain::all_slices(v_domains)
        && v_domains.size() != 2)
        throw DomainsTypeException(CtcDeriv::m_ctc_name, v_domains, CtcDeriv::m_str_expected_doms);

      // Optimization: do not try to add a CtcDeriv contractor on tubes if it has already
      // been added (todo: generalize this optimization to any contractor?)
      if(&dyn_ctc == m_ctc_deriv // if we are dealing with the interval CtcDeriv
        && !Domain::all_slices(v_domains)) // and if the domains are not slices
      {
        // todo ^: use "typeid(dyn_ctc) == typeid(CtcDeriv)" instead of "&dyn_ctc == m_ctc_deriv" ?
        pair<Domain*,Domain*> p = make_pair(add_dom(v_domains[0]), add_dom(v_domains[1]));

        if(find(m_domains_related_to_ctcderiv.begin(), m_domains_related_to_ctcderiv.end(), p)
          != m_domains_related_to_ctcderiv.end())
        {
          return; // contractor already added
        }

        else
        {
          // Notifying that the contractor will be added
          m_domains_related_to_ctcderiv.push_back(p);

          // Then, add the contractor in the following..
        }
      }

      // If possible, breaking down the constraint to slices level
      if(!dyn_ctc.is_intertemporal() && !Domain::all_slices(v_domains))
      {
        // Not inter-temporal => 
        if(!Domain::all_dyn(v_domains))
          throw Exception(__func__, "all domains are not slices nor tubes nor tube vectors");
        if(!Domain::dyn_same_slicing(v_domains))
          throw Exception(__func__, "domains do not have same slicing");

        vector<const Slice*> v_slices;

        // Vector initialization with first slices of each tube
        int nb_slices = -1;
        for(const auto& dom : v_domains)
        {
          add_dom(dom);

          switch(dom.type())
          {
            case Domain::Type::T_TUBE:
            {
              if(nb_slices == -1)
                nb_slices = dom.tube().nb_slices();

              v_slices.push_back(dom.tube().first_slice());
            }
            break;

            case Domain::Type::T_TUBE_VECTOR:
            {            
              for(int j = 0 ; j < dom.tube_vector().size() ; j++)
              {
                if(nb_slices == -1)
                  nb_slices = dom.tube_vector()[j].nb_slices();

                v_slices.push_back(dom.tube_vector()[j].first_slice());
              }
            }
            break;

            default:
              throw Exception(__func__, "domain is not a tube or a tube vector");
          }
        }

        // Adding each row of slices
        for(int k = 0 ; k < nb_slices ; k++)
        {
          vector<Domain> v_slices_domains(v_slices.size());
          for(size_t i = 0 ; i < v_slices.size() ; i++)
            v_slices_domains[i] = Domain(const_cast<Slice&>(*v_slices[i]));

          add(dyn_ctc, v_slices_domains); 

          for(auto& s : v_slices)
            s = s->next_slice();
        }
      }

      else // otherwise, dealing with the inter-temporal constraint as it is
      {
        // Creating a vector of pointers to domains
        vector<Domain*> v_dom_ptr(v_domains.size());
        for(size_t i = 0 ; i < v_domains.size() ; i++)
          v_dom_ptr[i] = add_dom(v_domains[i]);

        // Creating what would be this new contractors (namely defined with domains)
        Contractor ctc(dyn_ctc, v_dom_ptr);

        // Getting the actual contractor (maybe the same if not already added)
        Contractor *ctc_ptr = add_ctc(ctc);

        // Linking to the related domains
        for(auto& dom : v_dom_ptr)
          dom->add_ctc(ctc_ptr);
      }
    }

    void ContractorNetwork::add(ContractorNetwork& cn)
    {
      // Getting the actual contractor (maybe the same if not already added)
      Contractor *ctc_ptr = add_ctc(cn);

      // Sharing domains from sub_cn to cn
      for(auto& dom : cn.m_map_domains)
      {
        Domain* ad = add_dom(*dom.second);
        ad->add_ctc(ctc_ptr);
      }
    }
    
    void ContractorNetwork::add_data(Tube& tube, double t, const Interval& y)
    {
      Domain *ad = add_dom(Domain(tube));
      assert(ad->type() == Domain::Type::T_TUBE);
      ad->add_data(t, y, *this);
    }
    
    void ContractorNetwork::add_data(TubeVector& tube, double t, const IntervalVector& y)
    {
      Domain *ad = add_dom(Domain(tube));
      assert(ad->type() == Domain::Type::T_TUBE_VECTOR);
      ad->add_data(t, y, *this);
    }

  // Protected methods

    Domain* ContractorNetwork::add_dom(const Domain& ad)
    {
      if(ad.is_empty())
        throw Exception(__func__, "domain already empty when added to the CN");

      DomainHashcode hash(ad);
      if(m_map_domains.find(hash) != m_map_domains.end())
        return m_map_domains[hash];
    
      Domain *new_dom = new Domain(ad);
      m_map_domains[hash] = new_dom;

      // And add possible dependencies

        switch(new_dom->type())
        {
          case Domain::Type::T_INTERVAL:
            // nothing to do
            break;

          case Domain::Type::T_SLICE:
            // nothing to do
            break;

          case Domain::Type::T_TUBE_VECTOR:
          {
            vector<Domain*> v_doms(new_dom->tube_vector().size() + 1);
            v_doms[0] = new_dom;
            for(int i = 0 ; i < new_dom->tube_vector().size() ; i++)
              v_doms[i+1] = add_dom(Domain(new_dom->tube_vector()[i]));

            Contractor *ac_component = add_ctc(Contractor(Contractor::Type::T_COMPONENT, v_doms));

            new_dom->add_ctc(ac_component); // main vector
            for(auto& dom_i : v_doms)
              dom_i->add_ctc(ac_component); // and its components
          }
          break;

          case Domain::Type::T_INTERVAL_VECTOR:
          {
            vector<Domain*> v_doms(new_dom->interval_vector().size() + 1);
            v_doms[0] = new_dom; // main vector

            // And its components
            for(int i = 0 ; i < new_dom->interval_vector().size() ; i++)
              v_doms[i+1] = add_dom(Domain::vector_component(*new_dom, i));

            Contractor *ac_component = add_ctc(Contractor(Contractor::Type::T_COMPONENT, v_doms));
            for(auto& dom_i : v_doms)
              dom_i->add_ctc(ac_component);
          }
          break;

          case Domain::Type::T_TUBE:
          {
            vector<Domain*> v_doms(new_dom->tube().nb_slices() + 1);
            v_doms[0] = new_dom;
            int i = 0;
            for(Slice *s = new_dom->tube().first_slice() ; s ; s = s->next_slice())
            {
              i++;
              v_doms[i] = add_dom(Domain(*s));
            }

            // Dependencies tube <-> slice
            Contractor *ac_component = add_ctc(Contractor(Contractor::Type::T_COMPONENT, v_doms));

            new_dom->add_ctc(ac_component);
            for(auto& dom_i : v_doms)
              dom_i->add_ctc(ac_component);

            // Dependencies slice <-> slice
            for(Slice *s = new_dom->tube().first_slice() ; s->next_slice() ; s = s->next_slice())
            {
              Domain *dom_i1 = add_dom(Domain(*s));
              Domain *dom_i2 = add_dom(Domain(*(s->next_slice())));

              Contractor *ac_component_slices = add_ctc(Contractor(Contractor::Type::T_COMPONENT, {dom_i1, dom_i2}));

              dom_i1->add_ctc(ac_component_slices);
              dom_i2->add_ctc(ac_component_slices);
            }
          }
          break;
        }

      return new_dom;
    }

    Contractor* ContractorNetwork::add_ctc(const Contractor& ac)
    {
      ContractorHashcode hash(ac);
      map<ContractorHashcode,Contractor*>::iterator it = m_map_ctc.find(hash);

      if(it == m_map_ctc.end())
      {
        // todo: trigger only "contracting" contractors?
        Contractor *new_ctc = new Contractor(ac);
        m_map_ctc[hash] = new_ctc;
        add_ctc_to_queue(new_ctc, m_deque);
        return new_ctc;
      }

      else
        return it->second;
    }
}