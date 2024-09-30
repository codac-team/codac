/** 
 *  \file codac2_Ellipsoid.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Matrix.h"
#include "codac2_Vector.h"
#include "codac2_AnalyticFunction.h"

namespace codac2 {
    class Ellipsoid {
    public:

        Ellipsoid(size_t n);

        Ellipsoid(const Vector &mu_, const Matrix &G_);

        size_t size() const;

    public:

        Vector mu;
        Matrix G;
    };


    Ellipsoid operator+(const Ellipsoid &e1, const Ellipsoid &e2);

    Ellipsoid linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b);

    Ellipsoid linear_mapping_guaranteed(const Ellipsoid &e, const Matrix &A, const Vector &b);

    Ellipsoid nonlinear_mapping_base(const Ellipsoid &e, const Matrix &J, const IntervalMatrix &J_box);

//    template<typename T>
//    requires std::is_base_of_v<OpValueBase, T>
    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorOpValue> &f);

    std::ostream &operator<<(std::ostream &os, const Ellipsoid &e);

//    template<typename T>
//    requires std::is_base_of_v<OpValueBase, T>
//    bool stability_analysis(const AnalyticFunction<VectorOpValue> &f, int alpha_max);

//    bool inclusion_test(const Ellipsoid &e1, const Ellipsoid &e2);

    IntervalVector enclose_elli_by_box(const Ellipsoid& e);
}