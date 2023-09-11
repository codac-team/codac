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
        : Eigen::Matrix<double,R,C>()
      {
        
      }
    
      Matrix_(size_t nb_rows, size_t nb_cols)
        : Eigen::Matrix<double,R,C>(nb_rows, nb_cols)
      {
        assert(R == Dynamic || R == (int)nb_rows);
        assert(C == Dynamic || C == (int)nb_cols);
      }
      
      explicit Matrix_(size_t nb_rows, size_t nb_cols, double values[])
        : Matrix_<R,C>(nb_rows, nb_cols)
      {
        size_t k = 0;
        for(size_t i = 0 ; i < nb_rows ; i++)
          for(size_t j = 0 ; j < nb_cols ; j++)
          {
            if(values == 0) // in case the user called Matrix_(r,c,0) and 0 is interpreted as NULL!
              (*this)(i,j) = 0.;
            else
              (*this)(i,j) = values[k];
            k++;
          }
      }
      
      explicit Matrix_(double values[])
        : Matrix_<R,C>(R, C, values)
      { }
      
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

      auto operator+(const Matrix_<R,C>& x) const
      {
        auto y = *this;
        return y += x;
      }

      auto operator-(const Matrix_<R,C>& x) const
      {
        auto y = *this;
        return y -= x;
      }

      auto operator&(const Matrix_<R,C>& x) const
      {
        auto y = *this;
        return y &= x;
      }

      auto operator|(const Matrix_<R,C>& x) const
      {
        auto y = *this;
        return y |= x;
      }

      auto& operator+=(const Matrix_<R,C>& x)
      {
        (*this).noalias() += x;//.template cast<Interval>();
        return *this;
      }
      
      auto& operator-=(const Matrix_<R,C>& x)
      {
        (*this).noalias() -= x;//.template cast<Interval>();
        return *this;
      }

      static Matrix_<R,C> zeros()
      {
        return Eigen::Matrix<double,R,C>::Zero();
      }

  };

} // namespace codac

#endif