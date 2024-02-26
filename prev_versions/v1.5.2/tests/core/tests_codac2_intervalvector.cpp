#include "catch_interval.hpp"
#include "vibes.h"

#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_cart_prod.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace codac2;

// Most of these tests come from the IBEX library (G. Chabert)
// They have been revised to fit the codac2::IntervalVector class

TEST_CASE("Tests from IBEX IntervalVector")
{
  SECTION("cons01")
  {
    IntervalVector x(2);
    x[0]=Interval::all_reals();
    x[1]=Interval::all_reals();
    CHECK(x==IntervalVector(2));
    CHECK(x==IntervalVector(x));
    IntervalVector y(2);
    y = x;
    CHECK(x==y);
  }

  SECTION("cons02")
  {
    IntervalVector x(2);
    x[0]=Interval(0,1);
    x[1]=Interval(0,1);
    CHECK(x==IntervalVector(2,Interval(0,1)));
    CHECK(x==IntervalVector(x));
    IntervalVector y(2);
    y = x;
    CHECK(x==y);
  }

  SECTION("cons03")
  {
    IntervalVector x(2);
    x[0]=Interval(0,1);
    x[1]=Interval(2,3);
    CHECK(x==IntervalVector(x));
    IntervalVector y(2);
    y = x;
    CHECK(x==y);
  }

  SECTION("cons04")
  {
    double bounds[][2] = {{0,1},{2,3}};
    IntervalVector x(2);
    x[0]=Interval(0,1);
    x[1]=Interval(2,3);
    CHECK(x==IntervalVector(2,bounds));
    IntervalVector y(2,bounds);
    y = x;
    CHECK(x==y);
  }

  SECTION("cons05")
  {
    IntervalVector x(2);
    x[0].set_empty();
    x[1].set_empty();
    CHECK(x==IntervalVector::empty_set(2));
    CHECK(x.is_empty());
  }

  SECTION("consInitList")
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

  SECTION("set_empty01")
  {
    IntervalVector x(2);
    CHECK(!x.is_empty());
    x.set_empty();
    CHECK(x.is_empty());
  }

  SECTION("is_empty01")
  {
    CHECK(IntervalVector::empty_set(2).is_empty());
  }

  SECTION("is_empty02")
  {
    CHECK(!IntervalVector(2).is_empty());
  }

  SECTION("resize01")
  {
    IntervalVector x(1);
    x[0]=Interval(1,2);
    x.resize(3);
    CHECK(x.size()==3);
    CHECK(x[0]==Interval(1,2));
    CHECK(x[1]==Interval::all_reals());
    CHECK(x[2]==Interval::all_reals());
  }

  SECTION("resize02")
  {
    IntervalVector x(1);
    x[0]=Interval(1,2);
    x.resize(1);
    CHECK(x.size()==1);
    CHECK(x[0]==Interval(1,2));
  }

  SECTION("resize03")
  {
    IntervalVector x(2);
    x[0]=Interval(1,2);
    x.set_empty();
    x.resize(3);
    CHECK(x.size()==3);
    CHECK(x.is_empty());
    CHECK(x[2]==Interval::all_reals());
  }

  SECTION("resize04")
  {
    IntervalVector x(5);
    x[0]=Interval(1,2);
    x[1]=Interval(3,4);
    x.resize(2);
    CHECK(x.size()==2);
    CHECK(x[0]==Interval(1,2));
    CHECK(x[1]==Interval(3,4));
  }

  static double _x[][2]={{0,1},{2,3},{4,5}};

  SECTION("subvector01")
  {
    double _x01[][2]={{0,1},{2,3}};
    CHECK(IntervalVector(3,_x).subvector(0,1)==IntervalVector(2,_x01));
  }

  SECTION("subvector02")
  {
    double _x12[][2]={{2,3},{4,5}};
    CHECK(IntervalVector(3,_x).subvector(1,2)==IntervalVector(2,_x12));
  }

  SECTION("subvector03")
  {
    double _x11[][2]={{2,3}};
    CHECK(IntervalVector(3,_x).subvector(1,1)==IntervalVector(1,_x11));
  }

  SECTION("subvector04")
  {
    double _x22[][2]={{4,5}};
    CHECK(IntervalVector(3,_x).subvector(2,2)==IntervalVector(1,_x22));
  }

  SECTION("subvector05")
  {
    CHECK(IntervalVector(3,_x).subvector(0,2)==IntervalVector(3,_x));
  }

  SECTION("subvector06")
  {
    IntervalVector x = IntervalVector::empty_set(3);
    CHECK(x.subvector(1,2).is_empty());
  }

  SECTION("cart_prod01")
  {
    CHECK(codac2::cart_prod(IntervalVector(3,_x),IntervalVector::empty_set(3)).is_empty());
    CHECK(codac2::cart_prod(IntervalVector::empty_set(3),IntervalVector(3,_x)).is_empty());
    CHECK(codac2::cart_prod(Interval(1,2),Interval(2,3),IntervalVector({{5,6},{8,9}}),Interval(5,6)) == IntervalVector({{1,2},{2,3},{5,6},{8,9},{5,6}}));
  }

  SECTION("inter01")
  {
    double _x1[][2]={{0,2},{4,6}};
    double _x2[][2]={{1,3},{5,7}};
    double _res[][2]={{1,2},{5,6}};
    CHECK(((IntervalVector(2,_x1)) &=IntervalVector(2,_x2))==IntervalVector(2,_res));
    CHECK(((IntervalVector(2,_x1)) & IntervalVector(2,_x2))==IntervalVector(2,_res));
  }

  SECTION("staticcartprod01")
  {
    IntervalVector_<2> x{{1,2},{3,5}};
    IntervalVector_<3> y{{1,2},{3,5},{-oo,oo}};
    Interval z{6,7};
    CHECK(cart_prod<6>(x,y,z)==IntervalVector({{1,2},{3,5},{1,2},{3,5},{-oo,oo},{6,7}}));
    CHECK(cart_prod<2>(z,z)==IntervalVector({{6,7},{6,7}}));
  }

  SECTION("inter02")
  {
    double _x1[][2]={{0,2},{4,6}};
    double _x2[][2]={{1,3},{7,8}};
    CHECK(((IntervalVector(2,_x1)) &=IntervalVector(2,_x2)).is_empty());
    CHECK(((IntervalVector(2,_x1)) & IntervalVector(2,_x2)).is_empty());
  }

  SECTION("inter03")
  {
    double _x1[][2]={{0,2},{4,6}};
    CHECK(((IntervalVector(2,_x1)) &=IntervalVector::empty_set(2)).is_empty());
    CHECK(((IntervalVector(2,_x1)) & IntervalVector::empty_set(2)).is_empty());
  }

  SECTION("inter04")
  {
    double _x1[][2]={{0,2},{4,6}};
    CHECK(((IntervalVector::empty_set(2)) &=IntervalVector(2,_x1)).is_empty());
    CHECK(((IntervalVector::empty_set(2)) & IntervalVector(2,_x1)).is_empty());
  }

  SECTION("hull01")
  {
    double _x1[][2]={{0,1},{4,5}};
    double _x2[][2]={{2,3},{6,7}};
    double _res[][2]={{0,3},{4,7}};
    CHECK(((IntervalVector(2,_x1)) |=IntervalVector(2,_x2))==IntervalVector(2,_res));
    CHECK(((IntervalVector(2,_x1)) | IntervalVector(2,_x2))==IntervalVector(2,_res));
  }

  SECTION("hull02")
  {
    double _x1[][2]={{0,1},{4,5}};
    IntervalVector x1(2,_x1);
    CHECK((x1 |= x1)==x1);
    CHECK((x1 | x1)==x1);
  }

  SECTION("hull03")
  {
    double _x1[][2]={{0,2},{4,6}};
    IntervalVector x1(2,_x1);
    CHECK((x1 |=IntervalVector::empty_set(2))==x1);
    CHECK((x1 | IntervalVector::empty_set(2))==x1);
  }

  SECTION("hull04")
  {
    double _x1[][2]={{0,2},{4,6}};
    IntervalVector x1(2,_x1);
    CHECK(((IntervalVector::empty_set(2)) |= x1)==x1);
    CHECK(((IntervalVector::empty_set(2)) | x1)==x1);
  }

  SECTION("eq01")
  {
    IntervalVector x(3,_x);
    CHECK(x==x);
    CHECK(!(x!=x));
  }

  SECTION("eq02")
  {
    IntervalVector x(3,_x);
    double _x01[][2]={{0,1},{2,3}};
    IntervalVector x1(2,_x01);
    CHECK(!(x==x1));
    CHECK(x!=x1);
  }

  SECTION("eq03")
  {
    double _x1[][2]={{0,1},{4,5}};
    double _x2[][2]={{2,3},{6,7}};
    IntervalVector x1(2,_x1);
    IntervalVector x2(2,_x2);
    x1.set_empty();
    x2.set_empty();
    CHECK(x1==x2);
    CHECK(!(x1!=x2));
  }

  SECTION("eq04")
  {
    CHECK(IntervalVector::empty_set(2)==IntervalVector::empty_set(2));
    CHECK(IntervalVector::empty_set(2)!=IntervalVector::empty_set(3));
    IntervalVector x(2);
    x.set_empty();
    CHECK(IntervalVector::empty_set(2)==x);
  }

  SECTION("mid01")
  {
    IntervalVector x(3,_x);
    Vector m=x.mid();
    CHECK(m[0]==0.5);
    CHECK(m[1]==2.5);
    CHECK(m[2]==4.5);
  }

  SECTION("is_flat01")
  {
    CHECK(!IntervalVector(3,_x).is_flat());
  }

  SECTION("is_flat02")
  {
    CHECK(IntervalVector::empty_set(3).is_flat());
  }

  SECTION("is_flat03")
  {
    CHECK(IntervalVector(1,Interval(0,0)).is_flat());
    CHECK(!IntervalVector(1,Interval(0,1)).is_flat());
  }

  SECTION("is_flat04")
  {
    double _x1[][2]={{0,1},{2,2},{3,4}};
    CHECK(IntervalVector(3,_x1).is_flat());
  }

  SECTION("is_flat05")
  {
    double _x1[][2]={{0,1},{2,3},{4,4}};
    CHECK(IntervalVector(3,_x1).is_flat());
  }

  SECTION("is_unbounded01")
  {
    CHECK(!IntervalVector::empty_set(3).is_unbounded());
  }

  SECTION("is_unbounded02")
  {
    double _x1[][2]={{0,1},{0,2},{-oo,0}};
    CHECK(IntervalVector(3,_x1).is_unbounded());
  }

  SECTION("is_unbounded03")
  {
    double _x1[][2]={{0,1},{0,2}};
    CHECK(!IntervalVector(2,_x1).is_unbounded());
  }

  SECTION("is_unbounded04")
  {
    CHECK(IntervalVector(1).is_unbounded());
  }

  SECTION("is_subset01")
  {
    double _x1[][2]={{0,2},{2,4}};
    double _x2[][2]={{0,1},{3,4}};
    IntervalVector x1(2,_x1);
    IntervalVector x2(2,_x2);

    CHECK(x1.is_superset(x2));
    CHECK(x2.is_subset(x1));
    CHECK(x1.is_strict_superset(x2));
    //CHECK(!x2.is_strict_interior_subset(x1));
  }

  SECTION("is_subset02")
  {
    double _x1[][2]={{0,2},{2,4}};
    double _x2[][2]={{1,1},{3,4}};
    IntervalVector x1(2,_x1);
    IntervalVector x2(2,_x2);

    CHECK(x1.is_superset(x2));
    CHECK(x2.is_subset(x1));
    CHECK(x1.is_strict_superset(x2));
    //CHECK(!x2.is_strict_interior_subset(x1));
  }

  SECTION("is_subset03")
  {
    double _x1[][2]={{0,2},{2,4}};
    double _x2[][2]={{0,1},{3,3}};
    IntervalVector x1(2,_x1);
    IntervalVector x2(2,_x2);

    CHECK(x1.is_superset(x2));
    CHECK(x2.is_subset(x1));
    CHECK(x1.is_strict_superset(x2));
    //CHECK(!x2.is_strict_interior_subset(x1));
  }

  SECTION("is_subset04")
  {
    double _x1[][2]={{0,2},{2,4}};
    double _x2[][2]={{1,1},{3,3}};
    IntervalVector x1(2,_x1);
    IntervalVector x2(2,_x2);

    CHECK(x1.is_superset(x2));
    CHECK(x2.is_subset(x1));
    CHECK(x1.is_strict_superset(x2));
    //CHECK(x2.is_strict_interior_subset(x1));
  }

  SECTION("is_subset05")
  {
    double _x1[][2]={{0,2},{2,4}};
    IntervalVector x1(2,_x1);
    IntervalVector x2(IntervalVector::empty_set(2));

    CHECK(x1.is_superset(x2));
    CHECK(x2.is_subset(x1));
    CHECK(x1.is_strict_superset(x2));
    //CHECK(x2.is_strict_interior_subset(x1));
  }

  SECTION("is_subset06")
  {
    double _x2[][2]={{1,1},{3,3}};

    IntervalVector x1(IntervalVector::empty_set(2));
    IntervalVector x2(2,_x2);

    CHECK(!x1.is_superset(x2));
    CHECK(!x2.is_subset(x1));
    CHECK(!x1.is_strict_superset(x2));
    //CHECK(!x2.is_strict_interior_subset(x1));
  }

  SECTION("is_subset07")
  {
    double _x1[][2]={{0,2},{2,4}};
    double _x2[][2]={{1,1},{3,5}};

    IntervalVector x1(2,_x1);
    IntervalVector x2(2,_x2);

    CHECK(!x1.is_superset(x2));
    CHECK(!x2.is_subset(x1));
    CHECK(!x1.is_strict_superset(x2));
    //CHECK(!x2.is_strict_interior_subset(x1));
  }

  SECTION("extr_diam_index01")
  {
    double _x1[][2]={{0,2},{0,1},{0,3}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==1);
    CHECK(x1.extr_diam_index(false)==2);
    CHECK(x1.min_diam()==1);
    CHECK(x1.max_diam()==3);
  }

  SECTION("extr_diam_index02")
  {
    double _x1[][2]={{0,1},{0,3},{0,2}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==1);
    CHECK(x1.min_diam()==1);
    CHECK(x1.max_diam()==3);
  }

  SECTION("extr_diam_index03")
  {
    double _x1[][2]={{0,3},{0,2},{0,1}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==2);
    CHECK(x1.extr_diam_index(false)==0);
    CHECK(x1.min_diam()==1);
    CHECK(x1.max_diam()==3);
  }

  SECTION("extr_diam_index04")
  {
    double _x1[][2]={{0,1},{0,2},{-oo,0}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==2);
    CHECK(x1.min_diam()==1);
    CHECK(x1.max_diam()==oo);
  }

  SECTION("extr_diam_index05")
  {
    double _x1[][2]={{-oo,0}};
    IntervalVector x1(1,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==0);
    CHECK(x1.min_diam()==oo);
    CHECK(x1.max_diam()==oo);
  }

  SECTION("extr_diam_index06")
  {
    double _x1[][2]={{-oo,0},{0,1},{-oo,1},{1,3}};
    IntervalVector x1(4,_x1);
    CHECK(x1.extr_diam_index(true)==1);
    CHECK(x1.extr_diam_index(false)==2);
    CHECK(x1.min_diam()==1);
    CHECK(x1.max_diam()==oo);
  }

  SECTION("extr_diam_index07")
  {
    double _x1[][2]={{-oo,0},{-2,oo},{-oo,1}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==1);
    CHECK(x1.min_diam()==oo);
    CHECK(x1.max_diam()==oo);
  }

  SECTION("extr_diam_index08")
  {
    double _x1[][2]={{-oo,0},{-oo,1},{-2,oo}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==2);
    CHECK(x1.min_diam()==oo);
    CHECK(x1.max_diam()==oo);
  }

  SECTION("extr_diam_index09")
  {
    double _x1[][2]={{-2,oo},{-oo,0},{-oo,1}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==1);
    CHECK(x1.extr_diam_index(false)==0);
    CHECK(x1.min_diam()==oo);
    CHECK(x1.max_diam()==oo);
  }

  SECTION("extr_diam_index10")
  {
    double _x1[][2]={{-2,oo},{-oo,1},{-oo,0}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==2);
    CHECK(x1.extr_diam_index(false)==0);
    CHECK(x1.min_diam()==oo);
    CHECK(x1.max_diam()==oo);
  }

  SECTION("volume01")
  {
    double _x1[][2]={{0,1},{0,oo}};
    CHECK(IntervalVector(2,_x1).volume()==oo);
  }

  SECTION("volume02")
  {
    double _x1[][2]={{0,1},{1,1}};
    CHECK(IntervalVector(2,_x1).volume()==0);
  }

  SECTION("volume03")
  {
    double _x1[][2]={{0,2},{2,5},{4,8}};
    CHECK(Approx(24.0)==IntervalVector(3,_x1).volume());
  }

  SECTION("minus01")
  {
    double _x1[][2]={{0,3},{0,2},{0,1}};
    double _x2[][2]={{-3,0},{-2,0},{-1,0}};
    CHECK((-IntervalVector(3,_x1))==IntervalVector(3,_x2));
  }

  SECTION("minus02")
  {
    double _x1[][2]={{0,1},{0,oo}};
    double _x2[][2]={{-1,0},{-oo,0}};
    CHECK(-IntervalVector(2,_x1)==IntervalVector(2,_x2));
  }

  SECTION("minus03")
  {
    CHECK(-IntervalVector::empty_set(2)==IntervalVector::empty_set(2));
  }

  SECTION("add01")
  {
    double _x1[][2]={{0,3},{0,2},{0,1}};
    double _x2[][2]={{0,1},{0,1},{0,1}};
    double _x3[][2]={{0,4},{0,3},{0,2}};

    IntervalVector x1(3,_x1);
    IntervalVector x2(3,_x2);
    IntervalVector x3(3,_x3);
    IntervalVector e(IntervalVector::empty_set(3));

    CHECK(x1+x2==x3);
    CHECK((x1+e)==e);
    CHECK((x1+e).is_empty());
    CHECK((IntervalVector(x1)+=e)==e);
    CHECK((IntervalVector(x1)+=e).is_empty());

    CHECK((e+x1)==e);
    CHECK((e+x1).is_empty());
    CHECK((e+=x1)==e);
    CHECK((e+=x1).is_empty());
    CHECK((e+e)==e);
    CHECK((e+e).is_empty());
    CHECK((e+=e)==e);
    CHECK((e+=e).is_empty());

    CHECK((IntervalVector(x1)+=x2)==x3);
    CHECK((IntervalVector(x1)+=e)==e);
    CHECK((IntervalVector(x1)+=e).is_empty());

    CHECK((IntervalVector(x2)+=x1)==x3);
  }

  SECTION("sub01")
  {
    double _x1[][2]={{0,3},{0,2},{0,1}};
    double _x2[][2]={{0,1},{0,1},{0,1}};
    double _x3[][2]={{-1,3},{-1,2},{-1,1}};
    IntervalVector x1(3,_x1);
    IntervalVector x2(3,_x2);
    IntervalVector x3(3,_x3);
    IntervalVector e(IntervalVector::empty_set(3));

    CHECK(x1-x2==x3);
    CHECK(x2-x1==-x3);
    CHECK((x1-e)==e);
    CHECK((x1-e).is_empty());
    CHECK((IntervalVector(x1)-=e)==e);
    CHECK((IntervalVector(x1)-=e).is_empty());

    CHECK((e-x1)==e);
    CHECK((e-x1).is_empty());
    CHECK((e-=x1)==e);
    CHECK((e-=x1).is_empty());
    CHECK((e-e)==e);
    CHECK((e-e).is_empty());
    CHECK((e-=e)==e);
    CHECK((e-=e).is_empty());

    CHECK((IntervalVector(x1)-=x2)==x3);
    CHECK((IntervalVector(x2)-=x1)==-x3);
  }
}

bool test_diff(const IntervalVector& x, const IntervalVector& y, const std::list<IntervalVector>& expected, bool compactness = true)
{
  auto c = x.diff(y, compactness);

  CHECK(!c.empty());
  CHECK(c.size()==expected.size());
  CHECK(c.front().size()==x.size());

  auto it = c.begin();
  while(it != c.end())
  {
    bool is_same = false;
    for(const auto& ri : expected)
      if(ri == *it)
      {
        is_same = true;
        break;
      }

    if(is_same)
      it = c.erase(it);
    else
      it++;
  }

  CHECK(c.empty()); // all complementary boxes have been checked
  return c.empty();
}

TEST_CASE("Tests from IBEX IntervalVector::diff")
{
  SECTION("compl01")
  {
    double _b[][2]={{0,1},{0,1}};
    IntervalVector b(2,_b);
    auto c = b.complementary();

    CHECK(c.size()==4);
    CHECK(c.front().size()==2);

    for(size_t i = 0 ; i < 4 ; i++)
    {
      if(c.front() == IntervalVector({Interval::neg_reals(),Interval::all_reals()})
        || c.front() == IntervalVector({Interval(1,oo),Interval::all_reals()})
        || c.front() == IntervalVector({Interval(0,1),Interval::neg_reals()})
        || c.front() == IntervalVector({Interval(0,1),Interval(1,oo)}))
        c.pop_front();
    }

    CHECK(c.empty()); // all complementary boxes have been checked
  }

  SECTION("compl02")
  {
    // complementary of an empty box = (-oo,oo)x...(-oo,oo)
    IntervalVector x = IntervalVector::empty_set(2);
    auto c = x.complementary();
    CHECK(c.size()==1);
    CHECK(c.front().size()==2);
    CHECK(c.front()[0]==Interval::all_reals());
    CHECK(c.front()[1]==Interval::all_reals());
  }

  SECTION("diff01")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, IntervalVector::empty_set(3), { {{-2,2},{-2,2},{-2,2}} }));
  }

  SECTION("diff02")
  {
    CHECK(test_diff(IntervalVector::empty_set(3), {{-2,2},{-2,2},{-2,2}}, { IntervalVector::empty_set(3) }));
  }

  SECTION("diff03")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{3,4},{-1,1}}, { {{-2,2},{-2,2},{-2,2}} }));
  }

  SECTION("diff04")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,-1},{-1,1}}, { {{-2,2},{-2,2},{-2,2}} }));
  }

  SECTION("diff05")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-1,1}}, {
        {{-2,-1},{-2,2},{-2,2}},
        {{1,2},{-2,2},{-2,2}},
        {{-1,1},{-2,-1},{-2,2}},
        {{-1,1},{1,2},{-2,2}},
        {{-1,1},{-1,1},{-2,-1}},
        {{-1,1},{-1,1},{1,2}}
    }));
  }

  SECTION("diff06")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-2,1}}, {
        {{-2,-1},{-2,2},{-2,2}},
        {{1,2},{-2,2},{-2,2}},
        {{-1,1},{-2,-1},{-2,2}},
        {{-1,1},{1,2},{-2,2}},
        {{-1,1},{-1,1},{1,2}}
    }));
  }

  SECTION("diff07")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-2,2}}, {
      {{-2,-1},{-2,2},{-2,2}},
      {{1,2},{-2,2},{-2,2}},
      {{-1,1},{-2,-1},{-2,2}},
      {{-1,1},{1,2},{-2,2}}
    }));
  }

  SECTION("diff08")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,1},{-1,1}}, {
      {{-2,-1},{-2,2},{-2,2}},
      {{1,2},{-2,2},{-2,2}},
      {{-1,1},{1,2},{-2,2}},
      {{-1,1},{-2,1},{-2,-1}},
      {{-1,1},{-2,1},{1,2}}
    }));
  }

  SECTION("diff09")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,2},{-1,1}}, {
      {{-2,-1},{-2,2},{-2,2}},
      {{1,2},{-2,2},{-2,2}},
      {{-1,1},{-2,2},{-2,-1}},
      {{-1,1},{-2,2},{1,2}}
    }));
  }

  SECTION("diff10")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-1,1},{-1,1}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{-2,-1},{-2,2}},
      {{-2,1},{1,2},{-2,2}},
      {{-2,1},{-1,1},{-2,-1}},
      {{-2,1},{-1,1},{1,2}}
    }));
  }

  SECTION("diff11")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-1,2},{-1,1}}, {
      {{-2,2},{-2,-1},{-2,2}},
      {{-2,2},{-1,2},{-2,-1}},
      {{-2,2},{-1,2},{1,2}}
    }));
    
  }

  SECTION("diff12")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,1},{-2,1}}, {
      {{-2,-1},{-2,2},{-2,2}},
      {{1,2},{-2,2},{-2,2}},
      {{-1,1},{1,2},{-2,2}},
      {{-1,1},{-2,1},{1,2}}
    })); 
  }

  SECTION("diff13")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-1,1},{-2,1},{-2,2}}, {
      {{-2,-1},{-2,2},{-2,2}},
      {{1,2},{-2,2},{-2,2}},
      {{-1,1},{1,2},{-2,2}}
    }));
  }

  SECTION("diff14")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-1,1},{-2,1}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{-2,-1},{-2,2}},
      {{-2,1},{1,2},{-2,2}},
      {{-2,1},{-1,1},{1,2}}
    }));  
  }

  SECTION("diff15")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-1,1},{-2,2}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{-2,-1},{-2,2}},
      {{-2,1},{1,2},{-2,2}}
    }));
  }

  SECTION("diff16")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,1},{-1,1}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{1,2},{-2,2}},
      {{-2,1},{-2,1},{-2,-1}},
      {{-2,1},{-2,1},{1,2}}
    }));
  }

  SECTION("diff17")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,2},{-1,1}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{-2,2},{-2,-1}},
      {{-2,1},{-2,2},{1,2}}
    }));
  }

  SECTION("diff18")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,1},{-2,1}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{1,2},{-2,2}},
      {{-2,1},{-2,1},{1,2}}
    }));
  }

  SECTION("diff19")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,1},{-2,2}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{1,2},{-2,2}}
    }));
  }

  SECTION("diff20")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,2},{-2,1}}, {
      {{1,2},{-2,2},{-2,2}},
      {{-2,1},{-2,2},{1,2}}
    }));
  }

  SECTION("diff21")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,1},{-2,1}}, {
      {{-2,2},{1,2},{-2,2}},
      {{-2,2},{-2,1},{1,2}}
    }));
  }

  SECTION("diff22")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,1},{-2,2},{-2,2}}, {
      {{1,2},{-2,2},{-2,2}}
    }));
  }

  SECTION("diff23")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,2},{-2,1}}, {
      {{-2,2},{-2,2},{1,2}}
    }));
  }

  SECTION("diff24")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,1},{-2,2}}, {
      {{-2,2},{1,2},{-2,2}}
    }));
  }

  SECTION("diff25")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{-2,2},{-2,2},{-2,2}}, {
      IntervalVector::empty_set(3)
    }));
  }

  SECTION("diff30")
  {
    CHECK(test_diff({{0,0},{-2,2},{-2,2}}, {{0,0},{-1,1},{-1,1}}, {
      {{0,0},{-2,-1},{-2,2}},
      {{0,0},{1,2},{-2,2}},
      {{0,0},{-1,1},{-2,-1}},
      {{0,0},{-1,1},{1,2}}
    }));
  }

  SECTION("diff31")
  {
    CHECK(test_diff({{0,0},{0,0},{-2,2}}, {{0,0},{0,0},{-1,1}}, {
      {{0,0},{0,0},{-2,-1}},
      {{0,0},{0,0},{1,2}}
    }));
  }

  SECTION("diff32")
  {
    CHECK(test_diff({{0,0},{-2,2},{0,0}}, {{0,0},{-1,1},{0,0}}, {
      {{0,0},{-2,-1},{0,0}},
      {{0,0},{1,2},{0,0}}
    }));
  }

  SECTION("diff33")
  {
    CHECK(test_diff({{-2,2},{0,0},{0,0}}, {{-1,1},{0,0},{0,0}}, {
      {{-2,-1},{0,0},{0,0}},
      {{1,2},{0,0},{0,0}}
    }));
  }

  SECTION("diff34")
  {
    CHECK(test_diff({{0,0},{-2,2},{-2,2}}, {{-1,1},{-1,1},{-1,1}}, {
      {{0,0},{-2,-1},{-2,2}},
      {{0,0},{1,2},{-2,2}},
      {{0,0},{-1,1},{-2,-1}},
      {{0,0},{-1,1},{1,2}}
    }));
  }

  SECTION("diff35")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{2,4},{-2,2},{-2,2}}, {
      {{-2,2},{-2,2},{-2,2}}
    }));
  }

  SECTION("diff36")
  {
    CHECK(test_diff({{-2,2},{-2,2},{-2,2}}, {{2,4},{-1,1},{-1,1}}, {
      {{-2,2},{-2,2},{-2,2}}
    }));
  }

  SECTION("diff37")
  {
    CHECK(test_diff({{-2,2},{-2,2}}, {{-2,2},{1,1}}, {
      {{-2,2},{-2,1}},
      {{-2,2},{1,2}}
    }, false));
  }

  SECTION("diff38")
  {
    CHECK(test_diff({{-2,2},{1,1}}, {{0,2},{-2,2}}, {
      {{-2,0},{1,1}}
    }, false));
  }

  SECTION("issue228")
  {
    CHECK(test_diff({{-1,-1},{-1,1},{-1,1}}, {{0,2},{0,2},{0,2}}, {
      {{-1,-1},{-1,1},{-1,1}}
    }));
  }
}


#if 0

// Tests from IBEX that are not (yet) considered in Codac:

void TestIntervalVector::is_relative_interior01() {
  double _x1[][2]={{1,1},{3,4}};
  double _x2[][2]={{1,1},{2,5}};

  IntervalVector x1(2,_x1);
  IntervalVector x2(2,_x2);

  CHECK(x1.is_relative_interior_subset(x2));
}

void TestIntervalVector::is_relative_interior02() {
  double _x1[][2]={{3,4},{3,3}};
  double _x2[][2]={{2,5},{3,3}};

  IntervalVector x1(2,_x1);
  IntervalVector x2(2,_x2);

  CHECK(x1.is_relative_interior_subset(x2));
}

void TestIntervalVector::is_relative_interior03() {
  double _x1[][2]={{0,0},{1,1}};
  double _x2[][2]={{1,1},{1,1}};

  IntervalVector x1(2,_x1);
  IntervalVector x2(2,_x2);

  CHECK(!x1.is_relative_interior_subset(x2));
}

void TestIntervalVector::is_relative_interior04() {
  double _x1[][2]={{0,0},{1,1}};
  double _x2[][2]={{0,0},{1,1}};

  IntervalVector x1(2,_x1);
  IntervalVector x2(2,_x2);

  CHECK(x1.is_relative_interior_subset(x2));
}

void TestIntervalVector::is_relative_interior05() {
  CHECK(IntervalVector::empty_set(2).is_relative_interior_subset(IntervalVector(2)));
}

void TestIntervalVector::sort_indices01() {
  double _x[][2]={{0,2},{-oo,0},{0,1},{3,3},{-10,10}};
  IntervalVector x(5,_x);
  int tab[5];
  x.sort_indices(true,tab);
  CHECK(tab[0]==3);
  CHECK(tab[1]==2);
  CHECK(tab[2]==0);
  CHECK(tab[3]==4);
  CHECK(tab[4]==1);
}

void TestIntervalVector::sort_indices02() {
  double _x[][2]={{0,2},{-oo,0},{0,1},{3,3},{-10,10}};
  IntervalVector x(5,_x);
  int tab[5];
  x.sort_indices(false,tab);
  CHECK(tab[0]==1);
  CHECK(tab[1]==4);
  CHECK(tab[2]==0);
  CHECK(tab[3]==2);
  CHECK(tab[4]==3);
}

void TestIntervalVector::rel_distance01() {
  IntervalVector box1(3);
  IntervalVector box2(3);
  box1[0]=Interval(0,0);
  box2[0]=Interval(0,0);
  box1[1]=Interval(-1,0);
  box2[1]=Interval(-1,0);
  box1[2]=Interval(1,4);
  box2[2]=Interval(1.5,3);
  CHECK_DOUBLES_EQUAL(1.0/3.0,box1.rel_distance(box2),ERROR);
}

void TestIntervalVector::perimeter01() {
  IntervalVector box1(3);
  IntervalVector box2(3);
  box1[0]=Interval(0,0);
  box2[0]=Interval(0,0);
  box1[1]=Interval(-1,0);
  box2[1]=Interval(-1,0);
  box1[2]=Interval(1,4);
  box2[2]=Interval(1.5,3);
  CHECK_DOUBLES_EQUAL(4.0,box1.perimeter(),ERROR);
  CHECK_DOUBLES_EQUAL(2.5,box2.perimeter(),ERROR);
}

void TestIntervalVector::perimeter02() {
  double _x1[][2]={{0,1},{0,oo}};
  CHECK(IntervalVector(2,_x1).perimeter()==oo);
}

void TestIntervalVector::random01() {
  double _b[][2]={{0,1},{0,1}};
  IntervalVector b(2,_b);
  IntervalVector r=b.random();

  CHECK(r.size()==2);
  CHECK(r.max_diam()==0);
  CHECK(b.is_superset(r));
}

void TestIntervalVector::random02() {
  double _b[][2]={{1,1},{2,2}};
  IntervalVector b(2,_b);
  IntervalVector r=b.random();

  CHECK(b==r);
}

#endif