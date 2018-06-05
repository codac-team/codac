/* ============================================================================
 *  tubex-lib - TubeComponent class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TubeComponent_HEADER
#define TubeComponent_HEADER

#include "ibex.h"
#include "tubex_Trajectory.h"
#include <vector>

namespace tubex
{
  class TubeSlice;
  class TubeNode;
  class Tube;

  class TubeComponent
  {
    public:

    /** Base: **/

      // Definition
      TubeComponent(const ibex::Interval& domain, const ibex::Interval& codomain = ibex::Interval::ALL_REALS);
      TubeComponent(const TubeComponent& x);
      ~TubeComponent();
      TubeComponent& operator=(const TubeComponent& x);
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
      Tube* tubeReference() const;
      virtual TubeNode* getParentOf(TubeComponent* component) = 0;

      // Access values
      virtual const ibex::Interval& codomain() const = 0;
      virtual double volume() const = 0;
      virtual const ibex::Interval operator[](int slice_id) const = 0;
      virtual const ibex::Interval operator[](double t) const = 0;
      virtual const ibex::Interval operator[](const ibex::Interval& t) const = 0;
      virtual ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& search_domain = ibex::Interval::ALL_REALS) const = 0;
      virtual const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const = 0;

      // Tests
      virtual bool operator==(const TubeComponent& x) const;
      virtual bool operator!=(const TubeComponent& x) const;
      virtual bool isSubset(const TubeComponent& x) const;
      virtual bool isStrictSubset(const TubeComponent& x) const;
      virtual bool isEmpty() const;
      virtual bool encloses(const Trajectory& x) const;

      // Setting values
      virtual void set(const ibex::Interval& y) = 0;
      virtual void setEmpty() = 0;
      virtual TubeComponent& inflate(double rad) = 0;

      // Operators

      // String
      friend std::ostream& operator<<(std::ostream& str, const TubeComponent& x);

    /** Integration: **/

    protected:

    /** Base: **/

      // Definition
      virtual void setTubeReference(Tube *tube) = 0;

      // Slices structure
      virtual void updateSlicesNumber() = 0;

      // Access values
      virtual void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const = 0;

      // Tests

      // Setting values
      virtual void checkData() const = 0;
      virtual void flagFutureDataUpdateFromRoot(int slice_id = -1) const = 0;

      // Operators

      // String

    /** Integration: **/

      virtual void checkPartialPrimitive() const = 0;
      virtual const std::pair<ibex::Interval,ibex::Interval>& getPartialPrimitiveValue() const = 0;
      virtual std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& t) const = 0;
      virtual void flagFuturePrimitiveUpdateFromRoot(int slice_id = -1) const = 0;
      
    /** Class variables **/

      // Subtube structure (no mutable needs)
      ibex::Interval m_domain;
      short int m_slices_number = 1;

      // Subtube attributes ('mutable' required: values may be updated from const methods)
      mutable ibex::Interval m_codomain;
      mutable double m_volume;
      mutable std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
      mutable bool m_primitive_update_needed = true;
      mutable bool m_data_update_needed = true;

      Tube *m_tube_ref = NULL; // a reference to the tube owning the node (used for data-structure's auto updates)

      friend class TubeNode;
      friend class TubeSlice; // todo: remove this? not sure
      friend class Tube; // todo: remove this? not sure
      friend class CtcDeriv; // todo: remove this
      friend class EmptyException; // todo: remove this
      friend void deserializeTube(std::ifstream& bin_file, Tube& tube);
  };
}

#endif