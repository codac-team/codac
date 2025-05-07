/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_transformations.h>
#include <codac2_math.h>
#include <codac2_AnalyticTraj.h>
#include <codac2_Figure2D.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("Affine transformation")
{
  // Building the src trajectory analytically

    ScalarVar t;
    AnalyticFunction f_src({t}, {
      cos(2*t)+0.1*cos(10*t),
      2*sin(t)+0.1*sin(10*t)
    });

    SampledTraj<Vector> src = AnalyticTraj(f_src,{-1,3}).sampled(0.01);

  // The dst trajectory is obtained analytically with a
  // transformation described by the parameters:

    double a = PI + PI/3; // rotation angle
    double b = 2.5; // scaling
    Vector T({ -3.2, 2.5 }); // translation
    //Matrix Rot {{cos(a),-sin(a)},{sin(a),cos(a)}};
    //Rot *= b;

    AnalyticFunction f_dst({t}, {
      b*cos(a)*f_src(t)[0]-b*sin(a)*f_src(t)[1] + T[0] + 0.05*cos(100*t),
      b*sin(a)*f_src(t)[0]+b*cos(a)*f_src(t)[1] + T[1] + 0.05*sin(100*t)
    });

    SampledTraj<Vector> dst = AnalyticTraj(f_dst,{-1,3}).sampled(0.01);

  //DefaultFigure::set_window_properties({75,75},{700,700});
  //DefaultFigure::set_axes(axis(0,{-8,4}), axis(1,{-4,8}));
  //DefaultFigure::draw_trajectory(src, Color::blue());
  //DefaultFigure::draw_trajectory(dst, Color::red());

  // Computing the transformation

    Eigen::Affine2d tr = affine_transformation(src, dst);

  // Reconstructing the dst trajectory using the estimated transformation

    SampledTraj<Vector> dst_estim;
    for(const auto& [ti,src_i] : src)
      dst_estim.set(ti, tr*Eigen::Vector2d(src_i));

  //DefaultFigure::draw_trajectory(dst_estim, Color::purple());

  double scale = tr.linear().col(0).norm();
  Matrix rotation_matrix = tr.linear() / scale;
  double rotation_angle = atan2(rotation_matrix(1, 0), rotation_matrix(0, 0));
  Vector translation = tr.translation();

  CHECK(Approx(scale,1e-3) == b);
  CHECK(Approx(rotation_angle,1e-3) == (a-2*PI));
  CHECK(Approx(Vector(translation),1e-3) == T);

  //cout << "  Scale = " << scale << " (" << b << ")" << endl;
  //cout << "  Angle = " << rotation_angle << " (" << a-2*PI << ")" << endl;
  //cout << "  Translation = " << translation << " (" << T << ")" << endl;
  //cout << "End." << endl;
}