/** 
 *  \file codac2_IntervalMatrixBase.h
 *  
 *  This class reuses some of the functions developed for ibex::IntervalMatrixBase. 
 *  The original IBEX code is revised in modern C++ and adapted to the template 
 *  structure proposed in Codac, based on the Eigen library.
 *  See ibex::IntervalMatrixBase (IBEX lib, author: Gilles Chabert)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Gilles Chabert
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_assert.h"
#include "codac2_MatrixBase.h"
#include "codac2_Domain.h"

namespace codac2
{
  class IntervalMatrix;

  template<typename S,typename V>
  class IntervalMatrixBase : virtual public MatrixBase<S,Interval>, public DomainInterface<S,V>
  {
    public:

      explicit IntervalMatrixBase(size_t r, size_t c)
        : MatrixBase<S,Interval>(r,c)
      { }

      explicit IntervalMatrixBase(size_t r, size_t c, const Interval& x)
        : MatrixBase<S,Interval>(r,c,x)
      { }
      
      explicit IntervalMatrixBase(size_t r, size_t c, const double bounds[][2])
        : MatrixBase<S,Interval>(r,c)
      {
        size_t k = 0;
        for(size_t i = 0 ; i < this->nb_rows() ; i++)
          for(size_t j = 0 ; j < this->nb_cols() ; j++)
          {
            (*this)(i,j) = Interval(bounds[k][0],bounds[k][1]);
            k++;
          }
        assert(k == this->size());
      }

      IntervalMatrixBase(std::initializer_list<std::initializer_list<Interval>> l)
        : MatrixBase<S,Interval>(l)
      { }

      template<typename OtherDerived>
      IntervalMatrixBase(const Eigen::MatrixBase<OtherDerived>& x)
        : MatrixBase<S,Interval>(x)
      { }

      double volume() const
      {
        if(is_empty())
          return 0.;

        double v = 0.;
        for(size_t i = 0 ; i < this->size() ; i++)
        {
          if((this->_e.data()+i)->is_unbounded()) return oo;
          if((this->_e.data()+i)->is_degenerated()) return 0.;
          v += std::log((this->_e.data()+i)->diam());
        }
        return std::exp(v);
      }

      bool is_empty() const
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->_e.data()+i)->is_empty())
            return true;
        return false;
      }

      void set_empty()
      {
        this->init(Interval::empty());
      }

      #define degenerate_mat(op) \
        V op(this->nb_rows(),this->nb_cols()); \
        \
        if(is_empty()) \
          op.init(std::numeric_limits<double>::quiet_NaN()); \
        \
        else \
        { \
          for(size_t i = 0 ; i < this->size() ; i++) \
            *(op._e.data()+i) = (this->_e.data()+i)->op(); \
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

      V rand() const
      {
        srand(time(NULL));
        degenerate_mat(rand);
      }

      V rad() const
      {
        degenerate_mat(rad);
      }

      V diam() const
      {
        degenerate_mat(diam);
      }

      double min_diam() const
      {
        return (this->_e.data()+extr_diam_index(true))->diam();
      }

      double max_diam() const
      {
        return (this->_e.data()+extr_diam_index(false))->diam();
      }

      size_t min_diam_index() const
      {
        return extr_diam_index(true);
      }

      size_t max_diam_index() const
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
        assert_release(!is_empty() && "Diameter of an empty IntervalVector is undefined");

        size_t i;

        for(i = 0 ; i < this->size() ; i++) 
        {
          if((this->_e.data()+i)->is_unbounded()) 
          {
            unbounded = true;
            if(!min) break;
          }
          else
          {
            double w = (this->_e.data()+i)->diam();
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
            if((this->_e.data()+i)->lb() == -oo)
            {
              if((this->_e.data()+i)->ub() == oo)
                if(!min)
                {
                  selected_index = i;
                  break;
                }

              if((min && (-(this->_e.data()+i)->ub() > pt)) || (!min && (-(this->_e.data()+i)->ub() < pt)))
              {
                selected_index = i;
                pt = -(this->_e.data()+i)->ub();
              }
            }

            else if((this->_e.data()+i)->ub() == oo)
              if((min && ((this->_e.data()+i)->lb() > pt)) || (!min && ((this->_e.data()+i)->lb() < pt)))
              {
                selected_index = i;
                pt = (this->_e.data()+i)->lb();
              }
          }
        }

        return selected_index;
      }

      bool contains(const V& x) const
      {
        assert_release(x.size() == this->size());

        if(is_empty())
          return false;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->contains(*(x._e.data()+i)))
            return false;

        return true;
      }

      bool interior_contains(const V& x) const
      {
        assert_release(x.size() == this->size());

        if(is_empty())
          return false;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->interior_contains(*(x._e.data()+i)))
            return false;

        return true;
      }

      bool is_unbounded() const
      {
        if(is_empty()) return false;
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->_e.data()+i)->is_unbounded())
            return true;
        return false;
      }

      bool is_degenerated() const
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->is_degenerated())
            return false;
        return true;
      }

      bool is_flat() const
      {
        if(is_empty()) return true;
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->_e.data()+i)->is_degenerated()) // don't use diam() because of roundoff
            return true;
        return false;
      }

      bool intersects(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return false;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->intersects(*(x._e.data()+i)))
            return false;

        return true;
      }

      bool is_disjoint(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return true;

        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->_e.data()+i)->is_disjoint(*(x._e.data()+i)))
            return true;

        return false;
      }

      bool overlaps(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return false;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->overlaps(*(x._e.data()+i)))
            return false;

        return true;
      }

      bool is_subset(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return true;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->is_subset(*(x._e.data()+i)))
            return false;

        return true;
      }

      bool is_strict_subset(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return true;

        if(!is_subset(x))
          return false;

        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->_e.data()+i)->is_strict_subset(*(x._e.data()+i)))
            return true;

        return false;
      }

      bool is_interior_subset(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return true;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->is_interior_subset(*(x._e.data()+i)))
            return false;

        return true;
      }

      bool is_strict_interior_subset(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return true;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(this->_e.data()+i)->is_strict_interior_subset(*(x._e.data()+i)))
            return false;

        return true;
      }

      bool is_superset(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return false;

        for(size_t i = 0 ; i < this->size() ; i++)
          if(!(x._e.data()+i)->is_subset(*(this->_e.data()+i)))
            return false;

        return true;
      }

      bool is_strict_superset(const S& x) const
      {
        assert_release(this->size() == x.size());

        if(is_empty())
          return false;

        if(!is_superset(x))
          return false;

        for(size_t i = 0 ; i < this->size() ; i++)
          if((x._e.data()+i)->is_strict_subset(*(this->_e.data()+i)))
            return true;

        return false;
      }

      bool is_bisectable() const
      {
        for(size_t i = 0 ; i < this->size() ; i++)
          if((this->_e.data()+i)->is_bisectable())
            return true;
        return false;
      }

      S& inflate(double r)
      {
        assert_release(r >= 0.);

        for(size_t i = 0 ; i < this->size() ; i++)
          (this->_e.data()+i)->inflate(r);
        return static_cast<S&>(*this);
      }

      S& inflate(const V& r)
      {
        assert_release(this->size() == r.size());
        assert_release(r.min_coeff() >= 0.);

        for(size_t i = 0 ; i < this->size() ; i++)
          (this->_e.data()+i)->inflate(*(r._e.data()+i));
        return static_cast<S&>(*this);
      }

      S& operator&=(const S& x)
      {
        assert_release(this->size() == x.size());
        
        if(x.is_empty())
        {
          set_empty();
          return static_cast<S&>(*this);
        }
        
        for(size_t i = 0 ; i < this->size() ; i++)
          *(this->_e.data()+i) &= *(x._e.data()+i);
        return static_cast<S&>(*this);
      }

      S& operator|=(const S& x)
      {
        assert_release(this->size() == x.size());

        if(x.is_empty())
          return static_cast<S&>(*this);

        for(size_t i = 0 ; i < this->size() ; i++)
          *(this->_e.data()+i) |= *(x._e.data()+i);
        return static_cast<S&>(*this);
      }
      
      S operator&(const S& x) const
      {
        auto y = *this;
        return y &= x;
      }
      
      S operator|(const S& x) const
      {
        auto y = *this;
        return y |= x;
      }

      friend bool operator==(const IntervalMatrixBase<S,V>& x1, const IntervalMatrixBase<S,V>& x2)
      {
        if(x1.is_empty() || x2.is_empty())
          return x1.is_empty() && x2.is_empty() && x1.size() == x2.size();

        return (MatrixBase<S,Interval>)x1 == (MatrixBase<S,Interval>)x2;
      }

      std::pair<S,S> bisect(size_t i, float ratio = 0.49) const
      {
        assert_release(i >= 0 && i < this->size());
        assert_release((this->_e.data()+i)->is_bisectable());
        assert_release(Interval(0,1).interior_contains(ratio));

        auto p = std::make_pair(*this,*this);
        auto pi = (this->_e.data()+i)->bisect(ratio);
        *(p.first._e.data()+i) = pi.first;
        *(p.second._e.data()+i) = pi.second;
        return p;
      }
      
      std::pair<S,S> bisect_largest(float ratio = 0.49) const
      {
        return bisect(max_diam_index(), ratio);
      }
  };
}