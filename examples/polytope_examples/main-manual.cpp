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

   Figure3D fig("manual_polytope");

   fig.draw_axes();

   // definition using vertices
   std::vector<Vector> vertices 
	{ {0,0,0}, {2,1,0}, {-1,1,0}, {0,1.5,2}, {-1.5,0,-1.5}, {0,3,0} };
   Polytope p1(vertices);
   // definition using facets
   std::vector<std::pair<Row,double>> facets 
       { { {1,1,0.75}, 3 },
         { {-1,-0.4,0.6} ,0.6 },
         { {-1,0.5,0.375} ,1.5 },
         { {-1,0.5,0} ,1.5 },
         { {0.5,-1,0.75} ,0 },
         { {-0.75,-1,0.75} ,0 },
         { {0.5,-1,-0.5} ,0 },
         { {1.0/3.0,1.0/3.0,-1} ,1 } };
   // the first argument is a bounding box, here the whole space
   Polytope p2(IntervalVector::Constant(3,Interval()), facets);

   // p1 and p2 are ``almost'' the same polytope

   fig.draw_polytope(p1,StyleProperties(Color::dark_red(0.8),"p1"));
   std::cout << p1 << std::endl;

   std::vector<IntervalVector> vp2 = p2.vertices();
   for (auto &v : vp2) std::cout << v << std::endl;
   fig.draw_polytope(p2,StyleProperties(Color::dark_blue(0.8),"p2"));

   return 0;
}
