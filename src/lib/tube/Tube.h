/* ============================================================================
 *  IBEX-Robotics - Tube class
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TubeIbex_HEADER
#define TubeIbex_HEADER

#include <map>
#include <vector>
#include <utility>
#include "ibex_IntervalVector.h"

class Tube
{
  public:

    /** Basic methods **/

        /**
         * \brief Create a tube over the domain defined by the interval intv_t.
         *
         * Values are set by default to Interval::ALL_REALS.
         *
         * \param intv_t tube's domain
         * \param time_step tube's precision corresponding to slice's width
         * \param default_value default y-values
         */
        Tube(const ibex::Interval &intv_t,
             double time_step,
             const ibex::Interval& default_value = ibex::Interval::ALL_REALS);

        /**
         * \brief Create a tube over a domain defined by a vector of intervals.
         *
         * Values are set by default to Interval::ALL_REALS.
         *
         * \param vector_dt an interval of slices domains
         * \param default_value default y-values
         */
        Tube(const std::vector<ibex::Interval>& vector_dt,
             const ibex::Interval& default_value = ibex::Interval::ALL_REALS);

        /**
         * \brief Create a copy of the given tube tu.
         *
         * \param tu the tube to be copied
         */
        Tube(const Tube& tu);

        /**
         * \brief Assign this Tube to tu.
         */
        Tube& operator=(const Tube& tu);

        /**
         * \brief Delete this tube.
         */
        ~Tube();

        /**
         * \brief Return the volume of this tube.
         *
         * The volume of a tube is defined by the surface enclosed by f- and f+.
         *
         * \return a double value, 0 if the tube is empty or if f- is equal to f+
         */
        double volume() const;

        /**
         * \brief Return the distance between two tubes.
         *
         * The distance is defined as the difference between the tubes volumes.
         *
         * \return a double value, 0 if the tubes are equal
         */
        double dist(const Tube& tube) const;

        /**
         * \brief Return tube's size: the number of slices defining the tube.
         *
         * \return an integer representing the size
         */
        int size() const;

        /**
         * \brief Return the width of a slice (i.e. tube's time step).
         *
         * \return the width of a slice
         */
        double dt() const;

        /**
         * \brief Test if the tube is a box (i.e. if it has only one slice).
         *
         * \return true if the tube has one slice, false otherwise
         */
        bool isSlice() const;

        /**
         * \brief Return the id of the slice containing the given input t.
         * Returned index is such that getT(i).lb() <= t < getT(i).ub()
         *
         * \param t the input
         * \return an identifier representing the slice in the current tube
         */
        int input2index(double t) const;

        /**
         * \brief Return an input value of the slice represented by the given index.
         * By default, returned value is getT(index).lb()
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
        const ibex::Interval& getT() const;

        /**
         * \brief Return the domain of the slice represented by index.
         *
         * \param index slice's id, between 0 and (size - 1)
         * \return an interval [t0,tf]
         */
        const ibex::Interval& getT(int index) const;

        /**
         * \brief Return the domain of the slice containing the given input.
         *
         * \param t the input
         * \return an interval [t0,tf]
         */
        ibex::Interval getT(double t) const;

        /**
         * \brief Return the y-value of the i^th slice, refered by index.
         *
         * \param index slice's id, between 0 and (size - 1)
         * \return the correspondingy-value
         */
        const ibex::Interval& operator[](int index) const;

        /**
         * \brief Return the y-value for the given time, refered by t.
         *
         * \param t the given time
         * \return the corresponding y-value
         */
        ibex::Interval operator[](double t) const;

        /**
         * \brief Return the y-value for the given bounded time, refered by intv_t.
         *
         * \param intv_t the given bounded time
         * \return the corresponding y-value
         */
        ibex::Interval operator[](const ibex::Interval& intv_t) const;

        /**
         * \brief Return the union-value over the whole domain.
         *
         * \return an interval [y1,y2]
         */
        const ibex::Interval& getY() const;

        /**
         * \brief Set the y-value intv_y for the slice refered by index.
         *
         * \param intv_y the y-value to be set
         * \param index slice's id, between 0 and (size - 1)
         */
        void setY(const ibex::Interval& intv_y, int index);

        /**
         * \brief Set the y-value intv_y for the slice containing the given input t.
         *
         * \param intv_y the y-value to be set
         * \param t the input
         */
        void setY(const ibex::Interval& intv_y, double t);

        /**
         * \brief Set the y-value intv_y over the domain represented by intv_t.
         *
         * \param intv_y the y-value to be set
         * \param intv_t the interval input, Interval::ALL_REALS by default
         */
        void setY(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);

        /**
         * \brief Add the y-value intv_y for the slice refered by index.
         *
         * The new y-value is add without destroying previous data: a union is made for each slice.
         *
         * \param intv_y the y-value to be added
         * \param index slice's id, between 0 and (size - 1)
         * \return the resulted union for the corresponding slice
         */
        const ibex::Interval feed(const ibex::Interval& intv_y, int index);

        /**
         * \brief Add the y-value intv_y for the slice containing the given input t.
         *
         * The new y-value is add without destroying previous data: a union is made for each slice.
         *
         * \param intv_y the y-value to be added
         * \param t the input
         * \return the resulted union for the corresponding time
         */
        const ibex::Interval feed(const ibex::Interval& intv_y, double t);

        /**
         * \brief Add y-values from a map.
         *
         * \param map_intv_y a map of [y-value] referenced by time
         */
        void feed(const std::map<double,ibex::Interval>& map_intv_y);

        /**
         * \brief Add y-values from a map.
         *
         * \param map_intv_y a map of y-values referenced by time
         * \param intv_uncertainty enclosed uncertainty that will be added to each value of the map
         */
        void feed(const std::map<double,double>& map_y, const ibex::Interval& intv_uncertainty);

        /**
         * \brief Add y-values from a map.
         *
         * \param map_intv_y a map of y-values referenced by time
         * \param intv_uncertainty enclosed uncertainty that will be added to each value of the map
         * \param y_no_uncertainties enclosed uncertainty will not be added for this given value
         */
        void feed(const std::map<double,double>& map_y, const ibex::Interval& intv_uncertainty, double y_no_uncertainties);

        /**
         * \brief Return enclosed bounds of tube's y-values over the domain represented by intv_t.
         *
         * \param intv_t the interval input, Interval::ALL_REALS by default
         * \return a pair of intervals representing all tube's minima and maxima: [[lb_min, lb_max], [ub_min, ub_max]]
         */
        const std::pair<ibex::Interval,ibex::Interval> getEnclosedBounds(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;

        /**
         * \brief Perform set-inversion on this.
         *
         * The set-inversion of this tube consists in determining the set intv_t such that intv_t = f^-1(intv_y)
         * Here the returned value intv_t corresponds to the union of the solutions.
         *
         * \param intv_y the y-value to invert
         * \return the resulted union of the set-inversion
         */
        ibex::Interval setInversion(const ibex::Interval& intv_y) const;

        /**
         * \brief Perform precise set-inversion on this.
         *
         * The set-inversion of this tube consists in determining the set intv_t such that intv_t = f^-1(intv_y)
         * Here the returned value v_intv_t corresponds to detailed solutions.
         * Vector v_intv_t is cleared before computation.
         *
         * \param intv_y the y-value to invert
         * \param v_intv_t a vector containing, at the end, each solutions of the set-inversion
         */
        void setInversion(const ibex::Interval& intv_y, std::vector<ibex::Interval> &v_intv_t) const;

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
         * \brief Display tube's values.

         * \param precision the decimal precision to be used to format floating-point values on output
         *        Default value: 0, meaning a default display without the use of setprecision
         */
        void print(int precision = 0) const;

        /**
         * \brief Stream out tube x.
         */
        friend std::ostream& operator<<(std::ostream& str, const Tube& x);


    /** Integration computation **/

        /**
         * \brief Return the primitive tube of this with initial condition.
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
        ibex::Interval timeIntegration(double t) const;

        /**
         * \brief Return the precise bounded-integral over the domain represented by [0,[t]].
         *
         * \param t the bounded upper bound of the domain
         * \return an interval [i1,i2]
         */
        ibex::Interval timeIntegration(const ibex::Interval& t) const;

        /**
         * \brief Return the precise and partial bounded-integral over the domain represented by [0,[t]] for f- and f+.
         *
         * \param t the bounded upper bound of the domain
         * \return a pair of integrals <[s1,s2],[S1,S2]>
         *   [s1,s2] corresponds to the bounded integral of f- over [0,[t]]
         *   [S1,S2] corresponds to the bounded integral of f+ over [0,[t]]
         */
        std::pair<ibex::Interval,ibex::Interval> partialTimeIntegration(const ibex::Interval& t) const;

        /**
         * \brief Return the precise bounded-integral over the domain represented by [[t1],[t2]].
         *
         * \param t1 the bounded lower bound of the domain
         * \param t2 the bounded upper bound of the domain
         * \return an interval [i1,i2]
         */
        ibex::Interval timeIntegration(const ibex::Interval& t1, const ibex::Interval& t2) const;

        /**
         * \brief Return the precise and partial bounded-integral over the domain represented by [[t1],[t2]] for f- and f+.
         *
         * \param t1 the bounded lower bound of the domain
         * \param t2 the bounded upper bound of the domain
         * \return a pair of integrals <[s1,s2],[S1,S2]>
         *   [s1,s2] corresponds to the bounded integral of f- over [[t1],[t2]]
         *   [S1,S2] corresponds to the bounded integral of f+ over [[t1],[t2]]
         */
        std::pair<ibex::Interval,ibex::Interval> partialTimeIntegration(const ibex::Interval& t1, const ibex::Interval& t2) const;


    /** Contractors **/

        /**
         * \brief To contract tube in forward (from the past to the future).
         *
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcFwd(const Tube& derivative_tube, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);

        /**
         * \brief To contract tube in backward (from the future to the past).
         *
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcBwd(const Tube& derivative_tube);

        /**
         * \brief To contract tube in forward/backward.
         *
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcFwdBwd(const Tube& derivative_tube);

        /**
         * \brief To be defined...
         */
        bool ctcIn(const Tube& derivative_tube, ibex::Interval& y, ibex::Interval& t);

        /**
         * \brief To be defined...
         */
        bool ctcOut(const ibex::Interval& y, const ibex::Interval& t);


  protected:

    /**
     * \brief Create a tube over a domain defined by a vector of intervals.
     *
     * Values are set by default to Interval::ALL_REALS.
     *
     * \param vector_slices an interval of slices domains
     * \param default_value default y-values
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
     * \brief Set this Tube to the hull of itself and another: tube x.
     *
     * Note: both tubes have to be similar (same domain, same number of slices).
     *
     * \param x the other tube
     * \return a reference to this
     */
    Tube& unionWith(const Tube& x);

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
     * \brief Set this Tube to the intersection of itself and another: tube x.
     *
     * Note: both tubes have to be similar (same domain, same number of slices).
     *
     * \param x the other tube
     * \return a reference to this
     */
    Tube& intersectWith(const Tube& x);

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
     * \input build_from_leafs true if the primitives have to be computed from the leafs
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
     * \param t the bounded upper bound of the domain, ibex::Interval::ALL_REALS by default
     * \return a pair of integrals <[s1,s2],[S1,S2]>
     *   [s1,s2] corresponds to the bounded integral of f- over [0,[t]]
     *   [S1,S2] corresponds to the bounded integral of f+ over [0,[t]]
     */
    std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;

    /**
     * \brief Perform precise set-inversion on this.
     *
     * The set-inversion of this tube consists in determining the set intv_t such that intv_t = f^-1(intv_y)
     * Here the returned value vector<intv_t> corresponds to detailed solutions.
     *
     * \param intv_y the y-value to invert
     * \param concatenate_results results may be adjacent, so a concatenation on vector's values can be requested
     * \return a vector containing each solutions of the set-inversion
     */
    void setInversion(const ibex::Interval& intv_y, std::vector<ibex::Interval> &v_intv_t, bool concatenate_results) const;

    /** Class variables **/

    double m_dt;
    int m_slices_number;
    mutable bool m_tree_computation_needed;
    mutable bool m_primitive_computation_needed;
    mutable Tube *m_first_subtube, *m_second_subtube;
    mutable ibex::Interval m_intv_t, m_intv_y;
    mutable std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
    mutable std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
};

    /** Arithmetic **/

    Tube operator+(const Tube& x1, const Tube& x2);
    Tube operator+(const Tube& x1, double x2);
    Tube operator+(double x1, const Tube& x2);
    Tube operator+(const Tube& x1, const ibex::Interval& x2);
    Tube operator+(const ibex::Interval& x1, const Tube& x2);

    Tube operator-(const Tube& x);
    Tube operator-(const Tube& x1, const Tube& x2);
    Tube operator-(const Tube& x1, double x2);
    Tube operator-(double x1, const Tube& x2);
    Tube operator-(const Tube& x1,  const ibex::Interval& x2);
    Tube operator-(const ibex::Interval& x1, const Tube& x2);

    Tube operator*(const Tube& x1, const Tube& x2);
    Tube operator*(const Tube& x1, double x2);
    Tube operator*(double x1, const Tube& x2);
    Tube operator*(const ibex::Interval& x1, const Tube& x2);
    Tube operator*(const Tube& x1, const ibex::Interval& x2);

    Tube operator/(const Tube& x1, const Tube& x2);
    Tube operator/(const Tube& x1, double x2);
    Tube operator/(double x1, const Tube& x2);
    Tube operator/(const ibex::Interval& x1, const Tube& x2);
    Tube operator/(const Tube& x1, const ibex::Interval& x2);

    Tube operator|(const Tube& x1, const Tube& x2);
    Tube operator|(const Tube& x1, double x2);
    Tube operator|(double x1, const Tube& x2);
    Tube operator|(const ibex::Interval& x1, const Tube& x2);
    Tube operator|(const Tube& x1, const ibex::Interval& x2);

    Tube operator&(const Tube& x1, const Tube& x2);
    Tube operator&(const Tube& x1, double x2);
    Tube operator&(double x1, const Tube& x2);
    Tube operator&(const ibex::Interval& x1, const Tube& x2);
    Tube operator&(const Tube& x1, const ibex::Interval& x2);

    Tube abs(const Tube& x);
    Tube sqr(const Tube& x);
    Tube sqrt(const Tube& x);
    Tube pow(const Tube& x, int p);
    Tube pow(const Tube& x, double p);
    Tube pow(const Tube &x, const ibex::Interval& p);
    Tube root(const Tube& x, int p);
    Tube exp(const Tube& x);
    Tube log(const Tube& x);
    Tube cos(const Tube& x);
    Tube sin(const Tube& x);
    Tube tan(const Tube& x);
    Tube acos(const Tube& x);
    Tube asin(const Tube& x);
    Tube atan(const Tube& x);
    Tube cosh(const Tube& x);
    Tube sinh(const Tube& x);
    Tube tanh(const Tube& x);
    Tube acosh(const Tube& x);
    Tube asinh(const Tube& x);
    Tube atanh(const Tube& x);

#endif