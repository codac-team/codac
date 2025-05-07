/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_geometry.h>
#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("orientation")
{
  // Align points
  CHECK(orientation({0.,0.},{5.,0.},{8.,0.}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0.,0.},{5.,0.},{-2.,0.}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0.,0.},{-2.,0.},{5.,0.}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0.,0.},{0.,0.},{5.,0.}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0.,0.},{0.,0.},{0.,0.}) == OrientationInterval::COLINEAR);

  // Clockwise points
  CHECK(orientation({0.,0.},{0.,-5.},{-2.,-5.}) == OrientationInterval::COUNTERCLOCKWISE);
  CHECK(orientation({-999.,0.1},{0.,0.},{-999.,-0.1}) == OrientationInterval::COUNTERCLOCKWISE);

  // Anticlockwise points
  CHECK(orientation({0.,0.},{0.,-5.},{2.,-5.}) == OrientationInterval::CLOCKWISE);
  CHECK(orientation({999.,0.1},{0.,0.},{999.,-0.1}) == OrientationInterval::CLOCKWISE);

  // Other tests
  CHECK(orientation({0,0},{1,1},{0,2}) == OrientationInterval::CLOCKWISE);
  CHECK(orientation({0,0},{1,1},{2,2}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0,0},{1,1},{2.2,2}) == OrientationInterval::COUNTERCLOCKWISE);
  CHECK(orientation({0,0},{2,2},{2,2}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0,0},{2,2},{4,4}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0,0},{8,8},{4,4}) == OrientationInterval::COLINEAR);
  CHECK(orientation({0,0},{10,1},{4,4}) == OrientationInterval::CLOCKWISE);
  CHECK(orientation({0,0},{2,2},{10,1}) == OrientationInterval::COUNTERCLOCKWISE);
}

TEST_CASE("Alignments")
{
  IntervalVector p1({0.,0.});
  IntervalVector p2({0.,1.});
  IntervalVector p3({0.,10.});
  IntervalVector p4({1.,10.});
  IntervalVector p5({0.,9.+1e-8*Interval(-1.,1.)});
  IntervalVector p6({0.+1e-8*Interval(-1.,1.),9.});
  IntervalVector p7({0.+1e-8*Interval(-1.,1.),9.+1e-8*Interval(-1.,1.)});

  CHECK(aligned(p1,p2,p3) == BoolInterval::TRUE);
  CHECK(aligned(p1,p2,p4) == BoolInterval::FALSE);
  CHECK(aligned(p1,p1,p4) == BoolInterval::TRUE);
  CHECK(aligned(p1,p2,p5) == BoolInterval::TRUE);
  CHECK(aligned(p1,p2,p6) == BoolInterval::UNKNOWN);
  CHECK(aligned(p1,p2,p7) == BoolInterval::UNKNOWN);
  CHECK(aligned({0,0},{1,1},{10,10}) == BoolInterval::TRUE);
  CHECK(aligned({{-oo,0},{-oo,0}},{1,1},{10,10}) == BoolInterval::UNKNOWN);
  CHECK(aligned(IntervalVector(2),{1,1},{10,10}) == BoolInterval::UNKNOWN);
}

TEST_CASE("convex_hull")
{
  vector<IntervalVector> v {
    {0,3},{1,1},{2,2},{4,4},{0,0},{1,2},{3,1},{3,3}
  };

  auto hull = convex_hull(v);

  CHECK(hull[0] == IntervalVector({0,0}));
  CHECK(hull[1] == IntervalVector({3,1}));
  CHECK(hull[2] == IntervalVector({4,4}));
  CHECK(hull[3] == IntervalVector({0,3}));
  CHECK(hull.size() == 4);

  //for(const auto& vi : v)
  //  DefaultFigure::draw_point(vi.mid(), Color::red());
  //for(size_t i = 0 ; i < hull.size() ; i++)
  //  DefaultFigure::draw_line(hull[i].mid(), hull[(i+1)%hull.size()].mid(), Color::red());

  v = vector<IntervalVector> {
    {1,3},{1,4},{1.5,2},{2,1},{2,2},{3,0},
    {3,3},{3,4.5},{4,2.5},{4,4},{5,1},{5,2},
    {4,0},{5,0},{5,5},{6,0},{7,2}
  };

  hull = convex_hull(v);

  CHECK(hull[0] == IntervalVector({3,0}));
  CHECK(hull[1] == IntervalVector({6,0}));
  CHECK(hull[2] == IntervalVector({7,2}));
  CHECK(hull[3] == IntervalVector({5,5}));
  CHECK(hull[4] == IntervalVector({3,4.5}));
  CHECK(hull[5] == IntervalVector({1,4}));
  CHECK(hull[6] == IntervalVector({1,3}));
  CHECK(hull[7] == IntervalVector({1.5,2}));
  CHECK(hull[8] == IntervalVector({2,1}));
  CHECK(hull.size() == 9);

  //for(const auto& vi : v)
  //  DefaultFigure::draw_point(vi.mid(), Color::blue());
  //for(size_t i = 0 ; i < hull.size() ; i++)
  //  DefaultFigure::draw_line(hull[i].mid(), hull[(i+1)%hull.size()].mid(), Color::blue());

  v = vector<IntervalVector> {
    {0.307007,0.0340742},
    {0.0340742,0.307},
    {0.1,0.2},{0.1,0.2},{0.2,0.1},{0.1,0.1}
  };

  hull = convex_hull(v);

  CHECK(hull[0] == IntervalVector({0.307007,0.0340742}));
  CHECK(hull[1] == IntervalVector({0.0340742,0.307}));
  CHECK(hull[2] == IntervalVector({0.1,0.1}));
  CHECK(hull.size() == 3);

  //for(const auto& vi : v)
  //  DefaultFigure::draw_point(vi.mid(), Color::blue());
  //for(size_t i = 0 ; i < hull.size() ; i++)
  //  DefaultFigure::draw_line(hull[i].mid(), hull[(i+1)%hull.size()].mid(), Color::blue());

  // Other test

  v = vector<IntervalVector> {
    {-4041.935273669676917052129283547401428223,-5492.667604696881426207255572080612182617},
    {-2103.177277725693329557543620467185974121,-5492.667604696881426207255572080612182617},
    {5720.923292917194885376375168561935424805,-975.4210340695084369144751690328121185303},
    {9206.843580880462468485347926616668701172,5062.370015818080901226494461297988891602},
    {52.79381299725321952109879930503666400909,5062.370015818080901226494461297988891602},
    {-4041.935273669676917052129283547401428223-1,-5592.667604696874150249641388654708862305},
    {9206.843580880462468485347926616668701172,6551.674997467660432448610663414001464844},
    {52.79381299725321952109879930503666400909,6551.674997467660432448610663414001464844},
    {-4041.935273669676917052129283547401428223,-540.603823869623056452837772667407989502}
  };
  auto v_save = v;

  hull = convex_hull(v);

  CHECK(hull[0] == v_save[5]);
  CHECK(hull[1] == v_save[1]);
  CHECK(hull[2] == v_save[2]);
  CHECK(hull[3] == v_save[3]);
  CHECK(hull[4] == v_save[6]);
  CHECK(hull[5] == v_save[7]);
  CHECK(hull[6] == v_save[8]);
  CHECK(hull.size() == 7);

  //for(const auto& vi : v)
  //  DefaultFigure::draw_point(vi.mid(), Color::blue());
  //for(size_t i = 0 ; i < hull.size() ; i++)
  //  DefaultFigure::draw_line(hull[i].mid(), hull[(i+1)%hull.size()].mid(), Color::blue());
}