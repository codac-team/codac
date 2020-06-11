/** 
 *  \file
 *  Contractor class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CONTRACTOR_H__
#define __TUBEX_CONTRACTOR_H__

#include <vector>
#include <functional>
#include "ibex_Ctc.h"
#include "tubex_DynCtc.h"
#include "tubex_Domain.h"
#include "tubex_ContractorNetwork.h"

namespace ibex
{
  class Ctc;
}

namespace tubex
{
  class Domain;
  class ContractorNetwork;
  class DynCtc;

  class Contractor
  {
    public:

      enum class Type { T_COMPONENT, T_EQUALITY, T_IBEX, T_TUBEX };

      Contractor(Type type, const std::vector<Domain*>& v_domains);
      Contractor(ibex::Ctc& ctc, const std::vector<Domain*>& v_domains);
      Contractor(DynCtc& ctc, const std::vector<Domain*>& v_domains);
      Contractor(const Contractor& ac);
      ~Contractor();

      int id() const;
      Type type() const;

      ibex::Ctc& ibex_ctc();
      DynCtc& tubex_ctc();

      bool is_active() const;
      void set_active(bool active);

      std::vector<Domain*>& domains();
      const std::vector<Domain*>& domains() const;

      bool operator==(const Contractor& x) const;

      void contract();

      const std::string name() const;
      void set_name(const std::string& name);

      friend std::ostream& operator<<(std::ostream& str, const Contractor& x);


    protected:

      const Type m_type;
      double m_active = true;

      union
      {
        std::reference_wrapper<ibex::Ctc> m_static_ctc;
        std::reference_wrapper<DynCtc> m_dyn_ctc;
      };

      std::vector<Domain*> m_v_domains;

      std::string m_name;
      int m_ctc_id;

      static int ctc_counter;
  };
}

#endif