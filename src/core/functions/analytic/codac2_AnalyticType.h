/** 
 *  \file codac2_AnalyticType.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <functional>
#include "codac2_Interval.h"
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_Wrapper.h"

namespace codac2
{
  struct AnalyticTypeBase
  {
    virtual ~AnalyticTypeBase() = default;
  };

  template<typename T,typename D>
  struct AnalyticType : public AnalyticTypeBase
  {
    using Scalar = T;
    using Domain = D;

    D m;
    std::vector<D> a;
    IntervalMatrix da;
    bool def_domain;

    AnalyticType() = delete;

    AnalyticType(const D& a_, bool def_domain_)
      : a(1,a_), def_domain(def_domain_)
    { }

    AnalyticType(const D& m_, const D& a_, const IntervalMatrix& da_, bool def_domain_)
      : m(m_), a(1,a_), da(da_), def_domain(def_domain_)
    { }

    AnalyticType(const D& m_, const std::vector<D>& a_, const IntervalMatrix& da_, bool def_domain_)
      : m(m_), a(a_), da(da_), def_domain(def_domain_)
    { }

    AnalyticType<T,D>& operator&=(const AnalyticType<T,D>& x)
    {
      for (Index i=0;i<a.size();i++) {
         this->get_a(true,i) &= x.get_a(true,i);
      }
      // restore this? da &= x.da;
      def_domain &= x.def_domain;
      return *this;
    }

    inline const D& get_a() const {
       return a.back();
    }

    inline const D& get_a(bool mixed_diff, size_t i) const {
       if (!mixed_diff || a.size()==1) return a.back();
       assert_release(i<a.size());
       return a[i];
    }
     
    inline D& get_a() {
       return a.back();
    }

    inline D& get_a(bool mixed_diff, size_t i) {
       if (!mixed_diff || a.size()==1) return a.back();
       assert_release(i<a.size());
       return a[i];
    }
     

  };

  using ScalarType = AnalyticType<double,Interval>;
  using VectorType = AnalyticType<Vector,IntervalVector>;
  using MatrixType = AnalyticType<Matrix,IntervalMatrix>;

  template<typename... T>
  bool centered_form_not_available_for_args(const T&... a)
  {
    return ((a.da.size() == 0) || ...);
  }

  /* functional map of vector for operator evaluation of .a field */
  template <typename U, typename V>
  inline std::vector<V> vector_map(const std::vector<U> &t,
                  std::function<V(const U&)>f) {
      std::vector<V> ret(t.size());
      auto r1=t.cbegin();
      auto rt=ret.begin();
      while (rt!=ret.end()) {
         (*rt) = f(*r1);
         rt++; r1++;
      }
      return ret;
  }
  template <typename U1, typename U2, typename V>
  inline std::vector<V> vector_map(const std::vector<U1> &t1, 
				   const std::vector<U2> &t2,
                  std::function<V(const U1&, const U2&)>f) {
      std::vector<V> ret(std::max(t1.size(),t2.size()));
      for (size_t i=0;i<ret.size();i++) {
         ret[i]=f(i<t1.size() ? t1[i] : t1.back(),
		  i<t2.size() ? t2[i] : t2.back());
      }
      return ret;
  }
  template <typename U1, typename U2, typename U3,typename V>
  inline std::vector<V> vector_map(const std::vector<U1> &t1, 
				   const std::vector<U2> &t2,
				   const std::vector<U3> &t3,
                  std::function<V(const U1&, const U2&, const U3&)>f) {
      std::vector<V> ret(std::max({ t1.size(),t2.size(),t3.size() }));
      for (size_t i=0;i<ret.size();i++) {
         ret[i]=f(i<t1.size() ? t1[i] : t1.back(),
		  i<t2.size() ? t2[i] : t2.back(),
		  i<t3.size() ? t3[i] : t3.back());
      }
      return ret;
  }
}
