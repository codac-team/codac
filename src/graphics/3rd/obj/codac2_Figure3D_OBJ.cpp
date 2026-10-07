/** 
 *  \file codac2_Figure3D_OBJ.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard, Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure3D.h"
#include "codac2_Figure3D_OBJ.h"
#include "codac2_math.h"

using namespace std;
using namespace codac2;

Figure3D_OBJ::Figure3D_OBJ(const Figure3D& fig)
  : OutputFigure3D(fig)
{
  _file.open(fig.name() + ".obj");
  _is_open = _file.is_open();
}

Figure3D_OBJ::~Figure3D_OBJ()
{
  if(_file.is_open())
    _file.close();
}

void Figure3D_OBJ::update_axes()
{
}

void Figure3D_OBJ::clear()
{
  if(_file.is_open())
    _file.close();
  vertex_count = 0;
  _file.open(_fig.name() + ".obj", std::ofstream::trunc);
  _is_open = _file.is_open();
}

void Figure3D_OBJ::save(const std::string& filename)
{
  // Do not handle files explicitly meant for other backends (such as .rrd or .xml)
  if(filename.size() >= 4 && (filename.compare(filename.size() - 4, 4, ".rrd") == 0 ||
                              filename.compare(filename.size() - 4, 4, ".xml") == 0))
    return;

  std::string target = filename;
  if(target.find(".obj") == std::string::npos)
  if(target.size() < 4 || target.compare(target.size() - 4, 4, ".obj") != 0)
    target += ".obj";

  if(_file.is_open())
    _file.close();

  // If different filename than default, rename or copy
  // If different filename than default, copy
  if(target != _fig.name() + ".obj")
  {
    std::ifstream src(_fig.name() + ".obj", std::ios::binary);
    std::ofstream dst(target, std::ios::binary);
    dst << src.rdbuf();
  }
}

void Figure3D_OBJ::set_color_internal(const Color &c)
{
  _file << "newmtl " << c.hex_str().substr(1,6) << "\n";
  _file << "Kd " << c.rgb()[0]/255. << " " << c.rgb()[1]/255. << " " << c.rgb()[2]/255. << "\n";
  _file << "d "<< c.rgb()[3]/255.<<"\n";
  _file << "usemtl " << c.hex_str().substr(1,6) << "\n";
}

void Figure3D_OBJ::set_style_internal(const StyleProperties& style)
{
  if (lock_style) return;
  if (style.layer=="" || style.layer=="alpha") {
     _file<< "o "<< _fig.name()<<"\n";
  } else {
     _file<< "o "<< style.layer<<"\n";
  }
  this->set_color_internal(style.stroke_color.rgb());
}

size_t Figure3D_OBJ::move_write_v(const Vector &c, const Matrix &A, const Vector &p)
{
  Vector pos = c + A * p;
  _file << "v " << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
  return (++this->vertex_count);
}

void Figure3D_OBJ::draw_point(const Vector& c, const StyleProperties& style)
{
  // Point in OBJ represented by small box
  double s = style.line_width > 0 ? style.line_width * 0.01 : 0.02;
  IntervalVector b = { {c[0]-s, c[0]+s}, {c[1]-s, c[1]+s}, {c[2]-s, c[2]+s} };
  draw_box(b, style);
}

void Figure3D_OBJ::draw_polyline(const std::vector<Vector>& x, const StyleProperties& style)
{
  if (x.size() < 2) return;
  this->set_style_internal(style);
  std::vector<size_t> idx(x.size());
  for(size_t k = 0; k < x.size(); ++k)
    idx[k] = this->move_write_v(Vector::Zero(3), Matrix::Identity(3,3), x[k]);

  _file << "l";
  for(size_t k = 0; k < x.size(); ++k)
    _file << " " << idx[k];
  _file << "\n";
}

void Figure3D_OBJ::draw_triangle(const Vector &c, const Matrix &A,
                                const Vector &p1, const Vector &p2, const Vector &p3, 
                                const StyleProperties& style)
{
  this->set_style_internal(style);
  size_t ip1 = this->move_write_v(c,A,p1);
  size_t ip2 = this->move_write_v(c,A,p2);
  size_t ip3 = this->move_write_v(c,A,p3);
  _file << "f " << ip1 << " " << ip2 << " " << ip3 << "\n";
}

void Figure3D_OBJ::draw_triangle(const Vector &p1, const Vector &p2,
                                const Vector &p3, 
                                const StyleProperties& style)
{
  this->draw_triangle(Vector::zero(3), Matrix::Identity(3,3), p1, p2, p3, style);
}

void Figure3D_OBJ::draw_polygon(const Vector &c, const Matrix &A,
                               const std::vector<Vector> &l, 
                               const StyleProperties& style)
{
  if (l.size() <= 2) return;
  this->set_style_internal(style);
  size_t ip1 = this->move_write_v(c,A,l[0]);
  size_t ip2 = this->move_write_v(c,A,l[1]);
  for (size_t i = 2; i < l.size(); i++) {
     size_t ip3 = this->move_write_v(c,A,l[i]);
     _file << "f " << ip1 << " " << ip2 << " " << ip3 << "\n";
     ip2 = ip3;
  }
}

void Figure3D_OBJ::draw_parallelogram(const Vector &c, const Matrix &A,
                                     const Vector &p, const Vector &v1, const Vector &v2, 
                                     const StyleProperties& style)
{
  this->set_style_internal(style);
  size_t ip1 = this->move_write_v(c,A,p-v1-v2);
  size_t ip2 = this->move_write_v(c,A,p-v1+v2);
  size_t ip3 = this->move_write_v(c,A,p+v1+v2);
  size_t ip4 = this->move_write_v(c,A,p+v1-v2);
  _file << "f " << ip1 << " " << ip2 << " " << ip3 << " " << ip4 << "\n";
}

void Figure3D_OBJ::draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style)
{
  assert_release(p.c.size() == 3);
  assert_release(p.A.rows() == 3 && p.A.cols() == 3);

  this->set_style_internal(style);

  size_t ip0 = this->move_write_v(p.c,p.A,Vector({-1,-1,-1}));
  size_t ip1 = this->move_write_v(p.c,p.A,Vector({-1,-1,1}));
  size_t ip2 = this->move_write_v(p.c,p.A,Vector({-1,1,-1}));
  size_t ip3 = this->move_write_v(p.c,p.A,Vector({-1,1,1}));
  size_t ip4 = this->move_write_v(p.c,p.A,Vector({1,-1,-1}));
  size_t ip5 = this->move_write_v(p.c,p.A,Vector({1,-1,1}));
  size_t ip6 = this->move_write_v(p.c,p.A,Vector({1,1,-1}));
  size_t ip7 = this->move_write_v(p.c,p.A,Vector({1,1,1}));

  _file << "f " << ip0 << " " << ip1 << " " << ip3 << " " << ip2 << "\n";
  _file << "f " << ip4 << " " << ip5 << " " << ip7 << " " << ip6 << "\n";
  _file << "f " << ip0 << " " << ip4 << " " << ip6 << " " << ip2 << "\n";
  _file << "f " << ip1 << " " << ip5 << " " << ip7 << " " << ip3 << "\n";
  _file << "f " << ip0 << " " << ip4 << " " << ip5 << " " << ip1 << "\n";
  _file << "f " << ip2 << " " << ip6 << " " << ip7 << " " << ip3 << "\n";
}

void Figure3D_OBJ::draw_box(const IntervalVector& x, const StyleProperties& style)
{
  assert_release(x.size()==3);
  Vector inter = (x-x.mid()).ub();
  Matrix A ({{inter[0],0,0},{0,inter[1],0},{0,0,inter[2]}});
  draw_parallelepiped({x.mid(), A}, style);
}

void Figure3D_OBJ::draw_zonotope(const Zonotope& z, const StyleProperties& style)
{
  assert_release(z.c.size() == 3);
  Matrix id = Matrix::Identity(3,3);
  this->set_style_internal(style);
  lock_style = true;
  size_t nb = z.A.cols();
  for (size_t j = 1; j < nb; j++) {
     const Eigen::Vector3d &Aj = z.A.col(j);
     for (size_t i = 0; i < j; i++) {
         const Eigen::Vector3d &Ai = z.A.col(i);
         Eigen::Vector3d Norm = Ai.cross(Aj);
         if (Norm.lpNorm<Eigen::Infinity>() < 1e-8) continue;
         Eigen::Vector3d R1 = Eigen::Vector3d::Zero();
         Eigen::Vector3d R2 = Eigen::Vector3d::Zero();
         for (size_t k = 0; k < nb; k++) {
             if (k == i || k == j) continue;
             const Eigen::Vector3d &Ak = z.A.col(k);
             double prod = Norm.dot(Ak);
             if (std::fabs(prod) < 1e-8) {
                if (k > j) {
                  R2 -= Ak; 
                  continue;
                }
                Eigen::Vector3d N2 = Ai.cross(Ak);
                if (N2.lpNorm<Eigen::Infinity>() < 1e-8) {
                    if (Ai.dot(Ak) < 0 || i < k) R2 -= Ak; else R2 += Ak;
                    continue;
                }
                if (N2.dot(Norm) >= 0) R2 += Ak; else R2 -= Ak;
             } 
             else if (prod > 0.0) {
                R1 += Ak;
             }
             else {
                R1 -= Ak;
             }
        }
        this->draw_parallelogram(z.c, id, R1+R2, Ai, Aj, style);
        this->draw_parallelogram(z.c, id, -R1+R2, Ai, Aj, style);
     }
  }
  lock_style = false;
}

void Figure3D_OBJ::draw_arrow(const Vector& c, const Matrix &A,
                              const StyleProperties& style)
{
  Vector v = c + A * Vector({0.5, 0.0, 0.0});
  Matrix tr {{0.5, 0, 0}, {0, 0.01, 0}, {0, 0, 0.01}};
  double a = 0.05;
  this->set_style_internal(style);
  lock_style = true;
  draw_parallelepiped({v, A*tr}, style);
  std::vector<Vector> l_points { 
    { 1+4*a, 0, 0 },
    { 1, a, -a },
    { 1, -a, -a },
    { 1, -a, a },
    { 1, a, a },
    { 1, a, -a }
  };
  this->draw_polygon(c, A, l_points, style);
  lock_style = false;
}

void Figure3D_OBJ::draw_axes(double size, const Vector& origin)
{
  assert_release(origin.size() == 3);
  const std::string name = "axes";
  Vector z = origin;
  // X axis
  Matrix AX = Matrix::Identity(3,3);
  draw_arrow(z, size*AX, StyleProperties(Color::red(), name));
  // Y axis
  Matrix AY {{0,0,1},{1,0,0},{0,1,0}};
  draw_arrow(z, size*AY, StyleProperties(Color::green(), name));
  // Z axis
  Matrix AZ {{0,1,0},{0,0,1},{1,0,0}};
  draw_arrow(z, size*AZ, StyleProperties(Color::blue(), name));
  _file << "o " << _fig.name() << "\n";
}

void Figure3D_OBJ::draw_surface(const Vector &c, const Matrix &A,
                                const Interval &Ip1, double dp1,
                                const Interval &Ip2, double dp2,
                                std::function<Vector(double,double)> f,
                                const StyleProperties& style)
{
  this->set_style_internal(style);
  lock_style = true;
  for (double p1 = Ip1.lb(); p1 < Ip1.ub(); p1 += dp1) 
  for (double p2 = Ip2.lb(); p2 < Ip2.ub(); p2 += dp2) {
     Vector a0 = f(p1, p2);
     Vector a1 = f(p1+dp1, p2);
     Vector a2 = f(p1+dp1, p2+dp2);
     Vector a3 = f(p1, p2+dp2);
     draw_triangle(c, A, a0, a1, a2, style);
     draw_triangle(c, A, a0, a2, a3, style);
  } 
  lock_style = false;
}

void Figure3D_OBJ::draw_sphere(const Vector &c, const Matrix &A, const StyleProperties& style)
{
  this->draw_surface(c, A, Interval(-0.5*PI, 0.5*PI), 0.05*PI,
                     Interval(0, 2.0*PI), 0.05*PI,
                     [](double p1, double p2) -> Vector {
                       return { std::cos(p1)*std::cos(p2), std::cos(p1)*std::sin(p2), std::sin(p1) };
                     },
                     style);
}

void Figure3D_OBJ::draw_ellipsoid(const Ellipsoid &e, const StyleProperties& style)
{
  this->draw_sphere(e.mu, e.G, style);
}

void Figure3D_OBJ::draw_car(const Vector &c, const Matrix &A,
                            const StyleProperties& style)
{
  this->set_style_internal(style);
  lock_style = true;
  /* left side (y=-0.8) */
  std::vector<Vector> side { 
    {1.5, -0.8, 1},
    {2.8, -0.8, 1},
    {3, -0.8, 0},  
    {-1, -0.8, 0},
    {0, -0.8, 2},
    {1, -0.8, 2}
  };
  draw_polygon(c, A, side, style);
  for (auto &e : side) { e[1] = 0.8; }
  draw_polygon(c, A, side, style);
  /* rear and top */
  std::vector<Vector> reartop { 
    {0, -0.8, 2},
    {-1, -0.8, 0},
    {-1, 0.8, 0},  
    {0, 0.8, 2},
    {1, 0.8, 2},
    {1, -0.8, 2}
  };
  draw_polygon(c, A, reartop, style);
  /* front and motor */
  std::vector<Vector> front { 
    {2.8, -0.8, 1},
    {1.5, -0.8, 1},
    {1.5, 0.8, 1}, 
    {2.8, 0.8, 1},
    {3, 0.8, 0},
    {3, -0.8, 0}
  };
  draw_polygon(c, A, front, style);
  lock_style = false;
  /* windscreen */
  draw_parallelogram(c, A, Vector({1.25, 0.0, 1.5}),
                     Vector({0, 0.8, 0}), Vector({-0.25, 0, 0.5}), 
                     StyleProperties(Color::blue(0.5), style.layer));
}

void Figure3D_OBJ::draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up,
                             const StyleProperties& style)
{
  this->set_style_internal(style);
  lock_style = true;
  /* wings */
  std::vector<Vector> wings { 
    {-1.5, 0, 0},
    {-1.5, -1, 0},
    {-1, -1, 0}, 
    {0, 0, 0},
    {-1, 1, 0},
    {-1.5, 1, 0}
  };
  draw_polygon(c, A, wings, style);
  double vert = yaw_is_up ? 1.0 : -1.0;
  /* body */
  draw_triangle(c, A, {-1.5, 0, 0}, {1.5, 0, 0}, {0.5, -0.1, 0.5*vert}, style);
  draw_triangle(c, A, {-1.5, 0, 0}, {1.5, 0, 0}, {0.5, 0.1, 0.5*vert}, style);
  /* vertical stabilizer */
  draw_triangle(c, A, {-1.5, 0, 0}, {-1.5, 0, vert}, {-0.5, 0, 0}, style);
  lock_style = false;
}

void Figure3D_OBJ::draw_AUV(const Vector &c, const Matrix &A,
                            const StyleProperties& style)
{
  this->set_style_internal(style);
  lock_style = true;
  // AUV body: main cylinder/capsule along X axis
  // Main hull triangles
  const int N = 8;
  double r = 0.3;
  double len = 2.0;
  for(int i = 0; i < N; ++i)
  {
    double th1 = 2 * PI * i / N;
    double th2 = 2 * PI * (i + 1) / N;
    Vector p1_nose { len * 0.5 + 0.5, 0, 0 };
    Vector p1_front { len * 0.5, r * std::cos(th1), r * std::sin(th1) };
    Vector p2_front { len * 0.5, r * std::cos(th2), r * std::sin(th2) };
    draw_triangle(c, A, p1_nose, p1_front, p2_front, style);

    Vector p1_back { -len * 0.5, r * std::cos(th1), r * std::sin(th1) };
    Vector p2_back { -len * 0.5, r * std::cos(th2), r * std::sin(th2) };
    draw_triangle(c, A, p1_front, p1_back, p2_back, style);
    draw_triangle(c, A, p1_front, p2_back, p2_front, style);

    Vector p_tail { -len * 0.5 - 0.4, 0, 0 };
    draw_triangle(c, A, p_tail, p2_back, p1_back, style);
  }
  // Fins
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2, 0, 0.6}, {-1.0, 0, 0}, style);
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2, 0, -0.6}, {-1.0, 0, 0}, style);
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2, 0.6, 0}, {-1.0, 0, 0}, style);
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2, -0.6, 0}, {-1.0, 0, 0}, style);
  lock_style = false;
}
