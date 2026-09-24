/** 
 *  codac2_Figure3D_Rerun.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure3D.h"
#include "codac2_Figure3D_Rerun.h"
#include "codac2_Paving.h"
#include "codac2_math.h"
#include "codac2_trunc.h"
#include <cstdlib>

using namespace std;
using namespace codac2;

Figure3D_Rerun::Figure3D_Rerun(const Figure3D& fig)
#ifdef CODAC_WITH_RERUN
  : OutputFigure3D(fig), _rec(fig.name())
{
  const char* spawn_env = std::getenv("CODAC_RERUN_SPAWN");
  if(spawn_env && std::string(spawn_env) != "0")
  {
    _rec.spawn().exit_on_failure();
  }
}
#else
  : OutputFigure3D(fig)
{
}
#endif

Figure3D_Rerun::~Figure3D_Rerun()
{
#ifdef CODAC_WITH_RERUN
  if(!_is_saved)
  {
    save(_fig.name() + ".rrd");
  }
  else
  {
    _rec.flush_blocking();
  }
#endif
}

void Figure3D_Rerun::update_axes()
{
}

void Figure3D_Rerun::clear()
{
#ifdef CODAC_WITH_RERUN
  _rec.log(sanitize_rerun_path(_fig.name()), rerun::Clear::RECURSIVE);
  _item_count = 0;
#endif
}

void Figure3D_Rerun::save([[maybe_unused]] const std::string& filename)
{
#ifdef CODAC_WITH_RERUN
  // Do not handle files explicitly meant for other backends (such as .obj or .xml)
  if(filename.size() >= 4 && (filename.compare(filename.size() - 4, 4, ".obj") == 0 ||
                              filename.compare(filename.size() - 4, 4, ".xml") == 0))
    return;

  std::string target = filename;
  if(target.find(".rrd") == std::string::npos)
  if(target.size() < 4 || target.compare(target.size() - 4, 4, ".rrd") != 0)
    target += ".rrd";

  (void)_rec.save(target);
  _rec.flush_blocking();
  _is_saved = true;
  _last_saved_filename = target;
#endif
}

void Figure3D_Rerun::draw_point([[maybe_unused]] const Vector& c, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(c.size() >= 3);
  auto entity = make_entity_path(_fig.name(), style.layer, "point3d", ++_item_count);
  float radius = style.line_width > 0.0 ? static_cast<float>(style.line_width * 0.02) : 0.05f;
  _rec.log(entity, rerun::Points3D({{ static_cast<float>(c[0]), static_cast<float>(c[1]), static_cast<float>(c[2]) }})
                    .with_colors({to_rerun_color(style.stroke_color)})
                    .with_radii({radius}));
#endif
}

void Figure3D_Rerun::draw_box([[maybe_unused]] const IntervalVector& x, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(x.size() == 3);
  float cx = 0.5f * static_cast<float>(x[0].lb() + x[0].ub());
  float cy = 0.5f * static_cast<float>(x[1].lb() + x[1].ub());
  float cz = 0.5f * static_cast<float>(x[2].lb() + x[2].ub());
  float hx = 0.5f * static_cast<float>(x[0].diam());
  float hy = 0.5f * static_cast<float>(x[1].diam());
  float hz = 0.5f * static_cast<float>(x[2].diam());

  auto entity = make_entity_path(_fig.name(), style.layer, "box3d", ++_item_count);
  Color c = style.fill_color == codac2::Color::none() ? style.stroke_color : style.fill_color;
  _rec.log(entity, rerun::Boxes3D::from_centers_and_half_sizes({{cx, cy, cz}}, {{hx, hy, hz}})
                    .with_colors({to_rerun_color(c)}));
#endif
}

void Figure3D_Rerun::draw_polyline([[maybe_unused]] const std::vector<Vector>& x, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  if(x.empty()) return;
  std::vector<rerun::components::Position3D> pts(x.size());
  for(size_t k = 0; k < x.size(); ++k)
  {
    assert(x[k].size() >= 3);
    pts[k] = rerun::components::Position3D(static_cast<float>(x[k][0]),
                               static_cast<float>(x[k][1]),
                               static_cast<float>(x[k][2]));
  }
  auto entity = make_entity_path(_fig.name(), style.layer, "polyline3d", ++_item_count);
  _rec.log(entity, rerun::LineStrips3D(std::vector<rerun::components::LineStrip3D>{rerun::components::LineStrip3D(pts)}).with_colors({to_rerun_color(style.stroke_color)}));
#endif
}

#ifdef CODAC_WITH_RERUN
static rerun::components::Vector3D compute_normal_3d(const Vector& p1, const Vector& p2, const Vector& p3)
{
  float dx1 = static_cast<float>(p2[0] - p1[0]);
  float dy1 = static_cast<float>(p2[1] - p1[1]);
  float dz1 = static_cast<float>(p2[2] - p1[2]);
  float dx2 = static_cast<float>(p3[0] - p1[0]);
  float dy2 = static_cast<float>(p3[1] - p1[1]);
  float dz2 = static_cast<float>(p3[2] - p1[2]);
  float nx = dy1 * dz2 - dz1 * dy2;
  float ny = dz1 * dx2 - dx1 * dz2;
  float nz = dx1 * dy2 - dy1 * dx2;
  float len = std::sqrt(nx * nx + ny * ny + nz * nz);
  if(len > 1e-6f)
  {
    nx /= len;
    ny /= len;
    nz /= len;
  }
  return rerun::components::Vector3D(nx, ny, nz);
}
#endif

void Figure3D_Rerun::draw_triangle([[maybe_unused]] const Vector &c, [[maybe_unused]] const Matrix &A,
                                   [[maybe_unused]] const Vector &p1, [[maybe_unused]] const Vector &p2, [[maybe_unused]] const Vector &p3, 
                                   [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  Vector v1 = c + A * p1;
  Vector v2 = c + A * p2;
  Vector v3 = c + A * p3;

  std::vector<rerun::components::Position3D> vertices = {
    rerun::components::Position3D(static_cast<float>(v1[0]), static_cast<float>(v1[1]), static_cast<float>(v1[2])),
    rerun::components::Position3D(static_cast<float>(v2[0]), static_cast<float>(v2[1]), static_cast<float>(v2[2])),
    rerun::components::Position3D(static_cast<float>(v3[0]), static_cast<float>(v3[1]), static_cast<float>(v3[2]))
  };
  std::vector<rerun::TriangleIndices> indices = { rerun::TriangleIndices(0, 1, 2) };

  auto norm = compute_normal_3d(v1, v2, v3);
  std::vector<rerun::components::Vector3D> normals = { norm, norm, norm };

  auto entity = make_entity_path(_fig.name(), style.layer, "triangle", ++_item_count);
  Color col = style.fill_color == codac2::Color::none() ? style.stroke_color : style.fill_color;
  _rec.log(entity, rerun::Mesh3D(vertices)
                    .with_triangle_indices(indices)
                    .with_vertex_normals(normals)
                    .with_albedo_factor(to_rerun_albedo(col)));
#endif
}

void Figure3D_Rerun::draw_polygon([[maybe_unused]] const Vector &c, [[maybe_unused]] const Matrix &A,
                                  [[maybe_unused]] const std::vector<Vector> &l, 
                                  [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  if(l.size() < 3) return;

  std::vector<rerun::components::Position3D> vertices(l.size());
  std::vector<Vector> world_pts(l.size());
  for(size_t i = 0; i < l.size(); ++i)
  {
    world_pts[i] = c + A * l[i];
    vertices[i] = rerun::components::Position3D(static_cast<float>(world_pts[i][0]), static_cast<float>(world_pts[i][1]), static_cast<float>(world_pts[i][2]));
  }

  std::vector<rerun::TriangleIndices> indices;
  indices.reserve(l.size() - 2);
  for(size_t i = 1; i + 1 < l.size(); ++i)
  {
    indices.push_back(rerun::TriangleIndices(0, static_cast<uint32_t>(i), static_cast<uint32_t>(i + 1)));
  }

  auto norm = compute_normal_3d(world_pts[0], world_pts[1], world_pts[2]);
  std::vector<rerun::components::Vector3D> normals(l.size(), norm);

  auto entity = make_entity_path(_fig.name(), style.layer, "polygon3d", ++_item_count);
  Color col = style.fill_color == codac2::Color::none() ? style.stroke_color : style.fill_color;
  _rec.log(entity, rerun::Mesh3D(vertices)
                    .with_triangle_indices(indices)
                    .with_vertex_normals(normals)
                    .with_albedo_factor(to_rerun_albedo(col)));
#endif
}

void Figure3D_Rerun::draw_parallelogram([[maybe_unused]] const Vector &c, [[maybe_unused]] const Matrix &A,
                                        [[maybe_unused]] const Vector &p, [[maybe_unused]] const Vector &v1, [[maybe_unused]] const Vector &v2, 
                                        [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  std::vector<Vector> poly = {
    p - v1 - v2,
    p - v1 + v2,
    p + v1 + v2,
    p + v1 - v2
  };
  draw_polygon(c, A, poly, style);
#endif
}

void Figure3D_Rerun::draw_parallelepiped([[maybe_unused]] const Parallelepiped& p, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert_release(p.c.size() == 3);
  assert_release(p.A.rows() == 3 && p.A.cols() == 3);

  const std::vector<std::vector<Vector>> face_corners = {
    { Vector({-1, -1, -1}), Vector({-1, -1,  1}), Vector({-1,  1,  1}), Vector({-1,  1, -1}) }, // -X
    { Vector({ 1, -1, -1}), Vector({ 1,  1, -1}), Vector({ 1,  1,  1}), Vector({ 1, -1,  1}) }, // +X
    { Vector({-1, -1, -1}), Vector({ 1, -1, -1}), Vector({ 1, -1,  1}), Vector({-1, -1,  1}) }, // -Y
    { Vector({-1,  1, -1}), Vector({-1,  1,  1}), Vector({ 1,  1,  1}), Vector({ 1,  1, -1}) }, // +Y
    { Vector({-1, -1, -1}), Vector({-1,  1, -1}), Vector({ 1,  1, -1}), Vector({ 1, -1, -1}) }, // -Z
    { Vector({-1, -1,  1}), Vector({ 1, -1,  1}), Vector({ 1,  1,  1}), Vector({-1,  1,  1}) }  // +Z
  };

  std::vector<rerun::components::Position3D> vertices;
  std::vector<rerun::components::Vector3D> normals;
  std::vector<rerun::TriangleIndices> indices;
  vertices.reserve(24);
  normals.reserve(24);
  indices.reserve(12);

  for(size_t f = 0; f < 6; ++f)
  {
    uint32_t base = static_cast<uint32_t>(vertices.size());
    Vector w0 = p.c + p.A * face_corners[f][0];
    Vector w1 = p.c + p.A * face_corners[f][1];
    Vector w2 = p.c + p.A * face_corners[f][2];
    Vector w3 = p.c + p.A * face_corners[f][3];

    auto n = compute_normal_3d(w0, w1, w2);

    vertices.push_back(rerun::components::Position3D(static_cast<float>(w0[0]), static_cast<float>(w0[1]), static_cast<float>(w0[2])));
    vertices.push_back(rerun::components::Position3D(static_cast<float>(w1[0]), static_cast<float>(w1[1]), static_cast<float>(w1[2])));
    vertices.push_back(rerun::components::Position3D(static_cast<float>(w2[0]), static_cast<float>(w2[1]), static_cast<float>(w2[2])));
    vertices.push_back(rerun::components::Position3D(static_cast<float>(w3[0]), static_cast<float>(w3[1]), static_cast<float>(w3[2])));

    normals.push_back(n);
    normals.push_back(n);
    normals.push_back(n);
    normals.push_back(n);

    indices.push_back(rerun::TriangleIndices(base, base + 1, base + 2));
    indices.push_back(rerun::TriangleIndices(base, base + 2, base + 3));
  }

  auto entity = make_entity_path(_fig.name(), style.layer, "parallelepiped", ++_item_count);
  Color col = style.fill_color == codac2::Color::none() ? style.stroke_color : style.fill_color;
  _rec.log(entity, rerun::Mesh3D(vertices)
                    .with_triangle_indices(indices)
                    .with_vertex_normals(normals)
                    .with_albedo_factor(to_rerun_albedo(col)));
#endif
}

void Figure3D_Rerun::draw_zonotope([[maybe_unused]] const Zonotope& z, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert_release(z.c.size() == 3);
  Matrix id = Matrix::Identity(3,3);
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
        draw_parallelogram(z.c, id, R1+R2, Ai, Aj, style);
        draw_parallelogram(z.c, id, -R1+R2, Ai, Aj, style);
     }
  }
#endif
}

void Figure3D_Rerun::draw_arrow([[maybe_unused]] const Vector &c, [[maybe_unused]] const Matrix& A, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(c.size() >= 3 && A.cols() >= 1 && A.rows() >= 3);
  Vector dir = A.col(0);
  auto entity = make_entity_path(_fig.name(), style.layer, "arrow3d", ++_item_count);
  _rec.log(entity, rerun::Arrows3D::from_vectors({{ static_cast<float>(dir[0]), static_cast<float>(dir[1]), static_cast<float>(dir[2]) }})
                    .with_origins({{ static_cast<float>(c[0]), static_cast<float>(c[1]), static_cast<float>(c[2]) }})
                    .with_colors({to_rerun_color(style.stroke_color)}));
#endif
}

void Figure3D_Rerun::draw_axes([[maybe_unused]] double size, [[maybe_unused]] const Vector& origin)
{
#ifdef CODAC_WITH_RERUN
  assert(origin.size() == 3);
  auto entity = sanitize_rerun_path(_fig.name()) + "/axes";
  float ox = static_cast<float>(origin[0]);
  float oy = static_cast<float>(origin[1]);
  float oz = static_cast<float>(origin[2]);
  float s = static_cast<float>(size);

  _rec.log(entity, rerun::Arrows3D::from_vectors({
    {s, 0.0f, 0.0f}, // X
    {0.0f, s, 0.0f}, // Y
    {0.0f, 0.0f, s}  // Z
  }).with_origins({
    {ox, oy, oz},
    {ox, oy, oz},
    {ox, oy, oz}
  }).with_colors({
    rerun::Color(255, 0, 0, 255),
    rerun::Color(0, 255, 0, 255),
    rerun::Color(0, 0, 255, 255)
  }));
#endif
}

void Figure3D_Rerun::draw_surface([[maybe_unused]] const Vector &c, [[maybe_unused]] const Matrix &A,
                                  [[maybe_unused]] const Interval &Ip1, [[maybe_unused]] double dp1,
                                  [[maybe_unused]] const Interval &Ip2, [[maybe_unused]] double dp2,
                                  [[maybe_unused]] std::function<Vector(double,double)> f,
                                  [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  std::vector<rerun::components::Position3D> vertices;
  std::vector<rerun::components::Vector3D> normals;
  std::vector<rerun::TriangleIndices> indices;

  for(double p1 = Ip1.lb(); p1 < Ip1.ub(); p1 += dp1)
  {
    for(double p2 = Ip2.lb(); p2 < Ip2.ub(); p2 += dp2)
    {
      double next_p1 = std::min(p1 + dp1, Ip1.ub());
      double next_p2 = std::min(p2 + dp2, Ip2.ub());

      Vector w0 = c + A * f(p1, p2);
      Vector w1 = c + A * f(next_p1, p2);
      Vector w2 = c + A * f(next_p1, next_p2);
      Vector w3 = c + A * f(p1, next_p2);

      auto rn = compute_normal_3d(w0, w1, w3);

      uint32_t base = static_cast<uint32_t>(vertices.size());
      vertices.push_back(rerun::components::Position3D(static_cast<float>(w0[0]), static_cast<float>(w0[1]), static_cast<float>(w0[2])));
      vertices.push_back(rerun::components::Position3D(static_cast<float>(w1[0]), static_cast<float>(w1[1]), static_cast<float>(w1[2])));
      vertices.push_back(rerun::components::Position3D(static_cast<float>(w2[0]), static_cast<float>(w2[1]), static_cast<float>(w2[2])));
      vertices.push_back(rerun::components::Position3D(static_cast<float>(w3[0]), static_cast<float>(w3[1]), static_cast<float>(w3[2])));

      normals.push_back(rn);
      normals.push_back(rn);
      normals.push_back(rn);
      normals.push_back(rn);

      indices.push_back(rerun::TriangleIndices(base, base + 1, base + 2));
      indices.push_back(rerun::TriangleIndices(base, base + 2, base + 3));
    }
  }

  auto entity = make_entity_path(_fig.name(), style.layer, "surface", ++_item_count);
  Color col = style.fill_color == codac2::Color::none() ? style.stroke_color : style.fill_color;
  _rec.log(entity, rerun::Mesh3D(vertices)
                    .with_triangle_indices(indices)
                    .with_vertex_normals(normals)
                    .with_albedo_factor(to_rerun_albedo(col)));
#endif
}

void Figure3D_Rerun::draw_sphere(const Vector &c, const Matrix &A, const StyleProperties& style)
{
  draw_surface(c, A, Interval(-0.5*PI, 0.5*PI), 0.1*PI,
               Interval(0, 2.0*PI), 0.1*PI,
               [](double p1, double p2) -> Vector {
                 return { std::cos(p1)*std::cos(p2), std::cos(p1)*std::sin(p2), std::sin(p1) };
               },
               style);
}

void Figure3D_Rerun::draw_ellipsoid(const Ellipsoid &e, const StyleProperties& style)
{
  draw_sphere(e.mu, e.G, style);
}

void Figure3D_Rerun::draw_car(const Vector &c, const Matrix &A, const StyleProperties& style)
{
  // Left side
  std::vector<Vector> side = { 
    {1.5, -0.8, 1}, {2.8, -0.8, 1}, {3, -0.8, 0}, {-1, -0.8, 0}, {0, -0.8, 2}, {1, -0.8, 2}
  };
  draw_polygon(c, A, side, style);

  // Right side
  for(auto &e : side) { e[1] = 0.8; }
  draw_polygon(c, A, side, style);

  // Rear and top
  std::vector<Vector> reartop = { 
    {0, -0.8, 2}, {-1, -0.8, 0}, {-1, 0.8, 0}, {0, 0.8, 2}, {1, 0.8, 2}, {1, -0.8, 2}
  };
  draw_polygon(c, A, reartop, style);

  // Front and motor
  std::vector<Vector> front = { 
    {2.8, -0.8, 1}, {1.5, -0.8, 1}, {1.5, 0.8, 1}, {2.8, 0.8, 1}, {3, 0.8, 0}, {3, -0.8, 0}
  };
  draw_polygon(c, A, front, style);

  // Windscreen
  StyleProperties ws_style = style;
  ws_style.stroke_color = Color::blue(0.6);
  ws_style.fill_color = Color::blue(0.3);
  draw_parallelogram(c, A, Vector({1.25, 0.0, 1.5}),
                     Vector({0, 0.8, 0}), Vector({-0.25, 0, 0.5}), ws_style);
}

void Figure3D_Rerun::draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up, const StyleProperties& style)
{
  // Wings
  std::vector<Vector> wings = { 
    {-1.5, 0, 0}, {-1.5, -1, 0}, {-1, -1, 0}, {0, 0, 0}, {-1, 1, 0}, {-1.5, 1, 0}
  };
  draw_polygon(c, A, wings, style);

  double vert = yaw_is_up ? 1.0 : -1.0;
  // Fuselage body
  draw_triangle(c, A, {-1.5, 0, 0}, {1.5, 0, 0}, {0.5, -0.1, 0.5*vert}, style);
  draw_triangle(c, A, {-1.5, 0, 0}, {1.5, 0, 0}, {0.5,  0.1, 0.5*vert}, style);
  // Vertical stabilizer
  draw_triangle(c, A, {-1.5, 0, 0}, {-1.5, 0, vert}, {-0.5, 0, 0}, style);
}

void Figure3D_Rerun::draw_AUV(const Vector &c, const Matrix &A, const StyleProperties& style)
{
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
  // Stabilizer fins
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2, 0,  0.6}, {-1.0, 0, 0}, style);
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2, 0, -0.6}, {-1.0, 0, 0}, style);
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2,  0.6, 0}, {-1.0, 0, 0}, style);
  draw_triangle(c, A, {-0.5, 0, 0}, {-1.2, -0.6, 0}, {-1.0, 0, 0}, style);
}

void Figure3D_Rerun::draw_paving([[maybe_unused]] const PavingOut& p, [[maybe_unused]] const StyleProperties& boundary_style)
{
#ifdef CODAC_WITH_RERUN
  if(!p.tree() || !p.tree()->left())
    return;

  std::vector<rerun::components::Position3D> centers;
  std::vector<rerun::components::HalfSize3D> half_sizes;

  p.tree()->left()->visit([&](std::shared_ptr<const PavingOut_Node> n) {
    const IntervalVector& outer = get<0>(n->boxes());
    if(n->is_leaf() && !outer.is_empty() && outer.size() == 3)
    {
      float cx = 0.5f * static_cast<float>(outer[0].lb() + outer[0].ub());
      float cy = 0.5f * static_cast<float>(outer[1].lb() + outer[1].ub());
      float cz = 0.5f * static_cast<float>(outer[2].lb() + outer[2].ub());
      float hx = 0.5f * static_cast<float>(outer[0].diam());
      float hy = 0.5f * static_cast<float>(outer[1].diam());
      float hz = 0.5f * static_cast<float>(outer[2].diam());
      centers.push_back(rerun::components::Position3D(cx, cy, cz));
      half_sizes.push_back(rerun::components::HalfSize3D(hx, hy, hz));
    }
    return true;
  });

  if(centers.empty())
    return;

  std::string layer = (boundary_style.layer.empty() || boundary_style.layer == "alpha") ? "boundary" : boundary_style.layer;
  auto entity = make_entity_path(_fig.name(), layer, "paving", ++_item_count);
  Color c = boundary_style.fill_color == codac2::Color::none() ? boundary_style.stroke_color : boundary_style.fill_color;
  _rec.log(entity, rerun::Boxes3D::from_centers_and_half_sizes(centers, half_sizes)
                    .with_colors({to_rerun_color(c)}));
#endif
}

void Figure3D_Rerun::draw_paving([[maybe_unused]] const PavingInOut& p, [[maybe_unused]] const StyleProperties& boundary_style,
                                 [[maybe_unused]] const StyleProperties& inside_style)
{
#ifdef CODAC_WITH_RERUN
  if(!p.tree())
    return;

  std::vector<rerun::components::Position3D> bound_centers, in_centers;
  std::vector<rerun::components::HalfSize3D> bound_half_sizes, in_half_sizes;

  p.tree()->visit([&](std::shared_ptr<const PavingInOut_Node> n) {
    const IntervalVector& outer = get<0>(n->boxes());
    const IntervalVector& inner = get<1>(n->boxes());

    IntervalVector hull = inner | outer;
    for(const auto& bi : hull.diff(inner))
    {
      if(!bi.is_empty() && bi.size() == 3)
      {
        float cx = 0.5f * static_cast<float>(bi[0].lb() + bi[0].ub());
        float cy = 0.5f * static_cast<float>(bi[1].lb() + bi[1].ub());
        float cz = 0.5f * static_cast<float>(bi[2].lb() + bi[2].ub());
        float hx = 0.5f * static_cast<float>(bi[0].diam());
        float hy = 0.5f * static_cast<float>(bi[1].diam());
        float hz = 0.5f * static_cast<float>(bi[2].diam());
        in_centers.push_back(rerun::components::Position3D(cx, cy, cz));
        in_half_sizes.push_back(rerun::components::HalfSize3D(hx, hy, hz));
      }
    }

    if(n->is_leaf())
    {
      auto boundary = inner & outer;
      if(!boundary.is_empty() && boundary.size() == 3)
      {
        float cx = 0.5f * static_cast<float>(boundary[0].lb() + boundary[0].ub());
        float cy = 0.5f * static_cast<float>(boundary[1].lb() + boundary[1].ub());
        float cz = 0.5f * static_cast<float>(boundary[2].lb() + boundary[2].ub());
        float hx = 0.5f * static_cast<float>(boundary[0].diam());
        float hy = 0.5f * static_cast<float>(boundary[1].diam());
        float hz = 0.5f * static_cast<float>(boundary[2].diam());
        bound_centers.push_back(rerun::components::Position3D(cx, cy, cz));
        bound_half_sizes.push_back(rerun::components::HalfSize3D(hx, hy, hz));
      }
    }
    return true;
  });

  if(!in_centers.empty())
  {
    std::string in_layer = (inside_style.layer.empty() || inside_style.layer == "alpha") ? "inside" : inside_style.layer;
    auto in_entity = make_entity_path(_fig.name(), in_layer, "paving_in", ++_item_count);
    Color c_in = inside_style.fill_color == codac2::Color::none() ? inside_style.stroke_color : inside_style.fill_color;
    _rec.log(in_entity, rerun::Boxes3D::from_centers_and_half_sizes(in_centers, in_half_sizes)
                          .with_colors({to_rerun_color(c_in)}));
  }

  if(!bound_centers.empty())
  {
    std::string bound_layer = (boundary_style.layer.empty() || boundary_style.layer == "alpha") ? "boundary" : boundary_style.layer;
    auto bound_entity = make_entity_path(_fig.name(), bound_layer, "paving_bound", ++_item_count);
    Color c_bound = boundary_style.fill_color == codac2::Color::none() ? boundary_style.stroke_color : boundary_style.fill_color;
    _rec.log(bound_entity, rerun::Boxes3D::from_centers_and_half_sizes(bound_centers, bound_half_sizes)
                             .with_colors({to_rerun_color(c_bound)}));
  }
#endif
}
