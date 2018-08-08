/* ============================================================================
 *  tubex-lib - TubeVectorComponent class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeVectorComponent.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  TubeVectorComponent::TubeVectorComponent()
    : Tube(NULL, -1)
  {
    m_destroy_tubevector = false;
  }

  TubeVectorComponent::TubeVectorComponent(TubeVector *tubevector, int component_id)
    : Tube(tubevector, component_id)
  {
    cout << "constructor TubeVectorComponent" << endl;
    cout << "-- " << *m_tubevector << endl;
    cout << "-- " << m_tubevector->domain() << endl;
    m_destroy_tubevector = false;
  }

  TubeVectorComponent::~TubeVectorComponent()
  {
    
  }
}