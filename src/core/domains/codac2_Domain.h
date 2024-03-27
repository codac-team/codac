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

#ifndef __CODAC2_DOMAIN_H__
#define __CODAC2_DOMAIN_H__

#include <cstdio>
#include <vector>
#include <string>

namespace codac2
{
  class Domain
  {
    public:

      virtual ~Domain() = default;
  };

  template<typename T,typename V>
  class DomainInterface : public Domain
  {
    public:

      DomainInterface()
      { }

      ~DomainInterface()
      { }

      virtual V lb() const = 0;
      virtual V ub() const = 0;
      virtual V mid() const = 0;
      virtual V rad() const = 0;
      virtual V diam() const = 0;
      virtual double volume() const = 0;
      virtual void set_empty() = 0;
      virtual bool is_empty() const = 0;
      virtual bool contains(const V& x) const = 0;
      virtual bool interior_contains(const V& x) const = 0;
      virtual bool is_unbounded() const = 0;
      virtual bool is_degenerated() const = 0;
      virtual bool intersects(const T &x) const = 0;
      virtual bool is_disjoint(const T& x) const = 0;
      virtual bool overlaps(const T &x) const = 0;
      virtual bool is_subset(const T& x) const = 0;
      virtual bool is_strict_subset(const T& x) const = 0;
      virtual bool is_interior_subset(const T& x) const = 0;
      virtual bool is_strict_interior_subset(const T& x) const = 0;
      virtual bool is_superset(const T& x) const = 0;
      virtual bool is_strict_superset(const T& x) const = 0;
  };

} // namespace codac

#endif