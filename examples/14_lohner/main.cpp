#include <codac>

using namespace std;
using namespace codac2;

int main()
{
    IntervalVector x0({Interval(1,1),Interval(1,1)});
    x0.inflate(0.1);

    auto ta =create_tdomain({0.,10.0},0.2);
    SlicedTube a(ta, IntervalVector(2));
    a.set(x0,0.0);

    auto tb =create_tdomain({0.,10.0},0.01);
    SlicedTube b(tb, IntervalVector(2));
    b.set(x0,0.0);

    VectorVar x(2);
    AnalyticFunction f({x}, {-x[0],-sin(x[1])});
    CtcLohner ctc_lohner(f);

    ctc_lohner.contract(a);
    ctc_lohner.contract(b);

    Figure2D fig1 ("Lohner_1",GraphicOutput::VIBES|GraphicOutput::IPE);
    Figure2D fig2 ("Lohner_2",GraphicOutput::VIBES|GraphicOutput::IPE);

    fig1.plot_tube(a[0]);
    fig1.plot_tube(b[0],{Color::blue(),Color::blue()});

    fig2.plot_tube(a[1]);
    fig2.plot_tube(b[1],{Color::blue(),Color::blue()});

    fig1.set_window_properties({100,50},{1000,400});
    fig2.set_window_properties({100,500},{1000,400});
}