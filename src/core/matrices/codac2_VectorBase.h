/** 
 *  \file codac2_VectorBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_MatrixBase.h"

namespace codac2
{
  template<typename S,typename M,typename T>
  class VectorBase : virtual public MatrixBase<S,T>
  {
    public:

      explicit VectorBase(size_t n)
        : MatrixBase<S,T>(n,1)
      {
        assert_release(n >= 0);
      }

      VectorBase(std::initializer_list<T> l)
        : MatrixBase<S,T>(l.size(),1)
      {
        assert(!std::empty(l));
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
      }

      VectorBase(const std::vector<T>& l)
        : MatrixBase<S,T>(l.size(),1)
      {
        assert(!std::empty(l));
        size_t i = 0;
        for(const auto& li : l)
          (*this)[i++] = li;
      }

      template<typename OtherDerived>
      VectorBase(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<S,T>(x)
      { }

      bool is_squared() const = delete;

      T& operator[](size_t i)
      {
        return const_cast<T&>(const_cast<const VectorBase<S,M,T>*>(this)->operator[](i));
      }

      const T& operator[](size_t i) const
      {
        assert_release(i >= 0 && i < this->size());
        return this->_e(i);
      }
      
      S subvector(size_t start_id, size_t end_id) const
      {
        assert_release(end_id >= 0 && start_id >= 0);
        assert_release(end_id < this->size() && start_id <= end_id);
        return this->_e.block(start_id, 0, end_id-start_id+1, 1);
      }

      void resize(size_t n)
      {
        assert_release(n >= 0);
        MatrixBase<S,T>::resize(n,1);
      }

      void put(size_t start_id, const S& x)
      {
        assert_release(start_id >= 0 && start_id < this->size());
        assert_release(start_id+x.size() <= this->size());
        this->_e.block(start_id,0,x.size(),1) << x._e;
      }

      M transpose() const
      {
        return this->_e.transpose();
      }

      M diag_matrix() const
      {
        M diag(this->size(),this->size(),0.);
        for(size_t i = 0 ; i < this->size() ; i++)
          diag(i,i) = (*this)[i];
        return diag;
      }

      static S zeros(size_t n)
      {
        assert_release(n >= 0);
        return EigenMatrix<T>::Zero(n,1);
      }

      static S ones(size_t n)
      {
        assert_release(n >= 0);
        return EigenMatrix<T>::Ones(n,1);
      }

  };

  template<typename S,typename M,typename T>
  std::ostream& operator<<(std::ostream& os, const VectorBase<S,M,T>& x)
  {
    os << "(";
    for(size_t i = 0 ; i < x.size() ; i++)
      os << x[i] << (i<x.size()-1 ? " ; " : "");
    os << ")";
    return os;
  }

}