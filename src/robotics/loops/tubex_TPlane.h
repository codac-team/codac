/** 
 *  \file
 *  TPlane class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TPLANE_H__
#define __TUBEX_TPLANE_H__

#include "tubex_Paving.h"
#include "tubex_TubeVector.h"
#include "tubex_ConnectedSubset.h"

namespace tubex
{
  /**
   * \class TPlane
   * \brief Temporal representation of loops
   * 
   * A t-plane is a 2d temporal space \f$[t_0,t_f]^2\f$ that may contain t-pairs corresponding to looped
   * trajectories. Considering 2d trajectories \f$\mathbf{p}(\cdot):[t_0,t_f]\to\mathbb{R}^2\f$, we temporally
   * define the set of loops as:
   * \f$\mathbb{T}^*=\left\{(t_1,t_2)\in [t_0,t_f]^2 \mid \mathbf{p}(t_1)=\mathbf{p}(t_2) , t_1 < t_2 \right\}\f$
   *
   * This class allows to detect and prove loops considering uncertainties on \f$\mathbf{p}(\cdot)\f$ 
   * or in cases where only the velocities \f$\mathbf{v}(\cdot)\f$ of the robot are at hand.
   *
   * This has been the object of the publications:
   *
   *     Loop detection of mobile robots using interval analysis
   *     C. Aubry, R. Desmare, L. Jaulin
   *     Automatica, 2013
   *
   *     Proving the existence of loops in robot trajectories
   *     S. Rohou, P. Franek, C. Aubry, L. Jaulin
   *     The International Journal of Robotics Research, 2018
   */
  class TPlane : public Paving
  {
    public:

      /**
       * \brief Creates a 2d temporal space \f$[t_0,t_f]^2\f$
       */
      TPlane();

      /**
       * \brief Computes this tplane as a subpaving, from the tube of positions \f$[\mathbf{p}](\cdot)\f$
       *        and the tube of velocities \f$[\mathbf{v}](\cdot)\f$.
       *
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       * \param p 2d TubeVector \f$[\mathbf{p}](\cdot)\f$ for positions
       * \param v 2d TubeVector \f$[\mathbf{v}](\cdot)\f$ for velocities
       */
      void compute_detections(float precision, const TubeVector& p, const TubeVector& v);

      /**
       * \brief Tries to prove the existence of loops in each detection set
       *
       * \note The tplane must have been computed beforehand.
       *
       * \param f inclusion of the uncertain function \f$\mathbf{f}^*\f$ that vanishes in case of a loop
       */
      void compute_proofs(ibex::IntervalVector (*f)(const ibex::IntervalVector& b));

      /**
       * \brief Returns the number of loop detections
       *
       * \note The tplane must have been computed beforehand.
       *
       * \return the number of connected subsets corresponding to feasible loops
       */
      int nb_loops_detections() const;

      /**
       * \brief Returns the number of proven loops
       *
       * \note The tplane and the proofs must have been computed beforehand.
       *
       * \return the number of connected subsets for which at least one loop has been verified
       */
      int nb_loops_proofs() const;

      /**
       * \brief Returns the set of detected loops
       *
       * \note The tplane must have been computed beforehand.
       *
       * \return the set of connected subsets corresponding to loop detections
       */
      const std::vector<ConnectedSubset>& get_detected_loops() const;

      /**
       * \brief Returns the set of proven loops
       *
       * \note The tplane and the proofs must have been computed beforehand.
       *
       * \return the set of connected subsets corresponding to loops proofs
       */
      const std::vector<ConnectedSubset>& get_proven_loops() const;

      /**
       * \brief Returns a temporal function of value 0 in case of no-detection, 1 in case of
       *        loop detection and 2 in case of loop proof
       *
       * This method is useful when used together with colormaps for display purposes.
       *
       * \return the corresponding temporal function (trajectory)
       */
      Trajectory traj_loops_summary() const;

    protected:

      /**
       * \brief Recursive computation of the tplane, from the tube of positions \f$[\mathbf{p}](\cdot)\f$
       *        and the tube of velocities \f$[\mathbf{v}](\cdot)\f$.
       *
       * \note This method is used as recursive algorithm.
       *
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       * \param p 2d TubeVector \f$[\mathbf{p}](\cdot)\f$ for positions
       * \param v 2d TubeVector \f$[\mathbf{v}](\cdot)\f$ for velocities
       * \param extract_subsets if `true`, a set of ConnectedSubset objects will be computed from this Paving
       */
      void compute_detections(float precision, const TubeVector& p, const TubeVector& v, bool extract_subsets);

      float m_precision = 0.; //!< precision of the SIVIA algorithm, used later on in traj_loops_summary()
      std::vector<ConnectedSubset> m_v_detected_loops; //!< set of loops detections
      std::vector<ConnectedSubset> m_v_proven_loops; //!< set of loops proofs
  };
}

#endif