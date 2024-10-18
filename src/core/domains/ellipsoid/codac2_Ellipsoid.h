/** 
 *  \file codac2_Ellipsoid.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louedec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Matrix.h"
#include "codac2_Vector.h"
#include "codac2_AnalyticFunction.h"

namespace codac2
{
  class Ellipsoid
  {
    public:

      Ellipsoid(size_t n);

      Ellipsoid(const Vector &mu_, const Matrix &G_);

      size_t size() const;

      /**
       * \brief Returns a random vector inside this ellipsoid
       *
       * \note The seed of the pseudo-random number generator is 
       *       voluntarily initialized outside this function, on demand.
       *
       * \return random value
       */
      Vector rand() const;
      
      /**
       * \brief Compute the tightest axis-aligned box containing this ellipsoid
       *
       * \return hull box
       */
      IntervalVector hull_box() const;

    public:

      Vector mu;
      Matrix G;
  };


    Ellipsoid operator+(const Ellipsoid &e1, const Ellipsoid &e2);

    Ellipsoid linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b);

    Ellipsoid linear_mapping_guaranteed(const Ellipsoid &e, const Matrix &A, const Vector &b);


    Ellipsoid nonlinear_mapping_base(const Ellipsoid &e, const Matrix &J, const IntervalMatrix &J_box,
                                     const Vector& trig1,
                                     const Vector& q);

    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorOpValue> &f);
    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorOpValue> &f,
                                const Vector& trig,const Vector& q);



    std::ostream &operator<<(std::ostream &os, const Ellipsoid &e);

    bool stability_analysis(const AnalyticFunction<VectorOpValue> &f, int alpha_max, Ellipsoid &e, Ellipsoid &e_out);

    bool concentric_inclusion_test(const Ellipsoid &e1, const Ellipsoid &e2);


    Matrix solve_discrete_lyapunov(const Matrix& a,const Matrix& q);

}