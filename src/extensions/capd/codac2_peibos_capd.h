/** 
 *  \file codac2_peibos_capd.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <tuple>
#include "codac2_peibos.h"
#include "codac2_capd.h"
#include "codac2_OctaSym.h"
#include <capd/poincare/lib.h>

namespace codac2
{
  /**
   * \struct PEIBOS_CAPD_Key
   * \brief Key structure for PEIBOS CAPD results.
   * 
   *  A PEIBOS_CAPD_Key represents \f$\mathbf{y}= \sigma(\psi_0(\mathbf{\text{box}})) + \text{offset}\f$.
   */
  struct PEIBOS_CAPD_Key
  {
    /// @brief The box \f$[\mathbf{x}]\f$
    IntervalVector box;
    /// @brief The transformation function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
    AnalyticFunction<VectorType> psi_0;
    /// @brief The symmetry operator \f$\sigma\f$ to construct the atlas
    OctaSym sigma;
    /// @brief The offset to add to \f$\sigma(\psi_0([x]))\f$
    Vector offset;
  };

  using PEIBOS_CAPD_Result = std::tuple<PEIBOS_CAPD_Key,IntervalVector,IntervalMatrix>;

  /**
   * \brief PEIBOS algorithm using CAPD for guaranteed ODE propagation.
   * 
   * \param i_map The CAPD interval map representing the ODE.
   * \param tf Final time for the propagation.
   * \param dt Time step for the output map.
   * \param psi_0 The transformation function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
   * \param Sigma The set of symmetry operators \f$\sigma\f$ to construct the atlas
   * \param epsilon The maximum diameter of the boxes to split \f$[-1,1]^m\f$ before computing the parallelepiped inclusions (each box is called "box" below)
   * \param verbose If true, print the time taken to compute the parallelepiped inclusions with other statistics 
   * 
   * \return A timed map of PEIBOS CAPD results. At each time \f$t\f$, the value is a vector of tuples. Each tuple contains:
   * \li A PEIBOS_CAPD_Key representing \f$\mathbf{x}(0)= \sigma(\psi_0(\mathbf{\text{box}}))\f$
   * \li The interval vector \f$\mathbf{z}\f$ containing the image \f$\bar{\mathbf{x}}(t))\f$
   * \li The interval Jacobian matrix \f$\mathbf{J_f}\f$ containing \f$D\mathbf{\left[x\right]}(t)\f$
   */
  std::map<double, std::vector<PEIBOS_CAPD_Result>> PEIBOS(const capd::IMap& i_map, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, bool verbose = false);
  
  /**
   * \brief PEIBOS algorithm using CAPD for guaranteed ODE propagation.
   * 
   * \param i_map The CAPD interval map representing the ODE.
   * \param tf Final time for the propagation.
   * \param dt Time step for the output map.
   * \param psi_0 The transformation function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
   * \param Sigma The set of symmetry operators \f$\sigma\f$ to construct the atlas
   * \param epsilon The maximum diameter of the boxes to split \f$[-1,1]^m\f$ before computing the parallelepiped inclusions (each box is called "box" below)
   * \param offset The offset to add to \f$\sigma(\psi_0([-1,1]^m))\f$ (used to translate the initial manifold)
   * \param verbose If true, print the time taken to compute the parallelepiped inclusions with other statistics 
   * 
   * \return A timed map of PEIBOS CAPD results. At each time \f$t\f$, the value is a vector of tuples. Each tuple contains:
   * \li A PEIBOS_CAPD_Key representing \f$\mathbf{x}(0)= \sigma(\psi_0(\mathbf{\text{box}})) + \text{offset}\f$
   * \li The interval vector \f$\mathbf{z}\f$ containing the image \f$\bar{\mathbf{x}}(t))\f$
   * \li The interval Jacobian matrix \f$\mathbf{J_f}\f$ containing \f$D\mathbf{\left[x\right]}(t)\f$
   */
  std::map<double, std::vector<PEIBOS_CAPD_Result>> PEIBOS(const capd::IMap& i_map, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, const Vector& offset, bool verbose = false);


  /**
   * \brief Compute the reach set parallelepipeds from the PEIBOS CAPD output.
   * 
   * \param peibos_output The output of the PEIBOS CAPD algorithm.
   * 
   * \return A timed map of reach set parallelepipeds. At each time \f$t\f$, the value is a vector of Parallelepipeds enclosing the reach set at time \f$t\f$.
   * The function \ref parallelepiped_inclusion is used to compute each Parallelepiped from the PEIBOS CAPD output.
   */
  std::map<double, std::vector<Parallelepiped>> reach_set(const std::map<double, std::vector<PEIBOS_CAPD_Result>>& peibos_output);
}