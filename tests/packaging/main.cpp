/* Consumer of an *installed* Codac, used by check_consumer_parity.sh to build
 * the same program twice -- once through find_package(CODAC), once through
 * pkg-config -- and check that the two describe the same library.
 *
 * It deliberately reaches across the module boundary: every call below is made
 * from codac-graphics into codac-core. That is the edge a consumer's link line
 * has to get right, and the one that used to be wrong -- CODAC_LIBRARIES named
 * codac-core before codac-graphics, and GNU ld, which resolves a static archive
 * against what is undefined when it reads it and never looks back, left every
 * such reference unresolved. Keeping the calls here means a regression shows up
 * as a failed link rather than as nothing at all.
 */

#include <codac>
#include <iostream>

using namespace std;
using namespace codac2;

int main()
{
  // Named without a space, and the check runs this from its build directory:
  // a Figure2D writes its output file into the current one, under the figure's
  // own name, and neither belongs in whatever directory the check was started
  // from.
  Figure2D fig("codac_packaging_check", GraphicOutput::IPE);
  fig.set_axes(axis(0, {-3, 3}), axis(1, {-3, 3}));

  fig.draw_box(IntervalVector({{-1, 1}, {-2, 2}}), Color::blue());

  Polygon p({{-1, -1}, {1, -1}, {0, 1}});          // Polygon::vertices()
  fig.draw_polygon(p, Color::red());

  Zonotope z(Vector({0, 0}), Matrix({{1, 0.5}, {0, 1}}));   // Zonotope::is_empty()
  fig.draw_zonotope(z, Color::green());

  Ellipsoid e(Vector({0, 0}), Matrix({{0.8, 0.0}, {0.0, 0.4}}));  // Ellipsoid::proj_2d()
  fig.draw_ellipsoid(e, Color::purple());

  auto tdom = create_tdomain(Interval(0, 5), 0.5); // TDomain::t0_tf(), Segment
  SlicedTube<Interval> tube(tdom, Interval(-1, 1));
  fig.plot_tube(tube, Color::dark_gray());

  cout << "polygon vertices: " << p.vertices().size() << endl;
  cout << "ellipsoid size:   " << e.size() << endl;
  cout << "tube tdomain:     " << tube.tdomain()->t0_tf() << endl;
  cout << "codac-packaging-check-ok" << endl;
  return 0;
}
