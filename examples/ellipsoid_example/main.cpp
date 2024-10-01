#include <codac>

using namespace std;
using namespace codac2;

int main() {
    // ----------------------------------------------------------
    // linear and nonlinear mappings
    // ----------------------------------------------------------

    Figure2D fig1("Linear and nonlinear mappings", GraphicOutput::VIBES);
    fig1.set_axes(FigureAxis(0, {0, 1.5}), FigureAxis(1, {-1., 0.5}));
    fig1.set_window_properties({0, 100}, {500, 500});

    // initial ellipsoid
    Vector mu({1., 0.});
    Matrix G({{0.05, 0.0},
              {0.,   0.05}});
    Ellipsoid e1(mu, G);
    fig1.draw_ellipsoid(e1, {Color::red(), Color::red(0.3)});
    cout << "Initial ellipsoid e1:" << endl;
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
        Vector b(h.eval(e2.mu).mid()._e - A._e * e2.mu._e);
        e2 = linear_mapping(e2, A, b);
        fig1.draw_ellipsoid(e2, {Color::green(), Color::green(0.3)});
    }
    cout << "\nLinear Mapping - Image ellipsoid e2:" << endl;
    cout << e2 << endl;

    // nonlinear mapping
    Ellipsoid e3 = Ellipsoid(e1);
    for (int i = 0; i < N; i++) {
        e3 = nonlinear_mapping(e3, h);
        fig1.draw_ellipsoid(e3, {Color::blue(), Color::blue(0.3)});
    }
    cout << "\nNon Linear Mapping - Image ellipsoid e3:" << endl;
    cout << e3 << endl;

    // particle cloud (draw the evolution of 200 points in the ellipsoid)
    int Np = 200;
    for (int i = 0; i < Np; i++) {
        Vector x0 = e1.sample();
        fig1.draw_box(IntervalVector(x0).inflate(0.0001), {Color::black(), Color::black(0.3)});
        for (int j = 0; j < N; j++) {
            x0 = h.eval(x0).mid();
            fig1.draw_box(IntervalVector(x0).inflate(0.0001), {Color::black(), Color::black(0.3)});
        }
    }

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
    fig4.set_window_properties({0, 600}, {500, 500});

    fig2.set_axes(FigureAxis(0, {-3, 3}), FigureAxis(1, {-3, 3}));
    fig3.set_axes(FigureAxis(1, {-3, 3}), FigureAxis(2, {-3, 3}));
    fig4.set_axes(FigureAxis(0, {-3, 3}), FigureAxis(2, {-3, 3}));

    fig2.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});
    fig3.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});
    fig4.draw_ellipsoid(e4, {Color::blue(), Color::blue(0.3)});

    fig2.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});
    fig3.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});
    fig4.draw_ellipsoid(e5, {Color::red(), Color::red(0.3)});

    fig2.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});
    fig3.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});
    fig4.draw_ellipsoid(e6, {Color::green(), Color::green(0.3)});

    // particle cloud (draw the evolution of 200 points in the ellipsoid)
    for (int i = 0; i < Np; i++) {
        IntervalVector x5(e5.sample());
        x5.inflate(0.001);
        fig2.draw_box(x5, {Color::black(), Color::black(0.3)});
        fig3.draw_box(x5, {Color::black(), Color::black(0.3)});
        fig4.draw_box(x5, {Color::black(), Color::black(0.3)});
    }

    // ----------------------------------------------------------
    // inclusion tests
    // ----------------------------------------------------------

    bool res1 = concentric_inclusion_test(e5, e4);
    cout << "\nInclusion test e5 in e4: " << res1 << endl;

    bool res2 = concentric_inclusion_test(e4, e5);
    cout << "Inclusion test e4 in e5: " << res2 << endl;

    bool res3 = concentric_inclusion_test(e6, e4);
    cout << "Inclusion test e4 in e6: " << res3 << endl;

    bool res4 = concentric_inclusion_test(e5, e6);
    cout << "Inclusion test e5 in e6: " << res4 << endl;

    // ----------------------------------------------------------
    // guaranteed and non guaranteed linear mappings
    // ----------------------------------------------------------
    Matrix A({{0.9, 0.5},
              {-.5, 1.1}});
    Vector b({0.1, 0.2});

    Ellipsoid e7 = linear_mapping(e1, A, b);
    Ellipsoid e8 = linear_mapping_guaranteed(e1, A, b);

    cout << "\nLinear Mapping - Image ellipsoid e7:" << endl;
    cout << e7 << endl;

    cout << "\nLinear Mapping Guaranteed - Image ellipsoid e8:" << endl;
    cout << e8 << endl;

    cout << "\nDifference between e7 and e8:" << endl;
    cout << "mu diff norm is " << (e7.mu._e - e8.mu._e).norm() << endl;
    cout << "G diff norm is " << (e7.G._e - e8.G._e).norm() << endl;

    // ----------------------------------------------------------
    // singular case for nonlinear mapping
    // ----------------------------------------------------------

    Figure2D fig5("singular mappings and degenerated ellipsoids", GraphicOutput::VIBES);
    fig5.set_axes(FigureAxis(0, {-5, 5}), FigureAxis(1, {-5., 5}));
    fig5.set_window_properties({700, 600}, {500, 500});

    Ellipsoid e9(Vector({0., 0.5}), Matrix({{0.25, 0.},
                                            {0.,   0.}}));
    Ellipsoid e10(Vector({0., -0.5}), Matrix({{0.25, 0.},
                                              {0.,   0.25}}));

    fig5.draw_ellipsoid(e9, {Color::blue(), Color::red(0.3)});
    fig5.draw_ellipsoid(e10, {Color::red(), Color::red(0.3)});

    AnalyticFunction h2{
            {x}, vec(x[0] + 0.5 * x[1] + 1., -0.5 * sin(x[0]) + 0.9 * x[1])};
    AnalyticFunction h3{
            {x}, vec(x[0] + 0.5 * x[1] + 1., x[0] + 0.5 * x[1])};

    Vector q(0);
    cout << "q size is " << q.size() << endl;
    Ellipsoid e11 = nonlinear_mapping(e9, h2);
    Ellipsoid e12 = nonlinear_mapping(e10, h3);

    fig5.draw_ellipsoid(e11, {Color::green(), Color::green(0.3)});
    fig5.draw_ellipsoid(e12, {Color::green(), Color::green(0.3)});

    // particle cloud (draw the evolution of 200 points in the ellipsoid)
    for (int i = 0; i < Np; i++) {
        Vector x0 = e9.sample();
        fig5.draw_box(IntervalVector(x0).inflate(0.0001), {Color::black(), Color::black(0.3)});
        x0 = h2.eval(x0).mid();
        fig5.draw_box(IntervalVector(x0).inflate(0.0001), {Color::black(), Color::black(0.3)});

        x0 = e10.sample();
        fig5.draw_box(IntervalVector(x0).inflate(0.0001), {Color::black(), Color::black(0.3)});
        x0 = h3.eval(x0).mid();
        fig5.draw_box(IntervalVector(x0).inflate(0.0001), {Color::black(), Color::black(0.3)});
    }
}
