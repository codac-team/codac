/** 
 *  Codac tests
 *  
 *  Most of these tests come from the IBEX library (Gilles Chabert)
 *  See more: https://ibex-lib.readthedocs.io
 *  They have been revised to fit with Codac (v2)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

void CHECK_diff(const IntervalVector& x, const IntervalVector& y, bool compactness, const IntervalMatrix& result)
{
  auto c = x.diff(y, compactness);

  if(c.empty())
  {
    CHECK(result.is_empty());
    return;
  }

  for(const auto& ci : c)
  {
    bool found = false;
    for(Index i = 0 ; i < result.rows() ; i++)
      if(ci == IntervalMatrix(result.row(i)).transpose().col(0))
      {
        found = true;
        break;
      }
    CHECK(found);
  }
}

TEST_CASE("IntervalVector")
{
  {
    IntervalVector x(2);
    x[0] = Interval(-oo,oo);
    x[1] = Interval(-oo,oo);
    CHECK(x == IntervalVector(2));
    CHECK(x == IntervalVector(x));
    CHECK(x == (IntervalVector(2)=x));
  }

  {
    IntervalVector x(2);
    x[0] = Interval(0,1);
    x[1] = Interval(0,1);
    CHECK(x == IntervalVector({{0,1},{0,1}}));
    CHECK(x == IntervalVector(x));
    CHECK(x == (IntervalVector(2)=x));
  }

  {
    IntervalVector x(2);
    x[0] = Interval(0,1);
    x[1] = Interval(2,3);
    CHECK(x == IntervalVector(x));
    CHECK(x == (IntervalVector(2)=x));
  }

  {
    IntervalVector x(2);
    x[0] = Interval(0,1);
    x[1] = Interval(2,3);
    CHECK(x == IntervalVector({{0,1},{2,3}}));
    CHECK(x == (IntervalVector(2)=x));
  }

  {
    IntervalVector x(2);
    x[0].set_empty();
    x[1].set_empty();
    CHECK(x == IntervalVector::empty(2));
    CHECK(x.is_empty());
    CHECK((IntervalVector(2)=x).is_empty());
  }

  {
    IntervalVector x{
      {1.0, 2.0},
      {2.0, 3.0},
      {4}
    };
    CHECK(x.size() == 3);
    CHECK(x[0] == Interval(1.0, 2.0));
    CHECK(x[1] == Interval(2.0, 3.0));
    CHECK(x[2] == Interval(4.0, 4.0));
  }

  {
    IntervalVector x(2);
    CHECK(!x.is_empty());
    x.set_empty();
    CHECK(x.is_empty());
    CHECK(IntervalVector::empty(2).is_empty());
    CHECK(!IntervalVector(2).is_empty());
  }

  {
    IntervalVector x(1);
    x[0] = Interval(1,2);
    x.resize_save_values(3);
    CHECK(x.size() == 3);
    CHECK(x[0] == Interval(1,2));
    CHECK(x[1] == Interval(-oo,oo));
    CHECK(x[2] == Interval(-oo,oo));
  }

  {
    IntervalVector x(1);
    x[0] = Interval(1,2);
    x.resize_save_values(1);
    CHECK(x.size() == 1);
    CHECK(x[0] == Interval(1,2));
  }

  {
    IntervalVector x(2);
    x[0] = Interval(1,2);
    x.set_empty();
    x.resize_save_values(3);
    CHECK(x.size() == 3);
    CHECK(x.is_empty());
    CHECK(x[2] == Interval(-oo,oo));
  }

  {
    IntervalVector x(5);
    x[0] = Interval(1,2);
    x[1] = Interval(3,4);
    x.resize_save_values(2);
    CHECK(x.size() == 2);
    CHECK(x[0] == Interval(1,2));
    CHECK(x[1] == Interval(3,4));
  }

  {
    Interval xi(-5,6);
    IntervalVector x({xi});
    CHECK(x.size() == 1);
    CHECK(x[0] == Interval(-5,6));
  }

  {
    IntervalVector x({Interval(4.0397,5.40),Interval(1.9089,2.45)});
    CHECK(x[0] == Interval(4.0397,5.40));
    CHECK(x[1] == Interval(1.9089,2.45));
  }

  {
    IntervalVector x({{4.0397,5.40},{1.9089,2.45}});
    CHECK(x[0] == Interval(4.0397,5.40));
    CHECK(x[1] == Interval(1.9089,2.45));
  }

  CHECK(IntervalVector({{0,1},{2,3},{4,5}}).subvector(0,1) == IntervalVector({{0,1},{2,3}}));
  CHECK(IntervalVector({{0,1},{2,3},{4,5}}).subvector(1,2) == IntervalVector({{2,3},{4,5}}));
  CHECK(IntervalVector({{0,1},{2,3},{4,5}}).subvector(1,1) == IntervalVector({{2,3}}));
  CHECK(IntervalVector({{0,1},{2,3},{4,5}}).subvector(2,2) == IntervalVector({{4,5}}));
  CHECK(IntervalVector({{0,1},{2,3},{4,5}}).subvector(0,2) == IntervalVector({{0,1},{2,3},{4,5}}));
  CHECK(IntervalVector::empty(3).subvector(1,2).is_empty());

  CHECK(((IntervalVector({{0,2},{4,6}})) &=IntervalVector({{1,3},{5,7}})) == IntervalVector({{1,2},{5,6}}));
  CHECK(((IntervalVector({{0,2},{4,6}})) & IntervalVector({{1,3},{5,7}})) == IntervalVector({{1,2},{5,6}}));

  CHECK(((IntervalVector({{0,2},{4,6}})) &=IntervalVector({{1,3},{7,8}})).is_empty());
  CHECK(((IntervalVector({{0,2},{4,6}})) & IntervalVector({{1,3},{7,8}})).is_empty());

  CHECK(((IntervalVector({{0,2},{4,6}})) &=IntervalVector::empty(2)).is_empty());
  CHECK(((IntervalVector({{0,2},{4,6}})) & IntervalVector::empty(2)).is_empty());

  CHECK(((IntervalVector::empty(2)) &=IntervalVector({{0,2},{4,6}})).is_empty());
  CHECK(((IntervalVector::empty(2)) & IntervalVector({{0,2},{4,6}})).is_empty());

  CHECK(((IntervalVector({{0,1},{4,5}})) |=IntervalVector({{2,3},{6,7}})) == IntervalVector({{0,3},{4,7}}));
  CHECK(((IntervalVector({{0,1},{4,5}})) | IntervalVector({{2,3},{6,7}})) == IntervalVector({{0,3},{4,7}}));

  IntervalVector x(3), x1(2), x2(2), x3(2);

  x1 = IntervalVector({{0,1},{4,5}});
  CHECK((x1 |= x1) == x1);
  CHECK((x1 | x1) == x1);

  x1 = IntervalVector({{0,2},{4,6}});
  CHECK((x1 |=IntervalVector::empty(2)) == x1);
  CHECK((x1 | IntervalVector::empty(2)) == x1);

  x1 = IntervalVector({{0,2},{4,6}});
  CHECK(((IntervalVector::empty(2)) |= x1) == x1);
  CHECK(((IntervalVector::empty(2)) | x1) == x1);

  x = IntervalVector({{0,1},{2,3},{4,5}});
  CHECK(x == x);
  CHECK(!(x!=x));

  x = IntervalVector({{0,1},{2,3},{4,5}});
  x1 = IntervalVector({{0,1},{2,3}});
  CHECK(!(x == x1));
  CHECK(x!=x1);

  x1 = IntervalVector({{0,1},{4,5}});
  x2 = IntervalVector({{2,3},{6,7}});
  x1.set_empty();
  x2.set_empty();
  CHECK(x1 == x2);
  CHECK(!(x1!=x2));

  CHECK(IntervalVector::empty(2) == IntervalVector::empty(2));
  CHECK(IntervalVector::empty(2)!=IntervalVector::empty(3));
  x = IntervalVector(2);
  x.set_empty();
  CHECK(IntervalVector::empty(2) == x);

  x = IntervalVector({{0,1},{2,3},{4,5}});
  Vector m = x.mid();
  CHECK(m[0] == 0.5);
  CHECK(m[1] == 2.5);
  CHECK(m[2] == 4.5);

  CHECK(!IntervalVector({{0,1},{2,3},{4,5}}).is_flat());
  CHECK(IntervalVector::empty(3).is_flat());
  CHECK(IntervalVector({{0,0}}).is_flat());
  CHECK(!IntervalVector({{0,1}}).is_flat());
  CHECK(IntervalVector({{0,1},{2,2},{3,4}}).is_flat());
  CHECK(IntervalVector({{0,1},{2,3},{4,4}}).is_flat());
  CHECK(!IntervalVector::empty(3).is_unbounded());
  CHECK(IntervalVector({{0,1},{0,2},{-oo,0}}).is_unbounded());
  CHECK(!IntervalVector({{0,1},{0,2}}).is_unbounded());
  CHECK(IntervalVector(1).is_unbounded());

  x1 = IntervalVector({{0,2},{2,4}});
  x2 = IntervalVector({{0,1},{3,4}});

  CHECK(x1.is_superset(x2));
  CHECK(x2.is_subset(x1));
  CHECK(x1.is_strict_superset(x2));
  CHECK(!x2.is_strict_interior_subset(x1));

  x1 = IntervalVector({{0,2},{2,4}});
  x2 = IntervalVector({{1,1},{3,4}});

  CHECK(x1.is_superset(x2));
  CHECK(x2.is_subset(x1));
  CHECK(x1.is_strict_superset(x2));
  CHECK(!x2.is_strict_interior_subset(x1));

  x1 = IntervalVector({{0,2},{2,4}});
  x2 = IntervalVector({{0,1},{3,3}});

  CHECK(x1.is_superset(x2));
  CHECK(x2.is_subset(x1));
  CHECK(x1.is_strict_superset(x2));
  CHECK(!x2.is_strict_interior_subset(x1));

  x1 = IntervalVector({{0,2},{2,4}});
  x2 = IntervalVector({{1,1},{3,3}});

  CHECK(x1.is_superset(x2));
  CHECK(x2.is_subset(x1));
  CHECK(x1.is_strict_superset(x2));
  CHECK(x2.is_strict_interior_subset(x1));

  x1 = IntervalVector({{0,2},{2,4}});
  x2 = IntervalVector(IntervalVector::empty(2));

  CHECK(x1.is_superset(x2));
  CHECK(x2.is_subset(x1));
  CHECK(x1.is_strict_superset(x2));
  CHECK(x2.is_strict_interior_subset(x1));

  x1 = IntervalVector(IntervalVector::empty(2));
  x2 = IntervalVector({{1,1},{3,3}});

  CHECK(!x1.is_superset(x2));
  CHECK(!x2.is_subset(x1));
  CHECK(!x1.is_strict_superset(x2));
  CHECK(!x2.is_strict_interior_subset(x1));

  x1 = IntervalVector({{0,2},{2,4}});
  x2 = IntervalVector({{1,1},{3,5}});

  CHECK(!x1.is_superset(x2));
  CHECK(!x2.is_subset(x1));
  CHECK(!x1.is_strict_superset(x2));
  CHECK(!x2.is_strict_interior_subset(x1));


  x1 = IntervalVector({{0,2},{0,1},{0,3}});
  CHECK(x1.extr_diam_index(true) == 1);
  CHECK(x1.extr_diam_index(false) == 2);
  CHECK(x1.min_diam() == 1);
  CHECK(x1.max_diam() == 3);

  x1 = IntervalVector({{0,1},{0,3},{0,2}});
  CHECK(x1.extr_diam_index(true) == 0);
  CHECK(x1.extr_diam_index(false) == 1);
  CHECK(x1.min_diam() == 1);
  CHECK(x1.max_diam() == 3);

  x1 = IntervalVector({{0,3},{0,2},{0,1}});
  CHECK(x1.extr_diam_index(true) == 2);
  CHECK(x1.extr_diam_index(false) == 0);
  CHECK(x1.min_diam() == 1);
  CHECK(x1.max_diam() == 3);

  x1 = IntervalVector({{0,1},{0,2},{-oo,0}});
  CHECK(x1.extr_diam_index(true) == 0);
  CHECK(x1.extr_diam_index(false) == 2);
  CHECK(x1.min_diam() == 1);
  CHECK(x1.max_diam() == oo);

  x1 = IntervalVector({{-oo,0}});
  CHECK(x1.extr_diam_index(true) == 0);
  CHECK(x1.extr_diam_index(false) == 0);
  CHECK(x1.min_diam() == oo);
  CHECK(x1.max_diam() == oo);

  x1 = IntervalVector({{-oo,0},{0,1},{-oo,1},{1,3}});
  CHECK(x1.extr_diam_index(true) == 1);
  CHECK(x1.extr_diam_index(false) == 2);
  CHECK(x1.min_diam() == 1);
  CHECK(x1.max_diam() == oo);

  x1 = IntervalVector({{-oo,0},{-2,oo},{-oo,1}});
  CHECK(x1.extr_diam_index(true) == 0);
  CHECK(x1.extr_diam_index(false) == 1);
  CHECK(x1.min_diam() == oo);
  CHECK(x1.max_diam() == oo);

  x1 = IntervalVector({{-oo,0},{-oo,1},{-2,oo}});
  CHECK(x1.extr_diam_index(true) == 0);
  CHECK(x1.extr_diam_index(false) == 2);
  CHECK(x1.min_diam() == oo);
  CHECK(x1.max_diam() == oo);

  x1 = IntervalVector({{-2,oo},{-oo,0},{-oo,1}});
  CHECK(x1.extr_diam_index(true) == 1);
  CHECK(x1.extr_diam_index(false) == 0);
  CHECK(x1.min_diam() == oo);
  CHECK(x1.max_diam() == oo);

  x1 = IntervalVector({{-2,oo},{-oo,1},{-oo,0}});
  CHECK(x1.extr_diam_index(true) == 2);
  CHECK(x1.extr_diam_index(false) == 0);
  CHECK(x1.min_diam() == oo);
  CHECK(x1.max_diam() == oo);

  CHECK(IntervalVector({{0,1},{0,oo}}).volume() == oo);
  CHECK(IntervalVector({{0,1},{1,1}}).volume() == 0);
  CHECK(Approx(Interval(24.0),1e-10) == IntervalVector({{0,2},{2,5},{4,8}}).volume());
  CHECK((-IntervalVector({{0,3},{0,2},{0,1}})) == IntervalVector({{-3,0},{-2,0},{-1,0}}));

  CHECK(-IntervalVector({{0,1},{0,oo}}) == IntervalVector({{-1,0},{-oo,0}}));
  CHECK(-IntervalVector::empty(2) == IntervalVector::empty(2));


  x1 = IntervalVector({{0,3},{0,2},{0,1}});
  x2 = IntervalVector({{0,1},{0,1},{0,1}});
  x3 = IntervalVector({{0,4},{0,3},{0,2}});
  IntervalVector e(IntervalVector::empty(3));

  CHECK(x1+x2 == x3);
  CHECK(x1+e == e);
  CHECK((x1+e).is_empty());
  CHECK((IntervalVector(x1)+=e) == e);
  CHECK((IntervalVector(x1)+=e).is_empty());
  CHECK(e+x1 == e);
  CHECK((e+x1).is_empty());
  CHECK((e+=x1) == e);
  CHECK((e+=x1).is_empty());
  CHECK(e+e == e);
  CHECK((e+e).is_empty());
  CHECK((e+=e) == e);
  CHECK((e+=e).is_empty());
  CHECK((IntervalVector(x1)+=x2) == x3);
  CHECK((IntervalVector(x1)+=e) == e);
  CHECK((IntervalVector(x1)+=e).is_empty());
  CHECK((IntervalVector(x2)+=x1) == x3);

  x1 = IntervalVector({{0,3},{0,2},{0,1}});
  x2 = IntervalVector({{0,1},{0,1},{0,1}});
  x3 = IntervalVector({{-1,3},{-1,2},{-1,1}});
  e = IntervalVector::empty(3);

  CHECK(x1-x2 == x3);
  CHECK(x2-x1 == -x3);
  CHECK(x1-e == e);
  CHECK((x1-e).is_empty());
  CHECK((IntervalVector(x1)-=e) == e);
  CHECK((IntervalVector(x1)-=e).is_empty());
  CHECK(e-x1 == e);
  CHECK((e-x1).is_empty());
  CHECK((e-=x1) == e);
  CHECK((e-=x1).is_empty());
  CHECK(e-e == e);
  CHECK((e-e).is_empty());
  CHECK((e-=e) == e);
  CHECK((e-=e).is_empty());
  CHECK((IntervalVector(x1)-=x2) == x3);
  CHECK((IntervalVector(x2)-=x1) == -x3);

  IntervalVector b({{0,1},{0,1}});
  auto c_l = b.complementary();
  std::vector<IntervalVector> c { std::begin(c_l), std::end(c_l) };

  CHECK(c.size() == 4);
  CHECK(c[0].size() == 2);

  CHECK(c[0][0] == Interval(-oo,0));
  CHECK(c[0][1] == Interval(-oo,oo));

  CHECK(c[1][0] == Interval(1,oo));
  CHECK(c[1][1] == Interval(-oo,oo));

  CHECK(c[2][0] == Interval(0,1));
  CHECK(c[2][1] == Interval(-oo,0));

  CHECK(c[3][0] == Interval(0,1));
  CHECK(c[3][1] == Interval(1,oo));

  c_l = IntervalVector::empty(2).complementary();
  c = std::vector<IntervalVector> { std::begin(c_l), std::end(c_l) };

  CHECK(c.size() == 1);
  CHECK(c[0].size() == 2);
  CHECK(c[0][0] == Interval(-oo,oo));
  CHECK(c[0][1] == Interval(-oo,oo));

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, IntervalVector::empty(3), true,
    {{{-2,2},{-2,2},{-2,2}}}
  );

  CHECK(IntervalVector::empty(3) == IntervalVector({Interval::empty(),Interval::empty(),Interval::empty()}));

  CHECK_diff(IntervalVector::empty(3), {{-2,2},{-2,2},{-2,2}}, true,
    {{Interval::empty(),Interval::empty(),Interval::empty()}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{3,4},{-1,1}}, true,
    {{{-2,2},{-2,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,-1},{-1,1}}, true,
    {{{-2,2},{-2,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-1,1}}, true,
    {{{-2,-1},{-2,2},{-2,2}},
     {{1,2},{-2,2},{-2,2}},
     {{-1,1},{-2,-1},{-2,2}},
     {{-1,1},{1,2},{-2,2}},
     {{-1,1},{-1,1},{-2,-1}},
     {{-1,1},{-1,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-2,1}}, true,
    {{{-2,-1},{-2,2},{-2,2}},
     {{1,2},{-2,2},{-2,2}},
     {{-1,1},{-2,-1},{-2,2}},
     {{-1,1},{1,2},{-2,2}},
     {{-1,1},{-1,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-2,2}}, true,
    {{{-2,-1},{-2,2},{-2,2}},
     {{1,2},{-2,2},{-2,2}},
     {{-1,1},{-2,-1},{-2,2}},
     {{-1,1},{1,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,1},{-1,1}}, true,
    {{{-2,-1},{-2,2},{-2,2}},
     {{1,2},{-2,2},{-2,2}},
     {{-1,1},{1,2},{-2,2}},
     {{-1,1},{-2,1},{-2,-1}},
     {{-1,1},{-2,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,2},{-1,1}}, true,
    {{{-2,-1},{-2,2},{-2,2}},
     {{1,2},{-2,2},{-2,2}},
     {{-1,1},{-2,2},{-2,-1}},
     {{-1,1},{-2,2},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-1,1},{-1,1}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{-2,-1},{-2,2}},
     {{-2,1},{1,2},{-2,2}},
     {{-2,1},{-1,1},{-2,-1}},
     {{-2,1},{-1,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-1,2},{-1,1}}, true,
    {{{-2,2},{-2,-1},{-2,2}},
     {{-2,2},{-1,2},{-2,-1}},
     {{-2,2},{-1,2},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,1},{-2,1}}, true,
    {{{-2,-1},{-2,2},{-2,2}},
     {{1,2},{-2,2},{-2,2}},
     {{-1,1},{1,2},{-2,2}},
     {{-1,1},{-2,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,1},{-2,2}}, true,
    {{{-2,-1},{-2,2},{-2,2}},
     {{1,2},{-2,2},{-2,2}},
     {{-1,1},{1,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-1,1},{-2,1}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{-2,-1},{-2,2}},
     {{-2,1},{1,2},{-2,2}},
     {{-2,1},{-1,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-1,1},{-2,2}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{-2,-1},{-2,2}},
     {{-2,1},{1,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,1},{-1,1}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{1,2},{-2,2}},
     {{-2,1},{-2,1},{-2,-1}},
     {{-2,1},{-2,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,2},{-1,1}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{-2,2},{-2,-1}},
     {{-2,1},{-2,2},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,1},{-2,1}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{1,2},{-2,2}},
     {{-2,1},{-2,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,1},{-2,2}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{1,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,2},{-2,1}}, true,
    {{{1,2},{-2,2},{-2,2}},
     {{-2,1},{-2,2},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,1},{-2,1}}, true,
    {{{-2,2},{1,2},{-2,2}},
     {{-2,2},{-2,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,2},{-2,2}}, true,
    {{{1,2},{-2,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,2},{-2,1}}, true,
    {{{-2,2},{-2,2},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,1},{-2,2}}, true,
    {{{-2,2},{1,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,2},{-2,2}}, true,
    {{Interval::empty(),Interval::empty(),Interval::empty()}}
  );

  CHECK_diff({{0,0},{-2,2},{-2,2}}, {{0,0},{-1,1},{-1,1}}, true,
    {{{0,0},{-2,-1},{-2,2}},
     {{0,0},{1,2},{-2,2}},
     {{0,0},{-1,1},{-2,-1}},
     {{0,0},{-1,1},{1,2}}}
  );

  CHECK_diff({{0,0},{0,0},{-2,2}}, {{0,0},{0,0},{-1,1}}, true,
    {{{0,0},{0,0},{-2,-1}},
     {{0,0},{0,0},{1,2}}}
  );

  CHECK_diff({{0,0},{-2,2},{0,0}}, {{0,0},{-1,1},{0,0}}, true,
    {{{0,0},{-2,-1},{0,0}},
     {{0,0},{1,2},{0,0}}}
  );

  CHECK_diff({{-2,2},{0,0},{0,0}}, {{-1,1},{0,0},{0,0}}, true,
    {{{-2,-1},{0,0},{0,0}},
     {{1,2},{0,0},{0,0}}}
  );

  CHECK_diff({{0,0},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-1,1}}, true,
    {{{0,0},{-2,-1},{-2,2}},
     {{0,0},{1,2},{-2,2}},
     {{0,0},{-1,1},{-2,-1}},
     {{0,0},{-1,1},{1,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{2,4},{-2,2},{-2,2}}, true,
    {{{-2,2},{-2,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2},{-2,2}}, {{2,4},{-1,1},{-1,1}}, true,
    {{{-2,2},{-2,2},{-2,2}}}
  );

  CHECK_diff({{-2,2},{-2,2}}, {{-2,2},{1,1}}, false,
    {{{-2,2},{-2,1}},
     {{-2,2},{1,2}}}
  );

  CHECK_diff({{-2,2},{1,1}}, {{0,2},{-2,2}}, false,
    {{{-2,0},{1,1}}}
  );

  CHECK_diff({{-1,-1},{-1,1},{-1,1}}, {{0,2},{0,2},{0,2}}, true,
    {{{-1,-1},{-1,1},{-1,1}}}
  );

  {
    IntervalVector a{Interval::empty(),{-1,1}};
    IntervalVector b{{-1,1},Interval::empty()};
    IntervalVector c = a | b;
    CHECK(c.is_empty());
  }
}

#if 0
// Tests from the IBEX lib that are not considered in this file:

  x1 = IntervalVector({{1,1},{3,4}});
  x2 = IntervalVector({{1,1},{2,5}});
  CHECK(x1.is_relative_interior_subset(x2));
  x1 = IntervalVector({{3,4},{3,3}});
  x2 = IntervalVector({{2,5},{3,3}});
  CHECK(x1.is_relative_interior_subset(x2));
  x1 = IntervalVector({{0,0},{1,1}});
  x2 = IntervalVector({{1,1},{1,1}});
  CHECK(!x1.is_relative_interior_subset(x2));
  x1 = IntervalVector({{0,0},{1,1}});
  x2 = IntervalVector({{0,0},{1,1}});
  CHECK(x1.is_relative_interior_subset(x2));
  CHECK(IntervalVector::empty(2).is_relative_interior_subset(IntervalVector(2)));

  x = IntervalVector({{0,2},{-oo,0},{0,1},{3,3},{-10,10}});
  int tab[5];
  x.sort_indices(true,tab);
  CHECK(tab[0] == 3);
  CHECK(tab[1] == 2);
  CHECK(tab[2] == 0);
  CHECK(tab[3] == 4);
  CHECK(tab[4] == 1);

  x = IntervalVector({{0,2},{-oo,0},{0,1},{3,3},{-10,10}});
  int tab[5];
  x.sort_indices(false,tab);
  CHECK(tab[0] == 1);
  CHECK(tab[1] == 4);
  CHECK(tab[2] == 0);
  CHECK(tab[3] == 2);
  CHECK(tab[4] == 3);

  IntervalVector box1(3);
  IntervalVector box2(3);
  box1[0] = Interval(0,0);
  box2[0] = Interval(0,0);
  box1[1] = Interval(-1,0);
  box2[1] = Interval(-1,0);
  box1[2] = Interval(1,4);
  box2[2] = Interval(1.5,3);
  CHECK_DOUBLES_EQUAL(1.0/3.0,box1.rel_distance(box2),ERROR);

  IntervalVector box1(3);
  IntervalVector box2(3);
  box1[0] = Interval(0,0);
  box2[0] = Interval(0,0);
  box1[1] = Interval(-1,0);
  box2[1] = Interval(-1,0);
  box1[2] = Interval(1,4);
  box2[2] = Interval(1.5,3);
  CHECK_DOUBLES_EQUAL(4.0,box1.perimeter(),ERROR);
  CHECK_DOUBLES_EQUAL(2.5,box2.perimeter(),ERROR);

  CHECK(IntervalVector({{0,1},{0,oo}}).perimeter() == oo);

  {
    IntervalVector b({{0,1},{0,1}});
    IntervalVector r = b.random();

    CHECK(r.size() == 2);
    CHECK(r.max_diam() == 0);
    CHECK(b.is_superset(r));
  }

  {
    IntervalVector b({{1,1},{2,2}});
    IntervalVector r = b.random();

    CHECK(b == r);
  }

#endif