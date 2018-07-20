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

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Tube::Tube(const Interval& domain, const Interval& codomain)
      : TubeVector(domain, IntervalVector(1, codomain))
    {

    }

    Tube::Tube(const Interval& domain, double timestep, const Interval& codomain)
      : TubeVector(domain, timestep, IntervalVector(1, codomain))
    {

    }

    Tube::Tube(const Interval& domain, double timestep, const Fnc& f)
      : TubeVector(domain, timestep, f)
    {
      DimensionException::check_scalar(f.image_dim());
    }

    Tube::Tube(const Tube& x)
      : TubeVector(x)
    {

    }

    Tube::Tube(int dim, const TubeVector& x)
      : TubeVector(x.domain())
    {
      // todo: check dim
      TubeSlice *prev_slice = NULL;
      const TubeSlice *slice_x = x.get_first_slice();

      while(slice_x != NULL)
      {
        TubeSlice *slice = new TubeSlice(slice_x->domain(), 1);
        slice->set_envelope(slice_x->codomain()[dim]);
        slice->set_input_gate(slice_x->input_gate()[dim]);
        slice->set_output_gate(slice_x->output_gate()[dim]);
        slice->set_tube_ref(this);
        m_v_slices.push_back(slice);

        if(prev_slice != NULL)
        {
          delete slice->m_input_gate;
          slice->m_input_gate = NULL;
          TubeSlice::chain_slices(prev_slice, slice);
        }
        prev_slice = slice;
        slice_x = slice_x->next_slice();
      }
    }

    Tube::Tube(const Tube& x, const Interval& codomain)
      : TubeVector(x, IntervalVector(1, codomain))
    {

    }

    Tube::Tube(const Trajectory& traj, double timestep)
      : TubeVector(traj, timestep)
    {
      DimensionException::check_scalar(traj.dim());
    }

    Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep)
      : TubeVector(lb, ub, timestep)
    {
      DimensionException::check_scalar(lb.dim());
      DimensionException::check_scalar(ub.dim());
    }

    Tube::Tube(const string& binary_file_name)
      : TubeVector(Interval(0.,1.), 1)
    {
      TubeVector x(binary_file_name);
      DimensionException::check_scalar(x.dim());
      *this = x;
    }

    Tube::Tube(const string& binary_file_name, Trajectory& traj)
      : TubeVector(Interval(0.,1.), 1)
    {
      TrajectoryVector traj_vector;
      TubeVector x(binary_file_name, traj_vector);

      DimensionException::check_scalar(x.dim());
      if(traj_vector.dim() != 0)
        DimensionException::check_scalar(traj_vector.dim());

      *this = x;
      traj = Trajectory(traj_vector);
    }

    Tube::Tube(const string& binary_file_name, vector<Trajectory>& v_trajs)
      : TubeVector(Interval(0.,1.), 1)
    {
      vector<TrajectoryVector> v_trajs_vector;
      TubeVector x(binary_file_name, v_trajs_vector);

      DimensionException::check_scalar(x.dim());
      if(v_trajs_vector.size() != 0)
        DimensionException::check_scalar(v_trajs_vector[0].dim());

      *this = x;
      for(int i = 0 ; i < v_trajs_vector.size() ; i++)
        v_trajs.push_back(v_trajs_vector[i]);
    }

    Tube::~Tube()
    {

    }
    
    const Tube Tube::primitive() const
    {
      Tube primitive(*this, Interval::ALL_REALS);
      primitive.set(0., primitive.domain().lb());
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(primitive, *this, FORWARD);
      return primitive;
    }

    void Tube::sample(double t, const Interval& gate)
    {
      TubeVector::sample(t, IntervalVector(1, gate));
    }

    // Access values

    const Interval Tube::codomain() const
    {
      return TubeVector::codomain()[0];
    }

    const Interval Tube::operator[](int slice_id) const
    {
      return TubeVector::operator[](slice_id)[0];
    }

    const Interval Tube::operator[](double t) const
    {
      return TubeVector::operator[](t)[0];
    }

    const Interval Tube::operator[](const Interval& t) const
    {
      return TubeVector::operator[](t)[0];
    }

    const Interval Tube::invert(const Interval& y, const Interval& search_domain) const
    {
      return TubeVector::invert(IntervalVector(1, y), search_domain);
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      TubeVector::invert(IntervalVector(1, y), v_t, search_domain);
    }

    const Interval Tube::invert(const Interval& y, const Tube& v, const Interval& search_domain) const
    {
      return TubeVector::invert(IntervalVector(1, y), v, search_domain);
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Tube& v, const Interval& search_domain) const
    {
      TubeVector::invert(IntervalVector(1, y), v_t, v, search_domain);
    }

    const pair<Interval,Interval> Tube::eval(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> p_box = TubeVector::eval(t);
      return make_pair(p_box.first[0], p_box.second[0]);
    }

    const Interval Tube::interpol(double t, const Tube& v) const
    {
      return TubeVector::interpol(t, v)[0];
    }

    const Interval Tube::interpol(const Interval& t, const Tube& v) const
    {
      return TubeVector::interpol(t, v)[0];
    }
    
    double Tube::max_thickness() const
    {
      return TubeVector::max_thickness()[0];
    }
    
    double Tube::max_gate_thickness(double& t) const
    {
      return TubeVector::max_gate_thickness(t)[0];
    }

    // Tests

    bool Tube::operator==(const Tube& x) const
    {
      return TubeVector::operator==(x);
    }

    bool Tube::operator!=(const Tube& x) const
    {
      return TubeVector::operator!=(x);
    }

    bool Tube::is_subset(const Tube& x) const
    {
      return TubeVector::is_subset(x);
    }

    bool Tube::is_strict_subset(const Tube& x) const
    {
      return TubeVector::is_strict_subset(x);
    }

    // Setting values

    void Tube::set(const Interval& y)
    {
      TubeVector::set(IntervalVector(1, y));
    }

    void Tube::set(const Interval& y, int slice_id)
    {
      TubeVector::set(IntervalVector(1, y), slice_id);
    }

    void Tube::set(const Interval& y, double t)
    {
      TubeVector::set(IntervalVector(1, y), t);
    }

    void Tube::set(const Interval& y, const Interval& t)
    {
      TubeVector::set(IntervalVector(1, y), t);
    }

    // Bisection

    const pair<Tube,Tube> Tube::bisect(double t, float ratio) const
    {
      pair<TubeVector,TubeVector> p = TubeVector::bisect(t, ratio);
      return make_pair(Tube(0, p.first), Tube(0, p.second));
    }

    // String
    
    std::ostream& operator<<(std::ostream& str, const Tube& x)
    {
      str << "Tube " << x.domain() << "↦" << x.codomain()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Integration

    const Interval Tube::integral(double t) const
    {
      return TubeVector::integral(t)[0];
    }

    const Interval Tube::integral(const Interval& t) const
    {
      return TubeVector::integral(t)[0];
    }

    const Interval Tube::integral(const Interval& t1, const Interval& t2) const
    {
      return TubeVector::integral(t1, t2)[0];
    }

    const pair<Interval,Interval> Tube::partial_integral(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> p_box = TubeVector::partial_integral(t);
      return make_pair(p_box.first[0], p_box.second[0]);
    }

    const pair<Interval,Interval> Tube::partial_integral(const Interval& t1, const Interval& t2) const
    {
      pair<IntervalVector,IntervalVector> p_box = TubeVector::partial_integral(t1, t2);
      return make_pair(p_box.first[0], p_box.second[0]);
    }

    // Contractors

    bool Tube::ctc_eval(Interval& t, Interval& z, Tube& w)
    {
      CtcEval ctc_eval(true);
      return ctc_eval.contract(t, z, *this, w);
    }

    // Serialization

    void Tube::serialize(const string& binary_file_name, int version_number) const
    {
      TubeVector::serialize(binary_file_name, version_number);
    }

    void Tube::serialize(const string& binary_file_name, const Trajectory& traj, int version_number) const
    {
      TubeVector::serialize(binary_file_name, traj, version_number);
    }

    void Tube::serialize(const string& binary_file_name, const vector<Trajectory>& v_trajs, int version_number) const
    {
      vector<TrajectoryVector> v_trajs_vector;
      for(int i = 0 ; i < v_trajs.size() ; i++)
        v_trajs_vector.push_back(v_trajs[i]);
      TubeVector::serialize(binary_file_name, v_trajs_vector, version_number);
    }
}