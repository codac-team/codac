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
#include "tubex_Ctc.h"
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
  class Ctc;

  class Contractor
  {
    public:

      enum class Type { COMPONENT, EQUALITY, IBEX, TUBEX };

      Contractor(Type type, const std::vector<Domain*>& v_domains);
      Contractor(ibex::Ctc& ctc, const std::vector<Domain*>& v_domains);
      Contractor(tubex::Ctc& ctc, const std::vector<Domain*>& v_domains);
      Contractor(const Contractor& ac);
      ~Contractor();

      int id() const;
      Type type() const;

      bool is_active() const;
      void set_active(bool active);

      const std::vector<Domain*>& domains();

      bool operator==(const Contractor& x) const;
      bool comes_from(const Contractor& x) const;

      void contract();

      const std::string name() const;
      void set_name(const std::string& name);

      friend std::ostream& operator<<(std::ostream& str, const Contractor& x);


    protected:

      const Type m_type;
      double m_active = true;

      union
      {
        std::reference_wrapper<ibex::Ctc> m_ibex_ctc;
        std::reference_wrapper<tubex::Ctc> m_tubex_ctc;
      };

      std::vector<Domain*> m_v_domains;

      std::string m_name;
      int m_ctc_id;

      static int ctc_counter;
  };
}

#endif