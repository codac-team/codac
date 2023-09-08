#include "catch_interval.hpp"
#include "vibes.h"

#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_cart_prod.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace codac2;


TEST_CASE("Test codac2::IntervalVector")
{
  // These tests come from the IBEX library (G. Chabert)

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
    CHECK(IntervalVector::empty_set(3).subvector(1,2).is_empty());
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
    double _x1[][2]={{0,1},{0,2},{NEG_INFINITY,0}};
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
    double _x1[][2]={{0,1},{0,2},{NEG_INFINITY,0}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==2);
    CHECK(x1.min_diam()==1);
    CHECK(x1.max_diam()==POS_INFINITY);
  }

  SECTION("extr_diam_index05")
  {
    double _x1[][2]={{NEG_INFINITY,0}};
    IntervalVector x1(1,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==0);
    CHECK(x1.min_diam()==POS_INFINITY);
    CHECK(x1.max_diam()==POS_INFINITY);
  }

  SECTION("extr_diam_index06")
  {
    double _x1[][2]={{NEG_INFINITY,0},{0,1},{NEG_INFINITY,1},{1,3}};
    IntervalVector x1(4,_x1);
    CHECK(x1.extr_diam_index(true)==1);
    CHECK(x1.extr_diam_index(false)==2);
    CHECK(x1.min_diam()==1);
    CHECK(x1.max_diam()==POS_INFINITY);
  }

  SECTION("extr_diam_index07")
  {
    double _x1[][2]={{NEG_INFINITY,0},{-2,POS_INFINITY},{NEG_INFINITY,1}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==1);
    CHECK(x1.min_diam()==POS_INFINITY);
    CHECK(x1.max_diam()==POS_INFINITY);
  }

  SECTION("extr_diam_index08")
  {
    double _x1[][2]={{NEG_INFINITY,0},{NEG_INFINITY,1},{-2,POS_INFINITY}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==0);
    CHECK(x1.extr_diam_index(false)==2);
    CHECK(x1.min_diam()==POS_INFINITY);
    CHECK(x1.max_diam()==POS_INFINITY);
  }

  SECTION("extr_diam_index09")
  {
    double _x1[][2]={{-2,POS_INFINITY},{NEG_INFINITY,0},{NEG_INFINITY,1}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==1);
    CHECK(x1.extr_diam_index(false)==0);
    CHECK(x1.min_diam()==POS_INFINITY);
    CHECK(x1.max_diam()==POS_INFINITY);
  }

  SECTION("extr_diam_index10")
  {
    double _x1[][2]={{-2,POS_INFINITY},{NEG_INFINITY,1},{NEG_INFINITY,0}};
    IntervalVector x1(3,_x1);
    CHECK(x1.extr_diam_index(true)==2);
    CHECK(x1.extr_diam_index(false)==0);
    CHECK(x1.min_diam()==POS_INFINITY);
    CHECK(x1.max_diam()==POS_INFINITY);
  }

  SECTION("volume01")
  {
    double _x1[][2]={{0,1},{0,POS_INFINITY}};
    CHECK(IntervalVector(2,_x1).volume()==POS_INFINITY);
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
    double _x1[][2]={{0,1},{0,POS_INFINITY}};
    double _x2[][2]={{-1,0},{NEG_INFINITY,0}};
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
    //CHECK(x1+e,e);
    CHECK((x1+e).is_empty());
    //CHECK(IntervalVector(x1)+=e,e);
    CHECK((IntervalVector(x1)+=e).is_empty());

    //CHECK(e+x1,e);
    CHECK((e+x1).is_empty());
    //CHECK(e+=x1,e);
    CHECK((e+=x1).is_empty());
    //CHECK(e+e,e);
    CHECK((e+e).is_empty());
    //CHECK(e+=e,e);
    CHECK((e+=e).is_empty());

    CHECK((IntervalVector(x1)+=x2)==x3);
    //CHECK(IntervalVector(x1)+=e,e);
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
    //CHECK(x1-e,e);
    CHECK((x1-e).is_empty());
    //CHECK(IntervalVector(x1)-=e,e);
    CHECK((IntervalVector(x1)-=e).is_empty());

    //CHECK(e-x1,e);
    CHECK((e-x1).is_empty());
    //CHECK(e-=x1,e);
    CHECK((e-=x1).is_empty());
    //CHECK(e-e,e);
    CHECK((e-e).is_empty());
    //CHECK(e-=e,e);
    CHECK((e-=e).is_empty());

    CHECK((IntervalVector(x1)-=x2)==x3);
    CHECK((IntervalVector(x2)-=x1)==-x3);
  }
}

#if 0 



void TestIntervalVector::compl01() {
  double _b[][2]={{0,1},{0,1}};
  IntervalVector b(2,_b);
  IntervalVector* c;
  int n=b.complementary(c);

  CHECK(n==4);
  CHECK(c[0].size()==2);

  CHECK(c[0][0]==Interval::neg_reals());
  CHECK(c[0][1]==Interval::all_reals());

  CHECK(c[1][0]==Interval(1,POS_INFINITY));
  CHECK(c[1][1]==Interval::all_reals());

  CHECK(c[2][0]==Interval(0,1));
  CHECK(c[2][1]==Interval::neg_reals());

  CHECK(c[3][0]==Interval(0,1));
  CHECK(c[3][1]==Interval(1,POS_INFINITY));

  delete[] c;
}

/**
 * complementary of an empty box = (-oo,oo)x...(-oo,oo)
 */
void TestIntervalVector::compl02() {

  IntervalVector* c;
  int n=IntervalVector::empty_set(2).complementary(c);
  CHECK(n==1);
  CHECK(c[0].size()==2);

  CHECK(c[0][0]==Interval::all_reals());
  CHECK(c[0][1]==Interval::all_reals());

  delete[] c;
}

bool TestIntervalVector::test_diff(int n, double _x[][2], double _y[][2], int m, double _z[][2], bool compactness, bool debug) {
  IntervalVector x(n,_x);
  IntervalVector y(n,_y);
  IntervalMatrix mz(m,n,_z);
  IntervalVector* c;
  int nn=x.diff(y,c,compactness);
  if (debug) {
    cout << x << " diff " << y << " gives:" << endl;
    for (int i=0; i<nn; i++) {
      cout << c[i] << endl;
    }
    cout << "==================================\n";
  }
  if (nn!=m) return false;
  for (int i=0; i<nn; i++) {
    if (c[i]!=mz[i]) {
      if (debug) cout << "i=" << i << c[i] << "!=" << mz[i] << endl;
      delete[] c;
      return false;
    }
  }
  delete[] c;
  return true;
}


void TestIntervalVector::diff01() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  IntervalVector* c;
  IntervalVector _x(3,x);
  CHECK(_x.diff(IntervalVector::empty_set(3),c)==1);
  CHECK(c[0]==_x);
  delete[] c;
}

void TestIntervalVector::diff02() {
  double y[][2]= {{-2,2},{-2,2},{-2,2}};
  IntervalVector* c;
  CHECK(IntervalVector::empty_set(3).diff(IntervalVector(3,y),c)==0);
  delete[] c;
}

void TestIntervalVector::diff03() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{3,4},{-1,1}};
  double z[][2]= {{-2,2},{-2,2},{-2,2}};
  CHECK(test_diff(3,x,y,1,z));
}

void TestIntervalVector::diff04() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-1,-1},{-1,1}};
  double z[][2]= {{-2,2},{-2,2},{-2,2}};
  CHECK(test_diff(3,x,y,1,z));
}

void TestIntervalVector::diff05() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-1,1},{-1,1}};
  double z[][2]= {{-2,-1},{-2,2},{-2,2},
              {1,2},{-2,2},{-2,2},
          {-1,1},{-2,-1},{-2,2},
          {-1,1},{1,2},{-2,2},
          {-1,1},{-1,1},{-2,-1},
          {-1,1},{-1,1},{1,2}};
  CHECK(test_diff(3,x,y,6,z));
}

void TestIntervalVector::diff06() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-1,1},{-2,1}};
  double z[][2]= {{-2,-1},{-2,2},{-2,2},
              {1,2},{-2,2},{-2,2},
          {-1,1},{-2,-1},{-2,2},
          {-1,1},{1,2},{-2,2},
          {-1,1},{-1,1},{1,2}};
  CHECK(test_diff(3,x,y,5,z));
}

void TestIntervalVector::diff07() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-1,1},{-2,2}};
  double z[][2]= {{-2,-1},{-2,2},{-2,2},
              {1,2},{-2,2},{-2,2},
          {-1,1},{-2,-1},{-2,2},
          {-1,1},{1,2},{-2,2}};
  CHECK(test_diff(3,x,y,4,z));
}

void TestIntervalVector::diff08() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-2,1},{-1,1}};
  double z[][2]= {{-2,-1},{-2,2},{-2,2},
              {1,2},{-2,2},{-2,2},
          {-1,1},{1,2},{-2,2},
          {-1,1},{-2,1},{-2,-1},
          {-1,1},{-2,1},{1,2}};
  CHECK(test_diff(3,x,y,5,z));
}

void TestIntervalVector::diff09() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-2,2},{-1,1}};
  double z[][2]= {{-2,-1},{-2,2},{-2,2},
              {1,2},{-2,2},{-2,2},
          {-1,1},{-2,2},{-2,-1},
          {-1,1},{-2,2},{1,2}};
  CHECK(test_diff(3,x,y,4,z));
}

void TestIntervalVector::diff10() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-1,1},{-1,1}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{-2,-1},{-2,2},
          {-2,1},{1,2},{-2,2},
          {-2,1},{-1,1},{-2,-1},
          {-2,1},{-1,1},{1,2}};
  CHECK(test_diff(3,x,y,5,z));
}

void TestIntervalVector::diff11() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,2},{-1,2},{-1,1}};
  double z[][2]= {{-2,2},{-2,-1},{-2,2},
          {-2,2},{-1,2},{-2,-1},
          {-2,2},{-1,2},{1,2}};
  CHECK(test_diff(3,x,y,3,z));
}

void TestIntervalVector::diff12() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-2,1},{-2,1}};
  double z[][2]= {{-2,-1},{-2,2},{-2,2},
              {1,2},{-2,2},{-2,2},
          {-1,1},{1,2},{-2,2},
          {-1,1},{-2,1},{1,2}};
  CHECK(test_diff(3,x,y,4,z));
}

void TestIntervalVector::diff13() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-2,1},{-2,2}};
  double z[][2]= {{-2,-1},{-2,2},{-2,2},
              {1,2},{-2,2},{-2,2},
          {-1,1},{1,2},{-2,2}};
  CHECK(test_diff(3,x,y,3,z));
}

void TestIntervalVector::diff14() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-1,1},{-2,1}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{-2,-1},{-2,2},
          {-2,1},{1,2},{-2,2},
          {-2,1},{-1,1},{1,2}};
  CHECK(test_diff(3,x,y,4,z));
}

void TestIntervalVector::diff15() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-1,1},{-2,2}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{-2,-1},{-2,2},
          {-2,1},{1,2},{-2,2}};
  CHECK(test_diff(3,x,y,3,z));
}

void TestIntervalVector::diff16() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-2,1},{-1,1}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{1,2},{-2,2},
          {-2,1},{-2,1},{-2,-1},
          {-2,1},{-2,1},{1,2}};
  CHECK(test_diff(3,x,y,4,z));
}

void TestIntervalVector::diff17() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-2,2},{-1,1}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{-2,2},{-2,-1},
          {-2,1},{-2,2},{1,2}};
  CHECK(test_diff(3,x,y,3,z));
}

void TestIntervalVector::diff18() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-2,1},{-2,1}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{1,2},{-2,2},
          {-2,1},{-2,1},{1,2}};
  CHECK(test_diff(3,x,y,3,z));
}

void TestIntervalVector::diff19() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-2,1},{-2,2}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{1,2},{-2,2}};
  CHECK(test_diff(3,x,y,2,z));
}

void TestIntervalVector::diff20() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-2,2},{-2,1}};
  double z[][2]= {{1,2},{-2,2},{-2,2},
          {-2,1},{-2,2},{1,2}};
  CHECK(test_diff(3,x,y,2,z));
}

void TestIntervalVector::diff21() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,2},{-2,1},{-2,1}};

  double z[][2]= {{-2,2},{1,2},{-2,2},
          {-2,2},{-2,1},{1,2}};
  CHECK(test_diff(3,x,y,2,z));
}

void TestIntervalVector::diff22() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,1},{-2,2},{-2,2}};
  double z[][2]= {{1,2},{-2,2},{-2,2}};
  CHECK(test_diff(3,x,y,1,z));
}

void TestIntervalVector::diff23() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,2},{-2,2},{-2,1}};
  double z[][2]= {{-2,2},{-2,2},{1,2}};
  CHECK(test_diff(3,x,y,1,z));
}

void TestIntervalVector::diff24() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,2},{-2,1},{-2,2}};
  double z[][2]= {{-2,2},{1,2},{-2,2}};
  CHECK(test_diff(3,x,y,1,z));
}

void TestIntervalVector::diff25() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{-2,2},{-2,2},{-2,2}};
  IntervalVector* c;
  CHECK(IntervalVector(3,x).diff(IntervalVector(3,y),c)==0);
  delete[] c;
}

void TestIntervalVector::diff26() {

}

void TestIntervalVector::diff27() {

}

void TestIntervalVector::diff28() {

}

void TestIntervalVector::diff29() {

}


void TestIntervalVector::diff30() {
  double x[][2]= {{0,0},{-2,2},{-2,2}};
  double y[][2]= {{0,0},{-1,1},{-1,1}};
  double z[][2]= {{0,0},{-2,-1},{-2,2},
              {0,0},{1,2},{-2,2},
          {0,0},{-1,1},{-2,-1},
          {0,0},{-1,1},{1,2}};
  CHECK(test_diff(3,x,y,4,z));
}

void TestIntervalVector::diff31() {
  double x[][2]= {{0,0},{0,0},{-2,2}};
  double y[][2]= {{0,0},{0,0},{-1,1}};
  double z[][2]= {{0,0},{0,0},{-2,-1},
              {0,0},{0,0},{1,2}};
  CHECK(test_diff(3,x,y,2,z));
}

void TestIntervalVector::diff32() {
  double x[][2]= {{0,0},{-2,2},{0,0}};
  double y[][2]= {{0,0},{-1,1},{0,0}};
  double z[][2]= {{0,0},{-2,-1},{0,0},
              {0,0},{1,2},{0,0}};
  CHECK(test_diff(3,x,y,2,z));
}

void TestIntervalVector::diff33() {
  double x[][2]= {{-2,2},{0,0},{0,0}};
  double y[][2]= {{-1,1},{0,0},{0,0}};
  double z[][2]= {{-2,-1},{0,0},{0,0},
              {1,2},{0,0},{0,0}};
  CHECK(test_diff(3,x,y,2,z));
}

void TestIntervalVector::diff34() {
  double x[][2]= {{0,0},{-2,2},{-2,2}};
  double y[][2]= {{-1,1},{-1,1},{-1,1}};
  double z[][2]= {{0,0},{-2,-1},{-2,2},
              {0,0},{1,2},{-2,2},
          {0,0},{-1,1},{-2,-1},
          {0,0},{-1,1},{1,2}};
  CHECK(test_diff(3,x,y,4,z));
}

void TestIntervalVector::diff35() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{2,4},{-2,2},{-2,2}};
  double z[][2]= {{-2,2},{-2,2},{-2,2}};
  CHECK(test_diff(3,x,y,1,z));
}

void TestIntervalVector::diff36() {
  double x[][2]= {{-2,2},{-2,2},{-2,2}};
  double y[][2]= {{2,4},{-1,1},{-1,1}};
  double z[][2]= {{-2,2},{-2,2},{-2,2}};
  CHECK(test_diff(3,x,y,1,z));
}

void TestIntervalVector::diff37() {
  double x[][2]= {{-2,2},{-2,2}};
  double y[][2]= {{-2,2},{1,1}};
  double z[][2]= {{-2,2},{-2,1},
                  {-2,2},{1,2}};
  CHECK(test_diff(2,x,y,2,z,false));
}

void TestIntervalVector::diff38() {
  double x[][2]= {{-2,2},{1,1}};
  double y[][2]= {{0,2},{-2,2}};
  double z[][2]= {{-2,0},{1,1}};
  CHECK(test_diff(2,x,y,1,z,false));
}

void TestIntervalVector::issue228() {
  double x[][2]= {{-1,-1},{-1,1},{-1,1}};
  double y[][2]= {{0,2},{0,2},{0,2}};
  double z[][2]= {{-1,-1},{-1,1},{-1,1}};
  CHECK(test_diff(3,x,y,1,z));
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




/*
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
  double _x[][2]={{0,2},{NEG_INFINITY,0},{0,1},{3,3},{-10,10}};
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
  double _x[][2]={{0,2},{NEG_INFINITY,0},{0,1},{3,3},{-10,10}};
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
  double _x1[][2]={{0,1},{0,POS_INFINITY}};
  CHECK(IntervalVector(2,_x1).perimeter()==POS_INFINITY);
}

*/