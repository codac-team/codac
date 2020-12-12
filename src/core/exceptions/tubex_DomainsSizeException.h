/** 
 *  \file
 *  DomainsSizeException class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DOMAINSSIZEEXCEPTION_H__
#define __TUBEX_DOMAINSSIZEEXCEPTION_H__

#include <vector>
#include <string>
#include "tubex_Exception.h"
#include "tubex_Domain.h"

namespace tubex
{
  /**
   * \class DomainsSizeException
   * \brief Exception raised if the size (dimension) of domains are not 
   *        consistent together, and with the contractor definition.
   */
  class DomainsSizeException : public Exception
  {
    public:

      /**
       * \brief Creates a `DomainsSizeException`
       *
       * \param ctc_name the contractor class name
       */
      DomainsSizeException(const std::string& ctc_name);
  };
}

#endif