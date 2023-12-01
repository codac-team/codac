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

#include <vector>

namespace codac2
{  
  class DomainVolume;

  class Domain
  {
    public:
      
      virtual ~Domain() = default;
      virtual DomainVolume dom_volume() const = 0;
  };

  class DomainVolume : public std::vector<double>
  {
    public:

      DomainVolume(size_t n = 0) : std::vector<double>(n)
      {

      }

      bool update(const Domain& x)
      {
        DomainVolume _v = *this;
        *this = x.dom_volume();
        return _v != *this;
      }
  };

} // namespace codac

#endif