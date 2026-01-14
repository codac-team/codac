#include <codac>

using namespace std;
using namespace codac2;


int main()
{
    // 2D case

    DefaultFigure::set_axes(axis(0,{-2.5,2.5}), axis(1,{-0.5,4.5}));
    DefaultFigure::set_window_properties({50,50},{500,500});

    // [1-beg]
    ScalarVar X_2d;
    AnalyticFunction f_2d({X_2d},{X_2d,sqr(X_2d)});
    // [1-end]

    // [2-beg]
    double dx_2d = 0.2;
    double x0_2d = -2.0;

    while (x0_2d<2.0)
    {
        Interval X0_2d(x0_2d, x0_2d+dx_2d);
        auto p = f_2d.parallelepiped_eval(X0_2d);
        DefaultFigure::draw_parallelepiped(p, Color::dark_green());

        x0_2d+=dx_2d;
    }
    // [2-end]

    // 3D case

    // [3-beg]
    VectorVar X_3d(2);
    AnalyticFunction f_3d({X_3d},{X_3d[0],X_3d[1],sqr(X_3d[0])+sqr(X_3d[1])});
    // [3-end]

    // [4-beg]
    Figure3D fig_3d ("3D");

    double dx_3d = 0.2;
    double x0_3d = -2.0;

    while (x0_3d<=2.0)
    {
        Interval X0_3d(x0_3d, x0_3d+dx_3d);
        double y0_3d=-2.0;

        while (y0_3d<=2.0)
        {
            Interval Y0_3d(y0_3d, y0_3d+dx_3d);
            auto b = f_3d.eval(IntervalVector({X0_3d,Y0_3d}));
            auto p = f_3d.parallelepiped_eval(IntervalVector({X0_3d,Y0_3d}));

            fig_3d.draw_box(b, StyleProperties(Color::blue(0.3),"box"));
            fig_3d.draw_parallelepiped(p, StyleProperties(Color::green(0.3),"parallelepiped"));

            y0_3d+=dx_3d;
        }

        x0_3d+=dx_3d;
    }
    // [4-end]

    // Thick case

    // [5-beg]
    ScalarVar X_if;
    AnalyticFunction f_if({X_if},{X_if,Interval(1.1,1.2)*sqr(X_if)});
    // [5-end]

    // [6-beg]
    AnalyticTraj f_lb (AnalyticFunction({X_if},1.1*sqr(X_if)),Interval(-2.0,2.0));
    AnalyticTraj f_ub (AnalyticFunction({X_if},1.2*sqr(X_if)),Interval(-2.0,2.0));

    DefaultFigure::plot_trajectory(f_lb.sampled(0.01));
    DefaultFigure::plot_trajectory(f_ub.sampled(0.01));
    // [6-end]

    // [7-beg]
    double dx_if = 0.1;
    double x0_if = -2.0;

    while (x0_if<=2.0)
    {
        Interval X0_if(x0_if, x0_if+dx_if);
        auto p = f_if.parallelepiped_eval(X0_if);
        DefaultFigure::draw_parallelepiped(p, Color::dark_green());
        
        x0_if+=dx_if;
    }
    // [7-end]
}