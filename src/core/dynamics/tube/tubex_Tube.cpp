/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_DimensionException.h"
#include "tubex_Arithmetic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Tube::Tube(TubeVector *tubevector, int component_id)
      : m_tubevector(tubevector), m_component_id(component_id)
    {
      
    }

    Tube::Tube(const Interval& domain, const Interval& codomain)
    {
      m_tubevector = new TubeVector(domain, IntervalVector(1, codomain));
    }

    Tube::Tube(const Interval& domain, double timestep, const Interval& codomain)
    {
      m_tubevector = new TubeVector(domain, timestep, IntervalVector(1, codomain));
    }

    Tube::Tube(const Interval& domain, double timestep, const Fnc& f)
    {
      DimensionException::check_scalar(f.image_dim());
      m_tubevector = new TubeVector(domain, timestep, f);
    }

    Tube::Tube(const Tube& x)
    {
      m_tubevector = new TubeVector(*x.m_tubevector);
    }

    Tube::Tube(int dim, const TubeVector& x)
    {
      /*m_tubevector = new TubeVector(x.domain());

      // todo: check dim
      TubeSlice *prev_slice = NULL, *slice = NULL;
      for(const TubeSlice *s = x.m_tubevector->get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(slice == NULL)
        {
          slice = new TubeSlice(s->domain(), 1);
          m_first_slice = slice;
        }

        else
        {
          slice->m_next_slice = new TubeSlice(s->domain(), 1);
          slice = slice->next_slice();
        }

        slice->set_envelope(s->codomain()[dim]);
        slice->set_input_gate(s->input_gate()[dim]);
        slice->set_output_gate(s->output_gate()[dim]);
        slice->set_tube_ref(this);

        if(prev_slice != NULL)
        {
          delete slice->m_input_gate;
          slice->m_input_gate = NULL;
          TubeSlice::chain_slices(prev_slice, slice);
        }

        prev_slice = slice;
      }*/
    }

    Tube::Tube(const Tube& x, const Interval& codomain)
    {
      m_tubevector = new TubeVector(*x.m_tubevector, IntervalVector(1, codomain));
    }

    Tube::Tube(const Trajectory& traj, double timestep)
    {
      DimensionException::check_scalar(traj.dim());
      // todo: m_tubevector = new TubeVector(traj, timestep);
    }

    Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep)
    {
      DimensionException::check_scalar(lb.dim());
      DimensionException::check_scalar(ub.dim());
      // todo: m_tubevector = new TubeVector(lb, ub, timestep);
    }

    Tube::Tube(const string& binary_file_name)
    {
      m_tubevector = new TubeVector(binary_file_name);
      DimensionException::check_scalar(m_tubevector->dim());
    }

    Tube::Tube(const string& binary_file_name, Trajectory& traj)
    {
      TrajectoryVector traj_vector;
      m_tubevector = new TubeVector(binary_file_name, traj_vector);

      // todo: DimensionException::check_scalar(m_tubevector->dim());
      // todo: if(traj_vector.dim() != 0)
      // todo:   DimensionException::check_scalar(traj_vector.dim());
      // todo: 
      // todo: traj = Trajectory(traj_vector);
    }

    Tube::Tube(const string& binary_file_name, vector<Trajectory>& v_trajs)
    {
      //vector<TrajectoryVector> v_trajs_vector;
      //TubeVector x(binary_file_name, v_trajs_vector);
      //
      //DimensionException::check_scalar(x.dim());
      //if(v_trajs_vector.size() != 0)
      //  DimensionException::check_scalar(v_trajs_vector[0].dim());
      //
      //*this = x;
      //for(int i = 0 ; i < v_trajs_vector.size() ; i++)
      //  v_trajs.push_back(v_trajs_vector[i]);
    }

    Tube::~Tube()
    {
      if(m_destroy_tubevector)
        delete m_tubevector;
    }
    
    const Tube Tube::primitive() const
    {
      Tube primitive(*this, Interval::ALL_REALS);
      primitive.set(0., primitive.domain().lb());
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(primitive, *this, FORWARD);
      return primitive;
    }

    const Tube& Tube::operator=(const Tube& x)
    {
      // todo
    }

    const Interval Tube::domain() const
    {
      return m_tubevector->domain();
    }
    
    int Tube::dim() const
    {
      return 1;
    }

    // Slices structure

    int Tube::nb_slices() const
    {
      return m_tubevector->nb_slices();
    }

    TubeSlice* Tube::get_slice(int slice_id)
    {
      return m_tubevector->get_slice(slice_id);
    }

    const TubeSlice* Tube::get_slice(int slice_id) const
    {
      return m_tubevector->get_slice(slice_id);
    }

    TubeSlice* Tube::get_slice(double t)
    {
      return m_tubevector->get_slice(t);
    }

    const TubeSlice* Tube::get_slice(double t) const
    {
      return m_tubevector->get_slice(t);
    }

    TubeSlice* Tube::get_first_slice()
    {
      return m_tubevector->get_first_slice();
    }

    const TubeSlice* Tube::get_first_slice() const
    {
      return m_tubevector->get_first_slice();
    }

    TubeSlice* Tube::get_last_slice()
    {
      return m_tubevector->get_last_slice();
    }

    const TubeSlice* Tube::get_last_slice() const
    {
      return m_tubevector->get_last_slice();
    }

    TubeSlice* Tube::get_wider_slice()
    {
      return m_tubevector->get_wider_slice();
    }

    const TubeSlice* Tube::get_wider_slice() const
    {
      return m_tubevector->get_wider_slice();
    }

    TubeSlice* Tube::get_largest_slice()
    {
      return m_tubevector->get_largest_slice();
    }

    const TubeSlice* Tube::get_largest_slice() const
    {
      return m_tubevector->get_largest_slice();
    }

    int Tube::input2index(double t) const
    {
      return m_tubevector->input2index(t);
    }

    int Tube::index(const TubeSlice* slice) const
    {
      return m_tubevector->index(slice);
    }

    void Tube::sample(double t)
    {
      return m_tubevector->sample(t);
    }

    void Tube::sample(double t, const Interval& gate)
    {
      IntervalVector gate_vector = (*m_tubevector)(t);
      ibex_overloaded_put(gate_vector, m_component_id, gate);
      m_tubevector->sample(t, gate_vector);
    }

    // Access values

    const Interval Tube::codomain() const
    {
      return m_tubevector->codomain()[m_component_id];
    }

    double Tube::volume() const
    {
      // todo: scalar implementation
      return m_tubevector->volume();
    }

    const Interval Tube::operator()(int slice_id) const
    {
      return (*m_tubevector)(slice_id)[m_component_id];
    }

    const Interval Tube::operator()(double t) const
    {
      return (*m_tubevector)(t)[m_component_id];
    }

    const Interval Tube::operator()(const Interval& t) const
    {
      return (*m_tubevector)(t)[m_component_id];
    }

    const Interval Tube::invert(const Interval& y, const Interval& search_domain) const
    {
      // todo: scalar implementation of it
      return m_tubevector->invert(IntervalVector(1, y), search_domain);
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      // todo: scalar implementation of it
      m_tubevector->invert(IntervalVector(1, y), v_t, search_domain);
    }

    const Interval Tube::invert(const Interval& y, const Tube& v, const Interval& search_domain) const
    {
      // todo: scalar implementation of it
      return m_tubevector->invert(IntervalVector(1, y), *v.m_tubevector, search_domain);
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Tube& v, const Interval& search_domain) const
    {
      // todo: scalar implementation of it
      m_tubevector->invert(IntervalVector(1, y), v_t, *v.m_tubevector, search_domain);
    }

    const pair<Interval,Interval> Tube::eval(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> p_box = m_tubevector->eval(t);
      return make_pair(p_box.first[m_component_id], p_box.second[m_component_id]);
    }

    const Interval Tube::interpol(double t, const Tube& v) const
    {
      return m_tubevector->interpol(t, *v.m_tubevector)[m_component_id];
    }

    const Interval Tube::interpol(const Interval& t, const Tube& v) const
    {
      return m_tubevector->interpol(t, *v.m_tubevector)[m_component_id];
    }
    
    double Tube::max_thickness() const
    {
      return m_tubevector->max_thickness()[m_component_id];
    }
    
    double Tube::max_gate_thickness(double& t) const
    {
      return m_tubevector->max_gate_thickness(t)[m_component_id];
    }

    // Tests

    bool Tube::operator==(const Tube& x) const
    {
      return m_tubevector->operator==(*x.m_tubevector);
    }

    bool Tube::operator!=(const Tube& x) const
    {
      return m_tubevector->operator!=(*x.m_tubevector);
    }

    bool Tube::is_subset(const Tube& x) const
    {
      return m_tubevector->is_subset(*x.m_tubevector);
    }

    bool Tube::is_strict_subset(const Tube& x) const
    {
      return m_tubevector->is_strict_subset(*x.m_tubevector);
    }

    bool Tube::is_empty() const
    {
      // todo: scalar implementation
      return m_tubevector->is_empty();
    }

    bool Tube::encloses(const Trajectory& x) const
    {
      // todo: return m_tubevector->encloses(x);
    }

    // Setting values

    void Tube::set(const Interval& y)
    {
      // todo: scalar implementation
      m_tubevector->set(IntervalVector(1, y));
    }

    void Tube::set(const Interval& y, int slice_id)
    {
      // todo: scalar implementation
      m_tubevector->set(IntervalVector(1, y), slice_id);
    }

    void Tube::set(const Interval& y, double t)
    {
      // todo: scalar implementation
      m_tubevector->set(IntervalVector(1, y), t);
    }

    void Tube::set(const Interval& y, const Interval& t)
    {
      // todo: scalar implementation
      m_tubevector->set(IntervalVector(1, y), t);
    }

    void Tube::set_empty()
    {
      set(Interval::EMPTY_SET);
    }

    const Tube& Tube::inflate(double rad)
    {
      // todo: scalar implementation
    }

    const Tube& Tube::inflate(const Trajectory& rad)
    {
      // todo: scalar implementation
    }

    // Bisection

    const pair<Tube,Tube> Tube::bisect(double t, float ratio) const
    {
      pair<TubeVector,TubeVector> p = m_tubevector->bisect(t, ratio);
      return make_pair(Tube(m_component_id, p.first), Tube(m_component_id, p.second));
    }

    // String
    
    std::ostream& operator<<(std::ostream& str, const Tube& x)
    {
      str << "Tube " << x.domain() << flush;/* << "↦" << x.codomain()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;*/
      return str;
    }

    // Integration

    const Interval Tube::integral(double t) const
    {
      return m_tubevector->integral(t)[m_component_id];
    }

    const Interval Tube::integral(const Interval& t) const
    {
      return m_tubevector->integral(t)[m_component_id];
    }

    const Interval Tube::integral(const Interval& t1, const Interval& t2) const
    {
      return m_tubevector->integral(t1, t2)[m_component_id];
    }

    const pair<Interval,Interval> Tube::partial_integral(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> p_box = m_tubevector->partial_integral(t);
      return make_pair(p_box.first[m_component_id], p_box.second[m_component_id]);
    }

    const pair<Interval,Interval> Tube::partial_integral(const Interval& t1, const Interval& t2) const
    {
      pair<IntervalVector,IntervalVector> p_box = m_tubevector->partial_integral(t1, t2);
      return make_pair(p_box.first[m_component_id], p_box.second[m_component_id]);
    }

    // Contractors

    bool Tube::ctc_deriv(const Tube& v)
    {
      CtcDeriv ctc_deriv(true);
      return ctc_deriv.contract(*this, v);
    }

    bool Tube::ctc_eval(Interval& t, Interval& z, Tube& w)
    {
      CtcEval ctc_eval(true);
      return ctc_eval.contract(t, z, *this, w);
    }

    // Serialization

    void Tube::serialize(const string& binary_file_name, int version_number) const
    {
      m_tubevector->serialize(binary_file_name, version_number);
    }

    void Tube::serialize(const string& binary_file_name, const Trajectory& traj, int version_number) const
    {
      // todo: m_tubevector->serialize(binary_file_name, traj, version_number);
    }

    void Tube::serialize(const string& binary_file_name, const vector<Trajectory>& v_trajs, int version_number) const
    {
      // todo: vector<TrajectoryVector> v_trajs_vector;
      // todo: for(int i = 0 ; i < v_trajs.size() ; i++)
      // todo:   v_trajs_vector.push_back(v_trajs[i]);
      // todo: m_tubevector->serialize(binary_file_name, v_trajs_vector, version_number);
    }

    const IntervalVector Tube::codomain_box() const
    {
      return IntervalVector(1, m_tubevector->codomain()[m_component_id]);
    }
}