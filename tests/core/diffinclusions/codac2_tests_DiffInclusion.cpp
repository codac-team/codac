/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_DiffInclusion.h>
#include <codac2_ReachabilityStep.h>
#include <codac2_DI_Integration.h>
#include <codac2_Approx.h>
#include <codac2_math.h>

using namespace std;
using namespace codac2;

TEST_CASE("AnalyticFunction")
{
    {
      ScalarVar x;
      ScalarVar u;
      AnalyticFunction f({x,u}, -x+u);
      DiffInclusion<ScalarVar,ScalarVar> di(f);
		/* with x(0)=1 and u=u_m + [-eps,eps], 
                   x \in u_m(1-e^-t) + e^-t  + [-1,1] eps | e^t - 1 | */
      
      DI_Integration<DiffInclusion<ScalarVar,ScalarVar>,
                     ReachabilityStep0> integration(di);
      {
        IntervalVector i1({{-5.0,5.0}}), i2({{-5.0,5.0}});

        
        auto r = integration.contract_one_step(Interval(0.0,0.1),
                     IntervalVector({1.0}), i1,i2, Interval(0.0,0.2));
			// "exact" gate : [0.9048, 0.9239]
			// "exact" inside : [0.9048, 1]
			// result obtained : [0.8999, 0.9301]
        CHECK(r); 
        CHECK(i1[0].is_superset(Interval(0.9048,1)));
        CHECK(i2[0].is_superset(Interval(0.9048,0.9239)));
      }
      {
        IntervalVector i1({{-5.0,5.0}}), i2({{-5.0,5.0}});
        bool r = integration.contract_one_step(Interval(-0.1,0.0),
               IntervalVector({1.0}), i1,i2, Interval(0.0,0.2),false);
			// "exact" : [1.0841, 1.1052]
			// "exact" : [1.0000, 1.1052]
			// result obtained : [1.0799, 1.1112]
        CHECK(r); 
        CHECK(i1[0].is_superset(Interval(1,1.1052)));
        CHECK(i2[0].is_superset(Interval(1.0841,1.1052)));
      }
    }
    {
      VectorVar x(2);
      ScalarVar u;
      AnalyticFunction f({x,u}, {-x[1]+u, x[0]});
      DiffInclusion<VectorVar,ScalarVar> di(f);
		/* with x(0)=(1,0) and u=[0], 
                   x = (cos(t),sin(t)) */
      DI_Integration<DiffInclusion<VectorVar,ScalarVar>,
                     ReachabilityStep0> integration(di);
      
      {
        IntervalVector i1 {{-5.0,5.0},{-5.0,5.0}};
        IntervalVector i2 {{-5.0,5.0},{-5.0,5.0}};
        bool r = integration.contract_one_step(Interval(0.0,0.1),
                        IntervalVector({1.0,0.0}),
			i1,i2, Interval(0.0,0.0),true);
			/* "exact" gate : [0.9950,0.9951],[0.0998,0.0999] */
		        /* obtained : [0.9899,1], [0.09899,0.1000] */
        CHECK(r); 
        CHECK(i1.contains(Vector({cos(0.1),sin(0.1)})));
      }
    }
}
