/** 
 *  \file
 *  Hashcode classes
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_HASHCODE_H__
#define __TUBEX_HASHCODE_H__

#include <cstdint>
#include "tubex_Contractor.h"
#include "tubex_Domain.h"

namespace tubex
{
  class Domain;
  class Contractor;
  
  class ContractorHashcode
  {
    public:

      ContractorHashcode(const Contractor& ctc);
      bool operator<(const ContractorHashcode& a) const;

    protected:

      size_t m_n;
      std::uintptr_t *m_ptr;
  };

  class DomainHashcode
  {
    public:

      DomainHashcode(const Domain& dom);
      bool operator<(const DomainHashcode& a) const;

      static std::uintptr_t uintptr(const Domain& dom);

    protected:

      std::uintptr_t m_ptr;
  };
}

#endif