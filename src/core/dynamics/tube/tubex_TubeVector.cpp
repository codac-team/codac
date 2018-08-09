/* ============================================================================
 *  tubex-lib - TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeVector.h"
#include "tubex_Exception.h"
#include "tubex_DomainException.h"
#include "tubex_TrajectorySerialization.h"
#include "tubex_DimensionException.h"
#include "tubex_SlicingException.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_Arithmetic.h"
#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeVector::TubeVector()
    {
      
    }

    TubeVector::TubeVector(const Interval& domain, int dim)
    {
      DomainException::check(domain);
      DimensionException::check(dim);
    }

    TubeVector::TubeVector(const Interval& domain, const IntervalVector& codomain)
    {
      DomainException::check(domain);
      DimensionException::check(codomain.size());

    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, int dim)
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      DimensionException::check(dim);

    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const IntervalVector& codomain)
    {
      DomainException::check(domain);
      DomainException::check(timestep);

    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const tubex::Fnc& f)
    {
      DomainException::check(domain);
      DomainException::check(timestep);

    }

    TubeVector::TubeVector(const TubeVector& x)
    {

    }

    TubeVector::TubeVector(const TubeVector& x, const IntervalVector& codomain)
    {

    }

    TubeVector::TubeVector(const TrajectoryVector& traj, double timestep)
    {
      DomainException::check(timestep);

    }

    TubeVector::TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep)
    {
      DomainException::check(timestep);
      DimensionException::check(lb, ub);

    }

    TubeVector::TubeVector(const string& binary_file_name)
    {

    }
    
    TubeVector::TubeVector(const string& binary_file_name, TrajectoryVector& traj)
    {

    }

    TubeVector::TubeVector(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {

    }
    
    TubeVector::~TubeVector()
    {

    }

    const TubeVector TubeVector::primitive() const
    {

    }

    const TubeVector& TubeVector::operator=(const TubeVector& x)
    {

    }

    const Interval TubeVector::domain() const
    {

    }

    int TubeVector::dim() const
    {

    }

    void TubeVector::resize(int n)
    {

    }
    
    const TubeVector TubeVector::subvector(int start_index, int end_index) const
    {

    }
    
    void TubeVector::put(int start_index, const TubeVector& subvec)
    {
      // todo: check size subvec
      // todo: check structure tubes

    }
  
    // Slices structure

    int TubeVector::nb_slices() const
    {

    }

    int TubeVector::input2index(double t) const
    {
      DomainException::check(*this, t);

    }

    int TubeVector::index(const Slice* slice) const
    {

    }

    void TubeVector::sample(double t)
    {
      DomainException::check(*this, t);

    }

    void TubeVector::sample(double t, const IntervalVector& gate)
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, gate);

    }

    // Access values

    const IntervalVector TubeVector::codomain() const
    {

    }

    double TubeVector::volume() const
    {

    }

    Tube& TubeVector::operator[](int index)
    {

    }

    const Tube& TubeVector::operator[](int index) const
    {

    }

    const IntervalVector TubeVector::operator()(int slice_id) const
    {
      SlicingException::check(*this, slice_id);

    }

    const IntervalVector TubeVector::operator()(double t) const
    {
      DomainException::check(*this, t);

    }

    const IntervalVector TubeVector::operator()(const Interval& t) const
    {
      DomainException::check(*this, t);

    }

    const Interval TubeVector::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);

    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);

    }

    const Interval TubeVector::invert(const IntervalVector& y, const TubeVector& v, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      SlicingException::check(*this, v);
      DomainException::check(*this, v);

    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const TubeVector& v, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);

    }

    const pair<IntervalVector,IntervalVector> TubeVector::eval(const Interval& t) const
    {

    }

    const IntervalVector TubeVector::interpol(double t, const TubeVector& v) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, v);
      SlicingException::check(*this, v);

    }

    const IntervalVector TubeVector::interpol(const Interval& t, const TubeVector& v) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, v);
      SlicingException::check(*this, v);

    }

    const Vector TubeVector::max_thickness() const
    {

    }

    const Vector TubeVector::max_gate_thickness(double& t) const
    {

    }

    // Tests


    bool TubeVector::operator==(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      SlicingException::check(*this, x);

    }

    bool TubeVector::operator!=(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      SlicingException::check(*this, x);

    }

    bool TubeVector::is_subset(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      SlicingException::check(*this, x);

    }

    bool TubeVector::is_strict_subset(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      SlicingException::check(*this, x);

    }

    bool TubeVector::is_empty() const
    {

    }

    bool TubeVector::encloses(const TrajectoryVector& x) const
    {
      DimensionException::check(*this, x);

    }

    // Setting values

    void TubeVector::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);

    }

    void TubeVector::set(const IntervalVector& y, int slice_id)
    {
      DimensionException::check(*this, y);
      SlicingException::check(*this, slice_id);

    }

    void TubeVector::set(const IntervalVector& y, double t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);

    }

    void TubeVector::set(const IntervalVector& y, const Interval& t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);

    }

    void TubeVector::set_empty()
    {

    }

    const TubeVector& TubeVector::inflate(double rad)
    {

    }

    const TubeVector& TubeVector::inflate(const TrajectoryVector& rad)
    {
      DimensionException::check(*this, rad);
      DomainException::check(*this, rad);

    }

    // Bisection
    
    const pair<TubeVector,TubeVector> TubeVector::bisect(double t, float ratio) const
    {
      DomainException::check(*this, t);

    }

    // Integration

    const IntervalVector TubeVector::integral(double t) const
    {
      DomainException::check(*this, t);

    }

    const IntervalVector TubeVector::integral(const Interval& t) const
    {
      DomainException::check(*this, t);

    }

    const IntervalVector TubeVector::integral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);

    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t) const
    {
      DomainException::check(*this, t);

    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);

    }

    // Contractors

    bool TubeVector::ctc_deriv(const TubeVector& v)
    {

    }

    bool TubeVector::ctc_eval(Interval& t, IntervalVector& z, TubeVector& w)
    {

    }
      
    // Serialization

    void TubeVector::serialize(const string& binary_file_name, int version_number) const
    {

    }

    void TubeVector::serialize(const string& binary_file_name, const TrajectoryVector& traj, int version_number) const
    {

    }
    
    void TubeVector::serialize(const string& binary_file_name, const vector<TrajectoryVector>& v_trajs, int version_number) const
    {

    }

  // Protected methods

    // Access values

    const IntervalVector TubeVector::codomain_box() const
    {

    }

    // Integration

    // Serialization

    void TubeVector::deserialize(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {

    }
}