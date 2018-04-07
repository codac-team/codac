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
      Tube(const ibex::Interval& domain, const ibex::Interval& value = ibex::Interval::ALL_REALS);
      Tube(double timestep, const ibex::Interval& domain, const ibex::Interval& value = ibex::Interval::ALL_REALS);
      Tube(const Tube& x, const ibex::Interval& value);
      Tube(const std::string& binary_file_name);
      Tube(const std::string& binary_file_name, Trajectory& traj);
      Tube(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);
      Tube primitive(const ibex::Interval& initial_value = ibex::Interval(0.)) const;
      std::pair<Tube,Tube> bisect(const Tube& derivative, double t, float ratio = 0.55) const;

      // Slices structure
      int nbSlices() const;
      const ibex::IntervalVector sliceBox(int slice_id) const;
      bool isSlice() const;
      TubeSlice* getSlice(int slice_id);
      const TubeSlice* getSlice(int slice_id) const;
      TubeSlice* getSlice(double t);
      const TubeSlice* getSlice(double t) const;
      void getSlices(std::vector<const TubeSlice*>& v_slices) const;
      int input2index(double t) const;
      double index2input(int slice_id) const;
      const ibex::Interval& sliceDomain(int slice_id) const;
      const ibex::Interval& sliceDomain(double t) const;

      void sample(int subslices_nb);
      void sample(const std::vector<ibex::Interval>& v_slices_domains);
      void sample(double t, const ibex::Interval& gate = ibex::Interval::ALL_REALS);
      TubeSlice* nextSlice() const;
      void setNextSlice(TubeSlice *next_slice);
      TubeSlice* prevSlice() const;
      void setPrevSlice(TubeSlice *prev_slice);
      const ibex::Interval inputGate() const;
      const ibex::Interval outputGate() const;
      void setInputGate(const ibex::Interval& input_gate);
      void setOuputGate(const ibex::Interval& output_gate);

      // Access values
      double volume() const;
      double dist(const Tube& x) const;
      const ibex::Interval& image() const;
      const ibex::Interval& operator()(int slice_id) const;
      ibex::Interval operator()(double t) const;
      ibex::Interval operator()(const ibex::Interval& t) const;
      const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      ibex::Interval interpol(double t, const Tube& derivative) const;
      ibex::Interval interpol(const ibex::Interval& t, const Tube& derivative) const;
      std::pair<ibex::Interval,ibex::Interval> partialInterpol(const ibex::Interval& t, const Tube& derivative) const;
      ibex::Interval invert(const ibex::Interval& y, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& t = ibex::Interval::ALL_REALS) const;
      double maxThickness() const;
      double maxThickness(int& first_id_max_thickness) const;

      // Tests
      bool isEmpty() const;
      bool isInteriorSubset(const Tube& outer_tube) const;
      bool encloses(const Trajectory& x) const;

      // Setting values
      void set(const ibex::Interval& y);
      void set(const ibex::Interval& y, int slice_id);
      void setEmpty();
      bool contract(const ibex::Interval& y, int slice_id);
      Tube& inflate(const ibex::Interval& y, int slice_id);
      Tube& inflate(const ibex::Interval& y, double t);
      Tube& inflate(double rad);

      // Operators
      Tube& operator=(const Tube& x);
      Tube& operator|=(const Tube& x);
      Tube& operator|=(const Trajectory& x);
      Tube& operator&=(const Tube& x);

      // String
      friend std::ostream& operator<<(std::ostream& str, const Tube& x);


    /** Contractors: see file tubex_Tube_contractors.cpp **/

      bool ctcFwd(const Tube& derivative, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);
      bool ctcBwd(const Tube& derivative);
      bool ctcFwdBwd(const Tube& derivative, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);
      bool ctcEval(const Tube& derivative, ibex::Interval& t, ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, ibex::Interval& t, const ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, const ibex::Interval& t, ibex::Interval& z, bool propagate = true);
      bool ctcEval(const Tube& derivative, const ibex::Interval& t, const ibex::Interval& z, bool propagate = true);
      bool ctcOut(const ibex::Interval& t, const ibex::Interval& z);
      bool ctcIntertemporal(ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcIntertemporal(ibex::Interval& z, ibex::Interval& t1, ibex::Interval& t2) const;
      bool ctcPeriodic(const ibex::Interval& period);

    /** Serialization: see file tubex_Tube_serialization.cpp **/

      bool serialize(const std::string& binary_file_name = "x.tube", int version_number = SERIALIZATION_VERSION) const;
      bool serialize(const std::string& binary_file_name, const Trajectory& traj, int version_number = SERIALIZATION_VERSION) const;
      bool serialize(const std::string& binary_file_name, const std::vector<const Trajectory&>& v_trajs, int version_number = SERIALIZATION_VERSION) const;
      void deserialize(const std::string& binary_file_name, Trajectory& traj);
      void deserialize(const std::string& binary_file_name, std::vector<Trajectory>& v_trajs);

    /** Integration: see file tubex_Subtube_integration.cpp **/

      ibex::Interval integral(double t) const;
      ibex::Interval integral(const ibex::Interval& t) const;
      ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;
      std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;

    protected:

    /** Base: see file tubex_Subtube_base.cpp **/

      // Definition
      Tube(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);
      void createTube(const std::vector<ibex::Interval>& v_slices_domains, const ibex::Interval& value);

      // Slices/tree structure
      const Tube* getFirstSubtube() const;
      const Tube* getSecondSubtube() const;
      void getSubtubeNodes(std::vector<Tube*> &v_nodes);
      void getSubtubeNodes(std::vector<const Tube*> &v_nodes) const;
      void updateTree() const;
      void flagFutureTreeUpdate(int slice_id = -1) const;
      bool treeUpdateNeeded() const;

      // Access values
      void invert(const ibex::Interval& y, std::vector<ibex::Interval> &v_t, const ibex::Interval& search_domain, bool concatenate_results) const;

      // Tests

      // Setting values

      // Operators
      void unionWith_localUpdate(const Tube *x);
      void intersectWith_localUpdate(const Tube *x);

      // String

    /** Contractors: see file tubex_Tube_contractors.cpp **/

      void ctcEval_computeIndex(const ibex::Interval& z, const ibex::Interval& t, int& index_lb, int& index_ub);

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