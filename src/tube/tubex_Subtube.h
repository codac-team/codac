/* ============================================================================
 *  tubex-lib - Subtube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef SUBTUBE_HEADER
#define SUBTUBE_HEADER

#include <map>
#include <vector>
#include <utility>
#include "ibex_IntervalVector.h"
#include "ibex_Function.h"
#include "tubex_Trajectory.h"

#define SERIALIZATION_VERSION 1

namespace tubex
{
  class Subtube
  {
    public:

    /** Base: see file tubex_Subtube_base.cpp **/

      // Definition
      Subtube();
      Subtube(const ibex::Interval& domain, const ibex::Interval& value);
      Subtube(const ibex::Interval& domain, double timestep, const ibex::Interval& value);
      Subtube(const Subtube& x);
      Subtube(const Subtube& x, const ibex::Interval& value);
      ~Subtube();
      const ibex::Interval& domain() const;

      // Slices structure
      int nbSlices() const;
      const ibex::IntervalVector sliceBox(int slice_id) const;
      bool isSlice() const;
      Subtube* getSlice(int slice_id);
      const Subtube* getSlice(int slice_id) const;
      Subtube* getSlice(double t);
      const Subtube* getSlice(double t) const;
      void getSlices(std::vector<const Subtube*>& v_slices) const;
      int input2index(double t) const;
      double index2input(int slice_id) const;
      const ibex::Interval& sliceDomain(int slice_id) const;
      const ibex::Interval& sliceDomain(double t) const;
      void sampleSlice(double timestep = 0.);
      void sampleSlice(const std::vector<ibex::Interval>& v_slices_domains);

      // Access values
      double volume() const;
      double dist(const Subtube& x) const;
      const ibex::Interval& image() const;
      const ibex::Interval& operator[](int slice_id) const;
      ibex::Interval operator[](double t) const;
      ibex::Interval operator[](const ibex::Interval& t) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      ibex::Interval interpol(double t, const Subtube& derivative) const;
      ibex::Interval interpol(const ibex::Interval& t, const Subtube& derivative) const;
      std::pair<ibex::Interval,ibex::Interval> partialInterpol(const ibex::Interval& t, const Subtube& derivative) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      double maxThickness() const;
      double maxThickness(int& first_id_max_thickness) const;

      // Tests
      bool isEmpty() const;
      bool operator==(const Subtube& x) const;
      bool operator!=(const Subtube& x) const;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void setEmpty();
      bool contract(const ibex::Interval& y, int slice_id);
      Subtube& inflate(const ibex::Interval& y, int slice_id);
      Subtube& inflate(const ibex::Interval& y, double t);
      Subtube& inflate(double rad);

      // Operators
      Subtube& operator=(const Subtube& x);
      Subtube& operator|=(const Subtube& x);
      Subtube& operator|=(const Trajectory& x);
      Subtube& operator&=(const Subtube& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const Subtube& x);

    /** Integration: see file tubex_Subtube_integration.cpp **/

      ibex::Interval integral(double t) const;
      ibex::Interval integral(const ibex::Interval& t) const;
      ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    protected:

    /** Base: see file tubex_Subtube_base.cpp **/

      // Definition
      Subtube(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);

      // Slices/tree structure
      const Subtube* getFirstSubtube() const;
      const Subtube* getSecondSubtube() const;
      void getSubtubeNodes(std::vector<Subtube*> &v_nodes);
      void getSubtubeNodes(std::vector<const Subtube*> &v_nodes) const;
      void updateTree() const;
      void flagFutureTreeUpdate(int slice_id = -1) const;
      bool treeUpdateNeeded() const;

      // Access values
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      // Tests

      // Setting values

      // Operators
      void unionWith_localUpdate(const Subtube *x);
      void intersectWith_localUpdate(const Subtube *x);

      // String

    /** Integration: see file tubex_Subtube_integration.cpp **/

      void computePartialPrimitive(bool build_from_leafs = false) const;
      void flagFuturePrimitiveComputation() const;
      bool primitiveUpdateNeeded() const;
      std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      
    /** Class variables **/

      // Subtube structure (no mutable needs)
      ibex::Interval m_domain;
      Subtube *m_first_subtube = NULL, *m_second_subtube = NULL;
      short int m_slices_number = 1;

      // Subtube attributes ('mutable' required: values may be updated from const methods)
      mutable double m_volume;
      mutable ibex::Interval m_image;
      mutable std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
      mutable std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
      mutable bool m_tree_computation_needed = true;
      mutable bool m_primitive_computation_needed = true;
  };
}

#endif