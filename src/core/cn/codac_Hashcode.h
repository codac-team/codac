/** 
 *  \file
 *  Hashcode classes
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_HASHCODE_H__
#define __CODAC_HASHCODE_H__

#include <cstdint>
#include "codac_Contractor.h"
#include "codac_Domain.h"

namespace codac
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