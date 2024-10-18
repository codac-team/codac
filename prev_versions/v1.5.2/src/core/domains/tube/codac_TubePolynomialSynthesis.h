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
  struct Polynomial
  {
    // Form: ax² + bx + c + offset = 0
    std::array<double,POLYNOMIAL_ORDER+1> coeff;
    double offset = 0.;

    Interval f(const Interval& t) const
    {
      Interval x = offset;
      for(int i = 0 ; i < POLYNOMIAL_ORDER+1 ; i++)
        x += coeff[i]*pow(t,i);
      return x;
    }
  };

  struct PolynomialFactoredForm
  {
    // Form: c(x-a)² + b + offset = 0
    double a, b, c;
    double offset = 0.;

    Interval f(const Interval& t) const
    {
      return c*sqr(t-a) + b + offset;
    }
  };

  class VIBesFigTube;

  class TubePolynomialTreeSynthesis
  {
    public:

      TubePolynomialTreeSynthesis(const Tube& x, const Interval& tdomain, bool upper_bound, double eps, VIBesFigTube& m_fig);
      ~TubePolynomialTreeSynthesis();
      Interval operator()(const Interval& t) const;


    protected:

      Polynomial polyfit(const std::vector<double> &t, const std::vector<double> &v) const;
      void get_bounds(const Interval& tdomain, bool upper_bound, std::vector<double>& t, std::vector<double>& v) const;
      Trajectory traj_from_polynom(const PolynomialFactoredForm& p) const;
      PolynomialFactoredForm polynomial_factoredform(bool upper_bound) const;


    protected:

      const Tube& m_tube_ref;
      const Interval m_tdomain;
      bool m_upper_bound;
      PolynomialFactoredForm m_p;
      Interval m_codomain;
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