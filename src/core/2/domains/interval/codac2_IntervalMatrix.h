/** 
 *  \file
 *  
 *  This class reuses many of the functions developed for ibex::IntervalMatrix. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrix (IBEX lib, author: G. Chabert)
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

#include <math.h>
#include <codac2_Interval.h>
#include <codac2_eigen.h>
#include <ibex_LargestFirst.h>
#include <codac2_Matrix.h>
#include <codac2_Domain.h>

namespace codac2
{
  using Eigen::Dynamic;

  template<int R=Dynamic,int C=Dynamic>
  class IntervalMatrix_ : public Eigen::Matrix<Interval,R,C>, public Domain
  {
    public:

      IntervalMatrix_()
        : Eigen::Matrix<Interval,R,C>()
      {

      }

      explicit IntervalMatrix_(size_t nb_rows, size_t nb_cols)
        : Eigen::Matrix<Interval,R,C>(nb_rows, nb_cols)
      {
        assert(R == Dynamic || R == (int)nb_rows);
        assert(C == Dynamic || C == (int)nb_cols);
      }

      explicit IntervalMatrix_(size_t nb_rows, size_t nb_cols, const Interval& x)
        : IntervalMatrix_<R,C>(nb_rows, nb_cols)
      {
        init(x);
      }
      
      explicit IntervalMatrix_(size_t nb_rows, size_t nb_cols, const double bounds[][2])
        : IntervalMatrix_<R,C>(nb_rows, nb_cols)
      {
        size_t k = 0;
        for(size_t i = 0 ; i < nb_rows ; i++)
          for(size_t j = 0 ; j < nb_cols ; j++)
          {
            if(bounds == 0) // in case the user called IntervalMatrix_(r,c,0) and 0 is interpreted as NULL!
              (*this)(i,j) = Interval::zero();
            else
              (*this)(i,j) = Interval(bounds[k][0],bounds[k][1]);
            k++;
          }
        assert(k == this->size());
      }
      
      explicit IntervalMatrix_(const double bounds[][2])
        : IntervalMatrix_<R,C>(R, C, bounds)
      { }
      
      explicit IntervalMatrix_(const Matrix_<R,C>& lb, const Matrix_<R,C>& ub)
        : IntervalMatrix_<R,C>(lb.rows(), lb.cols())
      {
        assert(lb.rows() == ub.rows() && lb.cols() == ub.cols());
        for(size_t i = 0 ; i < this->rows() ; i++)
          for(size_t j = 0 ; j < this->cols() ; j++)
              (*this)(i,j) = Interval(lb(i,j),ub(i,j));
      }
      
      explicit IntervalMatrix_(const IntervalMatrix_<R,C>& x)
        : IntervalMatrix_<R,C>(x.rows(), x.cols())
      {
        for(size_t i = 0 ; i < size() ; i++)
          *(this->data()+i) = *(x.data()+i);
      }
      
      IntervalMatrix_(std::initializer_list<std::initializer_list<Interval>> l)
        : IntervalMatrix_<R,C>()
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
      }

      // This constructor allows you to construct IntervalMatrix_ from Eigen expressions
      template<typename OtherDerived>
      IntervalMatrix_(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<Interval,R,C>(other.template cast<Interval>())
      { }
 
      // This method allows you to assign Eigen expressions to IntervalMatrix_
      template<typename OtherDerived>
      IntervalMatrix_& operator=(const Eigen::MatrixBase<OtherDerived>& other)
      {
        this->Eigen::Matrix<Interval,R,C>::operator=(other);
        return *this;
      }

      constexpr size_t size() const
      {
        return this->Eigen::Matrix<Interval,R,C>::size();
      }

      void resize(size_t nb_rows, size_t nb_cols)
      {
        // With resize of Eigen, the data is reallocated and all previous values are lost.
        auto copy = *this;
        this->Eigen::Matrix<Interval,R,C>::resize(nb_rows, nb_cols);
        for(size_t i = 0 ; i < std::min((size_t)copy.rows(),nb_rows) ; i++)
          for(size_t j = 0 ; j < std::min((size_t)copy.cols(),nb_cols) ; j++)
            (*this)(i,j) = copy(i,j);
      }

      bool is_empty() const
      {
        for(size_t i = 0 ; i < size() ; i++)
          if((this->data()+i)->is_empty())
            return true;
        return false;
      }

      static IntervalMatrix_<R,C> empty_set(size_t nb_rows = R, size_t nb_cols = C)
      {
        return IntervalMatrix_<R,C>(nb_rows, nb_cols, Interval::empty_set());
      }

      static IntervalMatrix_ eye()
      {
        return Eigen::Matrix<Interval,R,C>::Identity();
      }

      bool is_flat() const
      {
        if(is_empty()) return true;
        for(size_t i = 0 ; i < size() ; i++)
          if((this->data()+i)->is_degenerated()) // don't use diam() because of roundoff
            return true;
        return false;
      }

      bool is_unbounded() const
      {
        if(is_empty()) return false;
        for(size_t i = 0 ; i < size() ; i++)
          if((this->data()+i)->is_unbounded())
            return true;
        return false;
      }

      bool is_subset(const IntervalMatrix_<R,C>& x) const
      {
        if(is_empty()) return true;
        if(x.is_empty()) return false;
        for(size_t i = 0 ; i < size() ; i++)
          if(!(this->data()+i)->is_subset(*(x.data()+i)))
            return false;
        return true;
      }

      bool is_strict_subset(const IntervalMatrix_<R,C>& x) const
      {
        if(x.is_empty()) return false;
        if(is_empty()) return true;
        bool one_dim_strict_subset = false;
        for(size_t i = 0 ; i < size() ; i++)
        {
          if((this->data()+i)->is_strict_subset(*(x.data()+i)))
            one_dim_strict_subset = true;
          if(!(this->data()+i)->is_subset(*(x.data()+i)))
            return false;
        }
        return one_dim_strict_subset;
      }

      bool is_superset(const IntervalMatrix_<R,C>& x) const
      {
        return x.is_subset(*this);
      }

      bool is_strict_superset(const IntervalMatrix_<R,C>& x) const
      {
        return x.is_strict_subset(*this);
      }

      bool contains(const Matrix_<R,C>& x) const
      {
        if(is_empty())
          return false;
        for(size_t i = 0 ; i < size() ; i++)
          if(!(this->data()+i)->contains(*(x.data()+i)))
            return false;
        return true;
      }

      bool interior_contains(const IntervalMatrix_<R,C>& x) const
      {
        if(is_empty())
          return false;
        for(size_t i = 0 ; i < size() ; i++)
          if(!(this->data()+i)->interior_contains(*(x.data()+i)))
            return false;
        return true;
      }

      bool intersects(const IntervalMatrix_<R,C>& x) const
      {
        if(is_empty() || x.is_empty())
          return false;
        for(size_t i = 0 ; i < size() ; i++)
          if(!(this->data()+i)->intersects(*(x.data()+i)))
            return false;
        return true;
      }

      bool overlaps(const IntervalMatrix_<R,C>& x) const
      {
        if(is_empty() || x.is_empty())
          return false;
        for(size_t i = 0 ; i < size() ; i++)
          if(!(this->data()+i)->overlaps(*(x.data()+i)))
            return false;
        return true;
      }

      bool is_disjoint(const IntervalMatrix_<R,C>& x) const
      {
        if(is_empty() || x.is_empty())
          return true;
        for(size_t i = 0 ; i < size() ; i++)
          if((this->data()+i)->is_disjoint(*(x.data()+i)))
            return true;
        return false;
      }

      bool is_bisectable() const
      {
        for(size_t i = 0 ; i < size() ; i++)
          if((this->data()+i)->is_bisectable())
            return true;
        return false;
      }

      DomainVolume dom_volume() const
      {
        DomainVolume v(size());
        for(size_t i = 0 ; i < size() ; i++)
          v[i] = (this->data()+i)->diam();
        return v;
      }

      double min_diam() const
      {
        return (this->data()+extr_diam_index(true))->diam();
      }

      double max_diam() const
      {
        return (this->data()+extr_diam_index(false))->diam();
      }

      size_t thinnest_diam_index() const
      {
        return extr_diam_index(true);
      }

      size_t largest_diam_index() const
      {
        return extr_diam_index(false);
      }

      size_t extr_diam_index(bool min) const
      {
        // This code originates from the ibex-lib
        // See: ibex_TemplateVector.h
        // Author: Gilles Chabert

        double d = min ? POS_INFINITY : -1; // -1 to be sure that even a 0-diameter interval can be selected
        int selected_index = -1;
        bool unbounded = false;
        assert(!this->is_empty() && "Diameter of an empty IntervalVector is undefined");

        size_t i;

        for(i = 0 ; i < this->size() ; i++) 
        {
          if((this->data()+i)->is_unbounded()) 
          {
            unbounded = true;
            if(!min) break;
          }
          else
          {
            double w = (this->data()+i)->diam();
            if(min ? w<d : w>d)
            {
              selected_index = i;
              d = w;
            }
          }
        }

        if(min && selected_index == -1)
        {
          assert(unbounded);
          // the selected interval is the first one.
          i = 0;
        }

        // The unbounded intervals are not considered if we look for the minimal diameter
        // and some bounded intervals have been found (selected_index!=-1)
        if(unbounded && (!min || selected_index == -1))
        {
          double pt = min ? NEG_INFINITY : POS_INFINITY; // keep the point less/most distant from +oo (we normalize if the lower bound is -oo)
          selected_index = i;
          for(; i < this->size() ; i++)
          {
            if((this->data()+i)->lb() == NEG_INFINITY)
            {
              if((this->data()+i)->ub() == POS_INFINITY)
              {
                if(!min)
                {
                  selected_index = i;
                  break;
                }
              }
              if((min && (-(this->data()+i)->ub() > pt)) || (!min && (-(this->data()+i)->ub() < pt)))
              {
                selected_index = i;
                pt = -(this->data()+i)->ub();
              }
            }
            else if((this->data()+i)->ub() == POS_INFINITY)
            {
              if((min && ((this->data()+i)->lb() > pt)) || (!min && ((this->data()+i)->lb() < pt)))
              {
                selected_index = i;
                pt = (this->data()+i)->lb();
              }
            }
          }
        }

        return selected_index;
      }

      auto bisect(float ratio = 0.49) const
      {
        size_t i = largest_diam_index();
        assert((this->data()+i)->is_bisectable());
        assert(Interval(0,1).interior_contains(ratio));

        auto p = std::make_pair(*this,*this);
        auto pi = (this->data()+i)->bisect(ratio);
        *(p.first.data()+i) = pi.first;
        *(p.second.data()+i) = pi.second;
        return p;
      }

      double volume() const
      {
        if(this->is_empty())
          return 0.;
        double v = 0.;
        for(size_t i = 0 ; i < this->size() ; i++)
        {
          if((this->data()+i)->is_unbounded()) return POS_INFINITY;
          if((this->data()+i)->is_degenerated()) return 0.;
          v += log((this->data()+i)->diam());
        }
        return exp(v);
      }

      Matrix_<R,C> lb() const
      {
        assert(!this->is_empty()); // todo: use nan instead of assert?
        Matrix_<R,C> lb(this->rows(), this->cols());
        for(size_t i = 0 ; i < this->size() ; i++)
          *(lb.data()+i) = (this->data()+i)->lb();
        return lb;
      }

      Matrix_<R,C> ub() const
      {
        assert(!this->is_empty()); // todo: use nan instead of assert?
        Matrix_<R,C> ub(this->rows(), this->cols());
        for(size_t i = 0 ; i < this->size() ; i++)
          *(ub.data()+i) = (this->data()+i)->ub();
        return ub;
      }

      Matrix_<R,C> mid() const
      {
        assert(!this->is_empty()); // todo: use nan instead of assert?
        Matrix_<R,C> mid(this->rows(), this->cols());
        for(size_t i = 0 ; i < this->size() ; i++)
          *(mid.data()+i) = (this->data()+i)->mid();
        return mid;
      }

      Matrix_<R,C> rad() const
      {
        assert(!this->is_empty()); // todo: use nan instead of assert?
        Matrix_<R,C> rad(this->rows(), this->cols());
        for(size_t i = 0 ; i < this->size() ; i++)
          *(rad.data()+i) = (this->data()+i)->rad();
        return rad;
      }

      Matrix_<R,C> diam() const
      {
        assert(!this->is_empty()); // todo: use nan instead of assert?
        Matrix_<R,C> diam(this->rows(), this->cols());
        for(size_t i = 0 ; i < this->size() ; i++)
          *(diam.data()+i) = (this->data()+i)->diam();
        return diam;
      }

      void init(const Interval& x)
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          *(this->data()+i) = x;
      }

      void set_empty()
      {
        init(Interval::empty_set());
      }

      auto& inflate(double r)
      {
        assert(r >= 0.);
        for(size_t i = 0 ; i < this->size() ; i++)
          (this->data()+i)->inflate(r);
        return *this;
      }

      auto& inflate(const Matrix_<R,C>& r)
      {
        assert(r.minCoeff() >= 0.);
        for(size_t i = 0 ; i < this->size() ; i++)
          (this->data()+i)->inflate(*(r.data()+i));
        return *this;
      }

      bool operator==(const IntervalMatrix_<R,C>& x) const
      {
        if(x.size() != this->size() || x.rows() != this->rows() || x.cols() != this->cols())
          return false;
        if(is_empty() || x.is_empty())
          return is_empty() && x.is_empty();
        for(size_t i = 0 ; i < this->size() ; i++)
          if(*(this->data()+i) != *(x.data()+i))
            return false;
        return true;
      }

      bool operator!=(const IntervalMatrix_<R,C>& x) const
      {
        return !(*this == x);
      }

      auto& operator&=(const IntervalMatrix_<R,C>& x)
      {
        if(!this->is_empty())
        {
          if(x.is_empty())
            this->set_empty();
          else
            for(size_t i = 0 ; i < this->size() ; i++)
              *(this->data()+i) &= *(x.data()+i);
        }
        return *this;
      }

      auto& operator|=(const IntervalMatrix_<R,C>& x)
      {
        if(!x.is_empty())
        {
          if(this->is_empty())
            *this = x;
          else
            for(size_t i = 0 ; i < this->size() ; i++)
              *(this->data()+i) |= *(x.data()+i);
        }
        return *this;
      }

      auto operator+(const IntervalMatrix_<R,C>& x) const
      {
        auto y = *this;
        return y += x;
      }

      auto operator-(const IntervalMatrix_<R,C>& x) const
      {
        auto y = *this;
        return y -= x;
      }

      auto operator&(const IntervalMatrix_<R,C>& x) const
      {
        auto y = *this;
        return y &= x;
      }

      auto operator|(const IntervalMatrix_<R,C>& x) const
      {
        auto y = *this;
        return y |= x;
      }

      auto& operator+=(const IntervalMatrix_<R,C>& x)
      {
        (*this).noalias() += x;//.template cast<Interval>();
        return *this;
      }
      
      auto& operator-=(const IntervalMatrix_<R,C>& x)
      {
        (*this).noalias() -= x;//.template cast<Interval>();
        return *this;
      }

      auto& operator+=(const Matrix_<R,C>& x)
      {
        (*this).noalias() += x.template cast<Interval>();
        return *this;
      }
      
      auto& operator-=(const Matrix_<R,C>& x)
      {
        (*this).noalias() -= x;//.template cast<Interval>();
        return *this;
      }
  };

  template<int R,int C>
  std::ostream& operator<<(std::ostream& os, const IntervalMatrix_<R,C>& x)
  {
    if(x.is_empty()) return os << "empty matrix";
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

  template<int R,int C>
  auto operator-(const IntervalMatrix_<R,C>& x)
  {
    auto y = x;
    y.init(0.);
    return y -= x;
  }

  template<int R,int C>
  auto operator*(double a, const IntervalMatrix_<R,C>& x)
  {
    return Interval(a)*x;
  }

  class IntervalMatrix : public IntervalMatrix_<>
  {
    public:

      explicit IntervalMatrix(size_t nb_rows, size_t nb_cols)
        : IntervalMatrix_<>(nb_rows, nb_cols)
      { }


      explicit IntervalMatrix(size_t nb_rows, size_t nb_cols, const Interval& x)
        : IntervalMatrix_<>(nb_rows, nb_cols, x)
      { }

      explicit IntervalMatrix(size_t nb_rows, size_t nb_cols, const double bounds[][2])
        : IntervalMatrix_<>(nb_rows, nb_cols, bounds)
      { }

      IntervalMatrix(const IntervalMatrix_<>& x)
        : IntervalMatrix_<>(x)
      { }
      
      IntervalMatrix(std::initializer_list<std::initializer_list<Interval>> l)
        : IntervalMatrix_<>(l)
      { }
      
      template<int R,int C>
      explicit IntervalMatrix(const Matrix_<R,C>& v)
        : IntervalMatrix_<>(v)
      { }

      static IntervalMatrix empty_set(size_t nb_rows, size_t nb_cols)
      {
        return IntervalMatrix_<>::empty_set(nb_rows,nb_cols);
      }
  };

} // namespace codac

#endif