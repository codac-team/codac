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

#ifndef __CODAC2_INTERVALMATRIX_H__
#define __CODAC2_INTERVALMATRIX_H__

#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <ibex_LargestFirst.h>

namespace codac2
{
  template<int NbRows,int NbCols>
  class IntervalMatrix_ : public Eigen::Matrix<Interval,NbRows,NbCols>
  {
    public:

  };

} // namespace codac

#endif