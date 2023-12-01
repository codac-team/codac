#include "codac2_DomainNode.h"

using namespace std;

namespace codac2
{
  ostream& operator<<(ostream& os, const DomainNodeBase& d)
  {
    os << "Domain: " << d.domain_class_name() << ", ctc=" << d.contractors().size() << endl;
    return os;
  }
}