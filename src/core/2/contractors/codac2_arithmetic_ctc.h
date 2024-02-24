/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCMINUS__
#define __CODAC2_CTCMINUS__

#include "codac2_Ctc.h"
#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
//  /**
//   * \class CtcMinus
//   */
//  class CtcMinus : public Ctc
//  {
//    public:
//
//      CtcMinus()
//      { }
//
//      void contract(Interval& a, Interval& b, Interval& c)
//      {
//        a &= b-c; b &= a+c; c &= b-a;
//      }
//
//      void contract(IntervalVector& a, IntervalVector& b, IntervalVector& c)
//      {
//        a &= b-c; b &= a+c; c &= b-a;
//      }
//
//      make_available_to_cn()
//  };
//
//  /**
//   * \class CtcPlus
//   */
//  class CtcPlus : public Ctc
//  {
//    public:
//
//      CtcPlus()
//      { }
//
//      void contract(Interval& a, Interval& b, Interval& c)
//      {
//        a &= b+c; b &= a-c; c &= a-b;
//      }
//
//      void contract(IntervalVector& a, IntervalVector& b, IntervalVector& c)
//      {
//        a &= b+c; b &= a-c; c &= a-b;
//      }
//
//      make_available_to_cn()
//  };
//
//  /**
//   * \class CtcMul
//   */
//  class CtcMul : public Ctc
//  {
//    public:
//
//      CtcMul()
//      { }
//
//      void contract(Interval& z, Interval& x, Interval& y)
//      {
//        z &= x*y; x &= z/y; y &= z/x;
//        bwd_mul(z,x,y);
//      }
//
//      make_available_to_cn()
//  };
//
//  /**
//   * \class CtcDet
//   */
//  class CtcDet : public Ctc
//  {
//    public:
//
//      CtcDet()
//      { }
//
//      void contract(Interval& det, IntervalVector& u, IntervalVector& v)
//      {
//        assert(u.size() == v.size());
//
//        CtcMinus ctc_minus;
//        CtcMul ctc_mul;
//
//        Interval z1 = u[0]*v[1];
//        Interval z2 = v[0]*u[1];
//        ctc_minus.contract(det,z1,z2);
//        ctc_mul.contract(z2,v[0],u[1]);
//        ctc_mul.contract(z1,u[0],v[1]);
//      }
//
//      make_available_to_cn()
//  };
//
//  /**
//   * \class CtcMax
//   */
//  class CtcMax : public Ctc
//  {
//    public:
//
//      CtcMax()
//      { }
//
//      void contract(Interval& m, Interval& z1, Interval& z2)
//      {
//        m &= max(z1,z2);
//        bwd_max(m,z1,z2);
//      }
//
//      make_available_to_cn();
//  };
//
//  namespace ctc
//  {
//    extern CtcPlus plus;
//    extern CtcMinus minus;
//    extern CtcMul mul;
//    extern CtcDet det;
//    extern CtcMax max;
//  }
}

#endif