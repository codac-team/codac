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
#include <Eigen/Core>
#include <Eigen/Dense>
#include <ibex_LargestFirst.h>
#include <codac2_Matrix.h>

namespace codac2
{
  using Eigen::Dynamic;

  template<int R=Dynamic,int C=Dynamic>
  class IntervalMatrix_ : public Eigen::Matrix<Interval,R,C>
  {
    public:

      IntervalMatrix_()
        : Eigen::Matrix<Interval,R,C>()
      {

      }

      IntervalMatrix_(size_t nb_rows, size_t nb_cols)
        : Eigen::Matrix<Interval,R,C>(nb_rows, nb_cols)
      {
        assert(R == Dynamic || R == nb_rows);
        assert(C == Dynamic || C == nb_cols);
      }

      template<typename OtherDerived>
      IntervalMatrix_(const Eigen::MatrixBase<OtherDerived>& other)
          : Eigen::Matrix<Interval,R,C>(other)
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

      bool is_empty() const
      {
        for(size_t i = 0 ; i < size() ; i++)
          if((this->data()+i)->is_empty())
            return true;
        return false;
      }

      bool intersects(const IntervalMatrix_<R,C>& x) const
      {
        for(size_t i = 0 ; i < size() ; i++)
          if(!(this->data()+i)->intersects(*(x.data()+i)))
            return false;
        return true;
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
        size_t selected_index = -1;
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
        if(unbounded && (!min || selected_index==-1))
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
        assert(Interval(0,1).interior_contains(ratio));
        size_t i = largest_diam_index();
        assert((this->data()+i)->is_bisectable());

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
        Matrix_<R,C> lb;
        for(size_t i = 0 ; i < this->size() ; i++)
          *(lb.data()+i) = (this->data()+i)->lb();
        return lb;
      }

      Matrix_<R,C> ub() const
      {
        assert(!this->is_empty()); // todo: use nan instead of assert?
        Matrix_<R,C> ub;
        for(size_t i = 0 ; i < this->size() ; i++)
          *(ub.data()+i) = (this->data()+i)->ub();
        return ub;
      }

      Matrix_<R,C> mid() const
      {
        assert(!this->is_empty()); // todo: use nan instead of assert?
        Matrix_<R,C> m;
        for(size_t i = 0 ; i < this->size() ; i++)
          *(m.data()+i) = (this->data()+i)->mid();
        return m;
      }

      void set_empty()
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          (this->data()+i)->set_empty();
      }

      static IntervalMatrix_<R,C> empty_set(size_t nb_rows = R, size_t nb_cols = C)
      {
        IntervalMatrix_<R,C> x(nb_rows, nb_cols);
        x.set_empty();
        return x;
      }

      auto& inflate(double r)
      {
        assert(r >= 0.);
        for(size_t i = 0 ; i < this->size() ; i++)
          (this->data()+i)->inflate(r);
        return *this;
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
  };

  class IntervalMatrix : public IntervalMatrix_<>
  {
    public:

      explicit IntervalMatrix(size_t nb_rows, size_t nb_cols)
        : IntervalMatrix_<>(nb_rows, nb_cols)
      { }

      IntervalMatrix(const IntervalMatrix_<>& x)
        : IntervalMatrix_<>(x)
      { }
      
      template<int R,int C>
      explicit IntervalMatrix(const Matrix_<R,C>& v)
        : IntervalMatrix_<>(v)
      {

      }
  };

} // namespace codac

#endif