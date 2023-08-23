/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_INTERVAL_H__
#define __CODAC2_INTERVAL_H__

#include <codac_Interval.h>

namespace codac2
{
  using codac::Interval;

} // namespace codac

namespace Eigen
{
  template<> struct NumTraits<codac2::Interval>
   : NumTraits<double> // permits to get the epsilon, dummy_precision, lowest, highest functions
  {
    typedef codac2::Interval Real;
    typedef codac2::Interval NonInteger;
    typedef codac2::Interval Nested;
   
    enum {
      IsComplex = 0,
      IsInteger = 0,
      IsSigned = 1,
      RequireInitialization = 1,
      ReadCost = 1,
      AddCost = 3,
      MulCost = 3
    };
  };
}

namespace codac2
{
  inline const Interval& conj(const Interval& x)  { return x; }
  inline const Interval& real(const Interval& x)  { return x; }
  inline Interval imag(const Interval&)           { return 0.; }
  inline Interval abs(const Interval& x)          { return ibex::abs(x); }
  inline Interval abs2(const Interval& x)         { return x*x; }

} // namespace codac

#endif