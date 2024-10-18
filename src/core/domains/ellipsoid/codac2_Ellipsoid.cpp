/** 
 *  codac2_Ellipsoid.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louedec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <unsupported/Eigen/MatrixFunctions>
#include "codac2_Ellipsoid.h"
#include "codac2_template_tools.h"

using namespace std;
using namespace codac2;

namespace codac2 {
    Ellipsoid::Ellipsoid(size_t n)
            : mu(Vector(n)), G(Matrix(n, n)) {
        assert_release(n > 0);
    }

    Ellipsoid::Ellipsoid(const Vector &mu_, const Matrix &G_)
            : mu(mu_), G(G_) {
        assert_release(mu_.size() == G_.nb_cols() && G_.is_squared());
    }

    size_t Ellipsoid::size() const {
        return mu.size();
    }

    Vector Ellipsoid::rand() const
    {
        auto xi = Vector::random(this->size());
        double rand_norm =  ((double) std::rand() / (RAND_MAX));
        return this->mu._e + this->G._e * xi._e / xi._e.norm() * rand_norm;
    }

    IntervalVector Ellipsoid::hull_box() const
    {
        IntervalVector hull(size());
        for(size_t i=0; i< size(); i++){
            double m = G._e.col(i).norm();
            hull[i] = Interval(-m, m);
        }
        return hull;
    }

    BoolInterval Ellipsoid::is_concentric_subset(const Ellipsoid& e) const
    {
        size_t n = size();
        assert_release(n == e.size());

        
        if((mu._e - e.mu._e).norm() > 1e-10) // check if the centers are the same
            return BoolInterval::FALSE; // not concentric

        auto I = Matrix::eye(n,n);
        auto G2_inv = e.G._e.inverse();
        IntervalMatrix D(I._e - G._e.transpose() * G2_inv.transpose() * G2_inv * G._e);

        // cholesky decomposition of D = L*L^T
        IntervalMatrix L(n,n); // matrix of the Cholesky decomposition

        for (size_t j = 0; j < n; j++) // for every column
        {
            // diagonal element
            Interval s = 0.;
            for (size_t k = 0; k < j; k++)
                s += L(j, k) * L(j, k);
            Interval u = D(j, j) - s;
            if (u.lb() < 0) {
                return BoolInterval::UNKNOWN; // cannot guarantee that D is positive definite
            }
            L(j,j) = sqrt(u);

            // then the rest of the column
            for (size_t i = j + 1; i<n;
            i++)
            {
                s = 0.;
                for (size_t k = 0; k < j; k++)
                    s += L(j,k) * L(i,k);
                L(i,j) = (D(i,j) - s) / L(j,j);
                L(j,i) = 0.;
            }
        }
        return BoolInterval::TRUE;
    }

    Ellipsoid operator+(const Ellipsoid &e1, const Ellipsoid &e2) {
        assert_release(e1.size() == e2.size());

        //if(e1.is_unbounded() || e2.is_unbounded())
        //  return Ellipsoid(e1.size());

        //if(e1.is_empty() || e2.is_empty())
        //  return Ellipsoid::empty(e1.size());

        auto Q1 = e1.G._e * e1.G._e.transpose();
        auto Q2 = e2.G._e * e2.G._e.transpose();

        double beta = std::sqrt(Q1.trace() / Q2.trace());
        return {
                e1.mu._e + e2.mu._e, // mu
                ((1. + (1. / beta)) * Q1 + (1. + beta) * Q2).sqrt() // G
        };
    }

    Ellipsoid unreliable_linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b) {
        assert_release(A.is_squared());
        assert_release(e.size() == A.nb_cols());
        assert_release(b.size() == A.nb_rows());
        return {
            A._e * e.mu._e + b._e, // mu
            A._e * e.G._e // G
        };
    }

    Ellipsoid linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b) {
        size_t n = e.size();

        assert_release(A.is_squared());
        assert_release(n == A.nb_cols());
        assert_release(n == b.size());

        Ellipsoid e_res = unreliable_linear_mapping(e, A, b);

        auto e_mu_ = e.mu._e.template cast<Interval>();
        auto e_res_mu_ = e_res.mu._e.template cast<Interval>();
        auto e_res_G_ = e_res.G._e.template cast<Interval>();
        auto e_G_ = e.G._e.template cast<Interval>();
        auto A_ = A._e.template cast<Interval>();
        auto b_ = b._e.template cast<Interval>();
        IntervalVector unit_box_(n, {-1,1});

        // compute rounding error as a small box
        auto mu_res_guaranteed = A_ * e_mu_ + b_;
        auto G_res_guaranteed = A_ * e_G_;
        auto error_box_ = mu_res_guaranteed - e_res_mu_ +
                (G_res_guaranteed - e_res_G_) * unit_box_._e;

        double rho = error_box_.norm().ub(); // max radius of error_box
        Ellipsoid elli_error(Vector::zeros(n),
                             Matrix::eye(n,n) * rho); // = rho*unit_ball
        return e_res + elli_error;
    }

    Matrix nonlinear_mapping_base(const Matrix &G, const Matrix &J, const IntervalMatrix &J_box, const Vector& trig, const Vector& q) {

        Matrix JG = J * G;
        IntervalMatrix G_(G);
        IntervalMatrix JG_ = IntervalMatrix(JG);
        IntervalVector unit_box(G.nb_rows(), Interval(-1, 1));

        // normal case
        IntervalMatrix I_ = IntervalMatrix(Eigen::MatrixXd::Identity(G.nb_rows(),G.nb_cols()));
        IntervalMatrix JG_inv_(JG._e.inverse()); // non rigourous inversion
        Matrix M(JG);
        auto W = JG_inv_._e;
        auto Z = I_._e;

        // check for singularities
        if(std::abs(JG._e.determinant()) < trig[0])
        {
            /* degenerated case from
             * Louedec, M., Jaulin, L., & Viel, C. (2024).
             * "Outer enclosures of nonlinear mapping with degenerate ellipsoids."
             * IFAC ACNDC June 2024*/
            assert(trig.size() == 2);
            assert(q.size() == G.nb_rows());

            // SVD decomposition of JG = U*E*V.T
            Eigen::BDCSVD<Eigen::MatrixXd> bdcsvd(JG._e,Eigen::ComputeFullU);
            IntervalMatrix U_(bdcsvd.matrixU()); // which is also the right part
            Vector Sv(bdcsvd.singularValues()); // vectors of singular values

            // select new singular values
            int dim = G.nb_rows();
            IntervalVector s_box(U_._e.transpose()*J_box._e*G_._e*unit_box._e);
            IntervalMatrix S_(Eigen::MatrixXd::Zero(dim,dim)); // diagonal matrix of the new singular value
            IntervalMatrix S_pinv_(Eigen::MatrixXd::Zero(dim,dim)); // pseudo inverse of S
            for(int i=0;i<dim;i++){
                if (Sv[i]>trig[1]){ // normal size singular values
                    S_(i,i) = Interval(Sv[i]);
                    S_pinv_(i,i) = 1/S_(i,i);
                }else{ // for very small singular values (0 included) use s_box
                    double val = s_box[i].ub();
                    S_(i,i) = Interval(q[i]*val);
                    S_pinv_(i,i)=1/S_(i,i);
                    }
                }
            M = (U_*S_).mid();
            W = S_pinv_._e*U_._e.transpose();
            Z = W*JG_._e;
        }

        auto b_box = (W * J_box._e * G_._e - Z) * unit_box._e;
        double rho = b_box.norm().ub(); // max radius of b_box
        return (1 + rho) * M;
    }

    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorOpValue>& f)
    {
        return nonlinear_mapping(e,f,Vector({1e-10,1e-9}),Vector::ones(e.size()));
    }

    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<VectorOpValue>& f,const Vector& trig, const Vector& q) {

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

    std::ostream &operator<<(std::ostream &os, const Ellipsoid &e) {
        return os << "mu : " << e.mu << "\n" << "G :\n" << e.G;
    }

    // Old implementations:

#if 0
    void draw_ellipse(const Ellipsoid& x, const std::string& color)
    {
      if(x.is_unbounded())
        cout << "Unbounded ellipse" << endl;

      else if(x.is_empty())
        cout << "Empty ellipse" << endl;

      else
      {
        Matrix Qmod = x.Q*x.Q.transpose();

        Eigen::EigenSolver<Eigen::MatrixXd> eig(Qmod);
        Vector D_ = eig.eigenvalues().real().cwiseSqrt();
        const Matrix& V = eig.eigenvectors().real();

        vector<double> vx, vy;

        double dr = 0.2;
        for(double t = -M_PI ; t < M_PI+dr ; t+=dr)
        {
          if(t > M_PI) t = M_PI;
          Vector p = x.mu+V*Vector({std::cos(t),std::sin(t)}).cwiseProduct(D_);
          vx.push_back(p[0]); vy.push_back(p[1]);

          /*if(!((Q.determinant() == 0 || // if Q is not invertible, no check
            fabs(1. - (p-mu).transpose()*Q.inverse()*(p-mu)) < 1e-10)))
            cout << Q.determinant() << "  " << (p-mu).transpose()*Q.inverse()*(p-mu) << endl;
          assert((Q.determinant() == 0 || // if Q is not invertible, no check
            fabs(1. - (p-mu).transpose()*Q.inverse()*(p-mu)) < 1e-10)
            && "the ellipse equation is not valid");*/
        }

        //vibes::drawPolygon(vx,vy,color);
        //vibes::drawCircle(x.mu[0], x.mu[1], 0.005, "blue");
      }
    }

    Ellipsoid update(const Ellipsoid& e1, const Vector& mu, const Matrix& Q_m, const Matrix_<1,2>& C)
    {
      if(e1.is_empty())
        return Ellipsoid::empty(e1.size());

      if(e1.is_unbounded())
        return Ellipsoid::all_reals();

      const Vector& mu_kp1 = e1.mu;
      const Vector& mu_mkp1 = mu;
      Matrix Q_kp1 = e1.Q*e1.Q.transpose();

      Matrix Pp_m = C.transpose()*C*Q_m.inverse();

      Matrix_<2,1> L_kp1 = Q_kp1*C.transpose()*((C*Q_kp1*C.transpose()+(C*Q_m*C.transpose())).inverse());
      Vector mu_kp1_tld = mu_kp1 + L_kp1*C*(mu_mkp1-mu_kp1);

      double xi_kp1 = 1. + (Q_kp1.pow(-.5)*(mu_kp1_tld-mu_kp1)).norm();
      double xi_mkp1 = 1. + (Pp_m.pow(.5)*(mu_kp1_tld-mu_mkp1)).norm();

      Matrix Qp_kp1 = 4.*(2.*((std::pow(xi_kp1,2)*Q_kp1).inverse()+std::pow(xi_mkp1,-2)*Pp_m)).inverse();

      return Ellipsoid(mu_kp1_tld, Qp_kp1);
    }

    Ellipsoid Ellipsoid::operator&(const Ellipsoid& e2) const
    {
      assert_release(this->size() == e2.size());

      const Ellipsoid& e1 = *this;

      if(e1.is_unbounded())
        return e2;

      if(e2.is_unbounded())
        return e1;

      if(e1.is_empty() || e2.is_empty())
        return Ellipsoid::empty(e1.size());

      Matrix_<3,3> Q1_mu_inv, Q2_mu_inv;

      Q1_mu_inv.block<2,2>(0,0) = e1.Q.inverse();
      Q1_mu_inv.block<2,1>(0,2) = -e1.Q.inverse()*e1.mu;
      Q1_mu_inv.block<1,2>(2,0) = -e1.mu.transpose()*e1.Q.inverse();
      Q1_mu_inv.block<1,1>(2,2) = e1.mu.transpose()*e1.Q.inverse()*e1.mu;

      Q2_mu_inv.block<2,2>(0,0) = e2.Q.inverse();
      Q2_mu_inv.block<2,1>(0,2) = -e2.Q.inverse()*e2.mu;
      Q2_mu_inv.block<1,2>(2,0) = -e2.mu.transpose()*e2.Q.inverse();
      Q2_mu_inv.block<1,1>(2,2) = e2.mu.transpose()*e2.Q.inverse()*e2.mu;

      Vector mu_kf;

      if(sel == 1)
        mu_kf = e1.mu;
      else if(sel == 2)
        mu_kf = e2.mu;
      else
      {
        Matrix L_KF = e1.Q*(e1.Q+e2.Q).inverse();
        mu_kf = e1.mu + L_KF*(e2.mu-e1.mu);
      }

      Matrix_<3,3> PD_inv = (2.*(Q1_mu_inv+Q2_mu_inv));

      Matrix Q_n  = PD_inv.block<2,2>(0,0).inverse();
      Matrix_<2,1> mu_n = -Q_n*PD_inv.block<2,1>(0,2);

      double P_22_inv = PD_inv(2,2);
      double scal = (1.+mu_n.transpose()*Q_n.inverse()*mu_n-P_22_inv);
      Q_n /= fabs(scal);

      Eigen::ColPivHouseholderQR<Eigen::MatrixXd> dec1(e1.Q.sqrt());
      Vector yy1 = dec1.solve(mu_kf-e1.mu);

      Eigen::ColPivHouseholderQR<Eigen::MatrixXd> dec2(e2.Q.sqrt());
      Vector yy2 = dec2.solve(mu_kf-e2.mu);

      Matrix Qa1 = std::pow(1. + yy1.norm(), 2)*e1.Q;
      Matrix Qa2 = std::pow(1. + yy2.norm(), 2)*e2.Q;

      return Ellipsoid(
        mu_kf,
        4.*(2.*(Qa1.inverse()+Qa2.inverse())).inverse()
      );

      return *this;
    }
#endif
}