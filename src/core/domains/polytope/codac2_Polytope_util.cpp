/**
 * \file codac2_Polytope_util.cpp utilities for polytope 
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <bitset>
#include <memory>
#include <map>
#include <forward_list>
#include <set>
#include <stdexcept>
#include <algorithm>

#include "codac2_Index.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_Facet.h"
#include "codac2_Polytope_dd.h"
#include "codac2_Polytope_util.h"


namespace codac2 {

/** read a .ine file and return a list of facets
 *  this is a very crude function, which assumes the format of the 
 *  file is correct, and no real check is done. Linearities are not treated. */
CollectFacets read_ineFile(const char *filename) {
   std::ifstream fichier(filename);
   assert(!fichier.fail());
   /* skip everything until begin */
   std::string line;
   while (getline(fichier,line)) {
      std::cout << line << std::endl;
      if (line.compare("begin")==0) break;
   }
   assert(fichier.good());
   Index nbfacets, dim;
   char dummy[50];
   fichier >> nbfacets >> dim >> dummy;
   std::cout << nbfacets << " facettes " << dim << " dimensions" << std::endl;
   dim = dim-1; /* non-normalised dimension */
   CollectFacets result;
   for (Index i=0;i<nbfacets;i++) {
     double rhs=0.0;
     Row row = Row::zero(dim);
     fichier >> rhs;
     for (Index j=0;j<dim;j++) {
        fichier >> row[j];
     }
     row = -row;
     result.insert_facet(row,rhs,false);
   }
   fichier.close();
   return result;
}

/** read a .ext file and return a list of vertices
 *  this is a very crude function, which assumes the format of the 
 *  file is correct, and no real check is done. Rays (or lines)
 *  are not treated for now. */
std::vector<Vector> read_extFile(const char *filename) {
   std::ifstream fichier(filename);
   assert(!fichier.fail());
   /* skip everything until begin */
   std::string line;
   while (getline(fichier,line)) {
      std::cout << line << std::endl;
      if (line.compare("begin")==0) break;
   }
   assert(fichier.good());
   Index nbvertices, dim;
   char dummy[50];
   fichier >> nbvertices >> dim >> dummy;
   std::cout << nbvertices << " sommets " << (dim-1) << " dimensions" << std::endl;
   dim = dim-1; /* non-normalised dimension */
   std::vector<Vector> result;
   for (Index i=0;i<nbvertices;i++) {
     double rhs=0.0;
     Vector vrt = Vector::zero(dim);
     fichier >> rhs; /* 'rhs' is supposed to be 1 */
     for (Index j=0;j<dim;j++) {
        fichier >> vrt[j];
     }
     result.push_back(vrt);
   }
   fichier.close();
   return result;
}

/* create a 2D facet in a 3D polyhedron, return the number of vertices built.
   fct = -1 : any facet */
Index fill_3Dfacet(const DDbuildF2V &build, 
	std::vector<Vector> &tofill, Index fct, double bound) {
    const std::forward_list<DDvertex> &vertices=build.get_vertices();
    std::forward_list<DDvertex>::const_iterator it=vertices.begin();
    while (it != vertices.end()) {
        if (it->vertex[0]!=0.0 && 
		(fct==-1 || std::find(it->fcts.begin(),
			it->fcts.end(),fct)!=it->fcts.end()))
        break;
        ++it;
    }
    if (it==vertices.end()) return 0;
    
    std::vector<std::forward_list<DDvertex>::const_iterator> used;
    used.push_back(it);
    Vector vt = build.compute_vertex(it->vertex).mid();
    tofill.push_back(vt);
    while (it != vertices.end()) {
       std::forward_list<DDvertex>::const_iterator it2;
       Index i=0;
       while (i<(Index)it->links.size()) {
          it2 = it->links[i];
          if ((fct==-1 ||
		std::find(it2->fcts.begin(),
                        it2->fcts.end(),fct)!=it2->fcts.end())
	      && (std::find(used.begin(),used.end(),it2)==used.end())) break;
          i++;
       }
       if (i==(Index)it->links.size()) break;
       it=it2;
       if (it->vertex[0]==0.0) /* ray */  {
          vt += bound*build.compute_vertex(it->vertex).mid();
       } else {
          vt = build.compute_vertex(it->vertex).mid();
       }
       tofill.push_back(vt);
       used.push_back(it);
//       std::cout << "add vert : " << it->Id << "\n";
    }
    return (Index)tofill.size();
}

std::vector<std::vector<Vector>> build_3Dfacets(const DDbuildF2V &build, double bound) {
   assert(build.get_dim()==3);
   if (build.is_empty()) return std::vector<std::vector<Vector>>();
   if (build.get_fdim()==0) { /* the only point has no coordinate */
       std::vector<std::vector<Vector>> result(1);
       Vector v = Vector::constant(1,1.0);
       result[0].push_back((build.get_M_EQ()).mid()*v);
       return result;
   } else 
   if (build.get_fdim()<3) { /* line or face are managed as a single face */
       std::vector<std::vector<Vector>> result(1);
       std::vector<Vector> &face = result[0];
       fill_3Dfacet(build,face,-1,bound);
       /* look for a initial vertex */
       return result;
   }
   std::vector<std::vector<Vector>> result;
   for (Index i=0;i<(Index)build.get_reffacets().size();i++) {
      if (build.get_reffacets()[i]==0) continue;
      std::vector<Vector> nfac;
      Index nbV = fill_3Dfacet(build, nfac, i, bound);
//      std::cout << "facet build, " << nbV << " vertices.\n";
      if (nbV<3) continue;
      result.push_back(nfac);
   }
   return result;
}

std::vector<Vector> build_2Dfacet(const DDbuildF2V &build, double bound) {
   assert(build.get_dim()==2);
   if (build.is_empty()) return std::vector<Vector>();
   if (build.get_fdim()==0) { /* the only point has no coordinate */
       std::vector<Vector> result(1);
       Vector v = Vector::constant(1,1.0);
       result[0]=(build.get_M_EQ()).mid()*v;
       return result;
   } else 
   { /* equivalent to finding one facet of a 3Dfacet */
       std::vector<Vector> face;
       fill_3Dfacet(build,face,-1,bound);
       /* look for a initial vertex */
       return face;
   }
}

}


