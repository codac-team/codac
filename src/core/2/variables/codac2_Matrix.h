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

#ifndef __CODAC2_MATRIX_H__
#define __CODAC2_MATRIX_H__

#include <unsupported/Eigen/MatrixFunctions>
#include <Eigen/Core>
#include <Eigen/Dense>

namespace codac2
{
  using Eigen::Dynamic;

  template<int R,int C>
  class Matrix_ : public Eigen::Matrix<double,R,C>
  {
    public:
    
      Matrix_()
      {
        
      }
    
      Matrix_(size_t nb_rows, size_t nb_cols)
        : Eigen::Matrix<double,R,C>(nb_rows, nb_cols)
      {
        assert(R == Dynamic || R == nb_rows);
        assert(C == Dynamic || C == nb_cols);
      }
      
      template<typename OtherDerived>
      Matrix_(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<double,R,C>(other)
      { }

      // This method allows you to assign Eigen expressions to Matrix_
      template<typename OtherDerived>
      Matrix_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<double,R,C>::operator=(other);
        return *this;
      }

      static Matrix_ eye()
      {
        return Eigen::Matrix<double,R,C>::Identity();
      }

  };

} // namespace codac

#endif