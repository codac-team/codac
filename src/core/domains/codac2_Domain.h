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
      
      Domain& operator=(const Domain& x)
      {
        _name = x._name;
        return *this;
      }

      virtual ~Domain() = default;

      std::string name() const
      {
        try {
          return _name.empty() ? "?" : _name;
        }
        catch(const std::exception& e) {
          return "!";
        };
      }

      void set_name(const std::string& name) const
      {
        _name = name;
      }

    protected:

      mutable std::string _name;
  };

} // namespace codac

#endif