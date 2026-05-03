/**
 * \file codac2_Polytope_util.h utilities for polytope 
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <cmath>
#include <bitset>
#include <memory>
#include <map>
#include <forward_list>
#include <set>
#include <stdexcept>

#include "codac2_Index.h"
#include "codac2_Vector.h"
#include "codac2_Row.h"
#include "codac2_Facet.h"
#include "codac2_Polytope_dd.h"


namespace codac2 {

/** read a .ine file and return a list of facets
 *  this is a very crude function, which assumes the format of the 
 *  file is correct. Linearities are not treated for now.
 *  @param filename name of the file 
 *  @return the CollectFacets structure containing the facets */
CollectFacets read_ineFile(const char *filename);

/** read a .ext file and return a list of vertices
 *  this is a very crude function, which assumes the format of the 
 *  file is correct. Rays are not treated for now.
 *  @param filename name of the file
 *  @return a set of vertices */
std::vector<Vector> read_extFile(const char *filename);

/** construct the list of facets of a 3D-polyhedron from the buildF2V 
 * structure 
 * use the mid of each vertex. For rays/line use an arbitrary bound 
 * @param build the buildF2V structure 
 * @param bound the bound for rays/line (not correctly handled)
 * @return the set of set of vertices
 */
std::vector<std::vector<Vector>> build_3Dfacets(const DDbuildF2V &build, double bound=50.0);
   
/** construct the facet of a 2D-polyhedron from the buildF2V 
 * structure 
 * use the mid of each vertex. For rays/line use an arbitrary bound 
 * @param build the buildF2V structure 
 * @param bound the bound for rays/line (not correctly handled)
 * @return the set of set of vertices
 */
std::vector<Vector> build_2Dfacet(const DDbuildF2V &build, double bound=50.0);
   
}

