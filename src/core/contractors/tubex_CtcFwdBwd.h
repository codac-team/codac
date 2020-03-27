/** 
 *  \file
 *  CtcFwdBwd class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCFWDBWD_H__
#define __TUBEX_CTCFWDBWD_H__

#include "tubex_CtcStatic.h"
#include "tubex_Function.h"

namespace tubex
{
  /**
   * \class CtcFwdBwd
   * \brief \f$\mathcal{C}_{\textrm{FwdBwd}}\f$ that contracts a tube \f$[\mathbf{x}](\cdot)\f$ with respect
   *        to the constraint \f$f(\mathbf{x})=0\f$
   * \note  This contractor is based on the \f$\mathcal{C}_{\textrm{FwdBwd}}\f$ of IBEX, also known
   *        as \f$\mathcal{C}_{\textrm{HC4Revise}}\f$ in the litterature.
   * \todo  Implement propagation methods?
   * \todo  Allow \f$f(\mathbf{x})\leq 0\f$ ?
   */
  class CtcFwdBwd : public CtcStatic
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_{\textrm{FwdBwd}}\f$ for tubes
       *
       * \param f the function \f$f\f$ defining the constraint \f$f(\mathbf{x})=0\f$
       */
      CtcFwdBwd(const tubex::Function& f);

      void contract(std::vector<Domain*>& v_domains);

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
       * \brief Constractor destructor
       */
      ~CtcFwdBwd();
      
    protected:

      tubex::Function *m_f; //!< the function need to be stored in order to keep the contractor working
  };
}

#endif