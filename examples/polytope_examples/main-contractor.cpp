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

   // definition using facets
   std::vector<std::pair<Row,double>> facets 
       { { {1,1}, 3.1 },
         { {-1,0.6} ,1.6 },
         { {0.5,0.2} ,1.4 },
         { {0.5,0.75} ,1.75 },
         { {-0.75,-1} ,1.33 },
         { {0.5,-1} ,1 },
         { {-1.0/3.0,-1} ,1 } };
   // the first argument is a bounding box, here the whole space
   Polytope p({{-2,3.0},{-1,2.1}}, facets);

   SepPolytope sep(p);
   
//   auto pv1 = pave({{-3.0,3.0},{-3.0,3.0}},sep,0.04);

   DefaultFigure::pave({{-2.0,3.0},{-3.0,3.0}},sep,0.04);
 
   DefaultFigure::draw_polytope(p);

   return 0;
}
