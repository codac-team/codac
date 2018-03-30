/* ============================================================================
 *  tubex-lib - TubeNode class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TUBENODE_HEADER
#define TUBENODE_HEADER

#include "ibex.h"
#include "tubex_Trajectory.h"
#include <vector>

namespace tubex
{
  class TubeSlice;

  class TubeNode
  {
    public:

    /** Base: **/

      // Definition
      TubeNode(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      TubeNode(const TubeNode& x, const ibex::Interval& codomain);
      TubeNode(const TubeNode& x);
      const ibex::Interval& domain() const;

      // Slices structure
      int nbSlices() const;
      virtual bool isSlice() const = 0;
      virtual TubeSlice* getSlice(int slice_id) = 0;
      virtual const TubeSlice* getSlice(int slice_id) const = 0;
      virtual TubeSlice* getSlice(double t) = 0;
      virtual const TubeSlice* getSlice(double t) const = 0;
      virtual void getSlices(std::vector<const TubeSlice*>& v_slices) const = 0;
      virtual int input2index(double t) const = 0;
      double index2input(int slice_id) const;
      virtual const ibex::IntervalVector sliceBox(int slice_id) const = 0;
      virtual const ibex::Interval& sliceDomain(int slice_id) const = 0;
      virtual const ibex::Interval& sliceDomain(double t) const = 0;

      // Access values
      virtual const ibex::Interval& codomain() const = 0;
      virtual double volume() const = 0;
      double dist(const TubeNode& x) const;
      const ibex::Interval& operator()(int slice_id) const;
      ibex::Interval interpol(double t, const TubeNode& derivative) const;
      ibex::Interval interpol(const ibex::Interval& t, const TubeNode& derivative) const;
      std::pair<ibex::Interval,ibex::Interval> partialInterpol(const ibex::Interval& t, const TubeNode& derivative) const;
      virtual ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& t = ibex::Interval::ALL_REALS) const = 0;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      double maxThickness() const;
      virtual double maxThickness(int& first_id_max_thickness) const = 0;
      virtual const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const = 0;

      // Tests
      bool isEmpty() const;
      virtual bool isInteriorSubset(const TubeNode& outer_tube) const = 0;
      virtual bool encloses(const Trajectory& x) const = 0;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void setEmpty();
      bool contract(const ibex::Interval& y, int slice_id);
      TubeNode& inflate(const ibex::Interval& y, int slice_id);
      TubeNode& inflate(const ibex::Interval& y, double t);
      TubeNode& inflate(double rad);

      // Operators
      TubeNode& operator=(const TubeNode& x);
      TubeNode& operator|=(const TubeNode& x);
      TubeNode& operator|=(const Trajectory& x);
      TubeNode& operator&=(const TubeNode& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeNode& x);

    /** Integration: **/

      ibex::Interval integral(double t) const;
      ibex::Interval integral(const ibex::Interval& t) const;
      ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    protected:

    /** Base: **/

      // Access values
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      // Tests

      // Setting values

      // Operators
      void unionWith_localUpdate(const TubeNode *x);
      void intersectWith_localUpdate(const TubeNode *x);

      // String

    /** Integration: **/

      std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      
    /** Class variables **/

      // Subtube structure (no mutable needs)
      const ibex::Interval m_domain;
      short int m_slices_number = 1;

      // Subtube attributes ('mutable' required: values may be updated from const methods)
      mutable ibex::Interval m_codomain;
      mutable double m_volume;
  };
}

#endif