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
  Tube::Tube(const Interval& domain, double timestep, const Interval& value)
  {

  }

  Tube::Tube(const Interval& domain, const Interval& value)
  {

  }
  
  Tube::Tube(const Tube& x, const Interval& value)
  {

  }
  
  Tube::Tube(const string& binary_file_name)
  {

  }
  
//  Tube::Tube(const string& binary_file_name, map<double,double> &real_values)
//  {
//
//  }
//  
//  Tube::Tube(const string& binary_file_name, vector<map<double,double> > &v_real_values)
//  {
//
//  }
//  
//  Tube::Tube(const Interval& domain, double timestep, const Function& fmin, const Function& fmax)
//  {
//
//  }
//  
//  Tube::Tube(const Interval& domain, double timestep, const Function& f, const Interval& thickness)
//  {
//
//  }
  
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
  
  bool Tube::operator==(const Tube& x) const
  {

  }
  
  bool Tube::operator!=(const Tube& x) const
  {

  }

  Interval Tube::operator[](double t) const
  {

  }

  Interval Tube::operator[](const Interval& t) const
  {
    return ((Subtube)*this)[t];
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
  
  void Tube::set(const Interval& y, int slice_id)
  {

  }

  void Tube::setEmpty()
  {

  }
  
  bool Tube::contract(const Interval& y, int slice_id)
  {

  }
  
  Tube& inflate(const Interval& y, int slice_id)
  {

  }
  
  Tube& inflate(const Interval& y, double t)
  {

  }

//  void Tube::feed(const map<double,Interval>& map_values)
//  {
//
//  }
//  
//  void Tube::feed(const map<double,double>& map_values, const map<double,double>& map_stddev)
//  {
//
//  }
//  
//  void Tube::feed(const map<double,double>& map_values, const Interval& intv_uncertainty)
//  {
//
//  }

  Tube& Tube::inflate(double rad)
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

  Interval Tube::invert(const Interval& y, const Interval& t) const
  {

  }

  void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& t) const
  {

  }

  Tube Tube::primitive(const Interval& initial_value) const
  {

  }

  void Tube::sample(int slice_id)
  {

  }
  
  pair<Tube,Tube> Tube::bisect(const Tube& derivative, double t, float ratio) const
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

  const string& Tube::name() const
  {

  }

  void Tube::setName(const string& name)
  {

  }
  
  ostream& operator<<(ostream& str, const Tube& x)
  {

  }

  void Tube::createFromSpecifications(const Interval& domain, double timestep, const Interval& value)
  {

  }
  
  void Tube::createTube(const vector<Interval>& v_slices_domains, const Interval& value)
  {

  }
  
  void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain, bool concatenate_results) const
  {

  }
}