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
      TubeTree(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      TubeTree(const TubeTree& x);
      //TubeTree(const TubeTree& x, const ibex::Interval& codomain);
      ~TubeTree();
      TubeTree& operator=(const TubeTree& x);

      // Slices structure
      bool isSlice() const;
      TubeNode* getFirstTubeNode() const;
      TubeNode* getSecondTubeNode() const;
      int sample(double t, const ibex::Interval& gate = ibex::Interval::ALL_REALS);
      void sample(const std::vector<double>& v_bounds);
      TubeSlice* getSlice(int slice_id);
      const TubeSlice* getSlice(int slice_id) const;
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(double t) const;
      void getSlices(std::vector<const TubeSlice*>& v_slices) const;
      int input2index(double t) const;
      void getTubeNodes(std::vector<const TubeNode*> &v_nodes) const;
      void setGateValue(double t, const ibex::Interval& gate);

      // Access values
      const ibex::Interval& codomain() const;
      double volume() const;
      const ibex::Interval& operator[](int slice_id) const;
      const ibex::Interval operator[](double t) const;
      const ibex::Interval operator[](const ibex::Interval& t) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      double maxThickness();
      double maxThickness(int& first_id_max_thickness);
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
/*
      // Tests
      bool isInteriorSubset(const TubeNode& outer_tube) const;
      bool encloses(const Trajectory& x) const;*/

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void set(const ibex::Interval& y, double t);
      void set(const ibex::Interval& y, const ibex::Interval& t);
      void setEmpty();

    protected:

    /** Base: **/

      // Definition
      /*TubeTree(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);
      void createTube(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);*/

      // Slices/tree structure
      void checkDataTree() const;
      void flagFutureTreeUpdate(int slice_id = -1) const;
      bool treeUpdateNeeded() const;

      // Access values
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      /*// Tests

      // Setting values

      // Operators

      // String

    /** Integration: **/

      /*void computePartialPrimitive(bool build_from_leafs = false) const;
      void flagFuturePrimitiveComputation() const;
      bool primitiveUpdateNeeded() const;

    /** Class variables **/

      TubeNode *m_first_tubenode = NULL, *m_second_tubenode = NULL;
      mutable bool m_tree_update_needed = true;
      mutable bool m_primitive_update_needed = true;
  };
}

#endif