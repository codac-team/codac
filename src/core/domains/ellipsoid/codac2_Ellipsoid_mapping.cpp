/** 
 *  codac2_Ellipsoid_mapping.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Ellipsoid.h"
#include "codac2_inversion.h"

using namespace std;
using codac2::Vector;

namespace codac2 {

    Ellipsoid unreliable_linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b) {
        assert_release(A.is_squared());
        assert_release(e.size() == A.cols());
        assert_release(b.size() == A.rows());
        return {
            A * e.mu + b, // mu
            A * e.G // G
        };
    }

    Ellipsoid linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b) {
        Index n = e.size();

        assert_release(A.is_squared());
        assert_release(n == A.cols());
        assert_release(n == b.size());

        Ellipsoid e_res = unreliable_linear_mapping(e, A, b);

        auto e_mu_ = e.mu.template cast<Interval>();
        auto e_res_mu_ = e_res.mu.template cast<Interval>();
        auto e_res_G_ = e_res.G.template cast<Interval>();
        auto e_G_ = e.G.template cast<Interval>();
        auto A_ = A.template cast<Interval>();
        auto b_ = b.template cast<Interval>();
        IntervalVector unit_box_ = IntervalVector::constant(n, {-1,1});

        // compute rounding error as a small box
        IntervalVector mu_res_guaranteed = A_ * e_mu_ + b_;
        IntervalMatrix G_res_guaranteed = A_ * e_G_;
        IntervalVector error_box_ = mu_res_guaranteed - e_res_mu_ +
                (G_res_guaranteed - e_res_G_) * unit_box_;

        double rho = error_box_.norm().ub(); // max radius of error_box
        Ellipsoid elli_error(Vector::zero(n),
                             Matrix::eye(n,n) * rho); // = rho*unit_ball
        return e_res + elli_error;
    }

    Matrix nonlinear_mapping_base(const Matrix &G, const Matrix &J, const IntervalMatrix &J_box, const Vector& trig, const Vector& q) 
    {
        assert(G.is_squared() && J.is_squared() && J_box.is_squared());
        assert(G.cols() == J.cols() && G.cols() == J_box.cols() && G.cols() == q.size());

        auto JG = J * G; // note: reliability may be lost here!
        auto G_ = G.template cast<Interval>();
        IntervalVector unit_box = IntervalVector::constant(G.rows(), {-1,1});

        // normal case
        IntervalMatrix I_ = IntervalMatrix::eye(G.rows(),G.cols());
        IntervalMatrix JG_inv_ = inverse_enclosure(JG); // rigourous inversion
        Matrix M(JG);
        IntervalMatrix W = JG_inv_;
        IntervalMatrix Z = I_;

        // check for singularities
        if(std::abs(JG.determinant()) < trig[0])
        {
            /* degenerated case from
             * Louedec, M., Jaulin, L., & Viel, C. (2024).
             * "Outer enclosures of nonlinear mapping with degenerate ellipsoids."
             * IFAC ACNDC June 2024*/
            assert(trig.size() == 2);
            assert(q.size() == G.rows());

            // SVD decomposition of JG = U*E*V.T
            Eigen::BDCSVD<Matrix,Eigen::ComputeFullU> bdcsvd(JG);
            IntervalMatrix U_ = bdcsvd.matrixU().template cast<Interval>(); // which is also the right part
            Vector Sv = bdcsvd.singularValues(); // vectors of singular values

            // select new singular values
            Index dim = G.rows();
            IntervalVector s_box = U_.transpose()*J_box*G_*unit_box;
            IntervalMatrix S_ = IntervalMatrix::zero(dim,dim); // diagonal matrix of the new singular value
            IntervalMatrix S_pinv_ = IntervalMatrix::zero(dim,dim); // pseudo inverse of S
            for(Index i=0;i<dim;i++){
                if (Sv[i]>trig[1]){ // normal size singular values
                    S_(i,i) = Interval(Sv[i]);
                }else{ // for very small singular values (0 included) use s_box
                    S_(i,i) = Interval(q[i])*s_box[i].ub();
                    }
                    S_pinv_(i,i) = 1./S_(i,i);
                }
            M = (U_*S_).mid();
            W = S_pinv_*U_.transpose();
            Z = W*JG.template cast<Interval>();
        }

        IntervalVector b_box = (W * J_box * G_ - Z) * unit_box;
        double rho = b_box.norm().ub(); // max radius of b_box
        return (1 + rho) * M;
    }

    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorType>& f)
    {
        return nonlinear_mapping(e,f,Vector({1e-10,1e-9}),Vector::ones(e.size()));
    }

    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorType>& f,const Vector& trig, const Vector& q) {

        assert_release(f.args().size() == 1 && "f must have only one arg");
        assert_release(e.size() == f.input_size());
        assert_release(trig.size() == 2);
        assert_release(q.size() == e.size());

        Matrix J = f.diff(e.mu).mid(); // compute the Jacobian of f at the center

        // compute the Jacobian of f over a box enclosing the ellipsoid
        IntervalMatrix J_box = f.diff(e.hull_box());

        return {
            f.eval(e.mu).mid(), // mu: compute the image of the center
            nonlinear_mapping_base(e.G, J, J_box,trig,q) // G
        };
    }
}