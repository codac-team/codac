/** 
 *  TPlane class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <ctime>
#include "codac_TPlane.h"

using namespace std;
using namespace ibex;

namespace codac
{
  TPlane::TPlane(const Interval& tdomain)
    : Paving(IntervalVector(2, tdomain), SetValue::UNKNOWN)
  {

  }

  void TPlane::compute_loops(float precision, const TubeVector& p, const TubeVector& v)
  {
    compute_detections(precision, p, v, true, true);
    compute_proofs(p, v);
  }

  void TPlane::compute_detections(float precision, const TubeVector& p)
  {
    compute_detections(precision, p, p, false, true);
  }

  void TPlane::compute_detections(float precision, const TubeVector& p, const TubeVector& v)
  {
    compute_detections(precision, p, v, true, true);
  }

  void TPlane::compute_detections(float precision, const TubeVector& p, const TubeVector& v, bool with_derivative, bool extract_subsets)
  {
    assert(precision > 0.);
    assert(p.tdomain().is_superset(box()[0]));

    if(with_derivative)
    {
      assert(p.tdomain() == v.tdomain());
      assert(p.size() == 2 && v.size() == 2);
    }

    if(m_box.is_unbounded())
      m_box = IntervalVector(2, p.tdomain()); // initializing
    m_precision = precision;
    
    if(value() == SetValue::OUT)
      return;

    else if(!is_leaf())
    {
      ((TPlane*)m_first_subpaving)->compute_detections(precision, p, v, with_derivative, false);
      ((TPlane*)m_second_subpaving)->compute_detections(precision, p, v, with_derivative, false);
    }

    else
    {
      const Interval t1 = m_box[0], t2 = m_box[1];
      const IntervalVector box_neg_reals(2, Interval::NEG_REALS);
      const IntervalVector box_pos_reals(2, Interval::POS_REALS);

      // Based on derivative information
      
      bool derivative_out = false, derivative_in = false;

      if(with_derivative)
      {
        const pair<IntervalVector, IntervalVector> partial_integ = v.partial_integral(t1, t2);
        const IntervalVector integ = IntervalVector(partial_integ.first.lb()) | partial_integ.second.ub();

        derivative_out = Interval::POS_REALS.is_strict_superset(t1 - t2)
                      || !integ.interior_contains(Vector(2, 0.)) 
                      || !v(t1 | t2).interior_contains(Vector(2, 0.));

        derivative_in = Interval::NEG_REALS.is_strict_superset(t1 - t2)
                     && box_neg_reals.is_strict_superset(partial_integ.first)
                     && box_pos_reals.is_strict_superset(partial_integ.second);
      }
      
      // Based on primitive information (<=> kernel)

        pair<IntervalVector,IntervalVector> uy1 = p.eval(t1);
        pair<IntervalVector,IntervalVector> uy2 = p.eval(t2);
        pair<IntervalVector,IntervalVector> enc_bounds = make_pair(
          IntervalVector(uy1.first.lb()  - uy2.second.ub()) | (uy1.first.ub()  - uy2.second.lb()),
          IntervalVector(uy1.second.lb() - uy2.first.ub())  | (uy1.second.ub() - uy2.first.lb()));

        bool primitive_out = Interval::POS_REALS.is_strict_superset(t1 - t2)
                             || Interval::POS_REALS.is_strict_superset(enc_bounds.first[0])
                             || Interval::POS_REALS.is_strict_superset(enc_bounds.first[1])
                             || Interval::NEG_REALS.is_strict_superset(enc_bounds.second[0])
                             || Interval::NEG_REALS.is_strict_superset(enc_bounds.second[1]);

        bool primitive_in = Interval::NEG_REALS.is_strict_superset(t1 - t2)
                             && Interval::NEG_REALS.is_strict_superset(enc_bounds.first[0])
                             && Interval::NEG_REALS.is_strict_superset(enc_bounds.first[1])
                             && Interval::POS_REALS.is_strict_superset(enc_bounds.second[0])
                             && Interval::POS_REALS.is_strict_superset(enc_bounds.second[1]);

      // Conclusion

        if(derivative_out || primitive_out)
          set_value(SetValue::OUT);

        else if(derivative_in && primitive_in)
          set_value(SetValue::IN);

        else if(std::max(t1.diam(), t2.diam()) < precision)
          set_value(SetValue::UNKNOWN);

        else
        {
          bisect();
          ((TPlane*)m_first_subpaving)->compute_detections(precision, p, v, with_derivative, false);
          ((TPlane*)m_second_subpaving)->compute_detections(precision, p, v, with_derivative, false);
        }
    }

    if(extract_subsets)
      m_v_detected_loops = get_connected_subsets();
  }

    // Inclusion functions
    
    IntervalVector f_pv(const TubeVector& p, const TubeVector& v, const IntervalVector& input)
    {
      return v.integral(input[0], input[1])
           & (p(input[1]) - p(input[0]));
    }
    
    IntervalVector f_p(const TubeVector& p, const IntervalVector& input)
    {
      return p(input[1]) - p(input[0]);
    }

  using namespace std::placeholders;

  void TPlane::compute_proofs(const TubeVector& p)
  {
    auto f = std::bind(&f_p, p, _1);
    compute_proofs(f);
  }

  void TPlane::compute_proofs(const TubeVector& p, const TubeVector& v)
  {
    auto f = std::bind(&f_pv, p, v, _1);
    compute_proofs(f);
  }

  void TPlane::compute_proofs(const function<IntervalVector(const IntervalVector&)>& f)
  {
    clock_t t_start = clock();
    m_v_proven_loops.clear();

    for(size_t i = 0 ; i < m_v_detected_loops.size() ; i++)
    {
      if(TPlane::m_verbose)
        cout << "Computing loop " << i << "/" << m_v_detected_loops.size() << ".." << flush;
      
      if(m_v_detected_loops[i].zero_proven(f))
      {
        m_v_proven_loops.push_back(m_v_detected_loops[i]);
        if(TPlane::m_verbose)
          cout << " proven." << endl;
      }

      else if(TPlane::m_verbose)
        cout << endl;
    }

    printf("%d proven loops. Computation time: %.2fs\n",
      (int)m_v_proven_loops.size(),
      (double)(clock() - t_start)/CLOCKS_PER_SEC);
  }

  int TPlane::nb_loops_detections() const
  {
    return m_v_detected_loops.size();
  }

  int TPlane::nb_loops_proofs() const
  {
    return m_v_proven_loops.size();
  }

  const vector<ConnectedSubset>& TPlane::detected_loops_subsets() const
  {
    return m_v_detected_loops;
  }

  const vector<IntervalVector> TPlane::detected_loops() const
  {
    return ConnectedSubset::get_boxed_hulls(m_v_detected_loops);
  }

  const vector<ConnectedSubset>& TPlane::proven_loops_subsets() const
  {
    return m_v_proven_loops;
  }

  const vector<IntervalVector> TPlane::proven_loops() const
  {
    return ConnectedSubset::get_boxed_hulls(m_v_proven_loops);
  }

  Trajectory TPlane::traj_loops_summary() const
  {
    Trajectory traj;
    traj.set(0., box()[0].lb());
    traj.set(0., box()[0].ub());
    traj.sample(m_precision);
    const map<double,double> map_values = traj.sampled_map();

    // Detected loops: value set to 1
    for(size_t i = 0 ; i < m_v_detected_loops.size() ; i++)
    {
      for(int j = 0 ; j < 2 ; j++)
      {
        double t = m_v_detected_loops[i].box()[j].lb();
        typename map<double,double>::const_iterator it = map_values.lower_bound(t);

        if(it->first != t && it != map_values.begin())
          it--;

        while(it != map_values.end() && it->first <= m_v_detected_loops[i].box()[j].ub())
        {
          traj.set(1., it->first);
          it++;
        }
      }
    }

    // Proven loops: value set to 2
    for(size_t i = 0 ; i < m_v_proven_loops.size() ; i++)
    {
      for(int j = 0 ; j < 2 ; j++)
      {
        double t = m_v_proven_loops[i].box()[j].lb();
        typename map<double,double>::const_iterator it = map_values.lower_bound(t);

        if(it->first != t && it != map_values.begin())
          it--;

        while(it != map_values.end() && it->first <= m_v_proven_loops[i].box()[j].ub())
        {
          traj.set(2., it->first);
          it++;
        }
      }
    }

    return traj;
  }

  bool TPlane::m_verbose = false;

  void TPlane::verbose(bool verbose)
  {
    TPlane::m_verbose = verbose;
  }
}