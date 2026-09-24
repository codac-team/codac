/** 
 *  codac2_Figure2D_Rerun.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure2D.h"
#include "codac2_Figure2D_Rerun.h"
#include "codac2_math.h"
#include "codac2_trunc.h"
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;
using namespace codac2;

Figure2D_Rerun::Figure2D_Rerun(const Figure2D& fig)
#ifdef CODAC_WITH_RERUN
  : OutputFigure2D(fig), _rec(fig.name())
{
  const char* spawn_env = std::getenv("CODAC_RERUN_SPAWN");
  if(spawn_env && std::string(spawn_env) != "0")
  {
    _rec.spawn().exit_on_failure();
  }
}
#else
  : OutputFigure2D(fig)
{
}
#endif

Figure2D_Rerun::~Figure2D_Rerun()
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

void Figure2D_Rerun::update_axes()
{
#ifdef CODAC_WITH_RERUN
  if(_fig.axes().size() >= 2)
  {
    const auto& ax = _fig.axes()[0];
    const auto& ay = _fig.axes()[1];
    auto entity = sanitize_rerun_path(_fig.name()) + "/axes";
    std::vector<rerun::Position2D> x_axis = {
      rerun::Position2D(static_cast<float>(ax.limits.lb()), 0.0f),
      rerun::Position2D(static_cast<float>(ax.limits.ub()), 0.0f)
    };
    std::vector<rerun::Position2D> y_axis = {
      rerun::Position2D(0.0f, static_cast<float>(ay.limits.lb())),
      rerun::Position2D(0.0f, static_cast<float>(ay.limits.ub()))
    };
    _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{
                       rerun::components::LineStrip2D(x_axis),
                       rerun::components::LineStrip2D(y_axis)
                     }).with_colors({rerun::Color(180, 180, 180, 200)}));
  }
#endif
}

void Figure2D_Rerun::update_window_properties()
{
}

void Figure2D_Rerun::center_viewbox([[maybe_unused]] const Vector& c, [[maybe_unused]] const Vector& r)
{
}

void Figure2D_Rerun::clear()
{
#ifdef CODAC_WITH_RERUN
  _rec.log(sanitize_rerun_path(_fig.name()), rerun::Clear::RECURSIVE);
  _item_count = 0;
#endif
}

void Figure2D_Rerun::save([[maybe_unused]] const std::string& filename)
{
#ifdef CODAC_WITH_RERUN
  // Do not handle files explicitly meant for other backends (such as .xml or .obj)
  if(filename.size() >= 4 && (filename.compare(filename.size() - 4, 4, ".xml") == 0 ||
                              filename.compare(filename.size() - 4, 4, ".obj") == 0))
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

void Figure2D_Rerun::draw_point([[maybe_unused]] const Vector& c, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= c.size());
  auto entity = make_entity_path(_fig.name(), style.layer, "point", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::Points2D({{ static_cast<float>(c[i()]), static_cast<float>(c[j()]) }})
                    .with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_box([[maybe_unused]] const IntervalVector& x, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= x.size());
  float min_x = static_cast<float>(graphic_trunc(x[i()].lb()));
  float max_x = static_cast<float>(graphic_trunc(x[i()].ub()));
  float min_y = static_cast<float>(graphic_trunc(x[j()].lb()));
  float max_y = static_cast<float>(graphic_trunc(x[j()].ub()));
  float cx = (min_x + max_x) * 0.5f;
  float cy = (min_y + max_y) * 0.5f;
  float w = max_x - min_x;
  float h = max_y - min_y;

  auto entity = make_entity_path(_fig.name(), style.layer, "box", ++_item_count);
  Color c = (style.fill_color != codac2::Color::none()) ? style.fill_color : ((style.stroke_color != codac2::Color::none()) ? style.stroke_color : Color::black());
  _rec.log(entity, rerun::Boxes2D::from_centers_and_sizes({{cx, cy}}, {{w, h}})
                    .with_colors({to_rerun_color(c)}));
#endif
}

void Figure2D_Rerun::draw_circle([[maybe_unused]] const Vector& c, [[maybe_unused]] double r, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= c.size());
  const int N = 48;
  std::vector<rerun::Position2D> pts(N + 1);
  for(int k = 0; k <= N; ++k)
  {
    double th = 2.0 * PI * k / N;
    pts[k] = rerun::Position2D(static_cast<float>(c[i()] + r * std::cos(th)),
                               static_cast<float>(c[j()] + r * std::sin(th)));
  }
  auto entity = make_entity_path(_fig.name(), style.layer, "circle", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{rerun::components::LineStrip2D(pts)}).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_ring([[maybe_unused]] const Vector& c, [[maybe_unused]] const Interval& r, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= c.size());
  const int N = 48;
  std::vector<rerun::Position2D> inner_pts(N + 1);
  std::vector<rerun::Position2D> outer_pts(N + 1);
  for(int k = 0; k <= N; ++k)
  {
    double th = 2.0 * PI * k / N;
    inner_pts[k] = rerun::Position2D(static_cast<float>(c[i()] + r.lb() * std::cos(th)),
                                     static_cast<float>(c[j()] + r.lb() * std::sin(th)));
    outer_pts[k] = rerun::Position2D(static_cast<float>(c[i()] + r.ub() * std::cos(th)),
                                     static_cast<float>(c[j()] + r.ub() * std::sin(th)));
  }
  auto entity = make_entity_path(_fig.name(), style.layer, "ring", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{
    rerun::components::LineStrip2D(inner_pts),
    rerun::components::LineStrip2D(outer_pts)
  }).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_polyline([[maybe_unused]] const std::vector<Vector>& x, [[maybe_unused]] float tip_length, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  if(x.empty()) return;
  std::vector<rerun::Position2D> pts(x.size());
  for(size_t k = 0; k < x.size(); ++k)
  {
    pts[k] = rerun::Position2D(static_cast<float>(graphic_trunc(x[k][i()])),
                               static_cast<float>(graphic_trunc(x[k][j()])));
  }
  auto entity = make_entity_path(_fig.name(), style.layer, "polyline", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{rerun::components::LineStrip2D(pts)}).with_colors({to_rerun_color(col)}));

  if(tip_length > 0.0f && pts.size() >= 2)
  {
    auto p_end = pts.back();
    auto p_prev = pts[pts.size() - 2];
    float dx = p_end.x() - p_prev.x();
    float dy = p_end.y() - p_prev.y();
    float norm = std::hypot(dx, dy);
    if(norm > 1e-6f)
    {
      float arrow_dx = (dx / norm) * tip_length;
      float arrow_dy = (dy / norm) * tip_length;
      auto arrow_entity = entity + "/tip";
      _rec.log(arrow_entity, rerun::Arrows2D::from_vectors({{arrow_dx, arrow_dy}})
                              .with_origins({{p_end.x() - arrow_dx, p_end.y() - arrow_dy}})
                              .with_colors({to_rerun_color(col)}));
    }
  }
#endif
}

void Figure2D_Rerun::draw_polygon([[maybe_unused]] const std::vector<Vector>& x, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  if(x.empty()) return;
  if(x.size() == 1)
  {
    draw_point(x[0], style);
    return;
  }
  std::vector<rerun::Position2D> pts(x.size() + 1);
  for(size_t k = 0; k < x.size(); ++k)
  {
    pts[k] = rerun::Position2D(static_cast<float>(graphic_trunc(x[k][i()])),
                               static_cast<float>(graphic_trunc(x[k][j()])));
  }
  pts.back() = pts.front(); // close polygon

  auto entity = make_entity_path(_fig.name(), style.layer, "polygon", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{rerun::components::LineStrip2D(pts)}).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_pie([[maybe_unused]] const Vector& c, [[maybe_unused]] const Interval& r, [[maybe_unused]] const Interval& theta, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= c.size());
  const int N = 32;
  double dtheta = theta.diam() / N;
  std::vector<rerun::Position2D> pts;
  pts.reserve(2 * N + 3);

  // Outer arc
  for(int k = 0; k <= N; ++k)
  {
    double th = theta.lb() + k * dtheta;
    pts.push_back(rerun::Position2D(static_cast<float>(c[i()] + r.ub() * std::cos(th)),
                                    static_cast<float>(c[j()] + r.ub() * std::sin(th))));
  }
  // Inner arc reverse
  for(int k = N; k >= 0; --k)
  {
    double th = theta.lb() + k * dtheta;
    pts.push_back(rerun::Position2D(static_cast<float>(c[i()] + r.lb() * std::cos(th)),
                                    static_cast<float>(c[j()] + r.lb() * std::sin(th))));
  }
  pts.push_back(pts.front());

  auto entity = make_entity_path(_fig.name(), style.layer, "pie", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{rerun::components::LineStrip2D(pts)}).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_ellipse([[maybe_unused]] const Vector& c, [[maybe_unused]] const Vector& ab, [[maybe_unused]] double theta, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(c.size() == 2 && ab.size() == 2);
  const int N = 48;
  std::vector<rerun::Position2D> pts(N + 1);
  double cos_th = std::cos(theta);
  double sin_th = std::sin(theta);
  for(int k = 0; k <= N; ++k)
  {
    double t = 2.0 * PI * k / N;
    double ex = ab[0] * std::cos(t);
    double ey = ab[1] * std::sin(t);
    double rx = c[0] + ex * cos_th - ey * sin_th;
    double ry = c[1] + ex * sin_th + ey * cos_th;
    pts[k] = rerun::Position2D(static_cast<float>(rx), static_cast<float>(ry));
  }
  auto entity = make_entity_path(_fig.name(), style.layer, "ellipse", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{rerun::components::LineStrip2D(pts)}).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_tank([[maybe_unused]] const Vector& x, [[maybe_unused]] float size, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= x.size() + 1);
  assert(j() + 1 < x.size());
  assert(size >= 0.0f);

  double heading = x[j() + 1];
  double cos_th = std::cos(heading);
  double sin_th = std::sin(heading);
  double scale = size / 4.0;
  double cx = x[i()];
  double cy = x[j()];

  // Tank contour vertices from Codac/VIBes/IPE definition
  const std::vector<std::pair<double, double>> raw_shape = {
    {1, -1.5}, {-1, -1.5}, {0, -1.5}, {0, -1}, {-1, -1}, {-1, 1},
    {0, 1}, {0, 1.5}, {-1, 1.5}, {1, 1.5}, {0, 1.5}, {0, 1},
    {3, 0.5}, {3, -0.5}, {0, -1}, {0, -1.5}
  };

  std::vector<rerun::Position2D> pts(raw_shape.size() + 1);
  for(size_t k = 0; k < raw_shape.size(); ++k)
    pts[k] = transform_2d(raw_shape[k].first, raw_shape[k].second, cx, cy, cos_th, sin_th, scale);
  pts.back() = pts.front();

  auto entity = make_entity_path(_fig.name(), style.layer, "tank", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{rerun::components::LineStrip2D(pts)}).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_AUV([[maybe_unused]] const Vector& x, [[maybe_unused]] float size, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= x.size() + 1);
  assert(j() + 1 < x.size());
  assert(size >= 0.0f);

  double heading = x[j() + 1];
  double cos_th = std::cos(heading);
  double sin_th = std::sin(heading);
  double scale = size / 7.0;
  double cx = x[i()];
  double cy = x[j()];

  const std::vector<std::pair<double, double>> body = {
    {-4, 0}, {-2, 1}, {2, 1}, {2.17365, 0.984808}, {2.34202, 0.939693},
    {2.5, 0.866025}, {2.64279, 0.766044}, {2.76604, 0.642788}, {2.86603, 0.5},
    {2.93969, 0.34202}, {2.98481, 0.173648}, {3, 0}, {2.98481, -0.173648},
    {2.93969, -0.34202}, {2.86603, -0.5}, {2.76604, -0.642788}, {2.64279, -0.766044},
    {2.5, -0.866025}, {2.34202, -0.939693}, {2.17365, -0.984808}, {2, -1}, {-2, -1}, {-4, 0}
  };

  const std::vector<std::pair<double, double>> prop = {
    {-4, 1}, {-3.25, 1}, {-3.25, -1}, {-4, -1}, {-4, 1}
  };

  std::vector<rerun::Position2D> body_pts(body.size());
  for(size_t k = 0; k < body.size(); ++k)
    body_pts[k] = transform_2d(body[k].first, body[k].second, cx, cy, cos_th, sin_th, scale);

  std::vector<rerun::Position2D> prop_pts(prop.size());
  for(size_t k = 0; k < prop.size(); ++k)
    prop_pts[k] = transform_2d(prop[k].first, prop[k].second, cx, cy, cos_th, sin_th, scale);

  auto entity = make_entity_path(_fig.name(), style.layer, "AUV", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{
    rerun::components::LineStrip2D(body_pts),
    rerun::components::LineStrip2D(prop_pts)
  }).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_motor_boat([[maybe_unused]] const Vector& x, [[maybe_unused]] float size, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= x.size() + 1);
  assert(j() + 1 < x.size());
  assert(size >= 0.0f);

  double heading = x[j() + 1];
  double cos_th = std::cos(heading);
  double sin_th = std::sin(heading);
  double scale = size / 408.0;
  double cx = x[i()];
  double cy = x[j()];

  const std::vector<std::pair<double, double>> hull = {
    {-72, -80}, {-72, 80}, {120, 80}, {184, 80}, {264, 64}, {312, 32},
    {328, 0}, {312, -32}, {264, -64}, {184, -80}, {120, -80}, {-72, -80}
  };

  const std::vector<std::pair<double, double>> engine = {
    {-24, 32}, {-24, -32}, {40, -32}, {40, 32}, {-24, 32}
  };

  const std::vector<std::pair<double, double>> left_prop = {
    {-72, 48}, {-72, 16}, {-80, 16}, {-80, 48}, {-72, 48}
  };

  const std::vector<std::pair<double, double>> right_prop = {
    {-72, -16}, {-72, -48}, {-80, -48}, {-80, -16}, {-72, -16}
  };

  auto transform_list = [&](const std::vector<std::pair<double, double>>& l) {
    std::vector<rerun::Position2D> res(l.size());
    for(size_t k = 0; k < l.size(); ++k)
      res[k] = transform_2d(l[k].first, l[k].second, cx, cy, cos_th, sin_th, scale);
    return res;
  };

  auto entity = make_entity_path(_fig.name(), style.layer, "motor_boat", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::LineStrips2D(std::vector<rerun::components::LineStrip2D>{
    rerun::components::LineStrip2D(transform_list(hull)),
    rerun::components::LineStrip2D(transform_list(engine)),
    rerun::components::LineStrip2D(transform_list(left_prop)),
    rerun::components::LineStrip2D(transform_list(right_prop))
  }).with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_text([[maybe_unused]] const std::string& text, [[maybe_unused]] const Vector& ul, [[maybe_unused]] double scale, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  assert(_fig.size() <= ul.size());
  auto entity = make_entity_path(_fig.name(), style.layer, "text", ++_item_count);
  Color col = (style.stroke_color != Color::none()) ? style.stroke_color : ((style.fill_color != Color::none()) ? style.fill_color : Color::black());
  _rec.log(entity, rerun::Points2D({{ static_cast<float>(ul[i()]), static_cast<float>(ul[j()]) }})
                    .with_labels({text})
                    .with_colors({to_rerun_color(col)}));
#endif
}

void Figure2D_Rerun::draw_raster([[maybe_unused]] const std::string& filename, [[maybe_unused]] const IntervalVector& bbox, [[maybe_unused]] const StyleProperties& style)
{
#ifdef CODAC_WITH_RERUN
  draw_box(bbox, style);
#endif
}
