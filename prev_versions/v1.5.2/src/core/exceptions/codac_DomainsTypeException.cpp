/** 
 *  DomainsTypeException class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_DomainsTypeException.h"

using namespace std;

namespace codac
{
  DomainsTypeException::DomainsTypeException(const string& ctc_name, 
                                             const vector<Domain*>& v_ptr_domains,
                                             const vector<string>& v_str_expected_doms)
    : Exception(ctc_name + "::contract",
                DomainsTypeException::exception_message(ctc_name, v_ptr_domains, v_str_expected_doms))
  {

  }

  DomainsTypeException::DomainsTypeException(const string& ctc_name, 
                                             const vector<Domain>& v_domains,
                                             const vector<string>& v_str_expected_doms)
    : Exception(ctc_name + "::contract",
                DomainsTypeException::exception_message(ctc_name, v_domains, v_str_expected_doms))
  {

  }

  string DomainsTypeException::exception_message(
    const string& ctc_name, const vector<Domain*>& v_ptr_domains, const vector<string>& v_str_expected_doms)
  {
    vector<Domain> v_domains(v_ptr_domains.size());
    for(size_t i = 0 ; i < v_ptr_domains.size() ; i++)
      v_domains[i] = *v_ptr_domains[i];
    return exception_message(ctc_name, v_domains, v_str_expected_doms);
  }

  string DomainsTypeException::exception_message(
    const string& ctc_name, const vector<Domain>& v_domains, const vector<string>& v_str_expected_doms)
  {
    string m_what_msg = "vector of domains not consistent with the " + ctc_name + " definition.\n";
    m_what_msg += "Expected domains:\n";
    for(const auto& str : v_str_expected_doms)
      m_what_msg += "  - (" + str + ")\n";
    m_what_msg += "Provided domains:\n";
    m_what_msg += "  - (";
    for(size_t i = 0 ; i < v_domains.size() ; i++)
      m_what_msg += (i != 0 ? ", " : "") + v_domains[i].dom_type_str();
    m_what_msg += ")\n";
    return m_what_msg;
  }
}