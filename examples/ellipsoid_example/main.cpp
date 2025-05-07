#include <codac>

using namespace std;
using namespace codac2;

int main() {
    // ----------------------------------------------------------
    // linear and nonlinear mappings
    // ----------------------------------------------------------
    cout << "Linear and nonlinear mappings:" << endl;

    Figure2D fig1("Linear and nonlinear mappings", GraphicOutput::VIBES);
    fig1.set_axes(axis(0, {-0.1, 1.3}), axis(1, {-1.2, 0.2}));
    fig1.set_window_properties({0, 100}, {500, 500});

    // initial ellipsoid
    Vector mu({1., 0.});
    Matrix G({{0.05, 0.0},{0.,   0.05}});
    Ellipsoid e1(mu,G);
    fig1.draw_ellipsoid(e1, {Color::red(), Color::red(0.3)});
    cout << "Initial ellipsoid e1 (red):" << endl;
    cout << e1 << endl;

    // discrete nonlinear pendulum system
    VectorVar x(2);
    AnalyticFunction h{
            {x}, vec(x[0] + 0.1 * x[1], -0.2 * sin(x[0]) + 0.9 * x[1])};

    // linear mapping
    int N = 10;
    Ellipsoid e2 = Ellipsoid(e1);
    for (int i = 0; i < N; i++) {
        Matrix A = h.diff(e2.mu).mid();
        Vector b(h.eval(e2.mu).mid() - A * e2.mu);
        e2 = unreliable_linear_mapping(e2, A, b);
        fig1.draw_ellipsoid(e2, {Color::green(), Color::green(0.3)});
    }
    cout << "\nLinear Mapping - Image ellipsoid e2 (green):" << endl;
    cout << e2 << endl;

    // nonlinear mapping
    Ellipsoid e3 = Ellipsoid(e1);
    for (int i = 0; i < N; i++) {
        e3 = nonlinear_mapping(e3, h);
        fig1.draw_ellipsoid(e3, {Color::blue(), Color::blue(0.3)});
    }
    cout << "\nNon Linear Mapping - Image ellipsoid e3 (blue):" << endl;
    cout << e3 << endl;

    // particle cloud (draw the evolution of 200 points in the ellipsoid)
    int Np = 200;
    for (int i = 0; i < Np; i++) {
        Vector x0 = e1.rand();
        fig1.draw_point(x0, {Color::black(), Color::black(0.3)});
        for (int j = 0; j < N; j++) {
            x0 = h.eval(x0).mid();
            fig1.draw_point(x0, {Color::black(), Color::black(0.3)});
        }
    }

    // ----------------------------------------------------------
    // ellipsoid projections
    // ----------------------------------------------------------

    Ellipsoid e4 {
        {1., 0., 0.}, // mu
        {{1.,  0.5, 0.}, // G
         {0.5, 2.,  0.2},
         {0.,  0.2, 3.}}
    };

    Ellipsoid e5 {
        e4.mu, // mu
        0.7 * e4.G // G
    };

    Ellipsoid e6 {
        e4.mu, // mu
        {{2., 0.,  0.5}, // G
         {0., 1.,  0.2},
         {0., 0.2, 3.}}
    };

    Figure2D fig2("Projected ellipsoid xy", GraphicOutput::VIBES);
    Figure2D fig3("Projected ellipsoid yz", GraphicOutput::VIBES);
    Figure2D fig4("Projected ellipsoid xz", GraphicOutput::VIBES);

    fig2.set_window_properties({700, 100}, {500, 500});
    fig3.set_window_properties({1200, 100}, {500, 500});
    fig4.set_window_properties({0, 600}, {500, 500});

    fig2.set_axes(axis(0, {-3.1, 3.1}), axis(1, {-3.1, 3.1}));
    fig2.set_axes(axis(0, {-3.1, 3.1}), axis(1, {-3.1, 3.1}));
    fig3.set_axes(axis(1, {-3.1, 3.1}), axis(2, {-3.1, 3.1}));
    fig4.set_axes(axis(0, {-3.1, 3.1}), axis(2, {-3.1, 3.1}));

    fig2.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});
    fig3.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});
    fig4.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});

    fig2.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});
    fig3.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});
    fig4.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});

    fig2.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});
    fig3.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});
    fig4.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});

    // particle cloud (draw the evolution of 200 points in the ellipsoid e5)
    for (int i = 0; i < Np; i++) {
        Vector x5 = e5.rand();
        fig2.draw_point(x5, {Color::black(), Color::black(0.3)});
        fig3.draw_point(x5, {Color::black(), Color::black(0.3)});
        fig4.draw_point(x5, {Color::black(), Color::black(0.3)});
    }

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
    cout << "\nNon-inclusion test, e4 in e6:" << endl;

    // step 1 identify the direction of non inclusion
    Eigen::MatrixXd G4 = e4.G; // shape matrix
    Eigen::MatrixXd G6 = e6.G; // shape matrix
    Eigen::MatrixXd G6_inv = G6.inverse();
    Eigen::MatrixXd D = Eigen::MatrixXd::Identity(3,3)- G4.transpose()*G6_inv.transpose()*G6_inv*G4;

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(D);
    Eigen::VectorXd eigenvalues = es.eigenvalues();

    // get the eigenvector of D associated with the minimum eigenvalue (negative real part)
    Eigen::VectorXd v;
    double min_eigenvalue = FLT_MAX ;
    for (int i = 0; i < eigenvalues.size(); i++){
        if (eigenvalues(i) < min_eigenvalue){
            min_eigenvalue = eigenvalues(i);
            v = es.eigenvectors().col(i); // the vector associated to the smallest eigenvalue
        }
    }
    cout << "The vector associated with the non-inclusion between e4 and e6 is v=" << v << endl;

    // step 2 - select the less included point and draw it on the figures
    Eigen::VectorXd p = e4.mu+e4.G*v;
    cout << "the red point p=" << p << " is the less included point" << endl;
    fig2.draw_point(p, {Color::red(),Color::red()});
    fig3.draw_point(p, {Color::red(),Color::red()});
    fig4.draw_point(p, {Color::red(),Color::red()});

    // step 3
    // consider  x2 = x + 0.5*min_eigenvalue* (x-mu4)/norm2(x-mu4)
    // x2 is not on borders of e4 and e6
    // let us prove by intervals that x2 is in e4 and out of e6
    // by proving (x2-mu6).T@G6^(-T)@G6^(-1)@(x2-mu6) > 1
    // and (x2-mu4).T@G4^(-T)@G4^(-1)@(x2-mu4) < 1

    Eigen::VectorXd p2 = e4.mu -min_eigenvalue/(1-min_eigenvalue)*(p-e4.mu);
    fig2.draw_point(p2, {Color::green(),Color::green()});
    fig3.draw_point(p2, {Color::green(),Color::green()});
    fig4.draw_point(p2, {Color::green(),Color::green()});

    IntervalVector w(p2);
    w -= IntervalVector(e6.mu);
    IntervalMatrix Q(G6_inv);
    Q = Q.transpose()*Q;
    IntervalMatrix res = w.transpose()*Q*w; //  (x2-mu6).T@G6^(-T)@G6^(-1)@(x2-mu6) > 1

    Eigen::MatrixXd G4_inv = G4.inverse();
    IntervalMatrix Qx(G4_inv);
    Qx = Qx.transpose()*Qx;
    IntervalMatrix res2 = w.transpose()*Qx*w; // (x2-mu4).T@G4^(-T)@G4^(-1)@(x2-mu4) < 1

    if(res(0,0).lb() > 1. and res2(0,0).ub() < 1.){
        cout << "the green point p2="<<p2<<" is guaranteed out of the ellipsoid e6" << endl;
        cout << "it also is guaranteed in the ellipsoid e4" << endl;
        cout << "so the non inclusion between e6 and e4 is guaranteed" << endl;
    }

    // ----------------------------------------------------------
    // guaranteed and non guaranteed linear mappings
    // ----------------------------------------------------------
    cout << "\nGuaranteed mappings:" << endl;
    Matrix A({{0.9, 0.5},
              {-.5, 1.1}});
    Vector b({0.1, 0.2});

    Ellipsoid e7 = unreliable_linear_mapping(e1, A, b);
    Ellipsoid e8 = linear_mapping(e1, A, b);

    cout << "\nLinear Mapping - Image ellipsoid e7:" << endl;
    cout << e7 << endl;

    cout << "\nLinear Mapping Guaranteed - Image ellipsoid e8:" << endl;
    cout << e8 << endl;

    cout << "\nDifference between e7 and e8:" << endl;
    cout << "mu diff norm is " << (e7.mu - e8.mu).norm() << endl;
    cout << "G diff norm is " << (e7.G - e8.G).norm() << endl;

    // ----------------------------------------------------------
    // singular case for nonlinear mapping
    // ----------------------------------------------------------
    cout << "\nSingular case for nonlinear mapping:" << endl;

    Figure2D fig5("singular mappings and degenerated ellipsoids", GraphicOutput::VIBES);
    fig5.set_axes(axis(0, {-0.5, 2}), axis(1, {-1.5, 1.}));
    fig5.set_window_properties({700, 600}, {500, 500});

    Ellipsoid e9 {
        {0., 0.5}, // mu
        {{0.25, 0.}, // G
         {0.,   0.}}
    };
    Ellipsoid e10 {
        {0., -0.5}, // mu
        {{0.25, 0.}, // G
         {0.,   0.25}}
    };

    fig5.draw_ellipsoid(e9, {Color::blue(), Color::red(0.3)});
    fig5.draw_ellipsoid(e10, {Color::red(), Color::red(0.3)});

    AnalyticFunction h2{
            {x}, vec(x[0] + 0.5 * x[1] + 0.75, -0.5 * sin(x[0]) + 0.9 * x[1] + 0.1*0.5)};
    AnalyticFunction h3{
            {x}, vec(x[0] + 0.5 * x[1] + 1.25, x[0] + 0.5 * x[1]-0.25)};

    Ellipsoid e11 = nonlinear_mapping(e9, h2);
    Ellipsoid e12 = nonlinear_mapping(e10, h3);

    fig5.draw_ellipsoid(e11, {Color::green(), Color::green(0.3)});
    fig5.draw_ellipsoid(e12, {Color::green(), Color::green(0.3)});

    cout << "\nDegenerate ellipsoid e9 (blue):\n" << e9 << endl;
    cout << "\nImage of degenerated ellipsoid e11 (green):\n" << e11 << endl;
    cout << "\nNon-degenerate ellipsoid e10 (red):\n" << e10 << endl;
    cout << "\nImage of singular mapping e12 (green):\n" << e12 << endl;

    // particle cloud (draw the evolution of 200 points in the ellipsoid)
    for (int i = 0; i < Np; i++) {
        Vector x0 = e9.rand();
        fig5.draw_point(x0, {Color::black(), Color::black(0.3)});
        fig5.draw_point(h2.eval(x0).mid(), {Color::black(), Color::black(0.3)});

        x0 = e10.rand();
        fig5.draw_point(x0, {Color::black(), Color::black(0.3)});
        fig5.draw_point(h3.eval(x0).mid(), {Color::black(), Color::black(0.3)});
    }

    // ----------------------------------------------------------
    // stability analysis
    // ----------------------------------------------------------
    cout << "\nStability analysis:" << endl;

    // pendulum example
    AnalyticFunction h4{
            {x}, vec(x[0] + 0.5 * x[1] , x[1] + 0.5 * (-x[1]-sin(x[0])))};
    Ellipsoid e13 {
        Vector::zero(2), // mu
        Matrix::zero(2,2) // G
    };
    Ellipsoid e13_out {
        Vector::zero(2), // mu
        Matrix::zero(2,2) // G
    };
    int alpha_max = 1;
    
    if(stability_analysis(h4,alpha_max, e13, e13_out) == BoolInterval::TRUE)
    {
        cout << "\nStability analysis: the system is stable" << endl;
        cout << "Ellipsoidal domain of attraction e13 (red):" << endl;
        cout << e13 << endl;
        cout << "Outter enclosure e13_out of the Image of e13 by h4 (green):" << endl;
        cout << e13_out << endl;
    }
    else
    {
        cout << "\nStability analysis: the method is not able to conclude" << endl;
    }
    Figure2D fig6("Stability analysis - pendulum example", GraphicOutput::VIBES);
    fig6.set_axes(axis(0, {-0.1, 0.1}), axis(1, {-0.1, 0.1}));
    fig6.set_window_properties({1200, 600}, {500, 500});
    fig6.draw_ellipsoid(e13, {Color::red(), Color::red(0.3)});
    fig6.draw_ellipsoid(e13_out, {Color::green(), Color::green(0.3)});
}
