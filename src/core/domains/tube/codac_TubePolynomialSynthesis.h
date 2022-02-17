/** 
 *  TubePolynomialSynthesis class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_TUBEPOLYNOMIALSYNTHESIS_H__
#define __CODAC_TUBEPOLYNOMIALSYNTHESIS_H__

#include <map>
#include <array>
#include <vector>
#include "codac_VIBesFigTube.h"
#include "codac_Interval.h"
#include "codac_Tube.h"
#include "codac_Trajectory.h"

#define POLYNOMIAL_ORDER 2

namespace codac
{
  struct PolynomialSubtraj
  {
    Interval codomain;
    std::array<double,POLYNOMIAL_ORDER+1> coeff;
    double offset = 0.;
    double a, b, c;
    bool factor_form = false;
  };

  class VIBesFigTube;

  class TubePolynomialTreeSynthesis
  {
    public:

      TubePolynomialTreeSynthesis(const Tube& x, const Interval& tdomain, bool upper_bound, double eps, VIBesFigTube& m_fig);
      ~TubePolynomialTreeSynthesis();
      Interval operator()(const Interval& t) const;


    protected:

      Interval f(const PolynomialSubtraj& p, const Interval& t) const;
      void polyfit(const std::vector<double> &t, const std::vector<double> &v, std::array<double,POLYNOMIAL_ORDER+1> &coeff) const;
      void get_bounds(const Interval& tdomain, bool upper_bound, std::vector<double>& t, std::vector<double>& v) const;
      Trajectory traj_from_polynom(const PolynomialSubtraj& p) const;
      PolynomialSubtraj polynomial_traj(bool upper_bound) const;


    protected:

      const Tube& m_tube_ref;
      const Interval m_tdomain;
      bool m_upper_bound;
      PolynomialSubtraj m_p;
      TubePolynomialTreeSynthesis *m_left = nullptr, *m_right = nullptr;
      VIBesFigTube& m_fig;
  };

  class TubePolynomialSynthesis
  {
    public:

      TubePolynomialSynthesis(const Tube& x, double eps);
      ~TubePolynomialSynthesis();
      Interval operator()(const Interval& t) const;


    protected:

      VIBesFigTube *m_fig;
      std::pair<TubePolynomialTreeSynthesis,TubePolynomialTreeSynthesis> m_trees;
  };
}

#endif