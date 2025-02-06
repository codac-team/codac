/** 
 *  codac2_Ellipsoid.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Ellipsoid_utils.h"
#include <unsupported/Eigen/MatrixFunctions>
#include <unsupported/Eigen/KroneckerProduct>

using namespace std;
using codac2::Matrix;
using codac2::Vector;
using codac2::BoolInterval;

namespace codac2
{
    Matrix solve_discrete_lyapunov(const Matrix& A, const Matrix& Q)
    {
        // implementation of the scipy solver for the discrete lyapunov equation (real matrix only)
        // works well under dimension 10
        // https://github.com/scipy/scipy/blob/v1.14.1/scipy/linalg/_solvers.py#L235-L323
        // Solves the discrete Lyapunov equation :math:`AXA^H - X + Q = 0`
        assert(A.is_squared() && Q.is_squared());
        assert(A.size() == Q.size());

        Matrix lhs = Eigen::KroneckerProduct(A,A);
        lhs = Matrix::eye(lhs.rows(),lhs.cols()) - lhs;
        Matrix x = lhs.colPivHouseholderQr().solve((Vector)Q.reshaped());
        return Matrix(x.reshaped(Q.rows(),Q.cols()));
    }

    BoolInterval stability_analysis(const AnalyticFunction<VectorType> &f, unsigned int alpha_max, Ellipsoid &e, Ellipsoid &e_out, bool verbose)
    {
        assert_release(f.args().size() == 1 && "f must have only one arg");

        // get the Jacobian of f at the origin
        Index n = f.input_size();
        Vector origin(Eigen::VectorXd::Zero(n));
        Matrix J = f.diff(IntervalVector(origin)).mid();

        // solve the axis aligned discrete lyapunov equation J.T * P * J − P = −J.T * J
        auto P = solve_discrete_lyapunov(J.transpose(),J.transpose()*J); // TODO solve the Lyapunov equation !!!
        auto G0 = P.inverse().sqrt();
        unsigned int alpha = 0;

        if(verbose)
            cout << "Stability analysis:" << endl;

        while(alpha <= alpha_max)
        {
            e = Ellipsoid(origin, std::pow(10.,-(int)alpha) * G0);
            e_out = nonlinear_mapping(e,f);

            if(verbose)
            {
                cout << "\t with alpha = " << alpha << endl;
                cout << "\t e is\n" << e << endl;
                cout << "\t e_out is\n" << e_out << endl;
            }

            if(e_out.is_concentric_subset(e) == BoolInterval::TRUE)
            {
                if(verbose)
                {
                    cout << "\t The system is stable" << endl;
                    cout << "\t Domain of attraction :\n" << e_out << endl;
                }
                return BoolInterval::TRUE;
            }

            alpha++;
        }

        if(verbose)
            cout << "\t The method is not able to conclude on the stability" << endl;

        return BoolInterval::UNKNOWN;
    }
}