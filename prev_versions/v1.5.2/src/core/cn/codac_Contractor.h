/** 
 *  \file
 *  Contractor class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CONTRACTOR_H__
#define __CODAC_CONTRACTOR_H__

#include <vector>
#include <functional>
#include "codac_Ctc.h"
#include "codac_DynCtc.h"
#include "codac_Domain.h"
#include "codac_Hashcode.h"

namespace ibex
{
  class Ctc;
}

namespace codac
{
  class Domain;
  class DynCtc;
  class ContractorNetwork;

  class Contractor
  {
    public:

      enum class Type { T_COMPONENT, T_EQUALITY, T_IBEX, T_CODAC, T_CN };

      Contractor(Type type, const std::vector<Domain*>& v_domains);
      Contractor(Ctc& ctc, const std::vector<Domain*>& v_domains);
      Contractor(DynCtc& ctc, const std::vector<Domain*>& v_domains);
      Contractor(ContractorNetwork& cn);
      Contractor(const Contractor& ac);
      ~Contractor();

      int id() const;
      Type type() const;

      Ctc& ibex_ctc();
      DynCtc& codac_ctc();
      ContractorNetwork& cn_ctc();

      bool is_active() const;
      void set_active(bool active);

      //std::vector<Domain*> domains();
      const std::vector<Domain*> domains() const;

      bool operator==(const Contractor& x) const;

      void contract();

      const std::string name() const;
      void set_name(const std::string& name);

      friend std::ostream& operator<<(std::ostream& str, const Contractor& x);


    protected:

      const Type m_type;
      bool m_active = true;

      union
      {
        std::reference_wrapper<Ctc> m_static_ctc;
        std::reference_wrapper<DynCtc> m_dyn_ctc;
        std::reference_wrapper<ContractorNetwork> m_cn_ctc;
      };

      std::vector<Domain*> m_v_domains;

      std::string m_name;
      int m_ctc_id;

      static int ctc_counter;
      
      friend class ContractorHashcode;
  };
}

#endif