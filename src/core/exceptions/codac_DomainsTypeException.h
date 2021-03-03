/** 
 *  \file
 *  DomainsTypeException class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_DOMAINSTYPEEXCEPTION_H__
#define __CODAC_DOMAINSTYPEEXCEPTION_H__

#include <vector>
#include <string>
#include "codac_Exception.h"
#include "codac_Domain.h"

namespace codac
{
  /**
   * \class DomainsTypeException
   * \brief Exception raised if the domains connected to a contractor in a CN are
   *        not consistent with the contractor definition.
   */
  class DomainsTypeException : public Exception
  {
    public:

      /**
       * \brief Creates a `DomainsTypeException` from a vector of pointers to `Domain` objects
       *
       * \param ctc_name the contractor class name
       * \param v_ptr_domains pointers to the domains provided by the user
       * \param v_str_expected_doms the allowed definitions for the contractor
       */
      DomainsTypeException(const std::string& ctc_name, 
                           const std::vector<Domain*>& v_ptr_domains,
                           const std::vector<std::string>& v_str_expected_doms);

      /**
       * \brief Creates a `DomainsTypeException` from a vector of `Domain` objects
       *
       * \param ctc_name the contractor class name
       * \param v_domains the domains provided by the user
       * \param v_str_expected_doms the allowed definitions for the contractor
       */
      DomainsTypeException(const std::string& ctc_name, 
                           const std::vector<Domain>& v_domains,
                           const std::vector<std::string>& v_str_expected_doms);

    protected:
      
      /**
       * \brief Generates the Exception message
       *
       * \param ctc_name the contractor class name
       * \param v_domains the domains provided by the user
       * \param v_str_expected_doms the allowed definitions for the contractor
       * \return exception message
       */
      static std::string exception_message(const std::string& ctc_name,
          const std::vector<Domain>& v_domains, const std::vector<std::string>& v_str_expected_doms);

      /**
       * \brief Generates the Exception message
       *
       * \param ctc_name the contractor class name
       * \param v_ptr_domains pointers to the domains provided by the user
       * \param v_str_expected_doms the allowed definitions for the contractor
       * \return exception message
       */
      static std::string exception_message(const std::string& ctc_name,
          const std::vector<Domain*>& v_ptr_domains, const std::vector<std::string>& v_str_expected_doms);
  };
}

#endif