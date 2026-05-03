/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Polytope.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("Polytope")
{
  /* polytope with vertices
       (0,0,0) , (2,0,1) , (1,0.1,0) , (0,-0.1,1), (1,2,0.5)
     inequalities
       -x + 20y + 82z <= 80        x - 20y - 2z <= 0
       -20.5x + 10y +z <= 0        -x + 10y - 38z <= 0
       3.9x + y - 3.8z <= 4	   x -10y - 2z <= 0
     + "redundant" inequality 
       -2.1 x + 2y + 4.2z <= 4 */
  std::vector<std::pair<Row,double>> facets 
       { { {-1,20,82}, 80 },
         { {-20.5,10,1} ,0 },
         { {3.9,1,-3.8} ,4 },
         { {1,-20,-2} ,0 },
         { {-1,10,-38} ,0 },
         { {1,-10,-2} ,0 } };

  SECTION("Definition, box, contains")
  {
    Polytope p(IntervalVector({{-4,4},{-4,4},{-4,4}}), facets, true);

    CHECK(!p.is_empty());
    CHECK(!p.is_flat());
    CHECK(Approx(p.box(),1e-8) == IntervalVector({{0.,2.},{-0.1,2.},{0.,1.}}));
    CHECK(IntervalVector({{0.,2.},{-0.1,2.},{0.,1.}}).is_subset(p.box()));
    CHECK(Approx(p.bound_row(Row({-2.1,2,4.2})),1e-8)==4.0);
    CHECK(Approx(p.bound_row(Row({1.0,1.0,1.0})),1e-8)==3.5);
    CHECK(p.contains(IntervalVector({1.0,1.0,0.5}))==BoolInterval::TRUE);
    CHECK(p.contains(IntervalVector({1.0,1.0,61.0/82.0}))==BoolInterval::UNKNOWN);
    CHECK(p.contains(IntervalVector({1.0,1.0,1.0}))==BoolInterval::FALSE);
  }
   
  SECTION("Subset")
  {
    Polytope p(IntervalVector({{-4,4},{-4,4},{-4,4}}), facets, true);

    Polytope q(IntervalVector({{0.2,0.4},{0.0,0.3},{0.3,0.5}}));
    CHECK(q.is_subset(p));
    Polytope r(3,true); /* empty polyhedron */
    CHECK(r.is_empty());
    CHECK(r.is_subset(q));
  }

  SECTION("Transformation")
  {
    Polytope p(IntervalVector({{-4,4},{-4,4},{-4,4}}), facets, true);
    Polytope q = p;
    q.inflate(0.5);
    CHECK(p.is_subset(q));
    Polytope r = p;
    r.inflate_ball(0.5);
    CHECK(r.is_subset(q));
    Polytope s = p;
    s.unflat(2,0.5);
    CHECK(s.is_subset(r));
    Polytope t = p;
    t.homothety(IntervalVector({0,0,-0.5}),2);
    CHECK(Approx(t.bound_row(Row({1.0,1.0,1.0})),1e-8)==7.5);
    Polytope u=t;
    u.meet_with_polytope(p);
    CHECK(u.is_subset(t));
    CHECK(u.is_subset(p));
    Polytope v = Polytope::union_of_polytopes({ p,t });
    CHECK(p.is_subset(v));
    CHECK(t.is_subset(v));
    t.inflate(0.1);
    CHECK(!t.is_subset(v));
    /* transformation : rotation of pi/3 degrees (reverse), center (1,0,0) */
    IntervalMatrix M { { cos(PI/3) , sin(PI/3) , 0 },
		       { -sin(PI/3), cos(PI/3) , 0 },
                       { 0 , 0 , 1 } };
    IntervalVector P {  1-cos(PI/3) , sin(PI/3) , 0 };
    Polytope w = p.reverse_affine_transform(M,P,
			IntervalVector({{-4,4},{-4,4},{-4,4}}));
    CHECK(Approx(w.box(),1e-5) ==
	 IntervalVector({{1.0-2*sin(PI/3),1.5},
                         {-sin(PI/3)-0.1*cos(PI/3),1.},
                         {0.,1.}}));
  }

}

