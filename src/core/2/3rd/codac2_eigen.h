#ifndef EIGEN_NO_DEBUG
/* Disables Eigen's assertions if defined.
 * Not defined by default, unless the NDEBUG macro is defined
 * (this is a standard C++ macro which disables all asserts).
 * https://eigen.tuxfamily.org/dox/TopicPreprocessorDirectives.html
 */
#define EIGEN_NO_DEBUG
#endif

#ifndef __CODAC2_EIGEN_H__
#define __CODAC2_EIGEN_H__

#include <Eigen/Core>
#include <Eigen/Dense>
#include "codac2_Interval.h"

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
  inline Interval abs2(const Interval& x)         { return ibex::sqr(x); }

} // namespace codac

#endif