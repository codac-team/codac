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

    Tube::Tube(const Interval& domain, double timestep, const Function& f)
      : TubeVector(domain, timestep, f)
    {
      DimensionException::checkScalar(f.image_dim());
    }

    Tube::Tube(const Tube& x)
      : TubeVector(x)
    {

    }

    Tube::Tube(const Tube& x, const Interval& codomain)
      : TubeVector(x, IntervalVector(1, codomain))
    {

    }

    Tube::Tube(const Tube& x, const Function& f)
      : TubeVector(x, f)
    {
      DimensionException::checkScalar(f.image_dim());
    }

    Tube::Tube(const Trajectory& traj, double timestep)
      : TubeVector(traj, timestep)
    {
      DimensionException::checkScalar(traj.dim());
    }

    Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep)
      : TubeVector(lb, ub, timestep)
    {
      DimensionException::checkScalar(lb.dim());
      DimensionException::checkScalar(ub.dim());
    }

    Tube::Tube(const string& binary_file_name)
      : TubeVector(Interval(0.,1.), 1) // to be changed
    {
      TubeVector x(binary_file_name);
      DimensionException::checkScalar(x.dim());
      *this = x;
    }

    Tube::Tube(const string& binary_file_name, Trajectory& traj)
      : TubeVector(Interval(0.,1.), 1) // to be changed
    {
      TrajectoryVector traj_vector;
      TubeVector x(binary_file_name, traj_vector);

      DimensionException::checkScalar(x.dim());
      if(traj_vector.dim() != 0)
        DimensionException::checkScalar(traj_vector.dim());

      *this = x;
    }

    Tube::Tube(const string& binary_file_name, vector<Trajectory>& v_trajs)
      : TubeVector(Interval(0.,1.), 1) // to be changed
    {
      vector<TrajectoryVector> v_trajs_vector;
      TubeVector x(binary_file_name, v_trajs_vector);

      DimensionException::checkScalar(x.dim());
      if(v_trajs_vector.size() != 0)
        DimensionException::checkScalar(v_trajs_vector[0].dim());

      *this = x;
      for(int i = 0 ; i < v_trajs_vector.size() ; i++)
        v_trajs.push_back(v_trajs_vector[i]);
    }

    Tube::~Tube()
    {

    }
    
    Tube Tube::primitive() const
    {
      Tube primitive(*this, Interval::ALL_REALS);
      CtcDeriv ctc_deriv;
      ctc_deriv.contractFwd(primitive, *this);
      return primitive;
    }

    void Tube::sample(double t, const Interval& gate)
    {
      TubeVector::sample(t, IntervalVector(1, gate));
    }

    // Access values

    const Interval Tube::codomain() const // todo: output const Interval& (reference)
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

    Interval Tube::invert(const Interval& y, const Interval& search_domain) const
    {
      return TubeVector::invert(IntervalVector(1, y), search_domain);
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      TubeVector::invert(IntervalVector(1, y), v_t, search_domain);
    }

    const pair<Interval,Interval> Tube::eval(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> p_box = TubeVector::eval(t);
      return make_pair(p_box.first[0], p_box.second[0]);
    }

    const Interval Tube::interpol(double t, const Tube& derivative) const
    {
      return TubeVector::interpol(t, derivative)[0];
    }

    const Interval Tube::interpol(const Interval& t, const Tube& derivative) const
    {
      return TubeVector::interpol(t, derivative)[0];
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

    bool Tube::isSubset(const Tube& x) const
    {
      return TubeVector::isSubset(x);
    }

    bool Tube::isStrictSubset(const Tube& x) const
    {
      return TubeVector::isStrictSubset(x);
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

    void Tube::set(const Function& function)
    {
      TubeVector::set(function);
    }

    // Bisection

    // String
    
    std::ostream& operator<<(std::ostream& str, const Tube& x)
    {
      str << "Tube " << x.domain() << "↦" << x.codomain()
          << ", " << x.nbSlices()
          << " slice" << (x.nbSlices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Integration

    Interval Tube::integral(double t) const
    {
      return TubeVector::integral(t)[0];
    }

    Interval Tube::integral(const Interval& t) const
    {
      return TubeVector::integral(t)[0];
    }

    Interval Tube::integral(const Interval& t1, const Interval& t2) const
    {
      return TubeVector::integral(t1, t2)[0];
    }

    pair<Interval,Interval> Tube::partialIntegral(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> p_box = TubeVector::partialIntegral(t);
      return make_pair(p_box.first[0], p_box.second[0]);
    }

    pair<Interval,Interval> Tube::partialIntegral(const Interval& t1, const Interval& t2) const
    {
      pair<IntervalVector,IntervalVector> p_box = TubeVector::partialIntegral(t1, t2);
      return make_pair(p_box.first[0], p_box.second[0]);
    }

    // Contractors

    bool Tube::ctcEval(Interval& t, Interval& z, const Tube& derivative, bool propagate)
    {
      IntervalVector z_box(1, z);
      bool ctc = TubeVector::ctcEval(t, z_box, derivative, propagate);
      z = z_box[0];
      return ctc;
    }

    // Serialization

}