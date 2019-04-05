/** 
 *  \file
 *  CtcIbex class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCIBEX_H__
#define __TUBEX_CTCIBEX_H__

#include "tubex_Ctc.h"
#include "ibex_Ctc.h"

namespace tubex
{
  /**
   * \class CtcIbex
   * \brief Generic static \f$\mathcal{C}\f$ that contracts a tube \f$[\mathbf{x}](\cdot)\f$ 
   *        with some IBEX contractor (for boxes).
   *        The contractor will be applied on each slice and gate.
   */
  class CtcIbex : public Ctc
  {
    public:

      /**
       * \brief Creates a static contractor object \f$\mathcal{C}\f$ for tubes
       *
       * \param ibex_ctc the IBEX contractor for boxes
       */
      CtcIbex(ibex::Ctc *ibex_ctc);

      /**
       * \brief Constractor destructor
       */
      ~CtcIbex();

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

    protected:

      /**
       * \brief Contracts an array of slices (representing a slice vector)
       *
       * \param v_x_slices the slices to be contracted
       * \param n the dimension of the vector
       */
      void contract(Slice **v_x_slices, int n);

      ibex::Ctc *m_ibex_ctc; //!< related IBEX contractor
  };
}

#endif