// Author : Damien Massé
// Transformation facets to vertex from a .ine file

#include <codac>
#include <vector>
#include <set>
#include <memory>

using namespace std;
using namespace codac2;

int main(int argc, char *argv[])
{
   std::cout << std::scientific << std::setprecision(9);

   const char *namefile = "cross12.ine";
   if (argc>1) namefile= argv[1];
   Polytope pol = Polytope::from_ineFile(namefile);
   std::cout << pol << std::endl;
   
   std::vector<IntervalVector> 
      lvect = pol.vertices();
   std::cout << lvect.size() << " vertices" << std::endl;
   for (IntervalVector a : lvect) 
      std::cout << a << std::endl;

#if 0
   IntervalVector box = IntervalVector::constant((*fc)[0]->first.row.size(),
		Interval(-10,10));
   DDbuildF2V build2(box.size(),box,fc,false);
   std::cout << "Initial build : " << build2 << std::endl;
   for (int i=0;i<fc->nbfcts();i++) {
      std::cout << "add facet " << (i+1) << " : " << (*(*fc)[i]) << std::endl;
      build2.add_facet(i);
//      std::cout << build2;
   }
      std::cout << build2;
#endif
   return 0;
}
