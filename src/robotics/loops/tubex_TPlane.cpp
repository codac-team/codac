/** 
 *  TPlane class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_TPlane.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TPlane::TPlane()
    : Paving(IntervalVector(2), VALUE_MAYBE)
  {

  }

  void TPlane::compute_detections(float precision, const TubeVector& p, const TubeVector& v)
  {
    compute_detections(precision, p, v, true);
  }

  void TPlane::compute_detections(float precision, const TubeVector& p, const TubeVector& v, bool extract_subsets)
  {
    assert(precision > 0.);
    assert(p.domain() == v.domain());
    assert(p.size() == 2 && v.size() == 2);

    if(m_box.is_unbounded())
      m_box = IntervalVector(2, p.domain()); // initializing
    m_precision = precision;
    
    if(value() == VALUE_OUT)
      return;

    else if(!is_leaf())
    {
      ((TPlane*)m_first_subpaving)->compute_detections(precision, p, v, false);
      ((TPlane*)m_second_subpaving)->compute_detections(precision, p, v, false);
    }

    else
    {
      const Interval t1 = m_box[0], t2 = m_box[1];
      const IntervalVector box_neg_reals(2, Interval::NEG_REALS);
      const IntervalVector box_pos_reals(2, Interval::POS_REALS);

      // Based on derivative information
      
        const pair<IntervalVector, IntervalVector> partial_integ = v.partial_integral(t1, t2);
        const IntervalVector integ = IntervalVector(partial_integ.first.lb()) | partial_integ.second.ub();

        bool derivative_out = Interval::POS_REALS.is_strict_superset(t1 - t2)
                           || !integ.interior_contains(Vector(2, 0.)) 
                           || !v(t1 | t2).interior_contains(Vector(2, 0.));

        bool derivative_in = Interval::NEG_REALS.is_strict_superset(t1 - t2)
                          && box_neg_reals.is_strict_superset(partial_integ.first)
                          && box_pos_reals.is_strict_superset(partial_integ.second);
      
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
          set_value(VALUE_OUT);

        else if(derivative_in && primitive_in)
          set_value(VALUE_IN);

        else if(max(t1.diam(), t2.diam()) < precision)
          set_value(VALUE_MAYBE);

        else
        {
          bisect();
          ((TPlane*)m_first_subpaving)->compute_detections(precision, p, v, false);
          ((TPlane*)m_second_subpaving)->compute_detections(precision, p, v, false);
        }
    }

    if(extract_subsets)
      m_v_detected_loops = get_connected_subsets();
  }

  void TPlane::compute_proofs(IntervalVector (*f)(const IntervalVector& b))
  {
    for(int i = 0 ; i < m_v_detected_loops.size() ; i++)
      if(m_v_detected_loops[i].zero_proven(f))
        m_v_proven_loops.push_back(m_v_detected_loops[i]);
  }

  int TPlane::nb_loops_detections() const
  {
    return m_v_detected_loops.size();
  }

  int TPlane::nb_loops_proofs() const
  {
    return m_v_proven_loops.size();
  }

  const vector<ConnectedSubset>& TPlane::get_detected_loops() const
  {
    return m_v_detected_loops;
  }

  const vector<ConnectedSubset>& TPlane::get_proven_loops() const
  {
    return m_v_proven_loops;
  }

  Trajectory TPlane::traj_loops_summary() const
  {
    Trajectory traj;
    traj.set(0., box()[0].lb());
    traj.set(0., box()[0].ub());
    traj.discretize(m_precision);
    const map<double,double> map_values = traj.sampled_map();

    // Detected loops: value set to 1
    for(int i = 0 ; i < m_v_detected_loops.size() ; i++)
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
    for(int i = 0 ; i < m_v_proven_loops.size() ; i++)
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
}