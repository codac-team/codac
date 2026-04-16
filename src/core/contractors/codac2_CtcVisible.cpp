/** 
 *  codac2_CtcVisible.cpp
 * ----------------------------------------------------------------------------
 * \date       2026
 * \author     Quentin Brateau
 * \copyright  Copyright 2026 Codac Team
 * \license    GNU Lesser General Public License (LGPL)
 */


 /** * codac2_CtcVisible.cpp
 */

#include "codac2_CtcVisible.h"
#include "codac2_det.h"
#include "codac2_arith_sub.h"
#include "codac2_max.h"
#include "codac2_min.h"
#include "codac2_IntervalVector.h"

using namespace std;
using namespace codac2;

namespace {

    // 1. Determinant contraction
    void core_det(IntervalVector& x, const IntervalVector& p, const IntervalVector& v, const double sign) {
        IntervalVector v_xp = x - p;
        IntervalVector v_fixed = v;
        DetOp::bwd(sign * Interval(0, oo), v_xp, v_fixed);
        x &= v_xp + p;
    }

    // 2. AABB Contraction
    // require_overlap = true  -> CtcNoVisible (Intersection)
    // require_overlap = false -> CtcVisible   (Union)
    void core_aabb(IntervalVector& x, const IntervalVector& a, const IntervalVector& e1, const IntervalVector& e2, bool require_overlap) {
        for(int i = 0; i < 2; ++i) {
            Interval xi = x[i], ai = Interval(a[i]);
            Interval obs_min = min(e1[i], e2[i]), obs_max = max(e1[i], e2[i]);

            Interval i1 = min(ai, xi), i2 = max(i1, obs_min);
            Interval i3 = max(ai, xi), i4 = min(i3, obs_max);
            Interval i5 = i2 - i4;

            Interval target = require_overlap ? Interval(-oo, 0) : Interval(0, oo);
            
            if ((i5 &= target).is_empty()) { x.set_empty(); return; }

            i2 &= i5 + i4; i4 &= i2 - i5;
            Interval t_max = obs_max, t_min = obs_min, t_a = ai;
            MinOp::bwd(i4, i3, t_max); MaxOp::bwd(i3, t_a, xi);
            t_a = ai; MaxOp::bwd(i2, i1, t_min); MinOp::bwd(i1, t_a, xi);
            x[i] &= xi;
        }
    }
}

CtcVisibleBase::CtcVisibleBase(const IntervalVector& a, const std::vector<Segment>& edges)
  : _a(a)
{
  for(const auto& s : edges)
  {
    VisibilityEdgeData ed;
    ed.e1 = IntervalVector(s[0]);
    ed.e2 = IntervalVector(s[1]);
    ed.v_e2e1 = ed.e2 - ed.e1;
    ed.v_ae1 = IntervalVector(_a) - ed.e1;
    ed.v_ae2 = IntervalVector(_a) - ed.e2;
    ed.s_box = s.box();

    Interval det_val = (_a[0] - ed.e1[0]) * (ed.v_e2e1[1]) - 
                      (_a[1] - ed.e1[1]) * (ed.v_e2e1[0]);

    if (det_val.lb() > 0) {
      ed.k = 1.0;
    } else if (det_val.ub() < 0) {
      ed.k = -1.0;
    } else {
      ed.k = 0.0; // Edge is collinear with point 'a'
    }

    _edges.push_back(ed);
  }
}

CtcVisible::CtcVisible(const IntervalVector& a, const Segment& s) : Ctc(2), CtcVisibleBase(a,{s}) { 

}

CtcVisible::CtcVisible(const IntervalVector& a, const std::vector<Segment>& s) : Ctc(2), CtcVisibleBase(a,s) {

}

CtcVisible::CtcVisible(const IntervalVector& a, const Polygon& p) : Ctc(2), CtcVisibleBase(a,p) {

}

void CtcVisible::contract(IntervalVector& x) const {
  for (const auto& ed : _edges) {
    IntervalVector x1(x), x2(x), x3(x), x4(x);
    
    core_det(x1, ed.e1, ed.v_e2e1, ed.k);
    core_det(x2, ed.e1, ed.v_ae1,  ed.k);
    core_det(x3, ed.e2, ed.v_ae2,  -ed.k);
    core_aabb(x4, _a, ed.e1, ed.e2, false); // Visible = Disjoint check

    x &= (x1 | x2 | x3 | x4);
    if (x.is_empty()) return;
  }
}

// CtcNoVisible implementation

CtcNoVisible::CtcNoVisible(const IntervalVector& a, const Segment& s) : Ctc(2), CtcVisibleBase(a,{s}) { 

}

CtcNoVisible::CtcNoVisible(const IntervalVector& a, const std::vector<Segment>& s) : Ctc(2), CtcVisibleBase(a,s) {

}

CtcNoVisible::CtcNoVisible(const IntervalVector& a, const Polygon& p) : Ctc(2), CtcVisibleBase(a,p) {

}

void CtcNoVisible::contract(IntervalVector& x) const {
    IntervalVector x_total_hidden = IntervalVector::empty(2);

    for (const auto& ed : _edges) {
        IntervalVector xi(x);
        
        core_det(xi, ed.e1, ed.v_e2e1, -ed.k);
        core_det(xi, ed.e1, ed.v_ae1, -ed.k);
        core_det(xi, ed.e2, ed.v_ae2, ed.k);
        core_aabb(xi, _a, ed.e1, ed.e2, true); // NoVisible = Overlap check

        x_total_hidden |= xi;
    }
    x &= x_total_hidden;
}
