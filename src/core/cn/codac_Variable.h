/** 
 *  \file
 *  Variable class
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_VARIABLE_H__
#define __CODAC_VARIABLE_H__

#include "codac_Interval.h"
#include "codac_IntervalVector.h"

namespace codac
{
  class Var
  {

  };

  /**
   * \class IntervalVar
   * \brief todo
   */
  class IntervalVar : private Interval, Var
  {
    public:

      /**
       * \brief todo
       */
      IntervalVar()
        : Interval()
      {

      }

    protected:

      friend class Domain;
      friend class Contractor;
      friend class ContractorNetwork;
  };

  /**
   * \class IntervalVectorVar
   * \brief todo
   */
  class IntervalVectorVar : private IntervalVector, Var
  {
    public:

      /**
       * \brief todo
       * *
       * \param n todo
       */
      IntervalVectorVar(int n)
        : IntervalVector(n)
      {

      }

      /**
       * \brief todo
       * 
       * \param index todo
       * 
       * \return todo
       */
      IntervalVar& operator[](int index)
      {
        return (IntervalVar&)IntervalVector::operator[](index);
      }

      /**
       * \brief todo
       * 
       * \return todo
       */
      int size()
      {
        return IntervalVector::size();
      }

    protected:

      friend class Domain;
      friend class Contractor;
      friend class ContractorNetwork;
  };
}

#endif