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

#ifndef TubeTree_HEADER
#define TubeTree_HEADER

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
      TubeTree(const TubeTree& x, const ibex::Interval& codomain);
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

      // Access values
      const ibex::Interval& codomain() const;
      double volume() const;
      const ibex::Interval& operator[](int slice_id) const;
      const ibex::Interval operator[](double t) const;
      const ibex::Interval operator[](const ibex::Interval& t) const;
      const ibex::Interval interpol(double t, const TubeTree& derivative) const;
      const ibex::Interval interpol(const ibex::Interval& t, const TubeTree& derivative) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      double maxThickness();
      double maxThickness(int& first_id_max_thickness);
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool operator==(const TubeTree& x) const;
      bool operator!=(const TubeTree& x) const;
      bool isSubset(const TubeTree& x) const;
      bool isStrictSubset(const TubeTree& x) const;
      bool isEmpty() const;
      bool encloses(const Trajectory& x) const;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void set(const ibex::Interval& y, double t);
      void set(const ibex::Interval& y, const ibex::Interval& t);
      void setEmpty();
      void setGate(double t, const ibex::Interval& gate);
      TubeTree& inflate(double rad);

      // Operators
      TubeTree& operator|=(const Trajectory& x);
      TubeTree& operator|=(const TubeTree& x);
      TubeTree& operator&=(const Trajectory& x);
      TubeTree& operator&=(const TubeTree& x);

    /** Integration: **/

    protected:

    /** Base: **/

      // Definition
      
      // Slices/tree structure
      void checkDataTree() const;
      void flagFutureTreeUpdate(int slice_id = -1) const;
      bool treeUpdateNeeded() const;

      // Access values
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      // Tests
      static bool nodesAreEqual(const TubeNode* node1, const TubeNode* node2);
      bool isEqual(const TubeTree& x) const;
      static bool nodesAreDifferent(const TubeNode* node1, const TubeNode* node2);
      bool isDifferent(const TubeTree& x) const;

      // Setting values

      // Operators

      // String

    /** Integration: **/

      void checkPartialPrimitive() const;
      void flagFuturePrimitiveUpdate(int slice_id = -1) const;

    /** Class variables **/

      TubeNode *m_first_tubenode = NULL, *m_second_tubenode = NULL;
      mutable bool m_tree_update_needed = true;
  };
}

#endif