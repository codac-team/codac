/** 
 *  \file
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCFUNCTION_H__
#define __CODAC_CTCFUNCTION_H__

#include <string>
#include "codac_Function.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_Domain.h"
#include "codac_TubeVector.h"

namespace codac
{
  /**
   * \class CtcFunction
   * \brief Generic static \f$\mathcal{C}\f$ that contracts a box \f$[\mathbf{x}]\f$ or a tube \f$[\mathbf{x}](\cdot)\f$
   *        according to the constraint \f$\mathbf{f}(\mathbf{x})=\mathbf{0}\f$ or \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$.
   *        It stands on the CtcFwdBwd of IBEX (HC4Revise).
   */
  class CtcFunction : public ibex::CtcFwdBwd
  {
    public:

      /**
       * \brief Creates a contractor for the constraint \f$\mathbf{f}(\mathbf{x})=\mathbf{0}\f$
       *
       * \param f the function \f$\mathbf{f}\f$
       */
      CtcFunction(const Function& f);

      /**
       * \brief Creates a contractor for the constraint \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$
       *        where \f$[\mathbf{y}]\f$ is a ibex::Domain object
       *
       * \param f the function \f$\mathbf{f}\f$
       * \param y the ibex::Domain object
       */
      CtcFunction(const Function& f, const ibex::Domain& y);

      /**
       * \brief Creates a contractor for the constraint \f$f(\mathbf{x})\in[y]\f$
       *        for the scalar case
       *
       * \param f the function \f$f\f$
       * \param y the interval \f$[y]\f$
       */
      CtcFunction(const Function& f, const Interval& y);

      /**
       * \brief Creates a contractor for the constraint \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$
       *        for the vectorial case
       *
       * \param f the function \f$\mathbf{f}\f$
       * \param y the IntervalVector \f$[\mathbf{y}]\f$
       */
      CtcFunction(const Function& f, const IntervalVector& y);
      
      /**
       * \brief \f$\mathcal{C}\big([\mathbf{x}]\big)\f$
       *
       * \param x the n-dimensional box \f$[\mathbf{x}]\f$ to be contracted
       */
      void contract(IntervalVector& x);
      
      /**
       * \brief \f$\mathcal{C}\big([\mathbf{x}](\cdot)\big)\f$
       *
       * \param x the n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ to be contracted
       */
      void contract(TubeVector& x);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       * \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       * \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
       * \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot),[x_6](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       * \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
       * \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
       * \param x6 the scalar tube \f$[x_6](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot),[x_6](\cdot),[x_7](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       * \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
       * \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
       * \param x6 the scalar tube \f$[x_6](\cdot)\f$ to be contracted
       * \param x7 the scalar tube \f$[x_7](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot),[x_6](\cdot),[x_7](\cdot),[x_8](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       * \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
       * \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
       * \param x6 the scalar tube \f$[x_6](\cdot)\f$ to be contracted
       * \param x7 the scalar tube \f$[x_7](\cdot)\f$ to be contracted
       * \param x8 the scalar tube \f$[x_8](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot),[x_6](\cdot),[x_7](\cdot),[x_8](\cdot),[x_9](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       * \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
       * \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
       * \param x6 the scalar tube \f$[x_6](\cdot)\f$ to be contracted
       * \param x7 the scalar tube \f$[x_7](\cdot)\f$ to be contracted
       * \param x8 the scalar tube \f$[x_8](\cdot)\f$ to be contracted
       * \param x9 the scalar tube \f$[x_9](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8, Tube& x9);

      /**
       * \brief \f$\mathcal{C}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot),[x_4](\cdot),[x_5](\cdot),[x_6](\cdot),[x_7](\cdot),[x_8](\cdot),[x_9](\cdot),[x_10](\cdot)\big)\f$
       *
       * \param x1 the scalar tube \f$[x_1](\cdot)\f$ to be contracted
       * \param x2 the scalar tube \f$[x_2](\cdot)\f$ to be contracted
       * \param x3 the scalar tube \f$[x_3](\cdot)\f$ to be contracted
       * \param x4 the scalar tube \f$[x_4](\cdot)\f$ to be contracted
       * \param x5 the scalar tube \f$[x_5](\cdot)\f$ to be contracted
       * \param x6 the scalar tube \f$[x_6](\cdot)\f$ to be contracted
       * \param x7 the scalar tube \f$[x_7](\cdot)\f$ to be contracted
       * \param x8 the scalar tube \f$[x_8](\cdot)\f$ to be contracted
       * \param x9 the scalar tube \f$[x_9](\cdot)\f$ to be contracted
       * \param x10 the scalar tube \f$[x_10](\cdot)\f$ to be contracted
       */
      void contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8, Tube& x9, Tube& x10);

      /**
       * \brief Contracts an array of slices (representing a slice vector)
       *
       * Propagates the contractions to the next slices
       *
       * \param v_x_slices the slices to be contracted
       */
      void contract(Slice **v_x_slices);
  };
}


#endif