/** 
 *  \file
 *  IntervalMatrix class
 *  
 *  This class reuses some of the functions developed for ibex::IntervalMatrix. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrix (IBEX lib, author: Gilles Chabert)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <math.h>
#include <limits>
#include <ibex_LargestFirst.h>
#include "codac2_Domain.h"
#include "codac2_Interval.h"
#include "codac2_MatrixTemplate.h"
#include "codac2_Matrix.h"

namespace codac2
{
  using Eigen::Dynamic;

  template<typename S,typename V,int R=Dynamic,int C=Dynamic>
  class IntervalMatrixTemplate_ : public MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>, public DomainInterface<S,V>
  {
    public:

      IntervalMatrixTemplate_()
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>()
      { }
    
      IntervalMatrixTemplate_(int nb_rows, int nb_cols)
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>(nb_rows, nb_cols)
      { }
    
      IntervalMatrixTemplate_(int nb_rows, int nb_cols, const Interval& x)
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>(nb_rows, nb_cols, x)
      { }
      
      explicit IntervalMatrixTemplate_(int nb_rows, int nb_cols, const double bounds[][2])
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>(nb_rows, nb_cols)
      {
        size_t k = 0;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
          {
            (*this)(i,j) = Interval(bounds[k][0],bounds[k][1]);
            k++;
          }
        assert(k == this->size());
      }
      
      explicit IntervalMatrixTemplate_(int nb_rows, int nb_cols, const Interval values[])
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>(nb_rows, nb_cols, values)
      { }
      
      explicit IntervalMatrixTemplate_(const Interval values[])
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>(values)
      {
        static_assert(R != Dynamic && C != Dynamic);
      }

      IntervalMatrixTemplate_(std::initializer_list<std::initializer_list<Interval>> l)
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>(l)
      { }

      IntervalMatrixTemplate_(const Matrix_<R,C>& x)
        : IntervalMatrixTemplate_<S,V,R,C>(x.template cast<Interval>())
      { }

      IntervalMatrixTemplate_(const Matrix_<R,C>& lb, const Matrix_<R,C>& ub)
        : IntervalMatrixTemplate_<S,V,R,C>(lb)
      {
        *this |= ub;
      }
      
      template<typename OtherDerived>
      IntervalMatrixTemplate_(const Eigen::MatrixBase<OtherDerived>& other)
        : MatrixTemplate_<IntervalMatrixTemplate_<S,V,R,C>,Interval,R,C>(other.template cast<Interval>())
      { }

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

        double d = min ? oo : -1; // -1 to be sure that even a 0-diameter interval can be selected
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
          double pt = min ? -oo : oo; // keep the point less/most distant from +oo (we normalize if the lower bound is -oo)
          selected_index = i;

          for(; i < this->size() ; i++)
          {
            if((this->data()+i)->lb() == -oo)
            {
              if((this->data()+i)->ub() == oo)
                if(!min)
                {
                  selected_index = i;
                  break;
                }

              if((min && (-(this->data()+i)->ub() > pt)) || (!min && (-(this->data()+i)->ub() < pt)))
              {
                selected_index = i;
                pt = -(this->data()+i)->ub();
              }
            }

            else if((this->data()+i)->ub() == oo)
              if((min && ((this->data()+i)->lb() > pt)) || (!min && ((this->data()+i)->lb() < pt)))
              {
                selected_index = i;
                pt = (this->data()+i)->lb();
              }
          }
        }

        return selected_index;
      }

      std::pair<S,S> bisect(size_t i, float ratio = 0.49) const
      {
        assert((this->data()+i)->is_bisectable());
        assert(Interval(0,1).interior_contains(ratio));

        auto p = std::make_pair(*this,*this);
        auto pi = (this->data()+i)->bisect(ratio);
        *(p.first.data()+i) = pi.first;
        *(p.second.data()+i) = pi.second;
        return p;
      }
      
      std::pair<S,S> bisect_largest(float ratio = 0.49) const
      {
        return bisect(largest_diam_index(), ratio);
      }

      #define degenerate_mat(op) \
        V op(this->rows(), this->cols()); \
        \
        if(this->is_empty()) \
          op.init(std::numeric_limits<double>::quiet_NaN()); \
        \
        else \
        { \
          for(size_t i = 0 ; i < this->size() ; i++) \
            *(op.data()+i) = (this->data()+i)->op(); \
        } \
        \
        return op; \

      V lb() const
      {
        degenerate_mat(lb);
      }

      V ub() const
      {
        degenerate_mat(ub);
      }

      V mid() const
      {
        degenerate_mat(mid);
      }

      V rad() const
      {
        degenerate_mat(rad);
      }

      V diam() const
      {
        degenerate_mat(diam);
      }

      double volume() const
      {
        if(this->is_empty())
          return 0.;
        double v = 0.;
        for(size_t i = 0 ; i < this->size() ; i++)
        {
          if((this->data()+i)->is_unbounded()) return oo;
          if((this->data()+i)->is_degenerated()) return 0.;
          v += std::log((this->data()+i)->diam());
        }
        return std::exp(v);
      }

      void set_empty()
      {
        this->init(Interval::empty());
      }

      bool is_empty() const
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->data()+i)->is_empty())
            return true;
        return false;
      }

      bool contains(const V& x) const
      {
        return contains_(x);
      }

      template<typename OtherDerived>
      bool contains(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return contains_(x);
      }

      template<typename OtherDerived>
      bool contains_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty())
          return false;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!(*this)(i,j).contains(x(i,j)))
              return false;
        return true;
      }

      bool interior_contains(const V& x) const
      {
        return interior_contains_(x);
      }

      template<typename OtherDerived>
      bool interior_contains(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return interior_contains_(x);
      }

      template<typename OtherDerived>
      bool interior_contains_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty())
          return false;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!(*this)(i,j).interior_contains(x(i,j)))
              return false;
        return true;
      }

      bool is_unbounded() const
      {
        if(is_empty()) return false;
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->data()+i)->is_unbounded())
            return true;
        return false;
      }

      bool is_degenerated() const
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->data()+i)->is_degenerated())
            return false;
        return true;
      }

      bool is_flat() const
      {
        if(is_empty()) return true;
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->data()+i)->is_degenerated()) // don't use diam() because of roundoff
            return true;
        return false;
      }

      bool intersects(const S& x) const
      {
        return intersects_(x);
      }

      template<typename OtherDerived>
      bool intersects(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return intersects_(x);
      }

      template<typename OtherDerived>
      bool intersects_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty())
          return false;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!(*this)(i,j).intersects(x(i,j)))
              return false;
        return true;
      }

      bool is_disjoint(const S& x) const
      {
        return is_disjoint_(x);
      }

      template<typename OtherDerived>
      bool is_disjoint(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return is_disjoint_(x);
      }

      template<typename OtherDerived>
      bool is_disjoint_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty())
          return true;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if((*this)(i,j).is_disjoint(x(i,j)))
              return true;
        return false;
      }

      bool overlaps(const S& x) const
      {
        return overlaps_(x);
      }

      template<typename OtherDerived>
      bool overlaps(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return overlaps_(x);
      }

      template<typename OtherDerived>
      bool overlaps_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty())
          return false;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!(*this)(i,j).overlaps(x(i,j)))
              return false;
        return true;
      }

      bool is_subset(const S& x) const
      {
        return is_subset_(x);
      }

      template<typename OtherDerived>
      bool is_subset(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return is_subset_(x);
      }

      template<typename OtherDerived>
      bool is_subset_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty()) return true;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!(*this)(i,j).is_subset(x(i,j)))
              return false;
        return true;
      }

      bool is_strict_subset(const S& x) const
      {
        return is_strict_subset_(x);
      }

      template<typename OtherDerived>
      bool is_strict_subset(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return is_strict_subset_(x);
      }

      template<typename OtherDerived>
      bool is_strict_subset_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty()) return true;
        bool one_dim_strict_subset = false;

        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
          {
            if((*this)(i,j).is_strict_subset(x(i,j)))
              one_dim_strict_subset = true;
            if(!(*this)(i,j).is_subset(x(i,j)))
              return false;
          }

        return one_dim_strict_subset;
      }

      bool is_interior_subset(const S& x) const
      {
        return is_interior_subset_(x);
      }

      template<typename OtherDerived>
      bool is_interior_subset(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return is_interior_subset_(x);
      }
      
      template<typename OtherDerived>
      bool is_interior_subset_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty())
          return true;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!(*this)(i,j).is_interior_subset(x(i,j)))
              return false;
        return true;
      }

      bool is_strict_interior_subset(const S& x) const
      {
        return is_strict_interior_subset_(x);
      }

      template<typename OtherDerived>
      bool is_strict_interior_subset(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return is_strict_interior_subset_(x);
      }
      
      template<typename OtherDerived>
      bool is_strict_interior_subset_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty())
          return true;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!(*this)(i,j).is_strict_interior_subset(x(i,j)))
              return false;
        return true;
      }

      bool is_superset(const S& x) const
      {
        return is_superset_(x);
      }

      template<typename OtherDerived>
      bool is_superset(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return is_superset_(x);
      }

      template<typename OtherDerived>
      bool is_superset_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty()) return false;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if(!x(i,j).is_subset((*this)(i,j)))
              return false;
        return true;
      }

      bool is_strict_superset(const S& x) const
      {
        return is_strict_superset_(x);
      }

      template<typename OtherDerived>
      bool is_strict_superset(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return is_strict_superset_(x);
      }

      template<typename OtherDerived>
      bool is_strict_superset_(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(is_empty()) return false;
        bool one_dim_strict_superset = false;

        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
          {
            if(x(i,j).is_strict_subset((*this)(i,j)))
              one_dim_strict_superset = true;
            if(!x(i,j).is_subset((*this)(i,j)))
              return false;
          }

        return one_dim_strict_superset;
      }

      bool is_bisectable() const
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->data()+i).is_bisectable())
            return true;
        return false;
      }
      
      template<typename OtherDerived>
      auto& inflate_(const Eigen::MatrixBase<OtherDerived>& r)
      {
        assert(r.minCoeff() >= 0.);
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            (*this)(i,j).inflate(r(i,j));
        return *this;
      }

      static S empty(size_t nb_rows = R, size_t nb_cols = C)
      {
        return IntervalMatrixTemplate_<S,V,R,C>(nb_rows, nb_cols, Interval::empty());
      }

      template<typename OtherDerived>
      bool operator==(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        if(x.rows() != this->rows() || x.cols() != this->cols())
          return false;
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            if((*this)(i,j) != x(i,j))
              return false;
        return true;
      }

      template<typename OtherDerived>
      bool operator!=(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        return !(*this == x);
      }

      auto operator&=(const S& x)
      {
        if(x.is_empty())
        {
          set_empty();
          return *this;
        }
        
        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            (*this)(i,j) &= x(i,j);
        return *this;
      }

      auto operator|=(const S& x)
      {
        if(x.is_empty())
          return *this;

        for(int i = 0 ; i < this->rows() ; i++)
          for(int j = 0 ; j < this->cols() ; j++)
            (*this)(i,j) |= x(i,j);
        return *this;
      }
      
      template<typename OtherDerived>
      S operator&(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        auto y = *this;
        return y &= x;
      }
      
      template<typename OtherDerived>
      S operator|(const Eigen::MatrixBase<OtherDerived>& x) const
      {
        auto y = *this;
        return y |= x;
      }
  };

}