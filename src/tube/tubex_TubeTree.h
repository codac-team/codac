/* ============================================================================
 *  tubex-lib - TubeTree class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TUBETREE_HEADER
#define TUBETREE_HEADER

#include "tubex_TubeNode.h"

namespace tubex
{
  class TubeTree : public TubeNode
  {
    public:

    /** Base: **/

      // Definition

      // Slices structure
      TubeSlice* getSlice(int slice_id);
      const TubeSlice* getSlice(int slice_id) const;
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(double t) const;
      void getSlices(std::vector<const TubeSlice*>& v_slices) const;
      int input2index(double t) const;
      const ibex::IntervalVector sliceBox(int slice_id) const;
      const ibex::Interval& sliceDomain(int slice_id) const;
      const ibex::Interval& sliceDomain(double t) const;

      // Access values
      const ibex::Interval& codomain() const;
      double volume() const;
      const ibex::Interval& operator()(int slice_id) const;
      ibex::Interval operator()(double t) const;
      ibex::Interval operator()(const ibex::Interval& t) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      double maxThickness(int& first_id_max_thickness) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool isInteriorSubset(const TubeNode& outer_tube) const;
      bool encloses(const Trajectory& x) const;

    protected:

    /** Base: **/

      // Definition
      TubeTree(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);
      void createTube(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);

      // Slices/tree structure
      bool isSlice() const;
      const TubeTree* getFirstSubtube() const;
      const TubeTree* getSecondSubtube() const;
      void getTubeNodes(std::vector<TubeNode*> &v_nodes);
      void getTubeNodes(std::vector<const TubeNode*> &v_nodes) const;
      void updateTree() const;
      void flagFutureTreeUpdate(int slice_id = -1) const;
      bool treeUpdateNeeded() const;

      // Tests

      // Setting values

      // Operators

      // String

    /** Contractors: **/

      void ctcEval_computeIndex(const ibex::Interval& z, const ibex::Interval& t, int& index_lb, int& index_ub);

    /** Integration: **/

      void computePartialPrimitive(bool build_from_leafs = false) const;
      void flagFuturePrimitiveComputation() const;
      bool primitiveUpdateNeeded() const;

    /** Class variables **/

      TubeNode *m_first_tubenode = NULL, *m_second_tubenode = NULL;
      mutable std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
      mutable std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
      mutable bool m_tree_update_needed = true;
      mutable bool m_primitive_update_needed = true;
  };
}

#endif