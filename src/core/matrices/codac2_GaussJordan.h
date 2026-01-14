/** 
 *  \file codac2_GaussJordan.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Luc Jaulin, Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>
#include "codac2_Matrix.h"

namespace codac2
{
  /**
   * \brief Gauss Jordan band diagonalization preconditioning.
   * 
   * Takes a ``Matrix`` \f$\mathbf{A}\f$ with ``A.cols >= A.rows`` and returns
   * \f$\mathbf{P}\f$ such that \f$\mathbf{P}\mathbf{A}\f$ is a *band matrix*.
   * The square left part of \f$\mathbf{A}\f$ must be full-rank.
   * This function does not involve interval computations.
   * 
   * \param A matrix with ``A.cols >= A.rows``
   * \return P such that \f$\mathbf{P}\mathbf{A}\f$ is a *band matrix*. Return Id if the
   * left part of \f$\mathbf{A}\f$ is not well-conditioned.
   *
   * From https://www.ensta-bretagne.fr/jaulin/centered.html
   */
  Matrix gauss_jordan(const Matrix& A);
}
