/** 
 *  codac2_Ellipsoid.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     
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
        assert_release(mu_.size() == G_.nb_cols() && mu_.size() == G_.nb_rows());
    }

    size_t Ellipsoid::size() const {
        return mu.size();
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
        return Ellipsoid(
                e1.mu._e + e2.mu._e,
                ((1. + (1. / beta)) * Q1 + (1. + beta) * Q2).sqrt()
        );
    }

    Ellipsoid linear_mapping(const Ellipsoid &e, const Matrix &A, const Vector &b) {
        // compute A*e+b
        Vector mu_res = A._e * e.mu._e + b._e;
        Matrix G_res = A._e * e.G._e;
        Ellipsoid res(mu_res, G_res);
        return res;
    }

    Ellipsoid linear_mapping_guaranteed(const Ellipsoid &e, const Matrix &A, const Vector &b) {
        // TODO à refaire avec la méthode d'andreas
        // compute A*e+b considering rounding error
        Ellipsoid e_ = linear_mapping(e, A, b);

        // compute rounding error as a small box
        IntervalVector mu_(e.mu);
        IntervalMatrix G_(e.G);
        IntervalMatrix A_(A);
        IntervalVector b_(b);
        IntervalVector mu_res_ = A_ * mu_ + b_; // TODO multiplication IntervalMatrix*IntervalVector
        IntervalMatrix G_res_ = A_ * G_;
        IntervalVector unit_box_(mu_.size(), Interval(-1, 1));
        IntervalMatrix error_box_ = mu_res_ - mu_res_ + (A_ * G_ - G_res_) * unit_box_;

        // compute the max radius of error_box
        double rho = error_box_.norm_2().upper(); // TODO norm 2 d'une boite

        Ellipsoid elli_error(mu_res_, Eigen::Identity(mu_.size()) * rho); // = rho*unit_ball
        return e_ + elli_error;
    }

    Matrix nonlinear_mapping_base(const Matrix &G, const Matrix &J, const IntervalMatrix &J_box) {
        Matrix E_out = J * G;
        IntervalMatrix JE = IntervalMatrix(E_out);
        IntervalMatrix JE_inv = JE.inverse(); // TODO inverse of interval matrix
        // TODO this is the rigorous version, but one could compute JE_inv = E_out.inv()

        // compute b_box
        IntervalVector unit_box(G.nb_rows(), Interval(-1, 1));
        IntervalMatrix I_ = IntervalMatrix(Eigen::Identity(e.size()));
        IntervalVector b_box = (JE_inv * J_box * IntervalMatrix(e.G) - I_) * unit_box;

        // get max radius of b_box
        double rho = b_box.norm_2().upper();  // TODO norm 2 d'une boite
        return (1 + rho) * E_out;
    }

    Ellipsoid nonlinear_mapping(const Ellipsoid &e, const AnalyticFunction<typename Wrapper<T>::Domain>& f) {
        // compute the image of the center
        Vector mu_res = f.centered_eval(e.mu);

        // compute the Jacobian of f at the center
        Matrix J = f.diff(e.mu);

        // compute the Jacoiban of f over a box enclosing the ellipsoid
        IntervalVector enclosing_box = enclose_elli_by_box(e); // TODO import this function

        IntervalMatrix J_box = f.diff(enclosing_box);

        Matrix E_out = nonlinear_mapping_base(e.G, J, J_box);
        return Ellipsoid(mu_res, E_out);
    }

    std::ostream &operator<<(std::ostream &os, const Ellipsoid &e) {
        return os << "mu : " << e.mu << "\n" << "G :\n" << e.G;
    }

    bool stability_analysis(const AnalyticFunction<typename Wrapper<T>::Domain> &f, int alpha_max)
    {
        // get the Jacobian of f at the origin
        Vector origin(Eigen::zeros(f.domain().size()));
        Matrix J = f.diff(origin);

        // solve the axis aligned discrete lyapunov equation J.T * P * J − P = −J.T * J
        Matrix P; //TODO
        Matrix G0 = sqrt(P.inv());
        int alpha = 0;
        bool res = false;

        while alpha < alpha_max
        {
            Ellipsoid e(origin,pow(10,-alpha) * G0);
            Ellipsoid e_out = nonlinear_mapping(e,f);

            res = inclusion_test(e,e_out);
            if(res)
            {
                cout << "The system is stable" << endl;
                cout << "Domain of attraction :\n" << e_out << endl;
                return true;
            }
            alpha++;
        }
        cout << "The method is not able to conclude on the stability" << endl;
        return false;
    }

    bool inclusion_test(const Ellipsoid &e1, const Ellipsoid &e2){
        // check if e1 included in e2

        // Identity matrix with the same size as Y
        IntervalMatrix I(Eigen::Idendity(e2.G.nb_rows()));
        IntervalMatrix G2_inv = e2.G.inverse(); // TODO pas garantie
        IntervalVector D = I - e1.G.transpose()*G2_inv.transpose()*G2_inv*e1.G;

        // cholesky decomposition of D
        //TODO

        return res;
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