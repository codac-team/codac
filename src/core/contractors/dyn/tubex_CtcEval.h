/** 
 *  \file
 *  CtcEval class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCEVAL_H__
#define __TUBEX_CTCEVAL_H__

#include "tubex_DynCtc.h"

namespace tubex
{
  /**
   * \class CtcEval
   * \brief \f$\mathcal{C}_\textrm{eval}\f$ that contracts a tube \f$[y](\cdot)\f$ with
   *        respect to its derivative tube \f$[w](\cdot)\f$ and a measurement \f$[t]\times[z]\f$
   *        according to the constraints \f$z=y(t)\f$ and \f$\dot{y}(\cdot)=w(\cdot)\f$
   * \note  This contractor is denoted \f$\mathcal{C}_\textrm{eval}\f$ in the literature.
   */
  class CtcEval : public DynCtc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_\textrm{eval}\f$
       */
      CtcEval();

      /*
       * \brief Contracts a set of abstract domains
       *
       * This method makes the contractor available in the CN framework.
       *
       * \param v_domains vector of Domain pointers
       */
      void contract(std::vector<Domain*>& v_domains);
      
      /**
       * \brief Enables a forward/backward temporal propagation of the contraction
       *
       * \note If disabled, then the contraction will only affect the slices over \f$[t]\f$.
       *
       * \param enable_propagation if true, the contractions will be propagated as far as possible across \f$[t_0,t_f]\f$
       */
      void enable_time_propag(bool enable_propagation);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big(t,[z],[y](\cdot),[w](\cdot)\big)\f$:
       *        contracts the tube \f$[y](\cdot)\f$ and the evaluation \f$[z]\f$.
       *
       * \note The slicing of \f$[y](\cdot)\f$ and \f$[w](\cdot)\f$ may be changed.
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
       * \note The slicing of \f$[y](\cdot)\f$ and \f$[w](\cdot)\f$ may be changed.
       *
       * \param t the uncertain tdomain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[z]\f$
       * \param y the scalar tube \f$[y](\cdot)\f$
       * \param w the scalar derivative tube \f$[w](\cdot)\f$
       */
      void contract(ibex::Interval& t, ibex::Interval& z, Tube& y, Tube& w);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big(t,[\mathbf{z}],[\mathbf{y}](\cdot),[\mathbf{w}](\cdot)\big)\f$:
       *        contracts the tube \f$[\mathbf{y}](\cdot)\f$ and the evaluation \f$[\mathbf{z}]\f$.
       *
       * \note The slicing of \f$[\mathbf{y}](\cdot)\f$ and \f$[\mathbf{w}](\cdot)\f$ may be changed.
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
       * \note The slicing of \f$[\mathbf{y}](\cdot)\f$ and \f$[\mathbf{w}](\cdot)\f$ may be changed.
       *
       * \param t the uncertain tdomain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[\mathbf{z}]\f$
       * \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
       * \param w the n-dimensional derivative tube \f$[\mathbf{w}](\cdot)\f$
       */
      void contract(ibex::Interval& t, ibex::IntervalVector& z, TubeVector& y, TubeVector& w);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[y](\cdot)\big)\f$:
       *        contracts the evaluation \f$[t]\times[z]\f$ only.
       *
       * \param t the uncertain tdomain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[z]\f$
       * \param y the scalar tube \f$[y](\cdot)\f$
       */
      void contract(ibex::Interval& t, ibex::Interval& z, const Tube& y);

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[\mathbf{z}],[\mathbf{y}](\cdot)\big)\f$:
       *        contracts the evaluation \f$[t]\times[\mathbf{z}]\f$ only.
       *
       * \param t the uncertain tdomain \f$[t]\f$ of the evaluation
       * \param z the bounded evaluation \f$[\mathbf{z}]\f$
       * \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$
       */
      void contract(ibex::Interval& t, ibex::IntervalVector& z, const TubeVector& y);

    protected:

      bool m_propagation_enabled = true; //!< if `true`, a complete temporal propagation will be performed
  };
}

#endif