#ifndef INCLUDE_TUBEX_CAPD2TUBEX_H
#define INCLUDE_TUBEX_CAPD2TUBEX_H


#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "ibex.h"
#include "capd/capdlib.h"
#include "capd/mpcapdlib.h"
#include "math.h"
#include "tubex_TubeVector.h"
#include "json/json.hpp"

namespace tubex
{
    /** \brief  Returns a std::vector<ibex::IntervalVector> corresponding to the guaranteed curve computed by CAPD
     * \param j json file that gives the parameter to CAPD to perform the guaranteed integration
     */
    std::vector<ibex::IntervalVector> capd2ibex(nlohmann::json j);
    /** \brief Convert a std::vector<ibex::IntervalVector> corresponding to the guaranteed curve computed by CAPD into a
     * tubex::TubeVector
     * \param ibex_curve a vector of ibex::IntervalVectors representing the curve computed by CAPD
     * */
    tubex::TubeVector ibex2tubex(std::vector<ibex::IntervalVector> ibex_curve);
    /** \brief Combination of capd2ibex and ibex2tubex, to generate a tube from a curve obtained by
     * the guaranteed integration of CAPD
     * \param j json file that gives the parameter to CAPD to perform the guaranteed integration
     */
    tubex::TubeVector capd2tubex (nlohmann::json j);
}

#endif // INCLUDE_TUBEX_CAPD2TUBEX_H