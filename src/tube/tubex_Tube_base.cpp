/* ============================================================================
 *  tubex-lib - Tube class (base methods)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "exceptions/tubex_Exception.h"
#include "exceptions/tubex_DomainException.h"
#include "exceptions/tubex_EmptyException.h"
#include "ibex.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition
    
      Tube::Tube(const Interval& domain, const Interval& value) : Subtube(domain, value)
      {

      }

      Tube::Tube(const Interval& domain, double timestep, const Interval& value) : Subtube(domain, timestep, value)
      {

      }
    
      Tube::Tube(const Tube& x, const Interval& value) : Subtube(x, value)
      {

      }
    
      Tube::Tube(const string& binary_file_name)
      {

      }
    
      Tube::Tube(const string& binary_file_name, Trajectory& traj)
      {

      }
    
      Tube::Tube(const string& binary_file_name, vector<Trajectory>& v_trajs)
      {

      }
    
      Tube Tube::primitive(const Interval& initial_value) const
      {

      }
    
      pair<Tube,Tube> Tube::bisect(const Tube& derivative, double t, float ratio) const
      {

      }

    // Slices structure

      void Tube::sample(int slice_id)
      {

      }
    
      const Interval& Tube::inputGate(int slice_id) const
      {

      }
    
      const Interval& Tube::outputGate(int slice_id) const
      {

      }
    
      void Tube::setInputGate(int slice_id)
      {

      }
    
      void Tube::setOutputGate(int slice_id)
      {

      }

    // Access values

      Interval Tube::operator[](double t) const
      {

      }
    
      Interval Tube::operator[](const Interval& t) const
      {

      }
    
      const pair<Interval,Interval> Tube::eval(const Interval& t) const
      {

      }
    
      Interval Tube::interpol(double t, const Tube& derivative) const
      {

      }
    
      Interval Tube::interpol(const Interval& t, const Tube& derivative) const
      {

      }
    
      pair<Interval,Interval> Tube::partialInterpol(const Interval& t, const Tube& derivative) const
      {

      }
    
      Interval Tube::invert(const Interval& y, const Interval& t) const
      {

      }
    
      void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& t) const
      {

      }

    // Tests

      bool Tube::operator==(const Tube& x) const
      {

      }
    
      bool Tube::operator!=(const Tube& x) const
      {

      }
    
      bool Tube::isEmpty() const
      {

      }
    
      bool Tube::isInteriorSubset(const Tube& outer_tube) const
      {

      }
    
      bool Tube::encloses(const Trajectory& x) const
      {

      }

    // Setting values

      void Tube::set(const Interval& y)
      {

      }

      void Tube::set(const Interval& y, int slice_id)
      {

      }
    
      void Tube::setEmpty()
      {

      }
    
      bool Tube::contract(const Interval& y, int slice_id)
      {

      }
    
      const Interval Tube::inflate(const Interval& y, int slice_id)
      {

      }
    
      const Interval Tube::inflate(const Interval& y, double t)
      {

      }
    
      Tube& Tube::inflate(double rad)
      {

      }

    // Operators

      Tube& Tube::operator=(const Tube& x)
      {
        
      }

      Tube& Tube::operator|=(const Tube& x)
      {
        
      }

      Tube& Tube::operator|=(const Trajectory& x)
      {
        
      }

      Tube& Tube::operator&=(const Tube& x)
      {
        
      }

    // String

      const string& Tube::name() const
      {

      }
    
      void Tube::setName(const string& name)
      {
        
      }

      ostream& operator<<(ostream& str, const Tube& x)
      {
        
      }

  // Protected methods

    // Definition

      void Tube::createTube(const vector<Interval>& v_slices_domains, const Interval& value)
      {
        
      }

    // Access values

      void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
      {

      }
}