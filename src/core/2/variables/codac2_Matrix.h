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

#include <codac_Matrix.h>
#include <codac2_eigen.h>

namespace codac2
{
  using Eigen::Dynamic;

  template<int R,int C>
  class Matrix_ : public Eigen::Matrix<double,R,C>
  {
    public:
    
      Matrix_()
        : Eigen::Matrix<double,R,C>()
      { }
    
      Matrix_(size_t nb_rows, size_t nb_cols)
        : Eigen::Matrix<double,R,C>(nb_rows, nb_cols)
      {
        assert(R == Dynamic || R == (int)nb_rows);
        assert(C == Dynamic || C == (int)nb_cols);
      }
    
      Matrix_(size_t nb_rows, size_t nb_cols, double x)
        : Eigen::Matrix<double,R,C>(nb_rows, nb_cols)
      {
        assert(R == Dynamic || R == (int)nb_rows);
        assert(C == Dynamic || C == (int)nb_cols);
        init(x);
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

      Matrix_(std::initializer_list<std::initializer_list<double>> l)
        : Matrix_<R,C>()
      {
        assert((R == Dynamic || (int)l.size() == R) && "ill-formed matrix");
        int cols = -1;
        for(const auto& ri : l) {
          assert(cols == -1 || cols == (int)ri.size() && "ill-formed matrix");
          cols = (int)ri.size();
        }
        this->resize(l.size(),cols);
        size_t i = 0;
        for(const auto& ri : l)
        {
          size_t j = 0;
          for(const auto& ci : ri)
            (*this)(i,j++) = ci;
          i++;
        }
        // todo: use thias as faster? std::copy(l.begin(), l.end(), vec);
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

      void init(double x)
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          *(this->data()+i) = x;
      }

      static Matrix_ eye()
      {
        return Eigen::Matrix<double,R,C>::Identity();
      }

      double min() const
      {
        return Eigen::Matrix<double,R,C>::minCoeff();
      }

      double max() const
      {
        return Eigen::Matrix<double,R,C>::maxCoeff();
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

      auto operator-() const
      {
        return Eigen::Matrix<double,R,C>::operator-();
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

      static Matrix_<R,C> ones()
      {
        return Eigen::Matrix<double,R,C>::Ones();
      }
  };

  template<int R,int C>
  std::ostream& operator<<(std::ostream& os, const Matrix_<R,C>& x)
  {
    os << "(";
    for(size_t i = 0 ; i < x.rows() ; i++)
    {
      os << "(";
      for(size_t j = 0 ; j < x.cols() ; j++)
        os << x(i,j) << (j<x.cols()-1 ? " ; " : "");
      os << ")";
      if(i < x.rows()-1) os << std::endl;
    }
    os << ")";
    return os;
  }
  
  template<int R,int C>
  Matrix_<R,C> floor(const Matrix_<R,C>& x)
  {
    Matrix_<R,C> f(x.rows(), x.cols());
    for(size_t i = 0 ; i < x.size() ; i++)
      *(f.data()+i) = std::floor(*(x.data()+i));
    return f;
  }

  template<int R,int C>
  Matrix_<R,C> round(const Matrix_<R,C>& x)
  {
    Matrix_<R,C> f(x.rows(), x.cols());
    for(size_t i = 0 ; i < x.size() ; i++)
      *(f.data()+i) = std::round(*(x.data()+i));
    return f;
  }

  template<int R,int C>
  Matrix_<R,C> ceil(const Matrix_<R,C>& x)
  {
    Matrix_<R,C> f(x.rows(), x.cols());
    for(size_t i = 0 ; i < x.size() ; i++)
      *(f.data()+i) = std::ceil(*(x.data()+i));
    return f;
  }

  template<int R,int C>
  Matrix_<R,C> abs(const Matrix_<R,C>& x)
  {
    Matrix_<R,C> f(x.rows(), x.cols());
    for(size_t i = 0 ; i < x.size() ; i++)
      *(f.data()+i) = std::fabs(*(x.data()+i));
    return f;
  }

} // namespace codac

#endif