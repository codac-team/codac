/** 
 *  \file
 *  Matrix class
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_MATRIX_H__
#define __CODAC2_MATRIX_H__

#include "codac2_MatrixTemplate.h"

namespace codac2
{
  template<int R=Dynamic,int C=Dynamic>
  class Matrix_ : public MatrixTemplate_<Matrix_<R,C>,double,R,C>
  {
    public:
    
      Matrix_()
        : MatrixTemplate_<Matrix_<R,C>,double,R,C>()
      { }
    
      Matrix_(int nb_rows, int nb_cols)
        : MatrixTemplate_<Matrix_<R,C>,double,R,C>(nb_rows, nb_cols, 0.)
      { }
    
      Matrix_(int nb_rows, int nb_cols, const double& x)
        : MatrixTemplate_<Matrix_<R,C>,double,R,C>(nb_rows, nb_cols, x)
      { }
      
      explicit Matrix_(int nb_rows, int nb_cols, const double values[])
        : MatrixTemplate_<Matrix_<R,C>,double,R,C>(nb_rows, nb_cols, values)
      { }
      
      explicit Matrix_(const double values[])
        : MatrixTemplate_<Matrix_<R,C>,double,R,C>(values)
      {
        static_assert(R != Dynamic && C != Dynamic);
      }

      Matrix_(std::initializer_list<std::initializer_list<double>> l)
        : MatrixTemplate_<Matrix_<R,C>,double,R,C>(l)
      { }
      
      template<typename OtherDerived>
      Matrix_(const Eigen::MatrixBase<OtherDerived>& other)
          : MatrixTemplate_<Matrix_<R,C>,double,R,C>(other)
      { }

      double min_coeff() const
      {
        return Eigen::Matrix<double,R,C>::minCoeff();
      }

      double max_coeff() const
      {
        return Eigen::Matrix<double,R,C>::maxCoeff();
      }
  };

  using Matrix = Matrix_<Dynamic,Dynamic>;
  
} // namespace codac

#endif