/** 
 *  codac2_Ellipsoid_checks.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Ellipsoid.h"

using namespace std;
using codac2::Vector;

namespace codac2 {

    BoolInterval Ellipsoid::is_concentric_subset(const Ellipsoid& e) const
    {
        Index n = size();
        assert_release(n == e.size());

        
        if((mu - e.mu).norm() > 1e-10) // check if the centers are the same
            return BoolInterval::FALSE; // not concentric

        auto I = Matrix::eye(n,n);
        auto G2_inv = e.G.inverse();
        IntervalMatrix D(I - G.transpose() * G2_inv.transpose() * G2_inv * G);

        // cholesky decomposition of D = L*L^T
        IntervalMatrix L(n,n); // matrix of the Cholesky decomposition

        for (Index j = 0; j < n; j++) // for every column
        {
            // diagonal element
            Interval s = 0.;
            for (Index k = 0; k < j; k++)
                s += L(j, k) * L(j, k);
            Interval u = D(j, j) - s;
            if (u.lb() < 0) {
                return BoolInterval::UNKNOWN; // cannot guarantee that D is positive definite
            }
            L(j,j) = sqrt(u);

            // then the rest of the column
            for (Index i = j + 1; i<n;
            i++)
            {
                s = 0.;
                for (Index k = 0; k < j; k++)
                    s += L(j,k) * L(i,k);
                L(i,j) = (D(i,j) - s) / L(j,j);
                L(j,i) = 0.;
            }
        }
        return BoolInterval::TRUE;
    }

}