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
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(int slice_id) const;
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

    /** Integration: **/

    protected:

    /** Base: **/

      // Definition
      
      // Slices/tree structure

      // Access values
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      // Tests
      static bool nodesAreEqual(const TubeNode* node1, const TubeNode* node2);
      bool isEqual(const TubeTree& x) const;
      static bool nodesAreDifferent(const TubeNode* node1, const TubeNode* node2);
      bool isDifferent(const TubeTree& x) const;

      // Setting values
      void checkData() const;
      void flagFutureDataUpdateFromRoot(int slice_id = -1) const;

      // Operators

      // String

    /** Integration: **/

      void checkPartialPrimitive() const;
      void flagFuturePrimitiveUpdateFromRoot(int slice_id = -1) const;
      const std::pair<ibex::Interval,ibex::Interval>& getPartialPrimitiveValue() const;
      std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& t) const;

    /** Class variables **/

      mutable std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
      TubeNode *m_first_tubenode = NULL, *m_second_tubenode = NULL;

      friend class TubeSlice;
      friend class CtcDeriv;
  };
}

#endif