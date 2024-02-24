#include "catch_interval.hpp"
#include "vibes.h"

#include "codac2_Tube.h"
#include "codac2_Interval.h"
#include "codac2_IntervalMatrix.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace codac2;

TEST_CASE("Test codac2::tubes templated")
{
  SECTION("Testing tube of templated IntervalMatrix")
  {
    auto tdomain = create_tdomain();
    Tube<IntervalMatrix_<2,3>> a(tdomain);
    a(2.3) = IntervalMatrix_<2,3>({{1.,2.,3.},{4.,5.,6.}});
    CHECK(a.nb_slices() == 3);

    vector<const Slice<IntervalMatrix_<2,3>>*> v;
    for(const auto& s : a)
      v.push_back(&s);
    CHECK(v[0]->t0_tf() == Interval(-oo,2.3));
    CHECK(v[0]->codomain() == (IntervalMatrix_<2,3>()));
    CHECK(v[1]->t0_tf() == Interval(2.3));
    CHECK(v[1]->codomain() == (IntervalMatrix_<2,3>({{1.,2.,3.},{4.,5.,6.}})));
    CHECK(v[2]->t0_tf() == Interval(2.3,oo));
    CHECK(v[2]->codomain() == (IntervalMatrix_<2,3>()));
  }
}