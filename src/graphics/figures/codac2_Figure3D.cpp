/** 
 *  codac2_Figure3D.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Index.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_Figure3D.h"
#include "codac2_math.h"


using namespace std;
using namespace codac2;

Figure3D::Figure3D(const std::string& name)
  : _name(name)
{
  _file.open(name + ".obj");
//  _file<< "o "<<name<<"\n";
}

Figure3D::~Figure3D()
{
  _file.close();
}

const std::string& Figure3D::name() const
{
  return _name;
}

void Figure3D::set_color_internal(const Color &c) {
  _file << "newmtl " << c.hex_str().substr(1,6) << "\n";
  _file << "Kd " << c.rgb()[0]/255. << " " << c.rgb()[1]/255. << " " << c.rgb()[2]/255. << "\n";
  _file << "d "<< c.rgb()[3]/255.<<"\n";
  _file << "usemtl " << c.hex_str().substr(1,6) << "\n";
}

void Figure3D::set_style_internal(const StyleProperties& s) {
  if (lock_style) return;
  if (s.layer=="" || s.layer=="alpha") {
     _file<< "o "<< _name<<"\n";
  } else {
     _file<< "o "<< s.layer<<"\n";
  }
  this->set_color_internal(s.stroke_color.rgb());
}

size_t Figure3D::move_write_v(const Vector &c, const Matrix &A, const Vector &p) {
   Vector pos = c+A*p;
   
   _file << "v " << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
   return (++this->vertex_count);
}

void Figure3D::draw_triangle(const Vector &c, const Matrix &A,
	const Vector &p1, const Vector &p2, const Vector &p3, 
	const StyleProperties& s) {

   this->set_style_internal(s);
   size_t ip1 = this->move_write_v(c,A,p1);
   size_t ip2 = this->move_write_v(c,A,p2);
   size_t ip3 = this->move_write_v(c,A,p3);
   _file << "f " << ip1 << " " << ip2 << " " << ip3 << "\n";
}

void Figure3D::draw_triangle(const Vector &p1, const Vector &p2,
	const Vector &p3, 
	const StyleProperties& s) {

   this->draw_triangle(Vector::zero(3),Matrix::Identity(3,3),p1,p2,p3,s);
}

void Figure3D::draw_polygon(const Vector &c, const Matrix &A,
        const std::vector<Vector> &l, 
        const StyleProperties& s) {

   if (l.size()<=2) return;
   this->set_style_internal(s);
   size_t ip1 = this->move_write_v(c,A,l[0]);
   size_t ip2 = this->move_write_v(c,A,l[1]);
   for (size_t i=2;i<l.size();i++) {
      size_t ip3 = this->move_write_v(c,A,l[i]);
      _file << "f " << ip1 << " " << ip2 << " " << ip3 << "\n";
      ip2=ip3;
   }
}


void Figure3D::draw_parallelogram(const Vector &c, const Matrix &A,
	const Vector &p, const Vector &v1, const Vector &v2, 
	const StyleProperties& s) {

   this->set_style_internal(s);
   size_t ip1 = this->move_write_v(c,A,p-v1-v2);
   size_t ip2 = this->move_write_v(c,A,p-v1+v2);
   size_t ip3 = this->move_write_v(c,A,p+v1+v2);
   size_t ip4 = this->move_write_v(c,A,p+v1-v2);
   _file << "f " << ip1 << " " << ip2 << " " << ip3 << " " << ip4 << "\n";
}

void Figure3D::draw_parallelepiped(const Vector& z, const Matrix& A, const StyleProperties& s)
{
  assert_release(z.size() == 3);
  assert_release(A.rows() == 3 && A.cols() == 3);

  this->set_style_internal(s);

  size_t ip0 = this->move_write_v(z,A,Vector({-1,-1,-1}));
  size_t ip1 = this->move_write_v(z,A,Vector({-1,-1,1}));
  size_t ip2 = this->move_write_v(z,A,Vector({-1,1,-1}));
  size_t ip3 = this->move_write_v(z,A,Vector({-1,1,1}));
  size_t ip4 = this->move_write_v(z,A,Vector({1,-1,-1}));
  size_t ip5 = this->move_write_v(z,A,Vector({1,-1,1}));
  size_t ip6 = this->move_write_v(z,A,Vector({1,1,-1}));
  size_t ip7 = this->move_write_v(z,A,Vector({1,1,1}));

  _file << "f " << ip0 << " " << ip1 << " " << ip3 << " " << ip2 << "\n";
  _file << "f " << ip4 << " " << ip5 << " " << ip7 << " " << ip6 << "\n";
  _file << "f " << ip0 << " " << ip4 << " " << ip6 << " " << ip2 << "\n";
  _file << "f " << ip1 << " " << ip5 << " " << ip7 << " " << ip3 << "\n";
  _file << "f " << ip0 << " " << ip4 << " " << ip5 << " " << ip1 << "\n";
  _file << "f " << ip2 << " " << ip6 << " " << ip7 << " " << ip3 << "\n";
}


void Figure3D::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  assert_release(x.size()==3);
  Vector inter = (x-x.mid()).ub();
  Matrix A ({{inter[0],0,0},{0,inter[1],0},{0,0,inter[2]}});
  draw_parallelepiped(x.mid(), A, s);
}


void Figure3D::draw_arrow(const Vector& c, const Matrix &A,
	const StyleProperties& s)
{
  Vector v = c+A*Vector({0.5,0.0,0.0});
  Matrix tr {{0.5,0,0},{0,0.01,0},{0,0,0.01}};
  double a=0.05;
  this->set_style_internal(s);
  lock_style=true;
  draw_parallelepiped(v, A*tr, s);
  std::vector<Vector> l_points { 
	{ 1+4*a,0,0 },
	{ 1,a,-a },
	{ 1,-a,-a },
	{ 1,-a,a },
	{ 1,a,a },
	{ 1,a,-a } };
  this->draw_polygon(c,A,l_points,s);
  lock_style=false;
}


void Figure3D::draw_axes(double size)
{
  const std::string name="axes";
  Vector z = Vector::zero(3);
  // X axis
  Matrix AX = Matrix::Identity(3,3);
  draw_arrow(z,size*AX,StyleProperties(Color::red(),name));
  // Y axis
  Matrix AY {{0,0,1},{1,0,0},{0,1,0}};
  draw_arrow(z,size*AY,StyleProperties(Color::green(),name));
  // Z axis
  Matrix AZ {{0,1,0},{0,0,1},{1,0,0}};
  draw_arrow(z,size*AZ,StyleProperties(Color::blue(),name));
  _file<< "o "<<_name<<"\n";
}

void Figure3D::draw_surface(const Vector &c, const Matrix &A,
                const Interval &Ip1, double dp1,
                const Interval &Ip2, double dp2,
                std::function<Vector(double,double)> f,
                const StyleProperties& s) {

  this->set_style_internal(s);
  lock_style=true;
  for (double p1 = Ip1.lb(); p1 < Ip1.ub(); p1+=dp1) 
  for (double p2 = Ip2.lb(); p2 < Ip2.ub(); p2+=dp2) {
     Vector a0 = f(p1,p2);
     Vector a1 = f(p1+dp1,p2);
     Vector a2 = f(p1+dp1,p2+dp2);
     Vector a3 = f(p1,p2+dp2);
     draw_triangle(c,A,a0,a1,a2,s);
     draw_triangle(c,A,a0,a2,a3,s);
  } 
  lock_style=false;
}

void Figure3D::draw_sphere(const Vector &c, const Matrix &A, const StyleProperties& s) {
  this->draw_surface(c,A,Interval(-0.5*PI,0.5*PI),0.05*PI,
		Interval(0,2.0*PI),0.05*PI,
	[](double p1, double p2) -> Vector {
	    Vector a({ std::cos(p1)*std::cos(p2),std::cos(p1)*std::sin(p2),std::sin(p1) });
            return a;
        },
	s
  );
}

void Figure3D::draw_car(const Vector &c, const Matrix &A,
                const StyleProperties& s) {
  this->set_style_internal(s);
  lock_style=true;
  /* left side (y=-0.8) */
  std::vector<Vector> side { {1.5,-0.8,1},
			     {2.8,-0.8,1},
			     {3,-0.8,0},	
			     {-1,-0.8,0},
   			     {0,-0.8,2},
	  		     {1,-0.8,2} };
  draw_polygon(c,A,side,s);
  for (auto &e : side) { e[1] = 0.8; }
  draw_polygon(c,A,side,s);
  /* rear and top  */
  std::vector<Vector> reartop { {0,-0.8,2},
			     {-1,-0.8,0},
			     {-1,0.8,0},	
			     {0,0.8,2},
   			     {1,0.8,2},
	  		     {1,-0.8,2} };
  draw_polygon(c,A,reartop,s);
  /* front and motor  */
  std::vector<Vector> front { {2.8,-0.8,1},
			     {1.5,-0.8,1},
			     {1.5,0.8,1},	
			     {2.8,0.8,1},
   			     {3,0.8,0},
	  		     {3,-0.8,0} };
  draw_polygon(c,A,front,s);
  lock_style=false;
  /* windscreen */
  draw_parallelogram(c,A,Vector({1.25,0.0,1.5}),
		Vector({0,0.8,0}), Vector({-0.25,0,0.5}), 
		StyleProperties(Color::blue(0.5),s.layer));
}

void Figure3D::draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up,
                const StyleProperties& s) {
  this->set_style_internal(s);
  lock_style=true;
  /* wings */
  std::vector<Vector> wings { {-1.5,0,0},
			     {-1.5,-1,0},
			     {-1,-1,0},	
			     {0,0,0},
   			     {-1,1,0},
	  		     {-1.5,1,0} };
  draw_polygon(c,A,wings,s);
  double vert = yaw_is_up ? 1.0 : -1.0;
  /* body */
  draw_triangle(c,A,{-1.5,0,0},{1.5,0,0},{0.5,-0.1,0.5*vert},s);
  draw_triangle(c,A,{-1.5,0,0},{1.5,0,0},{0.5,0.1,0.5*vert},s);
  /* vertical stabilizer */
  draw_triangle(c,A,{-1.5,0,0},{-1.5,0,vert},{-0.5,0,0},s);
  lock_style=false;
}


void Figure3D::draw_paving(const PavingOut& p,
  const StyleProperties& boundary_style)
{
  p.tree()->left()->visit([&]
    (std::shared_ptr<const PavingOut_Node> n)
    {
      const IntervalVector& outer = get<0>(n->boxes());

      if(n->is_leaf() && !outer.is_empty())
        draw_box(outer, boundary_style);

      return true;
    });
}

void Figure3D::draw_paving(const PavingInOut& p, const StyleProperties& boundary_style,
  const StyleProperties& inside_style)
{
  p.tree()->visit([&]
      (std::shared_ptr<const PavingInOut_Node> n)
      {
        const IntervalVector& outer = get<0>(n->boxes());
        const IntervalVector& inner = get<1>(n->boxes());

        IntervalVector hull = inner | outer;

        for(const auto& bi : hull.diff(inner))
          if (!bi.is_empty())
            draw_box(bi, inside_style);

        if(n->is_leaf())
          {
          auto boundary = inner & outer;
          if (!boundary.is_empty())
            draw_box(boundary, boundary_style);
          }
        return true;
      });
}
