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

#include "tubex_TubeComponent.h"

namespace tubex
{
  class TubeNode : public TubeComponent
  {
    public:

    /** Base: **/

      // Definition
      TubeNode(const TubeSlice& slice, double t);
      TubeNode(const TubeNode& x);
      TubeNode(const TubeNode& x, const ibex::Interval& codomain);
      ~TubeNode();
      TubeNode& operator=(const TubeNode& x);

      // Slices structure
      bool isSlice() const;
      TubeComponent* getFirstTubeComponent() const;
      TubeComponent* getSecondTubeComponent() const;
      TubeSlice* getSlice(int slice_id);
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(int slice_id) const;
      const TubeSlice* getSlice(double t) const;
      void getSlices(std::vector<const TubeSlice*>& v_slices) const;
      int input2index(double t) const;
      void getTubeComponents(std::vector<const TubeComponent*> &v_nodes) const;
      TubeNode* getParentOf(TubeSlice* slice);

      // Access values
      const ibex::Interval& codomain() const;
      double volume() const;
      const ibex::Interval& operator[](int slice_id) const;
      const ibex::Interval operator[](double t) const;
      const ibex::Interval operator[](const ibex::Interval& t) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;

      // Tests
      bool operator==(const TubeNode& x) const;
      bool operator!=(const TubeNode& x) const;
      bool isSubset(const TubeNode& x) const;
      bool isStrictSubset(const TubeNode& x) const;
      bool isEmpty() const;
      bool encloses(const Trajectory& x) const;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void setEmpty();
      TubeNode& inflate(double rad);

    /** Integration: **/

    protected:

    /** Base: **/

      // Definition
      void setTubeReference(Tube *tube_ref);

      // Slices structure
      void updateSlicesNumber();
      
      // Slices/tree structure

      // Access values
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      // Tests
      static bool nodesAreEqual(const TubeComponent* node1, const TubeComponent* node2);
      static bool nodesAreDifferent(const TubeComponent* node1, const TubeComponent* node2);

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
      TubeComponent *m_first_tubenode = NULL, *m_second_tubenode = NULL;

      friend class TubeSlice;
      friend class CtcDeriv;
  };
}

#endif