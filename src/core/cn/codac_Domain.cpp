/** 
 *  Domain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Tools.h"
#include "codac_Domain.h"
#include "codac_Figure.h" // for add_suffix
#include "codac_Exception.h"
#include "codac_ContractorNetwork.h"

using namespace std;

namespace codac
{
  int Domain::dom_counter = 0;

  Domain::Domain()
    : m_type(Type::T_INTERVAL), m_memory_type(MemoryRef::M_DOUBLE)
  {
    // todo: change these parameters ^
  }

  Domain::Domain(Type type, MemoryRef memory_type)
    : m_type(type), m_memory_type(memory_type)
  {
    dom_counter++;
    m_dom_id = dom_counter;

    switch(m_type)
    {
      case Type::T_INTERVAL:
        m_i_ptr = nullptr;
        m_init_i_ptr = nullptr;
        break;

      case Type::T_INTERVAL_VECTOR:
        m_iv_ptr = nullptr;
        m_init_iv_ptr = nullptr;
        break;

      case Type::T_TUBE:
        m_t_ptr = nullptr;
        break;

      case Type::T_TUBE_VECTOR:
        m_tv_ptr = nullptr;
        break;

      default:
        m_i_ptr = nullptr; // default value of the union
        m_init_i_ptr = nullptr;
        break;
    }
  }

  Domain::Domain(const Domain& ad)
    : Domain(ad.m_type, ad.m_memory_type)
  {
    *this = ad;
    // todo: keep default arguments above?
  }

  Domain::Domain(double& d)
    : Domain(Type::T_INTERVAL, MemoryRef::M_DOUBLE)
  {
    m_i_ptr = new Interval(d);
    m_init_i_ptr = nullptr;
    m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
    m_ref_memory_d = reference_wrapper<double>(d);
  }

  Domain::Domain(Interval& i)
    : Domain(Type::T_INTERVAL, MemoryRef::M_INTERVAL)
  {
    m_i_ptr = nullptr;
    m_init_i_ptr = nullptr;
    m_ref_values_i = reference_wrapper<Interval>(i);
    m_ref_memory_i = reference_wrapper<Interval>(i);
  }
  
  Domain::Domain(Interval& i, double& extern_d)
    : Domain(Type::T_INTERVAL, MemoryRef::M_DOUBLE)
  {
    m_i_ptr = nullptr;
    m_init_i_ptr = nullptr;
    m_ref_values_i = reference_wrapper<Interval>(i);
    m_ref_memory_d = reference_wrapper<double>(extern_d);
  }
  
  Domain::Domain(Interval& i, Interval& extern_i)
    : Domain(Type::T_INTERVAL, MemoryRef::M_INTERVAL)
  {
    m_i_ptr = nullptr;
    m_init_i_ptr = nullptr;
    m_ref_values_i = reference_wrapper<Interval>(i);
    m_ref_memory_i = reference_wrapper<Interval>(extern_i);
  }

  Domain::Domain(const Interval& i, bool interm_var)
    : Domain(Type::T_INTERVAL, MemoryRef::M_INTERVAL)
  {
    m_i_ptr = new Interval(i);
    m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
    m_ref_memory_i = reference_wrapper<Interval>(*m_i_ptr); // todo: use const ref here?
  
    if(interm_var)
      m_init_i_ptr = new Interval(i);
    else
      m_init_i_ptr = nullptr;
  }

  Domain::Domain(IntervalVar& i)
    : Domain(Type::T_INTERVAL, MemoryRef::M_INTERVAL_VAR)
  {
    m_i_ptr = nullptr;
    m_init_i_ptr = nullptr;
    m_ref_values_i = reference_wrapper<Interval>(static_cast<Interval&>(i));
    m_ref_memory_ivar = reference_wrapper<IntervalVar>(i);
  }

  Domain::Domain(Vector& v)
    : Domain(Type::T_INTERVAL_VECTOR, MemoryRef::M_VECTOR)
  {
    m_iv_ptr = new IntervalVector(v);
    m_init_iv_ptr = nullptr;
    m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
    m_ref_memory_v = reference_wrapper<Vector>(v);
  }

  // todo ? Domain::Domain(const Vector& v)
  // todo ?   : Domain(Type::T_INTERVAL_VECTOR, MemoryRef::M_INTERVAL_VECTOR)
  // todo ? {
  // todo ?   m_iv_ptr = new IntervalVector(v);
  // todo ?   m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
  // todo ?   m_ref_memory_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
  // todo ? }

  Domain::Domain(IntervalVector& iv)
    : Domain(Type::T_INTERVAL_VECTOR, MemoryRef::M_INTERVAL_VECTOR)
  {
    m_iv_ptr = nullptr;
    m_init_iv_ptr = nullptr;
    m_ref_values_iv = reference_wrapper<IntervalVector>(iv);
    m_ref_memory_iv = reference_wrapper<IntervalVector>(iv);
  }

  Domain::Domain(const IntervalVector& iv, bool interm_var)
    : Domain(Type::T_INTERVAL_VECTOR, MemoryRef::M_INTERVAL_VECTOR)
  {
    m_iv_ptr = new IntervalVector(iv);
    m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
    m_ref_memory_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);

    if(interm_var)
      m_init_iv_ptr = new IntervalVector(iv);
    else
      m_init_iv_ptr = nullptr;
  }

  Domain::Domain(IntervalVectorVar& iv)
    : Domain(Type::T_INTERVAL_VECTOR, MemoryRef::M_INTERVAL_VECTOR_VAR)
  {
    m_iv_ptr = nullptr;
    m_init_iv_ptr = nullptr;
    m_ref_values_iv = reference_wrapper<IntervalVector>(static_cast<IntervalVector&>(iv));
    m_ref_memory_ivvar = reference_wrapper<IntervalVectorVar>(iv);
  }

  Domain::Domain(Slice& s)
    : Domain(Type::T_SLICE, MemoryRef::M_SLICE)
  {
    m_ref_values_s = reference_wrapper<Slice>(s);
    m_ref_memory_s = reference_wrapper<Slice>(s);

    // todo: remove this (unbounded domains not supported for some contractors)
    s &= Slice(s.tdomain(),Interval(-99999.,99999.)); 
  }

  Domain::Domain(Tube& t)
    : Domain(Type::T_TUBE, MemoryRef::M_TUBE)
  {
    m_init_t_ptr = nullptr;
    m_ref_values_t = reference_wrapper<Tube>(t);
    m_ref_memory_t = reference_wrapper<Tube>(t);

    // todo: remove this (unbounded domains not supported for some contractors)
    t &= Interval(-99999.,99999.);
  }

  Domain::Domain(const Tube& t, bool interm_var)
    : Domain(Type::T_TUBE, MemoryRef::M_TUBE)
  {
    m_t_ptr = new Tube(t);
    m_ref_values_t = reference_wrapper<Tube>(*m_t_ptr);
    m_ref_memory_t = reference_wrapper<Tube>(*m_t_ptr);

    // todo: remove this (unbounded domains not supported for some contractors)
    *m_t_ptr &= Interval(-99999.,99999.);

    if(interm_var)
      m_init_t_ptr = new Tube(t);
    else
      m_init_t_ptr = nullptr;
  }

  Domain::Domain(TubeVector& tv)
    : Domain(Type::T_TUBE_VECTOR, MemoryRef::M_TUBE_VECTOR)
  {
    m_init_tv_ptr = nullptr;
    m_ref_values_tv = reference_wrapper<TubeVector>(tv);
    m_ref_memory_tv = reference_wrapper<TubeVector>(tv);

    // todo: remove this (unbounded domains not supported for some contractors)
    tv &= IntervalVector(tv.size(), Interval(-99999.,99999.));
  }

  Domain::Domain(const TubeVector& tv, bool interm_var)
    : Domain(Type::T_TUBE_VECTOR, MemoryRef::M_TUBE_VECTOR)
  {
    m_tv_ptr = new TubeVector(tv);
    m_ref_values_tv = reference_wrapper<TubeVector>(*m_tv_ptr);
    m_ref_memory_tv = reference_wrapper<TubeVector>(*m_tv_ptr);

    // todo: remove this (unbounded domains not supported for some contractors)
    *m_tv_ptr &= IntervalVector(tv.size(), Interval(-99999.,99999.));

    if(interm_var)
      m_init_tv_ptr = new TubeVector(tv);
    else
      m_init_tv_ptr = nullptr;
  }

  Domain::~Domain()
  {
    // todo: manage the delete of pointers

    switch(m_type)
    {
      case Type::T_INTERVAL:
        // todo: if(m_i_ptr) delete m_i_ptr;
        if(m_init_i_ptr) delete m_init_i_ptr;
        break;

      case Type::T_INTERVAL_VECTOR:
        // todo: if(m_iv_ptr) delete m_iv_ptr;
        if(m_init_iv_ptr) delete m_init_iv_ptr;
        break;

      case Type::T_TUBE:
        // todo: if(m_t_ptr) delete m_t_ptr;
        if(m_init_t_ptr) delete m_init_t_ptr;
        break;

      case Type::T_TUBE_VECTOR:
        // todo: if(m_tv_ptr) delete m_tv_ptr;
        if(m_init_tv_ptr) delete m_init_tv_ptr;
        break;

      default:
        // Nothing else to manage
        break;
    }
  }

  const Domain& Domain::operator=(const Domain& ad)
  {
    m_v_ctc = ad.m_v_ctc;
    m_name = ad.m_name;
    m_dom_id = ad.m_dom_id;

    m_type = ad.m_type;
    m_memory_type = ad.m_memory_type;

    // todo: verify the copy of the above pointers
    // todo: is this constructor useful?

    set_ref_values(ad);

    switch(ad.m_memory_type)
    {
      case MemoryRef::M_DOUBLE:
        m_ref_memory_d = ad.m_ref_memory_d;
        break;

      case MemoryRef::M_INTERVAL:
        /*
        // The following has been removed on 2020.12.02
        if(&ad.m_ref_memory_i.get() == ad.m_i_ptr)
          m_ref_memory_i = reference_wrapper<Interval>(*m_i_ptr);
        else*/
          m_ref_memory_i = ad.m_ref_memory_i;
        break;

      case MemoryRef::M_INTERVAL_VAR:
        m_ref_memory_ivar = ad.m_ref_memory_ivar;
        break;

      case MemoryRef::M_VECTOR:
        m_ref_memory_v = ad.m_ref_memory_v;
        break;

      case MemoryRef::M_INTERVAL_VECTOR:
        /*
        // The following has been removed on 2020.12.02
        if(&ad.m_ref_memory_iv.get() == ad.m_iv_ptr)
          m_ref_memory_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
        else*/
          m_ref_memory_iv = ad.m_ref_memory_iv;
        break;

      case MemoryRef::M_INTERVAL_VECTOR_VAR:
        m_ref_memory_ivvar = ad.m_ref_memory_ivvar;
        break;

      case MemoryRef::M_SLICE:
        m_ref_memory_s = ad.m_ref_memory_s;
        break;

      case MemoryRef::M_TUBE:
        m_ref_memory_t = ad.m_ref_memory_t;
        break;

      case MemoryRef::M_TUBE_VECTOR:
        m_ref_memory_tv = ad.m_ref_memory_tv;
        break;

      default:
        assert(false && "unhandled case");
    }

    return *this;
  }

  void Domain::set_ref_values(const Domain& ad)
  {
    assert(m_type == ad.m_type);

    m_volume = ad.m_volume;

    switch(ad.m_type)
    {
      case Type::T_INTERVAL:
        m_ref_values_i = reference_wrapper<Interval>(ad.m_ref_values_i);

        if(ad.m_init_i_ptr)
          m_init_i_ptr = new Interval(*ad.m_init_i_ptr);
        else
          m_init_i_ptr = nullptr;
        break;

      case Type::T_INTERVAL_VECTOR:
        m_ref_values_iv = reference_wrapper<IntervalVector>(ad.m_ref_values_iv);

        if(ad.m_init_iv_ptr)
          m_init_iv_ptr = new IntervalVector(*ad.m_init_iv_ptr);
        else
          m_init_iv_ptr = nullptr;
        break;

      case Type::T_SLICE:
        m_ref_values_s = ad.m_ref_values_s;
        break;

      case Type::T_TUBE:
        m_ref_values_t = ad.m_ref_values_t;

        if(ad.m_init_t_ptr)
          m_init_t_ptr = new Tube(*ad.m_init_t_ptr);
        else
          m_init_t_ptr = nullptr;
        break;

      case Type::T_TUBE_VECTOR:
        m_ref_values_tv = ad.m_ref_values_tv;

        if(ad.m_init_tv_ptr)
          m_init_tv_ptr = new TubeVector(*ad.m_init_tv_ptr);
        else
          m_init_tv_ptr = nullptr;
        break;

      default:
        assert(false && "unhandled case");
    }
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
    assert(m_type == Type::T_INTERVAL);
    return const_cast<Interval&>(static_cast<const Domain&>(*this).interval());
  }

  const Interval& Domain::interval() const
  {
    assert(m_type == Type::T_INTERVAL);
    return m_ref_values_i.get();
  }

  IntervalVector& Domain::interval_vector()
  {
    assert(m_type == Type::T_INTERVAL_VECTOR);
    return const_cast<IntervalVector&>(static_cast<const Domain&>(*this).interval_vector());
  }

  const IntervalVector& Domain::interval_vector() const
  {
    assert(m_type == Type::T_INTERVAL_VECTOR);
    return m_ref_values_iv.get();
  }

  Slice& Domain::slice()
  {
    assert(m_type == Type::T_SLICE);
    return const_cast<Slice&>(static_cast<const Domain&>(*this).slice());
  }

  const Slice& Domain::slice() const
  {
    assert(m_type == Type::T_SLICE);
    return m_ref_values_s.get();
  }

  Tube& Domain::tube()
  {
    assert(m_type == Type::T_TUBE);
    return const_cast<Tube&>(static_cast<const Domain&>(*this).tube());
  }

  const Tube& Domain::tube() const
  {
    assert(m_type == Type::T_TUBE);
    return m_ref_values_t.get();
  }

  TubeVector& Domain::tube_vector()
  {
    assert(m_type == Type::T_TUBE_VECTOR);
    return const_cast<TubeVector&>(static_cast<const Domain&>(*this).tube_vector());
  }

  const TubeVector& Domain::tube_vector() const
  {
    assert(m_type == Type::T_TUBE_VECTOR);
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
    //assert(find(m_v_ctc.begin(), m_v_ctc.end(), ctc) == m_v_ctc.end()); // not already added (todo?)
    m_v_ctc.push_back(ctc);
  }

  bool Domain::is_var() const
  {
    return m_memory_type == MemoryRef::M_INTERVAL_VAR || m_memory_type == MemoryRef::M_INTERVAL_VECTOR_VAR;
  }

  bool Domain::is_var_not_associated() const
  {
    if(is_var())
    {
      switch(m_type)
      {
        case Type::T_INTERVAL:
          return &m_ref_values_i.get() == &m_ref_memory_ivar.get();

        case Type::T_INTERVAL_VECTOR:
          return &m_ref_values_iv.get() == &m_ref_memory_ivvar.get();

        default:
          assert(false && "unhandled case");
      }
    }

    return false;
  }

  double Domain::compute_volume() const
  {
    switch(m_type)
    {
      case Type::T_INTERVAL:
        if(interval().is_empty())
          return 0.;
        
        else if(interval().is_unbounded())
          return 999999.; // todo: manage the unbounded case for fixed point detection
        
        else
          return interval().diam();

      case Type::T_INTERVAL_VECTOR:
        return interval_vector().volume();

      case Type::T_SLICE:
      {
        double vol = slice().volume();

        // todo: clean the following:

        if(slice().input_gate().is_empty())
          vol += 0.;
        else if(slice().input_gate().is_unbounded())
          vol += 999999.; // todo: manage the unbounded case for fixed point detection
        else
          vol += slice().input_gate().diam();

        if(slice().output_gate().is_empty())
          vol += 0.;
        else if(slice().output_gate().is_unbounded())
          vol += 999999.; // todo: manage the unbounded case for fixed point detection
        else
          vol += slice().output_gate().diam();

        return vol;
      }

      case Type::T_TUBE:
      {
        double vol = tube().volume();
        vol += tube().first_slice()->input_gate().diam();
        for(const Slice *s = tube().first_slice() ; s ; s = s->next_slice())
          vol += s->output_gate().diam();
        return vol;
      }

      case Type::T_TUBE_VECTOR:
      {
        // todo: improve this
        double vol = 0.;

        for(int i = 0 ; i < tube_vector().size() ; i++)
        {
          vol += tube_vector()[i].volume();
          vol += tube_vector()[i].first_slice()->input_gate().diam();
          for(const Slice *s = tube_vector()[i].first_slice() ; s ; s = s->next_slice())
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

  bool Domain::is_interm_var() const
  {
    switch(m_type)
    {
      case Type::T_INTERVAL:
        return m_init_i_ptr;

      case Type::T_INTERVAL_VECTOR:
        return m_init_iv_ptr;

      case Type::T_SLICE:
        return false; // to be changed

      case Type::T_TUBE:
        return m_init_t_ptr;

      case Type::T_TUBE_VECTOR:
        return m_init_tv_ptr;

      default:
        assert(false && "unhandled case");
        return false;
    }
  }

  void Domain::reset_value()
  {
    assert(is_interm_var());

    m_volume = -1.;
    
    switch(m_type)
    {
      case Type::T_INTERVAL:
        assert(m_init_i_ptr);
        m_ref_values_i.get() = *m_init_i_ptr;
        break;

      case Type::T_INTERVAL_VECTOR:
        assert(m_init_iv_ptr);
        m_ref_values_iv.get() = *m_init_iv_ptr;
        break;

      case Type::T_TUBE:
        assert(m_init_t_ptr);
        m_ref_values_t.get() = *m_init_t_ptr;
        break;

      case Type::T_TUBE_VECTOR:
        assert(m_init_tv_ptr);
        m_ref_values_tv.get() = *m_init_tv_ptr;
        break;

      default:
        assert(false && "unhandled case");
    }
  }

  bool Domain::is_empty() const
  {
    switch(m_type)
    {
      case Type::T_INTERVAL:
        assert(m_memory_type == MemoryRef::M_DOUBLE
            || m_memory_type == MemoryRef::M_INTERVAL
            || m_memory_type == MemoryRef::M_INTERVAL_VAR);
        return interval().is_empty();
        break;

      case Type::T_INTERVAL_VECTOR:
        assert(m_memory_type == MemoryRef::M_VECTOR
            || m_memory_type == MemoryRef::M_INTERVAL_VECTOR
            || m_memory_type == MemoryRef::M_INTERVAL_VECTOR_VAR);
        return interval_vector().is_empty();
        break;

      case Type::T_SLICE:
        assert(m_memory_type == MemoryRef::M_SLICE);
        return slice().is_empty();
        break;

      case Type::T_TUBE:
        assert(m_memory_type == MemoryRef::M_TUBE);
        return tube().is_empty();
        break;

      case Type::T_TUBE_VECTOR:
        assert(m_memory_type == MemoryRef::M_TUBE_VECTOR);
        return tube_vector().is_empty();
        break;

      default:
        assert(false && "unhandled case");
    }

    return false;
  }
  
  bool Domain::operator==(const Domain& x) const
  {
    // Possibilities of equality:

    // - 1. The two objects point to the same memory reference.
    //
    //    - 1.a. However, the domain is internally represented by a set.
    //           For instance, a double is represented by an interval stored in the CN.
    //   
    //    - 1.b. The two objects point to the same memory reference.
    //           For instance, the same tubes, stored outside the CN.
    //
    // - 2. One is a created_var (points to a variable stored in the CN) and
    //      the other one is external (it points to the previously created var)

    // Case 1: the two objects point to the same memory reference.
    if(m_memory_type == x.m_memory_type)
    {
      assert(m_type == x.m_type); // same memory reference => same type of domain

      switch(m_memory_type)
      {
        case MemoryRef::M_DOUBLE:
        {
          if(&m_ref_memory_d.get() == &x.m_ref_memory_d.get())
          {
            // This happens if a variable has changed since its last add,
            // for instance when iterating a "t" inside a loop of constraints.
            if(m_ref_values_i.get() != x.m_ref_values_i.get())
              throw Exception(__func__, "Values have changed since last add (double type). Use create_interm_var for local variables.");
          }
          return &m_ref_memory_d.get() == &x.m_ref_memory_d.get();
        }

        case MemoryRef::M_INTERVAL:
          return &m_ref_memory_i.get() == &x.m_ref_memory_i.get();

        case MemoryRef::M_INTERVAL_VAR:
          return &m_ref_memory_ivar.get() == &x.m_ref_memory_ivar.get();

        case MemoryRef::M_VECTOR:
        {
          if(&m_ref_memory_v.get() == &x.m_ref_memory_v.get())
          {
            // This happens if a variable has changed since its last add,
            // for instance when iterating a "t" inside a loop of constraints.
            if(m_ref_values_iv.get() != x.m_ref_values_iv.get())
              throw Exception(__func__, "Values have changed since last add (Vector type). Use create_interm_var for local variables.");
          }
          return &m_ref_memory_v.get() == &x.m_ref_memory_v.get();
        }

        case MemoryRef::M_INTERVAL_VECTOR:
          return &m_ref_memory_iv.get() == &x.m_ref_memory_iv.get();

        case MemoryRef::M_INTERVAL_VECTOR_VAR:
          return &m_ref_memory_ivvar.get() == &x.m_ref_memory_ivvar.get();

        case MemoryRef::M_SLICE:
          return &m_ref_memory_s.get() == &x.m_ref_memory_s.get();

        case MemoryRef::M_TUBE:
          return &m_ref_memory_t.get() == &x.m_ref_memory_t.get();

        case MemoryRef::M_TUBE_VECTOR:
          return &m_ref_memory_tv.get() == &x.m_ref_memory_tv.get();

        default:
          assert(false && "unhandled case");
          return false;
      }
    }

    // Case 2: one is a created_var (points to a variable stored in the CN) and
    // the other one is external (it points to the previously created var)
    switch(m_type)
    {
      case Type::T_INTERVAL:
        return &m_ref_values_i.get() == &x.m_ref_memory_i.get()
          || &x.m_ref_values_i.get() == &m_ref_memory_i.get();

      case Type::T_INTERVAL_VECTOR:
        return &m_ref_values_iv.get() == &x.m_ref_memory_iv.get()
          || &x.m_ref_values_iv.get() == &m_ref_memory_iv.get();

      case Type::T_SLICE:
        return &m_ref_values_s.get() == &x.m_ref_memory_s.get()
          || &x.m_ref_values_s.get() == &m_ref_memory_s.get();

      case Type::T_TUBE:
        return &m_ref_values_t.get() == &x.m_ref_memory_t.get()
          || &x.m_ref_values_t.get() == &m_ref_memory_t.get();

      case Type::T_TUBE_VECTOR:
        return &m_ref_values_tv.get() == &x.m_ref_memory_tv.get()
          || &x.m_ref_values_tv.get() == &m_ref_memory_tv.get();

      default:
        assert(false && "unhandled case");
        return false;

    }
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
    if((m_type == Type::T_INTERVAL && x.type() == Type::T_INTERVAL_VECTOR) || (m_type == Type::T_TUBE && x.type() == Type::T_TUBE_VECTOR))
    {
      switch(x.type())
      {
        case Type::T_INTERVAL_VECTOR:
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
    if(m_type == Type::T_SLICE && x.type() == Type::T_TUBE)
    {
      slice_id = 0;
      for(const Slice *s = x.tube().first_slice() ; s ; s=s->next_slice())
      {
        if(s == &slice())
          return true;

        slice_id++;
      }
    }

    return false;
  }
  
  void Domain::add_data(double t, const Interval& y, ContractorNetwork& cn)
  {
    assert(m_type == Type::T_TUBE);
    // Note: t may be defined outside the tube definition

    if(m_traj_lb.not_defined())
    {
      m_traj_lb.set(y.lb(), t);
      m_traj_ub.set(y.ub(), t);
      return; // cannot add data with a single point
    }

    double prev_t = m_traj_lb.tdomain().ub();
    if(t <= prev_t)
      throw Exception(__func__, "t does not represent new data since last call");

    // Updating the trajectory
    m_traj_lb.set(y.lb(), t);
    m_traj_ub.set(y.ub(), t);

    if(prev_t < tube().tdomain().lb())
      return; // nothing can be done yet (outside tube definition)

    Slice *prev_s;

    if(t < tube().tdomain().ub())
    {
      prev_s = tube().slice(t);
      if(prev_s == tube().first_slice())
        return; // the slice is not complete yet, and the previous one does not exist

      prev_s = prev_s->prev_slice();
    }

    else // if data goes beyond tube's definition domain
    {
      prev_s = tube().last_slice();
    }

    // Contracting the tube
    // A jump may have been done: several slices may exist between the current
    // t and the previous one.

    // So we iterate:
    while(prev_s && prev_s->tdomain().is_subset(m_traj_lb.tdomain()))
    {
      Interval new_slice_envelope = (m_traj_lb(prev_s->tdomain()) | m_traj_ub(prev_s->tdomain()));

      if(prev_s->codomain().is_subset(new_slice_envelope))
        break;

      prev_s->set_envelope(new_slice_envelope);

      // Flags a new change on the slice domain
      cn.trigger_ctc_related_to_dom(cn.add_dom(Domain(*prev_s)));

      // Iterates
      prev_s = prev_s->prev_slice();
    }
  }
  
  void Domain::add_data(double t, const IntervalVector& y, ContractorNetwork& cn)
  {
    assert(m_type == Type::T_TUBE_VECTOR);
    if(tube_vector().size() != y.size())
      throw Exception(__func__, "tube and box not of same dimension");

    for(int i = 0 ; i < tube_vector().size() ; i++)
    {
      Domain *tube_i = cn.add_dom(Domain(tube_vector()[i]));
      tube_i->add_data(t, y[i], cn);
    }
  }
  
  const string Domain::var_name(const map<DomainHashcode,Domain*> m_domains) const
  {
    string output_name = m_name;

    if(output_name.empty()) // looking for dependencies
    {
      switch(m_type)
      {
        // The variable may be a component of a vector one
        case Type::T_INTERVAL:
        case Type::T_TUBE:
          for(const auto& dom : m_domains) // looking for this possible vector
          {
            if(dom.second != this)
            {
              if(dom.second->type() == Type::T_INTERVAL_VECTOR || dom.second->type() == Type::T_TUBE_VECTOR)
              {
                int component_id = 0;
                if(is_component_of(*dom.second, component_id))
                  output_name = dom.second->var_name(m_domains) + std::to_string(component_id+1); // adding component id
              }
            }
          }
          break;

        // The variable may be a slice of a tube
        case Type::T_SLICE:
          for(const auto& dom : m_domains) // looking for this possible vector
          {
            if(dom.second != this && dom.second->type() == Type::T_TUBE)
            {
              int slice_id = 0;
              if(is_slice_of(*dom.second, slice_id))
              {
                output_name = dom.second->var_name(m_domains) + "^{(" + std::to_string(slice_id+1) + ")}"; // adding slice id
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
        if(ctc->type() == Contractor::Type::T_EQUALITY)
        {
          for(const auto& dom : ctc->domains())
          {
            if(dom != this)
            {
              string dom_var_name = dom->var_name(m_domains);
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
  
  bool Domain::all_slices(const vector<Domain>& v_domains)
  {
    for(const auto& dom : v_domains)
      if(dom.type() != Type::T_SLICE)
        return false;
    return true;
  }
  
  bool Domain::all_dyn(const vector<Domain>& v_domains)
  {
    for(const auto& dom : v_domains)
      if(dom.type() != Type::T_SLICE && dom.type() != Type::T_TUBE && dom.type() != Type::T_TUBE_VECTOR)
        return false;
    return true;
  }
  
  bool Domain::dyn_same_slicing(const vector<Domain>& v_domains)
  {
    // If domains are tubes or tube vectors, they must share the same slicing
    const Tube *slicing_ref = nullptr;
    for(const auto& dom: v_domains)
    {
      switch(dom.type())
      {
        case Domain::Type::T_TUBE:
          if(slicing_ref == nullptr)
            slicing_ref = &dom.tube();
          else
            if(!Tube::same_slicing(dom.tube(), *slicing_ref))
              return false;
          break;

        case Domain::Type::T_TUBE_VECTOR:
          if(slicing_ref == nullptr)
            slicing_ref = &dom.tube_vector()[0]; // first component is used as reference
          else
            if(!TubeVector::same_slicing(dom.tube_vector(), *slicing_ref))
              return false;
          break;

        default:
          // nothing to do
          break;
      }
    }

    return true;
  }
  
  int Domain::total_size(const vector<Domain>& v_domains)
  {
    int n = 0;

    for(const auto& dom: v_domains)
    {
      switch(dom.type())
      {
        // Scalar types:
        case Domain::Type::T_TUBE:
        case Domain::Type::T_INTERVAL:
        case Domain::Type::T_SLICE:
          n++;
          break;

        // Vector types:
        case Domain::Type::T_INTERVAL_VECTOR:
          n+=dom.interval_vector().size();
          break;

        case Domain::Type::T_TUBE_VECTOR:
          n+=dom.tube_vector().size();
          break;

        default:
          assert(false && "unhandled case");
      }
    }

    return n;
  }

  const string Domain::dom_name(const map<DomainHashcode,Domain*> m_domains) const
  {
    string output_name = var_name(m_domains);

    switch(m_type)
    {
      case Type::T_INTERVAL_VECTOR:
      case Type::T_TUBE_VECTOR:
        output_name = "\\mathbf{" + output_name + "}";
        break;

      default:
      {
        // Nothing
      }
    }

    switch(m_memory_type)
    {
      case MemoryRef::M_INTERVAL:
      case MemoryRef::M_INTERVAL_VAR:
      case MemoryRef::M_INTERVAL_VECTOR:
      case MemoryRef::M_INTERVAL_VECTOR_VAR:
      case MemoryRef::M_TUBE:
      case MemoryRef::M_TUBE_VECTOR:
        output_name = "[" + output_name + "]";
        break;

      case MemoryRef::M_SLICE:
        output_name = "[\\![" + output_name + "]\\!]";
        break;

      default:
      {
        // Nothing
      }
    }

    if(m_type == Type::T_TUBE || m_type == Type::T_TUBE_VECTOR)
      output_name += "(\\cdot)";

    return output_name;
  }

  const string Domain::dom_type_str() const
  {
    switch(m_type)
    {
      case Domain::Type::T_INTERVAL:
        return "Interval";
      case Domain::Type::T_INTERVAL_VECTOR:
        return "IntervalVector";
      case Domain::Type::T_SLICE:
        return "Slice";
      case Domain::Type::T_TUBE:
        return "Tube";
      case Domain::Type::T_TUBE_VECTOR:
        return "TubeVector";
      default:
        assert(false && "unhandled case");
        return "unhandled case";
    }
  }

  ostream& operator<<(ostream& str, const Domain& x)
  {
    str << "Domain:";

    str << "  mem=";
    switch(x.m_memory_type)
    {
      case Domain::MemoryRef::M_DOUBLE:
        str << "double              ";
        break;
      case Domain::MemoryRef::M_INTERVAL:
        str << "Interval            ";
        break;
      case Domain::MemoryRef::M_INTERVAL_VAR:
        str << "Interval (var)      ";
        break;
      case Domain::MemoryRef::M_VECTOR:
        str << "Vector              ";
        break;
      case Domain::MemoryRef::M_INTERVAL_VECTOR:
        str << "IntervalVector      ";
        break;
      case Domain::MemoryRef::M_INTERVAL_VECTOR_VAR:
        str << "IntervalVector (var)";
        break;
      case Domain::MemoryRef::M_SLICE:
        str << "Slice               ";
        break;
      case Domain::MemoryRef::M_TUBE:
        str << "Tube                ";
        break;
      case Domain::MemoryRef::M_TUBE_VECTOR:
        str << "TubeVector          ";
        break;
      default:
        assert(false && "unhandled case");
    }

    str << (x.is_interm_var() ? " (interm)" : "         ");

    str << " name=\"" << (x.m_name == "" ? "?" : x.m_name) << "\"";

    str << "\tval=";
    switch(x.m_type)
    {
      case Domain::Type::T_INTERVAL:
        str << x.interval();
        break;
      case Domain::Type::T_INTERVAL_VECTOR:
        str << x.interval_vector();
        break;
      case Domain::Type::T_SLICE:
        str << x.slice();
        break;
      case Domain::Type::T_TUBE:
        str << x.tube();
        break;
      case Domain::Type::T_TUBE_VECTOR:
        str << x.tube_vector();
        break;
      default:
        assert(false && "unhandled case");
    }

    str << ", hash: " << DomainHashcode::uintptr(x);

    return str;
  }
  
  Domain Domain::vector_component(Domain& x, int i)
  {
    assert(x.type() == Type::T_INTERVAL_VECTOR || x.type() == Type::T_TUBE_VECTOR);

    // Builds a Domain object for the ith component of this vector Domain,
    // and makes it point to the component of the memory reference

    Domain d;

    switch(x.type())
    {
      case Type::T_INTERVAL_VECTOR:
        switch(x.m_memory_type)
        {
          case MemoryRef::M_VECTOR:
            d = Domain(x.interval_vector()[i], x.m_ref_memory_v.get()[i]);
            break;

          case MemoryRef::M_INTERVAL_VECTOR:
            d = Domain(x.interval_vector()[i], x.m_ref_memory_iv.get()[i]);
            break;

          case MemoryRef::M_INTERVAL_VECTOR_VAR:
            d = Domain(x.interval_vector()[i], x.m_ref_memory_ivvar.get()[i]);
            break;

          default:
            assert(false && "unhandled case");
        }
        break;

      case Type::T_TUBE_VECTOR:

        break;

      default:
        assert(false && "domain is not a vector");
    }

    return d;
  }
}