/** 
 *  \file
 *  CtcDeriv class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCDERIV_H__
#define __TUBEX_CTCDERIV_H__

#include "tubex_Ctc.h"
#include "tubex_Slice.h"

namespace tubex
{
  /**
   * \class CtcDeriv
   * \brief \f$\mathcal{C}_{\frac{d}{dt}}\f$ that contracts a tube \f$[x](\cdot)\f$ with respect
   *        to its derivative tube \f$[v](\cdot)\f$ according to the constraint \f$\dot{x}(\cdot)=v(\cdot)\f$
   * \note  This contractor is called \f$\mathcal{C}_{\frac{d}{dt}}\f$ in the literature.
   * \todo  Contract the tube \f$[v](\cdot)\f$ if \f$[x](\cdot)\f$ degenerated?
   */
  class CtcDeriv : public Ctc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_{\frac{d}{dt}}\f$
       */
      CtcDeriv();

      /**
       * \brief \f$\mathcal{C}_{\frac{d}{dt}}\big([x](\cdot),[v](\cdot)\big)\f$:
       *        contracts the tube \f$[x](\cdot)\f$ with respect to its derivative \f$[v](\cdot)\f$.
       *
       * \pre \f$[x](\cdot)\f$ and \f$[v](\cdot)\f$ must share the same slicing and domain.
       *
       * \param x the scalar tube \f$[x](\cdot)\f$
       * \param v the scalar derivative tube \f$[v](\cdot)\f$
       * \param t_propa an optional temporal way of propagation
       *                (forward or backward in time, both ways by default)
       */
      void contract(Tube& x, const Tube& v, TPropagation t_propa = FORWARD | BACKWARD);

      /**
       * \brief \f$\mathcal{C}_{\frac{d}{dt}}\big([\mathbf{x}](\cdot),[\mathbf{v}](\cdot)\big)\f$:
       *        contracts the tube \f$[\mathbf{x}](\cdot)\f$ with respect to its derivative \f$[\mathbf{v}](\cdot)\f$.
       *
       * \pre \f$[\mathbf{x}](\cdot)\f$ and \f$[\mathbf{v}](\cdot)\f$ must share the same dimension, slicing and domain.
       *
       * \param x the n-dimensional tube \f$[\mathbf{x}](\cdot)\f$
       * \param v the n-dimensional derivative tube \f$[\mathbf{v}](\cdot)\f$
       * \param t_propa an optional temporal way of propagation
       *                (forward or backward in time, both ways by default)
       */
      void contract(TubeVector& x, const TubeVector& v, TPropagation t_propa = FORWARD | BACKWARD);

      /**
       * \brief \f$\mathcal{C}_{\frac{d}{dt}}\big(\llbracket x\rrbracket(\cdot),\llbracket v\rrbracket(\cdot)\big)\f$:
       *        contracts the slice \f$\llbracket x\rrbracket(\cdot)\f$ with respect to its derivative \f$\llbracket v\rrbracket(\cdot)\f$.
       *
       * \pre \f$\llbracket x\rrbracket(\cdot)\f$ and \f$\llbracket v\rrbracket(\cdot)\f$ must share the same domain.
       *
       * \param x the slice \f$\llbracket x\rrbracket(\cdot)\f$
       * \param v the derivative slice \f$\llbracket v\rrbracket(\cdot)\f$
       * \param t_propa an optional temporal way of propagation
       *                (forward or backward in time, both ways by default)
       */
      void contract(Slice& x, const Slice& v, TPropagation t_propa = FORWARD | BACKWARD);

    protected:

      /**
       * \brief Contracts input and ouput gates of a slice regarding its derivative set
       *
       * \pre \f$\llbracket x\rrbracket(\cdot)\f$ and \f$\llbracket v\rrbracket(\cdot)\f$ must share the same domain.
       *
       * \note Only the gates may be affected.
       *       This method is used for the \f$\mathcal{C}_\textrm{eval}\f$ contractor.
       *
       * \param x the slice \f$\llbracket x\rrbracket(\cdot)\f$
       * \param v the derivative slice \f$\llbracket v\rrbracket(\cdot)\f$
       */
      void contract_gates(Slice& x, const Slice& v);
      
      friend class CtcEval; // contract_gates used by CtcEval
  };
}

#endif