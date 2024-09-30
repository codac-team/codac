#include <codac>

using namespace std;
using namespace codac2;

int main()
{
    Figure2D fig1("Ellipsoid example", GraphicOutput::VIBES);
    fig1.set_axes(FigureAxis(0,{0,1.5}), FigureAxis(1,{-1.,0.5}));
    fig1.set_window_properties({250,250},{500,500});

    // initial ellipsoid
    Vector mu({1.,0.});
    Matrix G({{0.05,0.0},{0.,0.05}});
    Ellipsoid e1(mu, G);
    fig1.draw_ellipsoid(e1, {Color::red(),Color::red(0.3)});
    cout << "Initial ellipsoid e1:" << endl;
    cout << e1 << endl;

    // discrete nonlinear pendulum system
    VectorVar x(2);
    AnalyticFunction h {
            {x}, vec(x[0]+0.1*x[1],-0.2*sin(x[0])+0.9*x[1])};

    // linear mapping
    int N = 10;
    Ellipsoid e2 = Ellipsoid(e1);
    for(int i =0; i<N;i++){
        Matrix A = h.diff(e2.mu).mid();
        Vector b(h.eval(e2.mu).mid()._e - A._e*e2.mu._e);
        e2 = linear_mapping(e2, A, b);
        fig1.draw_ellipsoid(e2, {Color::green(),Color::green(0.3)});
    }
    cout << "\nLinear Mapping - Image ellipsoid e2:" << endl;
    cout << e2 << endl;
    // nonlinear mapping
    Ellipsoid e3 = Ellipsoid(e1);
    for(int i =0; i<N;i++){
        e3 = nonlinear_mapping(e3, h);
        fig1.draw_ellipsoid(e3, {Color::blue(),Color::blue(0.3)});
    }
    cout << "\nNon Linear Mapping - Image ellipsoid e3:" << endl;
    cout << e3 << endl;
}
