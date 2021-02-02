#ifndef CODAC_LIB_CODAC_CAPD_HELPERS_H
#define CODAC_LIB_CODAC_CAPD_HELPERS_H

#include "capd/capdlib.h"
#include "ibex.h"

namespace codac {
class CapdHelpers {
  public:
    static ibex::Interval c2i_i(const capd::interval &x);
    static ibex::IntervalVector c2i_v(const capd::IVector &x);
    static ibex::IntervalMatrix c2i_m(const capd::IMatrix &x);
    static capd::Interval i2c_i(const ibex::Interval &x);
    static capd::IVector i2c_v(const ibex::IntervalVector &x);
    static capd::IVector i2c_v_mid(const ibex::IntervalVector &x);
    static capd::IMatrix i2c_m(const ibex::IntervalMatrix &x);
    static capd::IMatrix i2c_m_mid(const ibex::IntervalMatrix &x);
};
}

#endif //CODAC_LIB_CODAC_CAPD_HELPERS_H
