/** 
 *  \file codac2_linear_ctc.h
 *  Contractors for linear systems of equations
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include "codac2_Collection.h"
#include "codac2_template_tools.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  class CtcLinearBase
  {
    public:

      virtual void contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b) const = 0;
      virtual std::shared_ptr<CtcLinearBase> copy() const = 0;
  };

  /**
   * \brief Contractor for a linear system of interval equations,
   * based on the classical Gauss elimination procedure.
   * 
   * No preconditioning is done (see `CtcLinearPrecond` for preconditioning).
   * 
   * The associated constraint is under the form \f$\mathbf{A}\cdot\mathbf{x}=\mathbf{b}\f$,
   * where \f$\mathbf{A}\f$ is a squared matrix in \f$\mathbb{R}^{n\times n}\f$ and
   * \f$\mathbf{x}\f$, \f$\mathbf{b}\f$ vectors in \f$\mathbb{R}^{n}\f$.
   * 
   * Note that because the condition \f$0\in[a_{ii}]\f$ is frequently satisfied for
   * some \f$i\f$, this contractor often fails to contract. It is however efficient,
   * for instance, when the interval matrix \f$[\mathbf{A}]\f$ is close to the identity matrix.
   * See the `CtcLinearPrecond` for this purpose.
   *
   * Note also that this contractor is idempotent: if it is run again,
   * \f$[\mathbf{x}]\f$ is not contracted any more.
   * 
   * Reference:
   *   Applied Interval Analysis
   *   Luc Jaulin, Michel Kieffer, Olivier Didrit, Éric Walter
   *   2001, Springer London
   *   doi: https://doi.org/10.1007/978-1-4471-0249-6
   *   Sec 4.2.2, Page 70
   */
  class CtcGaussElim : public CtcLinearBase
  {
    public:

      /**
       * \brief Creates a contractor based on the Gauss elimination.
       */
      CtcGaussElim()
      { }

      /**
       * \brief Creates the domains according to the linear system:
       * \f$\mathbf{A}\cdot\mathbf{x}=\mathbf{b}\f$
       *
       * \param A the domain \f$[\mathbf{A}]\f$
       * \param x the domain \f$[\mathbf{x}]\f$
       * \param b the domain \f$[\mathbf{b}]\f$
       */
      void contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b) const;

      virtual std::shared_ptr<CtcLinearBase> copy() const
      {
        return std::make_shared<CtcGaussElim>(*this);
      }
  };


  /**
   * \brief Contractor for a linear system of interval equations,
   * using a fixed-point approach based on the Gauss Seidel method.
   * 
   * No preconditioning is done (see `CtcLinearPrecond` for preconditioning).
   * 
   * The associated constraint is under the form \f$\mathbf{A}\cdot\mathbf{x}=\mathbf{b}\f$,
   * where \f$\mathbf{A}\f$ is a squared matrix in \f$\mathbb{R}^{n\times n}\f$ and
   * \f$\mathbf{x}\f$, \f$\mathbf{b}\f$ vectors in \f$\mathbb{R}^{n}\f$, provided that
   * \f$\textrm{diag}(\mathbf{A})\f$ is invertible (i.e., \f$\mathbf{A}\f$ has no zero entry
   * on its diagonal),
   * 
   * Note that it is efficient when the interval matrix \f$[\mathbf{A}]\f$ is close
   * to the identity matrix. See the `CtcLinearPrecond` for this purpose.
   * 
   * Note also that this contractor is not idempotent, better improvements can be obtained
   * with successive calls.
   * 
   * Reference:
   *   Applied Interval Analysis
   *   Luc Jaulin, Michel Kieffer, Olivier Didrit, Éric Walter
   *   2001, Springer London
   *   doi: https://doi.org/10.1007/978-1-4471-0249-6
   *   Sec 4.2.3, Page 73
   */
  class CtcGaussSeidel : public CtcLinearBase
  {
    public:

      /**
       * \brief Creates a contractor based on the Gauss Seidel method.
       */
      CtcGaussSeidel()
      { }

      /**
       * \brief Creates the domains according to the linear system:
       * \f$\mathbf{A}\cdot\mathbf{x}=\mathbf{b}\f$
       *
       * \param A the domain \f$[\mathbf{A}]\f$
       * \param x the domain \f$[\mathbf{x}]\f$
       * \param b the domain \f$[\mathbf{b}]\f$
       */
      void contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b) const;

      virtual std::shared_ptr<CtcLinearBase> copy() const
      {
        return std::make_shared<CtcGaussSeidel>(*this);
      }
  };


  /**
   * \brief Contractor for a linear system of interval equations,
   * using a preconditioning method before calling some provided contractor.
   * 
   * The associated constraint is under the form \f$\mathbf{A}\cdot\mathbf{x}=\mathbf{b}\f$,
   * where \f$\mathbf{A}\f$ is a squared matrix in \f$\mathbb{R}^{n\times n}\f$ and
   * \f$\mathbf{c}\f$, \f$\mathbf{b}\f$ vectors in \f$\mathbb{R}^{n}\f$.
   * 
   * Contractors such as `CtcGaussElim` or `CtcGaussSeidel` are then likely to be efficient
   * if they act on \f$\mathbf{A}\f$ matrices close to the identity.
   * This can be achieved provided that \f$w([\mathbf{A}])\f$ is small enough and that some
   * selected \f$\mathbf{A0}\f$ in \f$[\mathbf{A}]\f$ is well conditioned.
   * 
   * Reference:
   *   Applied Interval Analysis
   *   Luc Jaulin, Michel Kieffer, Olivier Didrit, Éric Walter
   *   2001, Springer London
   *   doi: https://doi.org/10.1007/978-1-4471-0249-6
   *   Sec 4.3.2, Page 84
   */
  class CtcLinearPrecond : public CtcLinearBase
  {
    public:

      /**
       * \brief Creates a contractor performing preconditioning before calling
       * some provided linear contractor.
       *
       * \param ctc_no_precond contractor object performing the contraction of a linear
       * system of equations, in the case of well-conditioned systems (when the interval
       * matrix \f$[\mathbf{A}]\f$ is close to the identity matrix).
       */
      template<typename C>
        requires (std::is_base_of_v<CtcLinearBase,C> || std::is_same_v<std::shared_ptr<CtcLinearBase>,C>)
      CtcLinearPrecond(const C& ctc_no_precond)
        : _ctc_no_precond(ctc_no_precond)
      { }

      /**
       * \brief Creates the domains according to the linear system:
       * \f$\mathbf{A}\cdot\mathbf{x}=\mathbf{b}\f$
       *
       * \param A the domain \f$[\mathbf{A}]\f$
       * \param x the domain \f$[\mathbf{x}]\f$
       * \param b the domain \f$[\mathbf{b}]\f$
       */
      void contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b) const
      {
        assert_release(A.is_squared() && A.rows() == x.size() && A.rows() == b.size());

        IntervalMatrix A0 = A.mid();
        IntervalMatrix A0_inv = A.mid().inverse().template cast<Interval>();
        IntervalMatrix Ap = A0_inv*A;
        IntervalVector bp = A0_inv*b;

        _ctc_no_precond.front().contract(Ap,x,bp);

        b &= A0*bp;
        A &= A0*Ap;
      }

      virtual std::shared_ptr<CtcLinearBase> copy() const
      {
        return std::make_shared<CtcLinearPrecond>(*this);
      }


    protected:

      const Collection<CtcLinearBase> _ctc_no_precond;
  };
}