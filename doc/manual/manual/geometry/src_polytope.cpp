// Author : Damien Massé
// Graphical illustration of the polytope test

#include <codac>
#include <vector>
#include <memory>

using namespace std;
using namespace codac2;

void draw_polytope(Figure3D &fig, const Polytope &P, const StyleProperties &st) {
   std::vector<std::vector<Vector>> facets3D=P.vertices_3Dfacets();
   Vector center = Vector::zero(3);
   Matrix transfo = Matrix::Identity(3,3);
   for (const std::vector<Vector> &vec : facets3D) {
      fig.draw_polygon(center,transfo,vec,st);
   }
}

int main(int argc, char *argv[])
{
//   std::cout << std::scientific << std::setprecision(20);

   Figure3D fig("manual_polytope");

   fig.draw_axes();

   // [polytope-1-beg]
   // definition using vertices
   std::vector<Vector> vertices 
       { {0,0,0}, {2,1,0}, {-1,1,0}, {0,1.5,2}, {-1.5,0,-1.5}, {0,3,0} };
   Polytope p1(vertices); // vertices {0,0,0}, {2,1,0}, etc.
   // definition using facets
   std::vector<std::pair<Row,double>> facets 
       { { {1,1,0.75}, 3 },        // x+y+0.75z <= 3
         { {-1,-0.4,0.6} ,0.6 },   // -x-0.4y+0.6z <= 0.6
         { {-1,0.5,0.375} ,1.5 },  // ...
         { {-1,0.5,0} ,1.5 },
         { {0.5,-1,0.75} ,0 },
         { {-0.75,-1,0.75} ,0 },
         { {0.5,-1,-0.5} ,0 },
         { {1.0/3.0,1.0/3.0,-1} ,1 } };
   // the first argument is a bounding box, here the whole space
   Polytope p2(IntervalVector(3), facets);
   // p1 and p2 are almost the same polytope
   // [polytope-1-end]


   // [polytope-2-beg]
   std::cout << p1 << std::endl;
   /* output, each facet is a row (sequences of double) and a right-hand-side
    Polytope(bbox [ [-1.5, 2] ; [0, 3] ; [-1.5, 2] ]) : 
    Collectfacets : 8 facets
    1 :    1    1 0.75<=3
    2 :        -1 -0.399999       0.6<=0.600001
    3 :    -1   0.5 0.375<=1.5
    4 :          -1         0.5 1.77636e-16<=1.5
    5 :  0.5   -1 0.75<=0
    6 :    -0.75       -1 0.750001<=5.92119e-16
    7 :  0.5   -1 -0.5<=0
    8 : 0.333334 0.333334       -1<=1
     end Collectfacets
    EndPolytope
   */
   std::vector<IntervalVector> vp2 = p2.vertices(); /* hull of p2 */
   for (auto &v : vp2) std::cout << v << std::endl;
   /* output : "vertices" are boxes which enclose the polytope, but each
      box may not enclose a "real vertice". E.g. the vertice (0,0,0) is
      not inside the second box displayed.
    [ [1.99999, 2.00001] ; [0.999999, 1.00001] ; [-2.4743e-14, -1.70641e-15] ]
    [ [-7.76463e-15, 1.69539e-14] ; [-3.15401e-14, -2.88916e-15] ; [-2.52802e-14, -2.40763e-15] ]
    [ [-1.50001, -1.5] ; [-1.16016e-14, -9.33785e-15] ; [-1.50001, -1.5] ]
    [ [-7.11673e-15, 1.609e-14] ; [1.49999, 1.50001] ; [1.99999, 2.00001] ]
    [ [-2.47539e-15, 9.90153e-16] ; [2.99999, 3.00001] ; [-2.31036e-15, 3.30051e-15] ]
    [ [-1.00001, -0.999999] ; [1, 1.00001] ; [1.83361e-15, 3.20883e-15] ]
   */
   // [polytope-2-end]

   fig.draw_polytope(p1,StyleProperties(Color::dark_red(0.8),"p1"));
   fig.draw_polytope(p2,StyleProperties(Color::dark_blue(0.8),"p2"));

   return 0;
}
