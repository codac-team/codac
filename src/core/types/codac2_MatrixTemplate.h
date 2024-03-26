/** 
 *  \file
 *  MatrixTemplate_ class
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_MATRIXTEMPLATE_H__
#define __CODAC2_MATRIXTEMPLATE_H__

#include <ostream>
#include "codac2_eigen.h"

namespace codac2
{
  template<typename S,typename T,int R=Dynamic,int C=Dynamic>
  class MatrixTemplate_ : public Eigen::Matrix<T,R,C>
  {
    public:
    
      MatrixTemplate_()
        : Eigen::Matrix<T,R,C>()
      {
        static_assert(R != Dynamic && C != Dynamic);
      }
    
      MatrixTemplate_(int nb_rows, int nb_cols)
        : Eigen::Matrix<T,R,C>(nb_rows, nb_cols)
      {
        assert(R == Dynamic || R == (int)nb_rows);
        assert(C == Dynamic || C == (int)nb_cols);
      }
    
      MatrixTemplate_(int nb_rows, int nb_cols, const T& x)
        : Eigen::Matrix<T,R,C>(nb_rows, nb_cols)
      {
        init(x);
      }
      
      explicit MatrixTemplate_(int nb_rows, int nb_cols, const T values[])
        : MatrixTemplate_<S,T,R,C>(nb_rows, nb_cols)
      {
        assert(nb_rows != Dynamic && nb_cols != Dynamic);

        if(values == 0)
          init(T(0.)); // in case the user called MatrixTemplate_(r,c,0) and 0 is interpreted as NULL!

        else
        {
          size_t k = 0;
          for(int i = 0 ; i < this->rows() ; i++)
            for(int j = 0 ; j < this->cols() ; j++)
                (*this)(i,j) = values[k++];
          assert(k == this->size());
        }
      }
      
      explicit MatrixTemplate_(const T values[])
        : MatrixTemplate_<S,T,R,C>(R, C, values)
      {
        static_assert(R != Dynamic && C != Dynamic);
      }

      MatrixTemplate_(std::initializer_list<std::initializer_list<T>> l)
        : Eigen::Matrix<T,R,C>()
      {
        assert((R == Dynamic || (int)l.size() == R) && "ill-formed matrix");
        int cols = -1;
        for(const auto& ri : l) {
          assert((cols == -1 || cols == (int)ri.size()) && "ill-formed matrix");
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
        // todo: use this instead (faster?) ? std::copy(l.begin(), l.end(), vec);
      }
      
      template<typename OtherDerived>
      MatrixTemplate_(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<T,R,C>(other)
      { }
 
      MatrixTemplate_& operator=(const MatrixTemplate_<S,T,R,C>& other)
      {
        this->Eigen::Matrix<T,R,C>::operator=(other);
        return *this;
      }

      template<typename OtherDerived>
      MatrixTemplate_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<T,R,C>::operator=(other);
        return *this;
      }

      size_t size() const
      {
        return this->Eigen::Matrix<T,R,C>::size();
      }

      void init(const T& x)
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          *(this->data()+i) = x;
      }

      void resize(size_t nb_rows, size_t nb_cols)
      {
        // With resize of Eigen, the data is reallocated and all previous values are lost.
        auto copy = *this;
        this->Eigen::Matrix<T,R,C>::resize(nb_rows, nb_cols);
        for(size_t i = 0 ; i < std::min((size_t)copy.rows(),nb_rows) ; i++)
          for(size_t j = 0 ; j < std::min((size_t)copy.cols(),nb_cols) ; j++)
            (*this)(i,j) = copy(i,j);
      }

      static S zeros(size_t nb_rows = R, size_t nb_cols = C)
      {
        assert(nb_rows > 0 && nb_cols > 0);
        assert(((int)nb_rows == R || R == Dynamic) && ((int)nb_cols == C || C == Dynamic));
        return Eigen::Matrix<T,R,C>::Zero(nb_rows,nb_cols);
      }

      static S ones(size_t nb_rows = R, size_t nb_cols = C)
      {
        assert(nb_rows > 0 && nb_cols > 0);
        assert(((int)nb_rows == R || R == Dynamic) && ((int)nb_cols == C || C == Dynamic));
        return Eigen::Matrix<T,R,C>::Ones(nb_rows,nb_cols);
      }

      static S eye(size_t nb_rows = R, size_t nb_cols = C)
      {
        assert(nb_rows > 0 && nb_cols > 0);
        assert(((int)nb_rows == R || R == Dynamic) && ((int)nb_cols == C || C == Dynamic));
        return Eigen::Matrix<T,R,C>::Identity(nb_rows,nb_cols);
      }

      auto diagonal_matrix() const
      {
        return this->diagonal().asDiagonal();
      }
  };

  template<typename S,typename T,int R,int C>
  std::ostream& operator<<(std::ostream& os, const MatrixTemplate_<S,T,R,C>& x)
  {
    os << "(";
    for(int i = 0 ; i < x.rows() ; i++)
    {
      os << "(";
      for(int j = 0 ; j < x.cols() ; j++)
        os << x(i,j) << (j<x.cols()-1 ? " ; " : "");
      os << ")";
      if(i < x.rows()-1) os << std::endl;
    }
    os << ")";
    return os;
  }

} // namespace codac2

#endif