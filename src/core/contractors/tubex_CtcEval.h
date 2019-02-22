/** 
 *  @file
 *  CtcEval class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTCEVAL_H__
#define __TUBEX_CTCEVAL_H__

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \class CtcEval
   * \brief \f$\mathcal{C}_\textrm{eval}\f$ that contracts a tube \f$[y](\cdot)\f$ with
   *        respect to its derivative tube \f$[w](\cdot)\f$ and a measurement \f$[t]\times[z]\f$
   *        according to the constraints \f$z=y(t)\f$ and \f$\dot{y}(\cdot)=w(\cdot)\f$
   * \note  This contractor is denoted \f$\mathcal{C}_\textrm{eval}\f$ in the literature.
   */
  class CtcEval : public Ctc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_\textrm{eval}\f$
       */
      CtcEval();

      /**
       * \brief Enables a forward/backward temporal propagation of the contraction
       *
       * \note If disabled, then the contraction will only affect the slices over \f$[t]\f$.
       *
       * \param enable_propagation if true, the contractions will be propagated as far as possible across \f$[t_0,t_f]\f$
       */
      void enable_temporal_propagation(bool enable_propagation);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big(t,[z],[y](\cdot),[w](\cdot)\big)\f$:
       *        contracts the tube \f$[y](\cdot)\f$ and the evaluation \f$[z]\f$.
       *
       * \param t the date \f$t\f$ of the evaluation
       * \param z the bounded evaluation \f$[z]\f$
       * \param y the scalar tube \f$[y](\cdot)\f$
       * \param w the scalar derivative tube \f$[w](\cdot)\f$
       */
      void contract(double t, ibex::Interval& z, Tube& y, Tube& w);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[y](\cdot),[w](\cdot)\big)\f$:
       *        contracts the tube \f$[y](\cdot)\f$ and the evaluation \f$[t]\times[z]\f$.
       *
       * \param t the uncertain domain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[z]\f$
       * \param y the scalar tube \f$[y](\cdot)\f$
       * \param w the scalar derivative tube \f$[w](\cdot)\f$
       */
      void contract(ibex::Interval& t, ibex::Interval& z, Tube& y, Tube& w);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[y](\cdot),[w](\cdot)\big)\f$:
       *        contracts the tube \f$[y](\cdot)\f$ only.
       *
       * \param t the uncertain domain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[z]\f$
       * \param y the scalar tube \f$[y](\cdot)\f$
       * \param w the scalar derivative tube \f$[w](\cdot)\f$
       */
      void contract(const ibex::Interval& t, const ibex::Interval& z, Tube& y, const Tube& w);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big(t,[\mathbf{z}],[\mathbf{y}](\cdot),[\mathbf{w}](\cdot)\big)\f$:
       *        contracts the tube \f$[\mathbf{y}](\cdot)\f$ and the evaluation \f$[\mathbf{z}]\f$.
       *
       * \param t the date \f$t\f$ of the evaluation
       * \param z the bounded evaluation \f$[\mathbf{z}]\f$
       * \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
       * \param w the n-dimensional derivative tube \f$[\mathbf{w}](\cdot)\f$
       */
      void contract(double t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[\mathbf{z}],[\mathbf{y}](\cdot),[\mathbf{w}](\cdot)\big)\f$:
       *        contracts the tube \f$[\mathbf{y}](\cdot)\f$ and the evaluation \f$[t]\times[\mathbf{z}]\f$.
       *
       * \param t the uncertain domain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[\mathbf{z}]\f$
       * \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
       * \param w the n-dimensional derivative tube \f$[\mathbf{w}](\cdot)\f$
       */
      void contract(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[\mathbf{z}],[\mathbf{y}](\cdot),[\mathbf{w}](\cdot)\big)\f$:
       *        contracts the tube \f$[\mathbf{y}](\cdot)\f$ only.
       *
       * \param t the uncertain domain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[\mathbf{z}]\f$
       * \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
       * \param w the n-dimensional derivative tube \f$[\mathbf{w}](\cdot)\f$
       */
      void contract(const ibex::Interval& t, const ibex::IntervalVector& z, TubeVector& y, const TubeVector& w);

    protected:

      bool m_propagation_enabled = true;
  };
}

#endif