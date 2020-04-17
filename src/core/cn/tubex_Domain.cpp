/** 
 *  Domain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Tools.h"
#include "tubex_Domain.h"
#include "tubex_Figure.h" // for add_suffix

using namespace std;
using namespace ibex;

namespace tubex
{
  int Domain::dom_counter = 0;

  Domain::Domain(Type type, ExternalRef extern_object_type)
    : m_type(type), m_extern_object_type(extern_object_type)
  {
    dom_counter++;
    m_dom_id = dom_counter;
  }

  Domain::Domain(const Domain& ad)
    : Domain(ad.m_type, ad.m_extern_object_type)
  {
    m_volume = ad.m_volume;
    m_v_ctc = ad.m_v_ctc;
    m_name = ad.m_name;
    m_dom_id = ad.m_dom_id;

    // todo: verify the copy of the above pointers
    // todo: is this constructor useful?

    switch(ad.m_type)
    {
      case Type::INTERVAL:
        if(ad.m_i_ptr != NULL)
        {
          m_i_ptr = new Interval(*ad.m_i_ptr);
          m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
        }

        else
        {
          assert(ad.m_extern_object_type != ExternalRef::NONE);
          m_ref_values_i = reference_wrapper<Interval>(ad.m_ref_values_i);
        }
        break;

      case Type::INTERVAL_VECTOR:
        if(ad.m_iv_ptr != NULL)
        {
          m_iv_ptr = new IntervalVector(*ad.m_iv_ptr);
          m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
        }

        else
        {
          assert(ad.m_extern_object_type != ExternalRef::NONE);
          m_ref_values_iv = reference_wrapper<IntervalVector>(ad.m_ref_values_iv);
        }
        break;

      case Type::SLICE:
        m_ref_values_s = ad.m_ref_values_s;
        break;

      case Type::TUBE:
        m_ref_values_t = ad.m_ref_values_t;
        break;

      case Type::TUBE_VECTOR:
        m_ref_values_tv = ad.m_ref_values_tv;
        break;

      default:
        assert(false && "unhandled case");
    }
    
    switch(ad.m_extern_object_type)
    {
      case ExternalRef::NONE:
        // Nothing to do
        break;
        
      case ExternalRef::DOUBLE:
        m_ref_extern_object_d = ad.m_ref_extern_object_d;
        break;

      case ExternalRef::INTERVAL:
        m_ref_extern_object_i = ad.m_ref_extern_object_i;
        break;

      case ExternalRef::VECTOR:
        m_ref_extern_object_v = ad.m_ref_extern_object_v;
        break;

      case ExternalRef::INTERVAL_VECTOR:
        m_ref_extern_object_iv = ad.m_ref_extern_object_iv;
        break;

      case ExternalRef::SLICE:
        m_ref_extern_object_s = ad.m_ref_extern_object_s;
        break;

      case ExternalRef::TUBE:
        m_ref_extern_object_t = ad.m_ref_extern_object_t;
        break;

      case ExternalRef::TUBE_VECTOR:
        m_ref_extern_object_tv = ad.m_ref_extern_object_tv;
        break;

      default:
        assert(false && "unhandled case");
    }
  }

  Domain::Domain(double& d)
    : Domain(Type::INTERVAL, ExternalRef::DOUBLE)
  {
    m_i_ptr = new Interval(d);
    m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
    m_ref_extern_object_d = reference_wrapper<double>(d);
  }

  Domain::Domain(Interval& i)
    : Domain(Type::INTERVAL, ExternalRef::INTERVAL)
  {
    m_i_ptr = NULL;
    m_ref_values_i = reference_wrapper<Interval>(i);
    m_ref_extern_object_i = reference_wrapper<Interval>(i);
    dom_counter++;
    m_dom_id = dom_counter;
  }

  Domain::Domain(const Interval& i)
    : Domain(Type::INTERVAL, ExternalRef::NONE)
  {
    m_i_ptr = new Interval(i);
    m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
    m_ref_extern_object_i = reference_wrapper<Interval>(*m_i_ptr); // todo: remove this?
  }

  Domain::Domain(Vector& v)
    : Domain(Type::INTERVAL_VECTOR, ExternalRef::VECTOR)
  {
    m_iv_ptr = new IntervalVector(v);
    m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
    m_ref_extern_object_v = reference_wrapper<Vector>(v);
  }

  Domain::Domain(IntervalVector& iv)
    : Domain(Type::INTERVAL_VECTOR, ExternalRef::INTERVAL_VECTOR)
  {
    m_iv_ptr = NULL;
    m_ref_values_iv = reference_wrapper<IntervalVector>(iv);
    m_ref_extern_object_iv = reference_wrapper<IntervalVector>(iv);
  }

  Domain::Domain(const IntervalVector& iv)
    : Domain(Type::INTERVAL_VECTOR, ExternalRef::NONE)
  {
    m_iv_ptr = new IntervalVector(iv);
    m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
    m_ref_extern_object_iv = reference_wrapper<IntervalVector>(*m_iv_ptr); // todo: remove this?
  }

  Domain::Domain(Slice& s)
    : Domain(Type::SLICE, ExternalRef::SLICE)
  {
    m_ref_values_s = reference_wrapper<Slice>(s);
    m_ref_extern_object_s = reference_wrapper<Slice>(s);

    // todo: remove this (unbounded domains not supported for some contractors)
    s &= Slice(s.domain(),Interval(-99999.,99999.)); 
  }

  Domain::Domain(Tube& t)
    : Domain(Type::TUBE, ExternalRef::TUBE)
  {
    m_ref_values_t = reference_wrapper<Tube>(t);
    m_ref_extern_object_t = reference_wrapper<Tube>(t);

    // todo: remove this (unbounded domains not supported for some contractors)
    t &= Interval(-99999.,99999.);
  }

  Domain::Domain(const Tube& t)
    : Domain(Type::TUBE, ExternalRef::NONE)
  {
    m_t_ptr = new Tube(t);
    m_ref_values_t = reference_wrapper<Tube>(*m_t_ptr);

    // todo: remove this (unbounded domains not supported for some contractors)
    *m_t_ptr &= Interval(-99999.,99999.);
  }

  Domain::Domain(TubeVector& tv)
    : Domain(Type::TUBE_VECTOR, ExternalRef::TUBE_VECTOR)
  {
    m_ref_values_tv = reference_wrapper<TubeVector>(tv);
    m_ref_extern_object_tv = reference_wrapper<TubeVector>(tv);

    // todo: remove this (unbounded domains not supported for some contractors)
    tv &= IntervalVector(tv.size(), Interval(-99999.,99999.));
  }

  Domain::Domain(const TubeVector& tv)
    : Domain(Type::TUBE_VECTOR, ExternalRef::NONE)
  {
    m_tv_ptr = new TubeVector(tv);
    m_ref_values_tv = reference_wrapper<TubeVector>(*m_tv_ptr);

    // todo: remove this (unbounded domains not supported for some contractors)
    *m_tv_ptr &= IntervalVector(tv.size(), Interval(-99999.,99999.));
  }

  Domain::~Domain()
  {
    if(m_i_ptr != NULL) delete m_i_ptr;
    if(m_iv_ptr != NULL) delete m_iv_ptr;
  }

  int Domain::id() const
  {
    return m_dom_id;
  }

  Domain::Type Domain::type() const
  {
    return m_type;
  }

  Interval& Domain::interval()
  {
    assert(m_type == Type::INTERVAL);
    return const_cast<Interval&>(static_cast<const Domain&>(*this).interval());
  }

  const Interval& Domain::interval() const
  {
    assert(m_type == Type::INTERVAL);

    if(m_extern_object_type == ExternalRef::NONE)
      return *m_i_ptr;

    else
    {
      assert(m_extern_object_type == ExternalRef::DOUBLE || m_extern_object_type == ExternalRef::INTERVAL);
      return m_ref_values_i.get();
    }
  }

  IntervalVector& Domain::interval_vector()
  {
    assert(m_type == Type::INTERVAL_VECTOR);
    return const_cast<IntervalVector&>(static_cast<const Domain&>(*this).interval_vector());
  }

  const IntervalVector& Domain::interval_vector() const
  {
    assert(m_type == Type::INTERVAL_VECTOR);

    if(m_extern_object_type == ExternalRef::NONE)
      return *m_iv_ptr;

    else
    {
      assert(m_extern_object_type == ExternalRef::VECTOR || m_extern_object_type == ExternalRef::INTERVAL_VECTOR);
      return m_ref_values_iv.get();
    }
  }

  Slice& Domain::slice()
  {
    assert(m_type == Type::SLICE);
    return const_cast<Slice&>(static_cast<const Domain&>(*this).slice());
  }

  const Slice& Domain::slice() const
  {
    assert(m_type == Type::SLICE);
    return m_ref_values_s.get();
  }

  Tube& Domain::tube()
  {
    assert(m_type == Type::TUBE);
    return const_cast<Tube&>(static_cast<const Domain&>(*this).tube());
  }

  const Tube& Domain::tube() const
  {
    assert(m_type == Type::TUBE);
    return m_ref_values_t.get();
  }

  TubeVector& Domain::tube_vector()
  {
    assert(m_type == Type::TUBE_VECTOR);
    return const_cast<TubeVector&>(static_cast<const Domain&>(*this).tube_vector());
  }

  const TubeVector& Domain::tube_vector() const
  {
    assert(m_type == Type::TUBE_VECTOR);
    return m_ref_values_tv.get();
  }

  vector<Contractor*>& Domain::contractors()
  {
    return const_cast<vector<Contractor*>&>(static_cast<const Domain&>(*this).contractors());
  }

  const vector<Contractor*>& Domain::contractors() const
  {
    return m_v_ctc;
  }
  
  void Domain::add_ctc(Contractor *ctc)
  {
    //assert(find(m_v_ctc.begin(), m_v_ctc.end(), ctc) == m_v_ctc.end()); // not already added
    m_v_ctc.push_back(ctc);
  }

  double Domain::compute_volume() const
  {
    switch(m_type)
    {
      case Type::INTERVAL:
        if(interval().is_empty())
          return 0.;
        
        else if(interval().is_unbounded())
          return 999999.; // todo: manager the unbounded case for fixed point detection
        
        else
          return interval().diam();

      case Type::INTERVAL_VECTOR:
        return interval_vector().volume();

      case Type::SLICE:
      {
        double vol = slice().volume();

        // todo: clean the following:

        if(slice().input_gate().is_empty())
          vol += 0.;
        else if(slice().input_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += slice().input_gate().diam();

        if(slice().output_gate().is_empty())
          vol += 0.;
        else if(slice().output_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += slice().output_gate().diam();

        return vol;
      }

      case Type::TUBE:
      {
        double vol = tube().volume();
        vol += tube().first_slice()->input_gate().diam();
        for(const Slice *s = tube().first_slice() ; s != NULL ; s = s->next_slice())
          vol += s->output_gate().diam();
        return vol;
      }

      case Type::TUBE_VECTOR:
      {
        // todo: improve this
        double vol = 0.;

        for(int i = 0 ; i < tube_vector().size() ; i++)
        {
          vol += tube_vector()[i].volume();
          vol += tube_vector()[i].first_slice()->input_gate().diam();
          for(const Slice *s = tube_vector()[i].first_slice() ; s != NULL ; s = s->next_slice())
            vol += s->output_gate().diam();
        }
        
        return vol;
      }

      default:
        assert(false && "unhandled case");
    }

    return 0.;
  }

  double Domain::get_saved_volume() const
  {
    return m_volume;
  }

  void Domain::set_volume(double vol)
  {
    m_volume = vol;
  }

  bool Domain::is_empty() const
  {
    switch(m_type)
    {
      case Type::INTERVAL:
        return interval().is_empty();

      case Type::INTERVAL_VECTOR:
        return interval_vector().is_empty();

      case Type::SLICE:
        return slice().is_empty();

      case Type::TUBE:
        return tube().is_empty();

      case Type::TUBE_VECTOR:
        return tube_vector().is_empty();

      default:
        assert(false && "unhandled case");
    }

    return false;
  }
  
  bool Domain::operator==(const Domain& x) const
  {
    // Possibilities of equality:

    // - 1. The two objects point to the same external reference.
    //
    //    - 1.a. However, the domain is internally represented by a set.
    //           For instance, a double is represented by an interval stored in the CN.
    //   
    //    - 1.b. The two objects point to the same external reference.
    //           For instance, the same tubes, stored outside the CN.
    //
    // - 2. One is a created_var (points to a variable stored in the CN) and
    //      the other one is external (it points to the previously created var)


    // Case 1: the two objects point to the same external reference.
    if(m_extern_object_type == x.m_extern_object_type && m_extern_object_type != ExternalRef::NONE)
    {
      assert(m_type == x.m_type); // same external reference => same type of domain

      switch(m_extern_object_type)
      {
        case ExternalRef::DOUBLE:
          return &m_ref_extern_object_d.get() == &x.m_ref_extern_object_d.get();

        case ExternalRef::INTERVAL:
          return &m_ref_extern_object_i.get() == &x.m_ref_extern_object_i.get();

        case ExternalRef::VECTOR:
          return &m_ref_extern_object_v.get() == &x.m_ref_extern_object_v.get();

        case ExternalRef::INTERVAL_VECTOR:
          return &m_ref_extern_object_iv.get() == &x.m_ref_extern_object_iv.get();

        case ExternalRef::SLICE:
          return &m_ref_extern_object_s.get() == &x.m_ref_extern_object_s.get();

        case ExternalRef::TUBE:
          return &m_ref_extern_object_t.get() == &x.m_ref_extern_object_t.get();

        case ExternalRef::TUBE_VECTOR:
          return &m_ref_extern_object_tv.get() == &x.m_ref_extern_object_tv.get();

        default:
          assert(false && "unhandled case");
          return false;
      }
    }

    // Case 2: one is a created_var (points to a variable stored in the CN) and
    // the other one is external (it points to the previously created var)
    else if(m_extern_object_type != ExternalRef::NONE && x.m_extern_object_type == ExternalRef::NONE)
    {
      return x.operator==(*this); // permutation of arguments
    }
    
    else if(m_extern_object_type == ExternalRef::NONE && x.m_extern_object_type != ExternalRef::NONE)
    {
      if(m_type != x.m_type)
        return false;

      switch(m_type)
      {
        case Type::INTERVAL:
          return &m_ref_values_i.get() == &x.m_ref_extern_object_i.get();

        case Type::INTERVAL_VECTOR:
          return &m_ref_values_iv.get() == &x.m_ref_extern_object_iv.get();

        case Type::SLICE:
          return &m_ref_values_s.get() == &x.m_ref_extern_object_s.get();

        case Type::TUBE:
          return &m_ref_values_t.get() == &x.m_ref_extern_object_t.get();

        case Type::TUBE_VECTOR:
          return &m_ref_values_tv.get() == &x.m_ref_values_tv.get();

        default:
          assert(false && "unhandled case");
          return false;
      }
    }

    else
      return false;
  }
  
  bool Domain::operator!=(const Domain& x) const
  {
    return !operator==(x);
    // todo: faster implementation?
  }
  
  bool Domain::is_component_of(const Domain& x) const
  {
    int id;
    return is_component_of(x, id);
  }

  bool Domain::is_component_of(const Domain& x, int& component_id) const
  {
    if((m_type == Type::INTERVAL && x.type() == Type::INTERVAL_VECTOR) || (m_type == Type::TUBE && x.type() == Type::TUBE_VECTOR))
    {
      switch(x.type())
      {
        case Type::INTERVAL_VECTOR:
          for(int i = 0 ; i < x.interval_vector().size() ; i++)
            if(&x.interval_vector()[i] == &interval())
            {
              component_id = i;
              return true;
            }
          break;

        default:
          return false;
      }
    }

    return false;
  }
  
  bool Domain::is_slice_of(const Domain& x) const
  {
    int id;
    return is_slice_of(x, id);
  }

  bool Domain::is_slice_of(const Domain& x, int& slice_id) const
  {
    if(m_type == Type::SLICE && x.type() == Type::TUBE)
    {
      slice_id = 0;
      for(const Slice *s = x.tube().first_slice() ; s != NULL ; s=s->next_slice())
      {
        if(s == &slice())
          return true;

        slice_id++;
      }
    }

    return false;
  }

  ostream& operator<<(ostream& str, const Domain& x)
  {
    str << "[" << Tools::add_int("type",(int)x.m_type) << "," << Tools::add_int("extern_type",(int)x.m_extern_object_type) << "]" << flush;

    switch(x.m_type)
    {
      case Domain::Type::INTERVAL:
        str << "Interval: " << x.interval() << flush;
        break;

      case Domain::Type::INTERVAL_VECTOR:
        str << "IntervalVector: " << x.interval_vector() << flush;
        break;

      case Domain::Type::SLICE:
        str << "Slice: " << x.slice() << flush;
        break;

      case Domain::Type::TUBE:
        str << "Tube: " << x.tube() << flush;
        break;

      case Domain::Type::TUBE_VECTOR:
        str << "TubeVector: " << x.tube_vector() << flush;
        break;

      default:
        assert(false && "unhandled case");
        break;
    }

    return str;
  }
  
  void Domain::add_data(double t, const Interval& y, ContractorNetwork& cn)
  {
    assert(m_type == Type::TUBE);

    m_map_data_s_lb.emplace(t, y.lb());
    m_map_data_s_ub.emplace(t, y.ub());
    Trajectory traj_lb(m_map_data_s_lb);
    Trajectory traj_ub(m_map_data_s_ub);

    Slice *prev_s = NULL;
    if(tube().domain().contains(t))
      prev_s = tube().slice(t)->prev_slice();
    else if(t > tube().domain().ub())
      prev_s = tube().last_slice();

    while(prev_s != NULL && prev_s->domain().is_subset(traj_lb.domain()))
    {
      Interval new_slice_envelope = (traj_lb(prev_s->domain()) | traj_ub(prev_s->domain()));

      if(!prev_s->codomain().is_superset(new_slice_envelope))
        break;

      prev_s->set_envelope(new_slice_envelope);

      // Flags a new change on the domain
      cn.propag_active_ctc_from_dom(cn.add_dom(Domain(*prev_s)));

      prev_s = prev_s->prev_slice();
    }
  }
  
  void Domain::add_data(double t, const IntervalVector& y, ContractorNetwork& cn)
  {
    assert(m_type == Type::TUBE_VECTOR);
    assert(tube_vector().size() == y.size());

    m_map_data_lb.emplace(t, y.lb());
    m_map_data_ub.emplace(t, y.ub());
    TrajectoryVector traj_lb(m_map_data_lb);
    TrajectoryVector traj_ub(m_map_data_ub);

    for(int i = 0 ; i < y.size() ; i++)
    {
      Slice *prev_s = NULL;
      if(tube_vector()[i].domain().contains(t))
        prev_s = tube_vector()[i].slice(t)->prev_slice();
      else if(t > tube_vector()[i].domain().ub())
        prev_s = tube_vector()[i].last_slice();

      while(prev_s != NULL && prev_s->domain().is_subset(traj_lb.domain()))
      {
        Interval new_slice_envelope = (traj_lb[i](prev_s->domain()) | traj_ub[i](prev_s->domain()));

        if(!prev_s->codomain().is_superset(new_slice_envelope))
          break;

        prev_s->set_envelope(new_slice_envelope);

        // Flags a new change on the domain
        cn.propag_active_ctc_from_dom(cn.add_dom(Domain(*prev_s)));

        prev_s = prev_s->prev_slice();
      }
    }
  }
  
  const string Domain::var_name(const vector<Domain*>& v_domains) const
  {
    string output_name = m_name;

    if(output_name.empty()) // looking for dependencies
    {
      switch(m_type)
      {
        // The variable may be a component of a vector one
        case Type::INTERVAL:
        case Type::TUBE:
          for(const auto& dom : v_domains) // looking for this possible vector
          {
            if(dom != this)
            {
              if(dom->type() == Type::INTERVAL_VECTOR || dom->type() == Type::TUBE_VECTOR)
              {
                int component_id = 0;
                if(is_component_of(*dom, component_id))
                  output_name = Tools::add_int(dom->var_name(v_domains), component_id+1); // adding component id
              }
            }
          }
          break;

        // The variable may be a slice of a tube
        case Type::SLICE:
          for(const auto& dom : v_domains) // looking for this possible vector
          {
            if(dom != this && dom->type() == Type::TUBE)
            {
              int slice_id = 0;
              if(is_slice_of(*dom, slice_id))
              {
                output_name = Tools::add_int(dom->var_name(v_domains), "^{(", slice_id+1, ")}"); // adding slice id
              }
            }
          }
          break;

        default:
          // Nothing to do
          break;
      }
    }

    if(output_name.empty() || output_name.find("?") != string::npos) // looking for equalities
    {
      output_name = ""; //reset
      
      // The variable may be an alias of another one (equality)
      for(const auto& ctc : m_v_ctc) // looking for a contractor of equality
      {
        if(ctc->type() == Contractor::Type::EQUALITY)
        {
          for(const auto& dom : ctc->domains())
          {
            if(dom != this)
            {
              string dom_var_name = dom->var_name(v_domains);
              if(!dom_var_name.empty() && dom_var_name.find("?") == string::npos)
                output_name += (!output_name.empty() ? "/" : "") + dom_var_name;
            }
          }
        }
      }
    }
    
    if(output_name.empty())
      output_name = "?";

    return output_name;
  }
  
  void Domain::set_name(const string& name)
  {
    m_name = name;
  }

  const string Domain::dom_name(const vector<Domain*>& v_domains) const
  {
    string output_name = var_name(v_domains);

    switch(m_type)
    {
      case Type::INTERVAL_VECTOR:
      case Type::TUBE_VECTOR:
        output_name = "\\mathbf{" + output_name + "}";
        break;

      default:
      {
        // Nothing
      }
    }

    switch(m_extern_object_type)
    {
      case ExternalRef::INTERVAL:
      case ExternalRef::INTERVAL_VECTOR:
      case ExternalRef::TUBE:
      case ExternalRef::TUBE_VECTOR:
      case ExternalRef::NONE:
        output_name = "[" + output_name + "]";
        break;

      case ExternalRef::SLICE:
        output_name = "[\\![" + output_name + "]\\!]";
        break;

      default:
      {
        // Nothing
      }
    }

    if(m_type == Type::TUBE || m_type == Type::TUBE_VECTOR)
      output_name += "(\\cdot)";

    return output_name;
  }
}