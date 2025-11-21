/** 
 *  \file codac2_peibos.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  // Forward declarations to reduce compilation load caused by heavy template use:
  class AnalyticTypeBase;
  
  template<typename T>
    requires std::is_base_of_v<AnalyticTypeBase,T>
  class AnalyticFunction;

  class Parallelepiped;
  class OctaSym;

  /**
   * \brief Used in PEIBOS. Compute a parallelepiped enclosing of \f$\mathbf{g}([\mathbf{x}])\f$ where \f$\mathbf{g} = \mathbf{f}\circ \sigma \circ \psi_0\f$.
   * 
   * \param Y The box enclosing \f$\mathbf{f}(\sigma(\psi_0(\bar{\mathbf{x}})))\f$.
   * \param Jf The interval Jacobian matrix containing \f$\frac{d\mathbf{f}}{d\mathbf{y}}([\mathbf{y}])\f$ where \f$\mathbf{y} = \sigma(\psi_0(\left[\mathbf{x}\right]))\f$
   * \param Jf_tild An approximation of \f$\frac{d\mathbf{f}}{d\mathbf{y}}(\sigma(\psi_0(\bar{\mathbf{x}})))\f$
   * \param psi_0 The analytic function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
   * \param sigma The symmetry operator to construct the atlas
   * \param X The box \f$[\mathbf{x}]\f$
   * 
   * \return A Parallelepiped enclosing \f$\mathbf{g}([\mathbf{x}])\f$
   */
  Parallelepiped parallelepiped_inclusion(const IntervalVector& Y, const IntervalMatrix& Jf, const Matrix& Jf_tild, const AnalyticFunction<VectorType>& psi_0, const OctaSym& sigma, const IntervalVector& X);

  /**
   * \brief Compute a set of parallelepipeds enclosing \f$\mathbf{f}(\sigma(\psi_0([-1,1]^m)))\f$ for each symmetry \f$\sigma\f$ in the set of symmetries \f$\Sigma\f$. Note that \f$\left\{\psi_0,\Sigma\right\}\f$ form a gnomonic atlas.
   * 
   * \param f The analytic function \f$\mathbf{f}:\mathbb{R}^n\rightarrow\mathbb{R}^p,p\geq n\f$ 
   * \param psi_0 The transformation function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
   * \param Sigma The set of symmetry operators \f$\sigma\f$ to construct the atlas
   * \param epsilon The maximum diameter of the boxes to split \f$[-1,1]^m\f$ before computing the parallelepiped inclusions
   * \param verbose If true, print the time taken to compute the parallelepiped inclusions with other statistics 
   * 
   * \return A vector of Parallelepipeds enclosing \f$\mathbf{f}(\sigma(\psi_0([-1,1]^m)))\f$ for each symmetry \f$\sigma\f$ in the set of symmetries \f$\Sigma\f$.
   */
  std::vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, bool verbose = false);

  /**
   * \brief Compute a set of parallelepipeds enclosing \f$\mathbf{f}(\sigma(\psi_0([-1,1]^m)) + offset) \f$ for each symmetry \f$\sigma\f$ in the set of symmetries \f$\Sigma\f$. Note that \f$\left\{\psi_0,\Sigma\right\}\f$ form a gnomonic atlas.
   * \param f The analytic function \f$\mathbf{f}:\mathbb{R}^n\rightarrow\mathbb{R}^p,p\geq n\f$ 
   * \param psi_0 The transformation function \f$\psi_0:\mathbb{R}^m\rightarrow\mathbb{R}^n\f$ to construct the atlas
   * \param Sigma The set of symmetry operators \f$\sigma\f$ to construct the atlas
   * \param epsilon The maximum diameter of the boxes to split \f$[-1,1]^m\f$ before computing the parallelepiped inclusions
   * \param offset The offset to add to \f$\sigma(\psi_0([-1,1]^m))\f$ (used to translate the initial manifold)
   * \param verbose If true, print the time taken to compute the parallelepiped inclusions with other statistics 
   * 
   * \return A vector of Parallelepipeds enclosing \f$\mathbf{f}(\sigma(\psi_0([-1,1]^m))+ offset)\f$ for each symmetry \f$\sigma\f$ in the set of symmetries \f$\Sigma\f$.
   */
  std::vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, const Vector& offset, bool verbose = false);  
}