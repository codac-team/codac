#include <iostream>
#include "codac2_ContractorNode.h"

using namespace std;

namespace codac2
{
  void ContractorNodeBase::set_call_flag(bool flag)
  {
    _to_be_called = flag;
  }

  bool ContractorNodeBase::to_be_called() const
  {
    return _to_be_called;
  }

  ostream& operator<<(ostream& os, const ContractorNodeBase& d)
  {
    os << "Contractor: " << d.contractor_class_name() << ", dom=" << d.nb_args() << endl;
    return os;
  }
}