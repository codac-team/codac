#include <cstdio>
#include "catch_interval.hpp"
#include "codac_SepTransform.h"
#include "ibex_SepFwdBwd.h"
#include "ibex_Sep.h"
#include "codac_sivia.h"
#include "ibex_SepInverse.h"
#include "ibex_SepInter.h"


 

#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;


TEST_CASE("SepTransform")
{
  SECTION("Test SepTransform Init")
  {
      Function f1 = Function("x", "y", "(x,y)");
      SepFwdBwd S = SepFwdBwd(f1, IntervalVector({Interval(-3, 3), Interval(-3, 3)}));

      // SepFwdBwd S1 = SepFwdBwd(f1, Interval(16, 25));
      double theta = 3.14/6;
      string f2_str = string_format("(cos(%f)  + sin(%f), -sin(%f) + cos(%f))", theta,theta,theta,theta);
      //cout << f2_str << std::endl;

      Function f2 = Function("x", "y", string_format("(x*cos(%f)  + y*sin(%f), -x*sin(%f) + y*cos(%f))", theta,theta,theta,theta).c_str());
      Function f2_inv = Function("x", "y", string_format("(x*cos(%f) - y*sin(%f), x*sin(%f) + y*cos(%f))", theta,theta,theta,theta).c_str());

      SepTransform T = SepTransform(S, f2, f2_inv);
      SepInverse Ti = SepInverse(S, f2);

      SepInter Sinter = SepInter(T, Ti);
      IntervalVector X0 = IntervalVector({Interval(-10, 10), Interval(-10, 10)});
      SIVIA(X0, T, 0.01, !true, "SepTransform");
      SIVIA(X0, Ti, 0.01, !true, "SepInverse");
      SIVIA(X0, S, 0.01, !true, "SepInverse_both");

  }


  //   SECTION("Test SepTransform Rotate")
  // {
      
  //     Function f1 = Function("x", "y", "x^2 + y^2 ");
  //     SepFwdBwd S1 = SepFwdBwd(f1, Interval(16, 25));

  //     Function f2 = Function("x", "y", "(y + sin(x), x)");
  //     Function f2_inv = Function("x", "y", "(y, x - sin(y))");

  //     SepTransform T = SepTransform(S1, f2, f2_inv);
  //     SepInverse Ti = SepInverse(S1, f2);

  //     SepInter Sinter = SepInter(T, Ti);
  //     IntervalVector X0 = IntervalVector({Interval(-10, 10), Interval(-10, 10)});
  //     SIVIA(X0, T, 0.01, true, "SepTransform");
  //     SIVIA(X0, Ti, 0.01, true, "SepInverse");
  //     SIVIA(X0, Sinter, 0.01, true, "SepInverse_both");

  // }

}