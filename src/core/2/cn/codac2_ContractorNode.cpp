#include <iostream>
#include "codac2_ContractorNode.h"

using namespace std;

namespace codac2
{
  ostream& operator<<(ostream& os, const ContractorNodeBase& d)
  {
    os << "Contractor: " << d.contractor_class_name() << ", dom=" << d.nb_args() << endl;
    return os;
  }
}