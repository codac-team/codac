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
  _file<< "o "<<name<<"\n";
}

Figure3D::~Figure3D()
{
  _file.close();
}

const std::string& Figure3D::name() const
{
  return _name;
}

void Figure3D::draw_axes(double size)
{
  _file<< "o axes\n";
  // X axis
  draw_box(IntervalVector({{-size/40.,size-size/40.},{-size/40.,size/40.},{-size/40.,size/40.}}),Color::red());
  draw_box(IntervalVector({{size-size/40.,size+size/10.},{-size/20.,size/20.},{-size/20.,size/20.}}),Color::red());
  // Y axis
  draw_box(IntervalVector({{-size/40.,size/40.},{-size/40.,size-size/40.},{-size/40.,size/40.}}),Color::green());
  draw_box(IntervalVector({{-size/20.,size/20.},{size-size/40.,size+size/10.},{-size/20.,size/20.}}),Color::green());
  // Z axis
  draw_box(IntervalVector({{-size/40.,size/40.},{-size/40.,size/40.},{-size/40.,size-size/40.}}),Color::blue());
  draw_box(IntervalVector({{-size/20.,size/20.},{-size/20.,size/20.},{size-size/40.,size+size/10.}}),Color::blue());
  _file<< "o "<<_name<<"\n";
}

void Figure3D::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  assert_release(x.size()==3);
  Vector inter = (x-x.mid()).ub();
  Matrix A ({{inter[0],0,0},{0,inter[1],0},{0,0,inter[2]}});
  draw_parallelepiped(x.mid(), A, s);

}

void Figure3D::draw_parallelepiped(const Vector& z, const Matrix& A, const StyleProperties& s)
{
  assert_release(z.size() == 3);
  assert_release(A.rows() == 3 && A.cols() == 3);

  Color c = s.stroke_color.rgb();

  _file << "newmtl " << c.hex_str().substr(1,6) << "\n";
  _file << "Kd " << c.rgb()[0]/255. << " " << c.rgb()[1]/255. << " " << c.rgb()[2]/255. << "\n";
  _file << "d "<<c.rgb()[3]/255.<<"\n";
  _file << "usemtl " << c.hex_str().substr(1,6) << "\n";

  Vector v0 = z - A.col(0) - A.col(1) - A.col(2);
  Vector v1 = z - A.col(0) - A.col(1) + A.col(2);
  Vector v2 = z - A.col(0) + A.col(1) - A.col(2);
  Vector v3 = z - A.col(0) + A.col(1) + A.col(2);
  Vector v4 = z + A.col(0) - A.col(1) - A.col(2);
  Vector v5 = z + A.col(0) - A.col(1) + A.col(2);
  Vector v6 = z + A.col(0) + A.col(1) - A.col(2);
  Vector v7 = z + A.col(0) + A.col(1) + A.col(2);

  _file << "v " << v0[0] << " " << v0[1] << " " << v0[2] << "\n";
  _file << "v " << v1[0] << " " << v1[1] << " " << v1[2] << "\n";
  _file << "v " << v2[0] << " " << v2[1] << " " << v2[2] << "\n";
  _file << "v " << v3[0] << " " << v3[1] << " " << v3[2] << "\n";
  _file << "v " << v4[0] << " " << v4[1] << " " << v4[2] << "\n";
  _file << "v " << v5[0] << " " << v5[1] << " " << v5[2] << "\n";
  _file << "v " << v6[0] << " " << v6[1] << " " << v6[2] << "\n";
  _file << "v " << v7[0] << " " << v7[1] << " " << v7[2] << "\n";

  size_t v = vertex_count + 1;
  _file << "f " << v + 0 << " " << v + 1 << " " << v + 3 << " " << v + 2 << "\n";
  _file << "f " << v + 4 << " " << v + 5 << " " << v + 7 << " " << v + 6 << "\n";
  _file << "f " << v + 0 << " " << v + 4 << " " << v + 6 << " " << v + 2 << "\n";
  _file << "f " << v + 1 << " " << v + 5 << " " << v + 7 << " " << v + 3 << "\n";
  _file << "f " << v + 0 << " " << v + 4 << " " << v + 5 << " " << v + 1 << "\n";
  _file << "f " << v + 2 << " " << v + 6 << " " << v + 7 << " " << v + 3 << "\n";

  vertex_count += 8;

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