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

#ifndef TubeNode_HEADER
#define TubeNode_HEADER

#include "ibex.h"
#include "tubex_Trajectory.h"
#include <vector>

namespace tubex
{
  class TubeSlice;
  class Tube;

  class TubeNode
  {
    public:

    /** Base: **/

      // Definition
      TubeNode(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      TubeNode(const TubeNode& x);
      ~TubeNode();
      TubeNode& operator=(const TubeNode& x);
      const ibex::Interval& domain() const;

      // Slices structure
      int nbSlices() const;
      virtual bool isSlice() const = 0;
      virtual TubeSlice* getSlice(int slice_id) = 0;
      virtual const TubeSlice* getSlice(int slice_id) const = 0;
      virtual TubeSlice* getSlice(double t) = 0;
      virtual const TubeSlice* getSlice(double t) const = 0;
      TubeSlice* getFirstSlice() const;
      TubeSlice* getLastSlice() const;
      virtual void getSlices(std::vector<const TubeSlice*>& v_slices) const = 0;
      virtual int input2index(double t) const = 0;
      double index2input(int slice_id) const;
      const ibex::IntervalVector sliceBox(int slice_id) const;
      const ibex::IntervalVector sliceBox(double t) const;
      const ibex::Interval& sliceDomain(int slice_id) const;
      const ibex::Interval& sliceDomain(double t) const;
      virtual void getTubeNodes(std::vector<const TubeNode*> &v_nodes) const = 0;

      // Access values
      virtual const ibex::Interval& codomain() const = 0;
      virtual double volume() const = 0;
      virtual const ibex::Interval operator[](double t) const = 0;
      virtual const ibex::Interval operator[](const ibex::Interval& t) const = 0;
      /*ibex::Interval interpol(double t, const TubeNode& derivative) const;
      ibex::Interval interpol(const ibex::Interval& t, const TubeNode& derivative) const;
      std::pair<ibex::Interval,ibex::Interval> partialInterpol(const ibex::Interval& t, const TubeNode& derivative) const;
      */virtual ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const = 0;
      virtual const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const = 0;

      // Tests
      virtual bool isEmpty() const = 0;
      virtual bool encloses(const Trajectory& x) const = 0;

      // Setting values
      virtual void set(const ibex::Interval& y) = 0;
      virtual void setEmpty() = 0;
      virtual TubeNode& inflate(double rad) = 0;

      // Operators

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeNode& x);

    /** Integration: **/

    protected:

    /** Base: **/

      // Definition

      // Access values
      virtual void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const = 0;

      // Tests
      bool isEqual(const TubeNode& x) const;
      bool isDifferent(const TubeNode& x) const;

      // Setting values

      // Operators

      // String

    /** Integration: **/

      virtual void checkPartialPrimitive() const = 0;
      virtual const std::pair<ibex::Interval,ibex::Interval>& getPartialPrimitiveValue() const = 0;
      virtual std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& t) const = 0;
      virtual void flagFuturePrimitiveUpdate(int slice_id = -1) const = 0;
      
    /** Class variables **/

      // Subtube structure (no mutable needs)
      ibex::Interval m_domain;
      short int m_slices_number = 1;

      // Subtube attributes ('mutable' required: values may be updated from const methods)
      mutable ibex::Interval m_codomain;
      mutable double m_volume;
      mutable std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
      mutable std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
      mutable bool m_primitive_update_needed = true;

      friend class TubeTree;
      friend void deserializeTube(std::ifstream& bin_file, Tube& tube);
  };
}

#endif