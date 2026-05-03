// Author : Damien Massé
// Graphical illustration of the polytope test

#include <codac>
#include <vector>
#include <memory>

using namespace std;
using namespace codac2;

int main(int argc, char *argv[])
{
//   std::cout << std::scientific << std::setprecision(20);

   Figure3D fig("test_polytope");

   fig.draw_axes();
/* polytope with vertices
       (0,0,0) , (2,0,1) , (1,0.1,0) , (0,-0.1,1), (1,2,0.5)
     inequalities
       -x + 20y + 82z <= 80        x - 20y - 2z <= 0
       -20.5x + 10y +z <= 0        -x + 10y - 38z <= 0
       3.9x + y - 3.8z <= 4        x -10y - 2z <= 0 */

   std::vector<std::pair<Row,double>> facets 
       { { {-1,20,82}, 80 },
         { {-20.5,10,1} ,0 },
         { {3.9,1,-3.8} ,4 },
         { {1,-20,-2} ,0 },
         { {-1,10,-38} ,0 },
         { {1,-10,-2} ,0 } };

   Polytope p(IntervalVector({{-4,4},{-4,4},{-4,4}}), facets, true);
   fig.draw_polytope(p,StyleProperties(Color::dark_red(0.8),
			"initial polytope"));

   std::vector<IntervalVector> vertices = p.vertices();
   
   fig.draw_box(IntervalVector({{0.2,0.4},{0.0,0.3},{0.3,0.5}}),
		StyleProperties(Color::black(),"box included"));

   Polytope q = p;
   q.inflate(0.5);
   for (IntervalVector &vt : vertices) {
      fig.draw_box(vt+IntervalVector::constant(3,Interval(-0.5,0.5)),
        StyleProperties(Color::orange(0.6),
 			"box_inflate"));
   }
   fig.draw_polytope(q,StyleProperties(Color::red(0.4),
			"inflate"));
   Polytope r = p;
   r.inflate_ball(0.5);
   for (IntervalVector &vt : vertices) {
      fig.draw_sphere(vt.mid(),0.5*Matrix::Identity(3,3),
        StyleProperties(Color::orange(0.6),
 			"ball_inflate"));
   }
   fig.draw_polytope(r,StyleProperties(Color::purple(0.4),
			"inflate_ball"));

   Polytope s = p;
   s.unflat(2,0.5);
   for (IntervalVector &vt : vertices) {
      fig.draw_box(vt+IntervalVector({{-0.01,0.01},{-0.01,0.01},{-0.5,0.5}}),
        StyleProperties(Color::black(),
 			"box_unflat"));
   }
   fig.draw_polytope(s,StyleProperties(Color::yellow(0.4),
                        "unflat"));

   Polytope t = p;
   t.homothety(IntervalVector({0,0,-0.5}),2);
   fig.draw_polytope(t,StyleProperties(Color::dark_blue(0.8),
                        "homothety"));

   Polytope u = t;
   u.meet_with_polytope(p);
   fig.draw_polytope(u,StyleProperties(Color::dark_gray(),
                        "intersection"));

   Polytope v = Polytope::union_of_polytopes({ p,t });
   fig.draw_polytope(v,StyleProperties(Color::cyan(0.4),
                        "union"));

   IntervalMatrix M { { cos(PI/3) , sin(PI/3) , 0 },
                       { -sin(PI/3), cos(PI/3) , 0 },
                       { 0 , 0 , 1 } };
   IntervalVector P {  1-cos(PI/3) , sin(PI/3) , 0 };
   Polytope w = p.reverse_affine_transform(M,P,
                        IntervalVector({{-4,4},{-4,4},{-4,4}}));

   fig.draw_polytope(w,StyleProperties(Color::blue(0.8),
                        "transformation"));

   return 0;
}
