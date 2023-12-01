#include "codac2_Contractor.h"

using namespace std;

namespace codac2
{
  Contractor1::Contractor1()
  {
    
  }

  void Contractor1::contract(IntervalVector_<3>& x)
  {
    cout << "Calling C1" << endl;
    x[0] = Interval(0,1);
    x[1] = Interval(0,1);
    x[2] = Interval(42,43);
  }


  Contractor2::Contractor2()
  {
    
  }

  void Contractor2::contract(IntervalVector_<2>& x, IntervalVector_<3>& y)
  {
    cout << "Calling C2" << endl;
    x[0] &= y[2];
    y[1] &= Interval(-5,6);
  }
}