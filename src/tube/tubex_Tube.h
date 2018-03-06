/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TUBEX_HEADER
#define TUBEX_HEADER

#include <map>
#include <vector>
#include <utility>
#include "ibex_IntervalVector.h"
#include "ibex_Function.h"

namespace tubex
{
  class Tube
  {
    public:

      /** Basic methods **/

        /**
         * \brief Default constructor
         */
        Tube();

        /**
         * \brief Create a tube over the given domain with some constant timestep.
         *
         * Values are set by default to [-oo,oo].
         *
         * \param domain tube's domain
         * \param timestep tube's precision corresponding to slices width
         * \param default_value default image values
         */
        Tube(const ibex::Interval& domain,
             double timestep,
             const ibex::Interval& default_value = ibex::Interval::ALL_REALS);

        /**
         * \brief Create a one-slice tube over the given domain.
         *
         * \param domain tube's domain
         * \param value image value
         */
        Tube(const ibex::Interval& domain,
             const ibex::Interval& value);

        /**
         * \brief Create a tube from an interval of two functions.
         * 
         * Note: fmin <= fmax
         *
         * \param domain tube's domain
         * \param timestep tube's precision corresponding to slices' constant width
         * \param fmin lower-bound ibex function
         * \param fmin upper-bound ibex function
         */
        Tube(const ibex::Interval& domain,
             double timestep,
             const ibex::Function& fmin, const ibex::Function& fmax);

        /**
         * \brief Create a tube as an envelope of an uncertain functions.
         *
         * \param domain tube's domain
         * \param timestep tube's precision corresponding to slices' constant width
         * \param f the ibex function to enclose
         * \param thickness the bounded uncertainty centered on f's values, [0.] by default
         */
        Tube(const ibex::Interval& domain,
             double timestep,
             const ibex::Function& f, const ibex::Interval& thickness = ibex::Interval::EMPTY_SET);

        /**
         * \brief Create a copy of the given tube tu.
         *
         * \param tu the tube to be copied
         */
        Tube(const Tube& tu);

        /**
         * \brief Create a copy of the given tube tu with custom values.
         *
         * Useful when working with tubes sharing the same structure.
         *
         * \param tu the tube to be copied
         * \param image custom image values, constant for all slices
         */
        Tube(const Tube& tu, const ibex::Interval& image);

        /**
         * \brief Deserialize a binary file into a tube.
         *
         * The binary file has to be created with the reciprocal method Tube::serialize()
         *
         * Maps of values can be stored within the same file. This feature allows to save trajectories
         * that may be used for further computations. For instance, the real (but unknown) signal to
         * approximate with the tube.
         *
         * \param binary_file_name the file name to deserialize
         * \param real_values an optional map to get map<double,double> values possibly stored in the file
         * \param v_real_values an optional vector of maps to get a set of map<double,double> values possibly stored in the file
         */
        Tube(const std::string& binary_file_name);
        Tube(const std::string& binary_file_name, std::map<double,double> &real_values);
        Tube(const std::string& binary_file_name, std::vector<std::map<double,double> > &v_real_values);

        /**
         * \brief Assign this tube to tu.
         */
        Tube& operator=(const Tube& tu);

        /**
         * \brief Delete this tube.
         */
        ~Tube();

        /**
         * \brief Return the volume of this tube.
         *
         * The volume of a tube is defined by the surface of the slices.
         *
         * \return a double value, 0 if the tube is empty or if slices are thin
         */
        double volume() const;

        /**
         * \brief Return the distance between two tubes.
         *
         * The distance is defined as the difference between the tubes' volumes.
         *
         * \param tu the tube to be compared
         * \return a double value, 0 if the tubes are equal
         */
        double dist(const Tube& tu) const;

        /**
         * \brief Return tube's size: the number of slices defining the tube.
         *
         * \return an integer: slices number
         */
        int size() const;

        /**
         * \brief Return a vector of tube's slices
         *
         * \return a vector of one-slice Tubes
         */
        std::vector<Tube*> slices() const;

        /**
         * \brief Test if the tube is a box (i.e. if it has only one slice).
         *
         * \return true if the tube has one slice, false otherwise
         */
        bool isSlice() const;

        /**
         * \brief Test if the tube is empty (i.e. if all its slices are empty)
         *
         * \return true if the tube is empty, false otherwise
         */
        bool isEmpty() const;

        /**
         * \brief Test if the tube is partially empty (i.e. if at least one slice is empty)
         *
         * \return true if the tube is partially empty, false otherwise
         */
        bool isPartiallyEmpty() const;

        /**
         * \brief Test if the tube is discontinuous
         *
         * (i.e. if one slice does not intersect with the previous one)
         *
         * \return true if the tube is discontinuous, false otherwise
         */
        bool isDiscontinuous() const;

        /**
         * \brief Return the id of the slice containing the given input t.
         *
         * Returned index 'i' is such that domain(i).lb() <= t < domain(i).ub()
         *
         * \param t the input
         * \return an identifier representing the slice in the current tube
         */
        int input2index(double t) const;

        /**
         * \brief Return an input value of the slice represented by the given index.
         *
         * Note: returned value is domain(index).lb()
         *
         * \param index slice's id, between 0 and (size - 1)
         * \return the lower bound of the slice's domain
         */
        double index2input(int index) const;

        /**
         * \brief Return the domain of the tube.
         *
         * \return an interval [t0,tf]
         */
        const ibex::Interval& domain() const;

        /**
         * \brief Return the domain of the slice identified by index.
         *
         * \param index slice's id, between 0 and (size - 1)
         * \return an interval [t0,tf]
         */
        const ibex::Interval& domain(int index) const;

        /**
         * \brief Return the domain of the slice containing the given input.
         *
         * \param t the input
         * \return an interval [t0,tf]
         */
        ibex::Interval domain(double t) const;

        /**
         * \brief Return the slice box [domain]x[image] represented by index.
         *
         * \param index slice's id, between 0 and (size - 1)
         * \return an IntervalVector [t0,tf]x[ylb,yub]
         */
        ibex::IntervalVector sliceBox(int index) const;

        /**
         * \brief Comparison operator between two tubes. Return true in case of equality.
         *
         * \param tu the tube to be compared
         * \return a boolean
         */
        bool operator==(const Tube& tu) const;

        /**
         * \brief Comparison operator between two tubes. Return true in case of inequality.
         *
         * \param tu the tube to be compared
         * \return a boolean
         */
        bool operator!=(const Tube& tu) const;

        /**
         * \brief Return the image (y-value) of the i^th slice, refered by index.
         *
         * \param index slice's id, between 0 and (size - 1)
         * \return the corresponding image
         */
        const ibex::Interval& operator[](int index) const;

        /**
         * \brief Return the image (y-value) for the given time, refered by t.
         *
         * \param t the given time
         * \return the corresponding image
         */
        ibex::Interval operator[](double t) const;

        /**
         * \brief Return the image (y-value) for the given bounded time, refered by intv_t.
         *
         * \param intv_t the given bounded time
         * \return the corresponding image
         */
        ibex::Interval operator[](const ibex::Interval& intv_t) const;

        /**
         * \brief Return the set of all images over the whole domain.
         *
         * \return an interval [y1,y2]
         */
        const ibex::Interval& image() const;

        /**
         * \brief Test if this is in the interior of outer_tube.
         *
         * \note In particular, (-oo,oo) is in the interior of (-oo,oo)
         *       and an empty tube is in the interior of an empty tube.
         *       (Ibex definition)
         * \note Always return true if this is empty.
         * \return a boolean
         */
        bool isInteriorSubset(const Tube& outer_tube) const;

        /**
         * \brief Set the image intv_y for the slice refered by index.
         *
         * \param intv_y the image to be set
         * \param index slice's id, between 0 and (size - 1)
         */
        void set(const ibex::Interval& intv_y, int index);

        /**
         * \brief Set the image intv_y for the slice containing the given input t.
         *
         * Note: this operation will affect the whole slice. For a guaranteed
         * contraction at t, see the ctcEval() method.
         *
         * \param intv_y the image to be set
         * \param t the input
         */
        void set(const ibex::Interval& intv_y, double t);

        /**
         * \brief Set the image intv_y over the domain represented by intv_t.
         *
         * Note: this operation will affect the whole domain. For a guaranteed
         * contraction over intv_t, see the ctcEval() method.
         *
         * \param intv_y the image to be set
         * \param intv_t the interval input, [-oo,oo] by default
         */
        void set(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);

        /**
         * \brief Add the image intv_y for the slice refered by index.
         *
         * The new y-value is add without destroying previous data: a union is made for each slice.
         *
         * \param intv_y the image to be added
         * \param index slice's id, between 0 and (size - 1)
         * \return the resulted union for the corresponding slice
         */
        const ibex::Interval feed(const ibex::Interval& intv_y, int index);

        /**
         * \brief Add the image intv_y for the slice containing the given input t.
         *
         * The new y-value is add without destroying previous data: a union is made for each slice.
         *
         * \param intv_y the image to be added
         * \param t the input
         * \return the resulted union for the corresponding time
         */
        const ibex::Interval feed(const ibex::Interval& intv_y, double t);

        /**
         * \brief Add y-values from a map.
         *
         * \param map_values a map of [y-value] referenced by time
         */
        void feed(const std::map<double,ibex::Interval>& map_values);

        /**
         * \brief Add y-values from a map considering some standard deviation.
         *
         * Will enclose the value within [y-2*stddev,y+2*stddev].
         * Both maps must share the same keys.
         *
         * \param map_values a map of y-values referenced by time
         * \param map_stddev a map of sigma values referenced by time
         */
        void feed(const std::map<double,double>& map_values, const std::map<double,double>& map_stddev);

        /**
         * \brief Add y-values from a map.
         *
         * \param map_values a map of y-values referenced by time
         * \param intv_uncertainty bounded uncertainty that will be added to each value of the map
         */
        void feed(const std::map<double,double>& map_values, const ibex::Interval& intv_uncertainty);

        /**
         * \brief Add [-rad,+rad] over each slice.
         *
         * \param rad radius of the inflation
         * \return a reference to this
         */
        Tube& inflate(double rad);

        /**
         * \brief Return enclosed bounds of tube's images over the domain represented by intv_t.
         *
         * \param intv_t the interval input, [-oo,oo] by default
         * \return a pair of intervals enclosing all tube's minima and maxima: [[lb_min, lb_max], [ub_min, ub_max]]
         */
        const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;

        /**
         * \brief Perform set-inversion on this.
         *
         * The set-inversion of this tube consists in determining the set intv_t such that intv_t = [f]^-1(intv_y).
         * The solution may be made of several subsets. 
         * Here, the returned value intv_t corresponds to the union of the solutions subsets.
         *
         * \param intv_y the y-value to invert
         * \param intv_t the optional t domain to consider, [-oo,oo] by default
         * \return the resulted union of the set-inversion
         */
        ibex::Interval invert(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;

        /**
         * \brief Perform precise set-inversion on this.
         *
         * The set-inversion of this tube consists in determining the set intv_t such that intv_t = [f]^-1(intv_y).
         * The solution may be made of several subsets. 
         * Here, the solutions subsets are stored within v_intv_t.
         *
         * \param intv_y the y-value to invert
         * \param v_intv_t a vector containing solutions subsets
         * \param intv_t the optional t domain to consider, [-oo,oo] by default
         */
        void invert(const ibex::Interval& intv_y, std::vector<ibex::Interval> &v_intv_t, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;

        /**
         * \brief Bisect a slice into two
         *
         * Bisection by time.
         *
         * \param index bisect the slice refered by index
         */
        void sample(int index);

        /**
         * \brief Bisect a tube into two tubes
         *
         * Bisection by value.
         *
         * \param derivative_tube the derivative of this
         * \param t the bisection time
         * \param ratio the bisection ratio
         * \return a pair of Tubes
         */
        std::pair<tubex::Tube,tubex::Tube> bisect(const tubex::Tube& derivative, double t, float ratio = 0.55);

        /**
         * \brief Return the maximum thickness of the tube
         *
         * \param first_id_max_thickness the first slice index corresponding to this thickness
         * \return thickness
         */
        double maxThickness();
        double maxThickness(int& first_id_max_thickness);

        /**
         * \brief Extract a subtube from this.
         *
         * intv_t may be a subset of the new tube's domain in order to share the same slices cutting.
         *
         * \param intv_t the domain to extract
         * \return a subtube defined over the intv_t domain
         */
        Tube subtube(const ibex::Interval& intv_t) const;

        /**
         * \brief Set this Tube to the hull of itself and another.
         *
         * Note: both tubes have to be similar (same domain, same number of slices).
         *
         * \param x the other tube
         * \return a reference to this
         */
        Tube& operator|=(const Tube& x);

        /**
         * \brief Set this Tube to the intersection of itself and another.
         *
         * Note: both tubes have to be similar (same domain, same number of slices).
         *
         * \param x the other tube
         * \return a reference to this
         */
        Tube& operator&=(const Tube& x);

        /**
         * \brief Perform an intersection over a slice refered by its index.
         *
         * \param intv_y the image to intersect
         * \param index slice's id, between 0 and (size - 1)
         * \return true if a contraction has been done, false otherwise
         */
        bool intersect(const ibex::Interval& intv_y, int slice_index);

        /**
         * \brief Display tube's values.

         * \param precision the decimal precision to be used to format floating-point values on output
         *        Default value: 0, meaning a default display without the use of setprecision
         */
        void print(int precision = 0) const;

        /**
         * \brief Stream out tube x.
         */
        friend std::ostream& operator<<(std::ostream& str, const Tube& x);

        /**
         * \brief To be defined...
         */
        ibex::Interval interpol(double t, const Tube& derivative_tube) const;
        ibex::Interval interpol(const ibex::Interval& intv_t, const Tube& derivative_tube) const;
        std::pair<ibex::Interval,ibex::Interval> partialInterpol(const ibex::Interval& intv_t, const Tube& derivative_tube) const;


      /** Integration computation **/

        /**
         * \brief Return the primitive tube of this, with custom initial condition.
         *
         * \param initial_value the initial condition for computation, [0.] by default
         * \return the primitive of this, defined over the same domain
         */
        Tube primitive(const ibex::Interval& initial_value = ibex::Interval(0.)) const;

        /**
         * \brief Return the precise bounded-integral over the domain represented by [0,t].
         *
         * \param t the upper bound of the domain
         * \return an interval [i1,i2]
         */
        ibex::Interval integral(double t) const;

        /**
         * \brief Return the precise bounded-integral over the domain represented by [0,[t]].
         *
         * \param t the bounded upper bound of the domain
         * \return an interval [i1,i2]
         */
        ibex::Interval integral(const ibex::Interval& t) const;

        /**
         * \brief Return the precise and partial bounded-integral over the domain represented by [0,[t]] for f- and f+.
         *
         * \param t the bounded upper bound of the domain
         * \return a pair of integrals <[s1,s2],[S1,S2]>
         *   [s1,s2] corresponds to the bounded integral of f- over [0,[t]]
         *   [S1,S2] corresponds to the bounded integral of f+ over [0,[t]]
         */
        std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;

        /**
         * \brief Return the precise bounded-integral over the domain represented by [[t1],[t2]].
         *
         * \param t1 the bounded lower bound of the domain
         * \param t2 the bounded upper bound of the domain
         * \return an interval [i1,i2]
         */
        ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;

        /**
         * \brief Return the precise and partial bounded-integral over the domain represented by [[t1],[t2]] for f- and f+.
         *
         * \param t1 the bounded lower bound of the domain
         * \param t2 the bounded upper bound of the domain
         * \return a pair of integrals <[s1,s2],[S1,S2]>
         *   [s1,s2] corresponds to the bounded integral of f- over [[t1],[t2]]
         *   [S1,S2] corresponds to the bounded integral of f+ over [[t1],[t2]]
         */
        std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2) const;


      /** Contractors **/

        /**
         * \brief Contract this in forward (from the past to the future)
         * in order to be consistent with its derivative tube.
         *
         * \param derivative_tube the derivative of this
         * \param initial_value an initial constraint to be considered ([-oo,oo] by default)
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcFwd(const Tube& derivative_tube, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);

        /**
         * \brief Contract this in backward (from the future to the past)
         * in order to be consistent with its derivative tube.
         *
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcBwd(const Tube& derivative_tube);

        /**
         * \brief Contract this in forward/backward (both ways)
         * in order to be consistent with its derivative tube.
         *
         * \param derivative_tube the derivative of this
         * \param initial_value an initial constraint to be considered ([-oo,oo] by default)
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcFwdBwd(const Tube& derivative_tube, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);

        /**
         * \brief Contract this and parameters from a given observation, such that y=x(t) with y\in[image], t\in[domain], x\in this
         *
         * The observation may be uncertain: [domain]x[image].
         * Note: the derivative tube is required for such contraction.
         *
         * The contraction may be kept local (fwd_bwd=false) or leading to 
         * a complete contraction of the tube over the whole domain (fwd_bwd=true).
         * This parameter is useful for optimization purposes.
         *
         * \param derivative_tube the derivative of this
         * \param t the domain containing the observation (may be contracted)
         * \param y the interval image containing the observation (may be contracted)
         * \param fwd_bwd a boolean allowing a propagation of the contraction over the whole domain
         * \param tube_contracted a boolean, 'true' if a contraction happend for this 
         * \param t_contracted a boolean, 'true' if a contraction happend for [domain]
         * \param y_contracted a boolean, 'true' if a contraction happend for [image]
         * \param bisection_required a boolean, 'true' if a bisection of [domain] should be considered to improve the contraction
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcEval(const Tube& derivative_tube, ibex::Interval& t, ibex::Interval& y, bool fwd_bwd = true);
        bool ctcEval(const Tube& derivative_tube, ibex::Interval& t, const ibex::Interval& y, bool fwd_bwd = true);
        bool ctcEval(const Tube& derivative_tube, const ibex::Interval& t, ibex::Interval& y, bool fwd_bwd = true);
        bool ctcEval(const Tube& derivative_tube, const ibex::Interval& t, const ibex::Interval& y, bool fwd_bwd = true);
        bool ctcEval_base(const Tube& derivative_tube, ibex::Interval& t, ibex::Interval& y,
                                bool& tube_contracted, bool& t_contracted, bool& y_contracted, 
                                bool& bisection_required, bool fwd_bwd = true);
        
        /**
         * \brief Contract this from a given non-observation, such that y\not=x(t) with y\in[image], t\in[domain], x\in this
         *
         * The non-observation may be uncertain: [domain]x[image].
         *
         * Note: DEPRECATED, this contractor will be removed.
         *
         * \param t the domain containing the non-observation
         * \param y the interval image containing the non-observation
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcOut(const ibex::Interval& t, const ibex::Interval& y);

        /**
         * \brief Contract parameters from the constraint x(t1)=x(t2), t1\in[t1], t2\in[t2], x\in this
         *
         * \param t1 the first domain
         * \param t2 the second domain
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcIntertemporal(ibex::Interval& t1, ibex::Interval& t2) const;

        /**
         * \brief Contract parameters from the constraint x(t1)=x(t2)=y, t1\in[t1], t2\in[t2], y\in[y], x\in this
         *
         * \param y image domain for y, may be contracted
         * \param t1 the first domain, may be contracted
         * \param t2 the second domain, may be contracted
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcIntertemporal(ibex::Interval& y, ibex::Interval& t1, ibex::Interval& t2) const;

        /**
         * \brief Contract this from the constraint x(t)=x(t+p), p\in[p], x\in this
         *
         * \param period the bounded periodicity of the signal enclosed by the tube
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcPeriodic(const ibex::Interval& period);
        
        /**
         * \brief Contract tubes given algebraic constraints.
         *
         * Constraints are defined thanks to an ibex function that has to vanish.
         * 
         * \param x* the tubes (variables) involved in the constraints
         * \param f the ibex function defining the constraints, the variables of which being the tubes
         * \return true if a contraction has been done, false otherwise
         */
        static bool contract(Tube& x1, Tube& x2, const ibex::Function& f);
        static bool contract(Tube& x1, Tube& x2, Tube& x3, const ibex::Function& f);
        static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, const ibex::Function& f);
        static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, const ibex::Function& f);
        static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, const ibex::Function& f);
        static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, const ibex::Function& f);
        static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8, const ibex::Function& f);


      /** Serialization **/

        /**
         * \brief Perform the serialization of this.
         *
         * The binary file will then be deserializable with
         * the reciprocal constructor Tube::Tube(file_name)
         *
         * \param binary_file_name the name of the output binary file
         * \param real_values an optional map to store map<double,double> values in the file
         * \param v_real_values an optional vector of maps to store a set of map<double,double> values in the file
         */
        bool serialize(const std::string& binary_file_name = "x.tube") const;
        bool serialize(const std::string& binary_file_name, const std::map<double,double>& real_values) const;
        bool serialize(const std::string& binary_file_name, const std::vector<std::map<double,double> >& v_real_values) const;


    protected:

      /**
       * \brief Create a tube over a domain defined by a vector of intervals.
       *
       * Values are set by default to [-oo,oo].
       * Note: for now, all timesteps must be identical in the tree.
       *
       * \param vector_dt an interval of slices domains
       * \param default_value default y-values ([-oo,oo] by default)
       */
      Tube(const std::vector<ibex::Interval>& vector_dt,
           const ibex::Interval& default_value = ibex::Interval::ALL_REALS);

      /**
       * \brief Create a tube based on the given parameters.
       *
       * Values are set by default to [-oo,oo].
       *
       * \param domain tube's domain
       * \param timestep tube's precision corresponding to slices' constant width
       * \param default_value default tube's image ([-oo,oo] by default)
       */
      void createFromSpecifications(const ibex::Interval& domain, double timestep, const ibex::Interval& default_value = ibex::Interval::ALL_REALS);

      /**
       * \brief Create a tube over a domain defined by a vector of intervals.
       *
       * Values are set by default to [-oo,oo].
       *
       * \param vector_slices an interval of slices domains
       * \param default_value default tube's image
       */
      void createFromSlicesVector(const std::vector<ibex::Interval>& vector_slices, const ibex::Interval& default_value = ibex::Interval::ALL_REALS);

      /**
       * \brief Return the half sub-tube [t0,tf/2[.
       *
       * \return a pointer to the first part of the tube
       */
      const Tube* getFirstSubTube() const;

      /**
       * \brief Return the half sub-tube [tf/2,tf].
       *
       * \return a pointer to the second part of the tube
       */
      const Tube* getSecondSubTube() const;

      /**
       * \brief Return the requested slice of the tube.
       *
       * \param index slice's id, between 0 and (size - 1)
       * \return a pointer to the Tube corresponding to the slice indexed by index
       */
      Tube* getSlice(int index);
      const Tube* getSlice(int index) const;

      /**
       * \brief Return all the nodes of the tree (not only the leafs).
       *
       * The root is also included in the vector.
       *
       * \param v_nodes a vector containing pointers to the nodes
       */
      void getTubeNodes(std::vector<Tube*> &v_nodes);
      void getTubeNodes(std::vector<const Tube*> &v_nodes) const;

      /**
       * \brief Perform the union on the considered node only.
       *
       * Parents of children nodes are not updated.
       * This method is used for multithreading purposes.
       *
       * \param x a pointer to the other node
       */
      void unionWith_localUpdate(const Tube *x);

      /**
       * \brief Perform the intersection on the considered node only.
       *
       * Parents of children nodes are not updated.
       * This method is used for multithreading purposes.
       *
       * \param x a pointer to the other node
       */
      void intersectWith_localUpdate(const Tube *x);

      /**
       * \brief Update tube's data structure.
       *
       * Tube's structure is based on a binary tree. When a leaf
       * is changed, the modification has to be propagated through
       * all upper branches.
       * This method is defined as 'const' because it does not change
       * logical values of the tube.
       * The computation can be done on demand with requestFutureTreeComputation()
       */
      void computeTree() const;

      /**
       * \brief Request a future update of the tree.
       *
       * Set a flag to 'true' as a request for future tree computation.
       * See computeTree() method.
       *
       * \input index the index of the updated slice,
       *        by default: -1 corresponding to an update of all slices
       */
      void requestFutureTreeComputation(int index = -1) const;

      /**
       * \brief Update tube's pre-computed primitives.
       *
       * For efficiency reasons, primitives are pre-computed
       * and stored into each node of the tree.
       * The computation can be done on demand with requestFuturePrimitiveComputation()
       *
       * \input build_from_leafs true if the primitives have to be computed from the leafs (false by default)
       */
      void computePartialPrimitive(bool build_from_leafs = false) const;

      /**
       * \brief Request a future computation of primitives.
       *
       * Set a flag to 'true' as a request for future primitives computation.
       * See computePartialPrimitive() method.
       */
      void requestFuturePrimitiveComputation() const;

      /**
       * \brief Return the precise and partial bounded-integral over the domain represented by [0,[t]] for f- and f+.
       *
       * \param t the bounded upper bound of the domain, [-oo,oo] by default
       * \return a pair of integrals <[s1,s2],[S1,S2]>
       *   [s1,s2] corresponds to the bounded integral of f- over [0,[t]]
       *   [S1,S2] corresponds to the bounded integral of f+ over [0,[t]]
       */
      std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;

      /**
       * \brief Perform precise set-inversion on this.
       *
       * The set-inversion of this tube consists in determining the set intv_t such that intv_t = [f]^-1(intv_y)
       * Here the returned value vector<intv_t> corresponds to detailed solutions.
       *
       * \param intv_y the image to invert
       * \param intv_t the optional t domain to consider
       * \param concatenate_results results may be adjacent, so a concatenation on vector's values can be requested
       * \return a vector containing each solutions of the set-inversion
       */
      void invert(const ibex::Interval& intv_y, std::vector<ibex::Interval> &v_intv_t, const ibex::Interval& intv_t, bool concatenate_results) const;

      /**
       * \brief To be defined...
       */
      void ctcEval_computeIndex(const ibex::Interval& y, const ibex::Interval& t, int& index_lb, int& index_ub);

      /**
       * \brief Deserialize a binary file into a tube.
       *
       * The binary file has to be created with the reciprocal method Tube::serialize()
       *
       * \param binary_file_name the file name to deserialize
       * \param real_values an optional map to get map<double,double> values possibly stored in the file
       * \param v_real_values an optional vector of maps to get sets of map<double,double> values possibly stored in the file
       */
      void deserialize(const std::string& binary_file_name, std::map<double,double>& real_values);
      void deserialize(const std::string& binary_file_name, std::vector<std::map<double,double> >& v_real_values);

      /** Class variables **/

        // Tube structure (no mutable needs)
        ibex::Interval m_domain;
        Tube *m_first_subtube, *m_second_subtube;
        std::vector<Tube*> m_v_slices;

        // Tube attributes ('mutable' required: values may be updated from const methods)
        mutable double m_volume;
        mutable double m_dt_specifications;
        mutable ibex::Interval m_image;
        mutable std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
        mutable std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
        mutable bool m_tree_computation_needed;
        mutable bool m_primitive_computation_needed;
  };
}

#endif