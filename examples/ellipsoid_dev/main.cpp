#include <codac>

using namespace std;
using namespace codac2;

int main() {
    // ----------------------------------------------------------
    // ellipsoid projections
    // ----------------------------------------------------------

    Vector mu4({1., 0., 0.});
    Matrix G4({{1.,  0.5, 0.},
               {0.5, 2.,  0.2},
               {0.,  0.2, 3.}});
    Ellipsoid e4(mu4, G4);

    Matrix G5 = 0.7 * G4;
    Ellipsoid e5(mu4, G5);

    Matrix G6({{2., 0.,  0.5},
               {0., 1.,  0.2},
               {0., 0.2, 3.}});
    Ellipsoid e6(mu4, G6);

    Figure2D fig2("Projected ellipsoid xy", GraphicOutput::VIBES);
    Figure2D fig3("Projected ellipsoid yz", GraphicOutput::VIBES);
    Figure2D fig4("Projected ellipsoid xz", GraphicOutput::VIBES);

    fig2.set_window_properties({700, 100}, {500, 500});
    fig3.set_window_properties({1200, 100}, {500, 500});
    fig4.set_window_properties({0, 100}, {500, 500});

    fig2.set_axes(axis(0, {-3, 3}), axis(1, {-3, 3}));
    fig3.set_axes(axis(1, {-3, 3}), axis(2, {-3, 3}));
    fig4.set_axes(axis(0, {-3, 3}), axis(2, {-3, 3}));

    fig2.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});
    fig3.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});
    fig4.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});

    fig2.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});
    fig3.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});
    fig4.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});

    fig2.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});
    fig3.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});
    fig4.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});

    // ----------------------------------------------------------
    // inclusion tests
    // ----------------------------------------------------------

    cout << "\nInclusion test e5 in e4: " << e5.is_concentric_subset(e4) << endl;

    cout << "Inclusion test e4 in e5: " << e4.is_concentric_subset(e5) << endl;

    cout << "Inclusion test e4 in e6: " << e6.is_concentric_subset(e4) << endl;

    cout << "Inclusion test e5 in e6: " << e5.is_concentric_subset(e6) << endl;

    // ----------------------------------------------------------
    // test the non inclusion (e4 in e6)
    // ----------------------------------------------------------

    // step 1 identify the direction of non inclusion
    Eigen::MatrixXd X = e4.G;
    Eigen::MatrixXd Y = e6.G;
    Eigen::MatrixXd Y_inv = Y.inverse();
    Eigen::MatrixXd D = Eigen::MatrixXd::Identity(3,3)- X.transpose()*Y_inv.transpose()*Y_inv*X;
    cout << "D = " << D << endl;

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(D);
    Eigen::VectorXd eigenvalues = es.eigenvalues();

    Eigen::VectorXd v;
    double min_eigenvalue = MAXFLOAT;
    for (int i = 0; i < eigenvalues.size(); i++){
            if (eigenvalues(i) < min_eigenvalue){
                min_eigenvalue = eigenvalues(i);
                v = es.eigenvectors().col(i); // the vector associated to the smallest eigenvalue
            }
    }
    cout << " the min eigenvalue of D is " << min_eigenvalue << endl;
    cout << " the vector associated to the min eigenvalue is " << v << endl;

    // select the less included point and draw it on the figures
    Eigen::VectorXd x = e4.mu+e4.G*v;
    cout << "the point " << x << " is the less included point" << endl;
    fig2.draw_point(x, {Color::black()});
    fig3.draw_point(x, {Color::black()});
    fig4.draw_point(x, {Color::black()});

//    IntervalVector x6(x);
//    x6.inflate(0.001);
//    fig2.draw_box(x6, {Color::black(), Color::black()});

    // now let us prove by intervals that x is out of e6
    // by proving (x-mu6).T@G6^(-T)@G6^(-1)@(x-mu6) > 1

    IntervalVector w(x);
    w -= IntervalVector(e6.mu);

    IntervalMatrix Q(Y_inv);
    Q = Q.transpose()*Q;

    IntervalMatrix res = w.transpose()*Q*w;
    cout << "the result of the inclusion test is " << res << endl;

    if(res(0,0).lb() > 1.) {
        cout << "the point is out of the ellipsoid" << endl;
        cout << "the non inclusion is guaranteed" << endl;
    }

    // can we also prove that x is in e4? -> we should take the point not on the boundary or
    // have guaranteed operations previously
    Eigen::MatrixXd X_inv = X.inverse();
    IntervalMatrix Qx(X_inv);
    Qx = Qx.transpose()*Qx;

    IntervalMatrix res2 = w.transpose()*Qx*w;
    cout << "the result of the inclusion test in e4 is " << res2 << endl;

}
