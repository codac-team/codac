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
  template<size_t R,size_t C>
  class Matrix : public Eigen::Matrix<double,R,C>
  {
    public:
    
      Matrix()
      {
        
      }
      
      template<typename OtherDerived>
      Matrix(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<double,R,C>(other)
      { }

      // This method allows you to assign Eigen expressions to MyVectorType
      template<typename OtherDerived>
      Matrix& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<double,R,C>::operator=(other);
        return *this;
      }

      static Matrix eye()
      {
        return Eigen::Matrix<double,R,C>::Identity();
      }

  };

} // namespace codac

#endif