#ifndef CODAC_LIB_CODAC_CAPD_HELPERS_H
#define CODAC_LIB_CODAC_CAPD_HELPERS_H

#include "capd/capdlib.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"

namespace codac {
class CapdHelpers {
  public:
    static Interval c2i_i(const capd::interval &x);
    static IntervalVector c2i_v(const capd::IVector &x);
    static IntervalMatrix c2i_m(const capd::IMatrix &x);
    static capd::Interval i2c_i(const Interval &x);
    static capd::IVector i2c_v(const IntervalVector &x);
    static capd::IVector i2c_v_mid(const IntervalVector &x);
    static capd::IMatrix i2c_m(const IntervalMatrix &x);
    static capd::IMatrix i2c_m_mid(const IntervalMatrix &x);
};
}

#endif //CODAC_LIB_CODAC_CAPD_HELPERS_H
