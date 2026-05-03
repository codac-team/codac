// Author : Damien Massé
// Generate random points on a ball of radius 10 and draw the convex hull

#include <codac>
#include <vector>
#include <set>
#include <memory>
#include <random>

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


std::random_device rd;  // Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(-1.0,1.0);

double radius=10.0;

Vector random_point() {
   double a=dis(gen)*radius;
   double b=dis(gen)*std::sqrt(radius*radius-a*a);
   double c=(dis(gen)>0.0 ? 1.0 : -1.0) *  std::sqrt(radius*radius-a*a-b*b);
   return { a,b,c }; 
}

int main(int argc, char *argv[])
{
   Figure3D fig("3D hull");

   std::cout << std::scientific << std::setprecision(9);
  
   int nbvertices=20;
   if (argc>1) nbvertices= std::atoi(argv[1]);
   std::vector<Vector> vertices;
   Matrix scale = 0.1*Matrix::Identity(3,3);
   for (int i=0;i<nbvertices;i++) {
       Vector point = random_point();
       vertices.push_back(point);
       fig.draw_sphere(point, scale, StyleProperties(Color::black()));
   }
   Polytope pol = Polytope(vertices);

   draw_polytope(fig,pol,StyleProperties(Color::dark_blue(0.2),"hull"));

   return 0;
}
