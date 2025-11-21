/** 
 *  codac2_Ellipsoid.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Ellipsoid.h"
#include <unsupported/Eigen/MatrixFunctions>

using namespace std;
using codac2::Vector;

namespace codac2 {
    Ellipsoid::Ellipsoid(Index n)
            : mu(Vector::zero(n)), G(Matrix::zero(n, n)) {
        assert_release(n > 0);
    }

    Ellipsoid::Ellipsoid(const Vector &mu_, const Matrix &G_)
            : mu(mu_), G(G_) {
        assert_release(mu_.size() == G_.cols() && G_.is_squared());
    }

    Index Ellipsoid::size() const {
        assert(mu.size() == G.cols() && G.is_squared());
        return mu.size();
    }

    Vector Ellipsoid::rand() const
    {
        auto xi = Vector::random(this->size());
        double rand_norm =  ((double) std::rand() / (RAND_MAX));
        return this->mu + this->G * xi / xi.norm() * rand_norm;
    }

    IntervalVector Ellipsoid::hull_box() const
    {
        IntervalVector hull(size());
        for(Index i=0; i< size(); i++){
            double m = G.col(i).norm();
            hull[i] = Interval(-m, m);
        }
        return hull;
    }

    Ellipsoid Ellipsoid::proj_2d(const Vector& d, const Vector& v, const Vector& u) const
    {
        // from [Pope S. B. - Algorithms for Ellipsoids - 2008]
        assert_release( d.size() == v.size());
        assert_release( d.size() == u.size());
        assert_release( d.size() == this->size());
        assert_release( (v.transpose()*u).norm() == 0); // u & v orthogonal

        // Normalized Projection matrix
        // the plane (d,u,v) is also the affine plan {x|x=d+Tt} with T = [u,v]
        Matrix T = Matrix(this->size(),2);
        T.col(0) = v/v.norm();
        T.col(1) = u/u.norm();

        auto TTG = T.transpose() * this->G;
        Eigen::BDCSVD<Matrix,Eigen::ComputeFullU> bdcsvd(TTG);
        Matrix U = bdcsvd.matrixU();
        Matrix E = bdcsvd.singularValues().asDiagonal();

        return {
            T.transpose() * (d + T * T.transpose() * (this->mu - d)),
            U * E
        };
    }

    Ellipsoid operator+(const Ellipsoid &e1, const Ellipsoid &e2) {
        assert_release(e1.size() == e2.size());

        Matrix Q1 = e1.G * e1.G.transpose();
        Matrix Q2 = e2.G * e2.G.transpose();

        double beta = std::sqrt(Q1.trace() / Q2.trace());
        return {
                e1.mu + e2.mu, // mu
                ((1. + (1. / beta)) * Q1 + (1. + beta) * Q2).sqrt() // G
        };
    }

  ostream& operator<<(ostream& os, const Ellipsoid& e)
  {
    os << "Ellipsoid " << e.size() << "d:\n"
      << "  mu=" << e.mu << "\n"
      << "   G=\n" << e.G;
    return os;
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