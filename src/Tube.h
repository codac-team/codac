/************************************************************/
/*    AUTH: Simon Rohou
/*    FILE: Tube.h
/*    PRJT: TubeIbex https://github.com/SimonRohou/TubeIbex
/*    DATE: 2015
/************************************************************/

#ifndef TubeIbex_HEADER
#define TubeIbex_HEADER

#include "ibex/ibex.h"

class Tube
{
  public:

    /**
     * \brief Create a tube over the domain defined by the interval intv_t.
     *
     * Values are set by default to Interval::EMPTY_SET.
     *
     * \param intv_t tube's domain
     * \param time_step tube's precision
     * \param default_value default y-values
     */
    Tube(const ibex::Interval &intv_t,
         double time_step,
         const ibex::Interval& default_value = ibex::Interval::EMPTY_SET);

    /**
     * \brief Create a tube over a domain defined by a vector of intervals.
     *
     * Values are set by default to Interval::EMPTY_SET.
     *
     * \param vector_dt an interval of slices domains
     * \param default_value default y-values
     */
    Tube(std::vector<ibex::Interval> vector_dt,
         const ibex::Interval& default_value = ibex::Interval::EMPTY_SET);

    /**
     * \brief Create a copy of the given tube tu.
     *
     * \param tu the tube to be copied
     */
    Tube(const Tube& tu);

    /**
     * \brief Delete this tube.
     */
    ~Tube();

    /**
     * \brief Return the volume of this tube.
     * 
     * The volume of a tube is defined by the surface enclosed by f- and f+.
     * 
     * \return a double value, 0 is the tube is empty or if f- is equal to f+
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
     * \brief Return the requested slice of the tube.
     *
     * \param index slice's number, between 0 and (size - 1)
     * \return an object of class Tube corresponding to the slice indexed by index
     */
    Tube* getSlice(int index);
    const Tube* getSlice(int index) const;

    /**
     * \brief Return the number of the slice containing the given input t.
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
     * \param index slice's number, between 0 and (size - 1)
     * \return the lower bound of the slice's domain
     */
    double index2input(int index);

    /**
     * \brief Return the domain of the tube.
     *
     * \return an interval [t0,tf]
     */
    const ibex::Interval& getT() const;

    /**
     * \brief Return the domain of the slice represented by index.
     *
     * \param index slice's number, between 0 and (size - 1)
     * \return an interval [t0,tf]
     */
    const ibex::Interval& getT(int index);
    const ibex::Interval& getT(int index) const;

    /**
     * \brief Return the domain of the slice containing the given input.
     *
     * \param t the input
     * \return an interval [t0,tf]
     */
    ibex::Interval getT(double t);

    /**
     * \brief Return the y-value of the i^th slice, refered by index.
     * 
     * \param index slice's number, between 0 and (size - 1)
     * \return the y-value
     */
    const ibex::Interval& operator[](int index);
    const ibex::Interval& operator[](int index) const;

    /**
     * \brief Return the y-value for the given time, refered by t.
     * 
     * \param t the given time
     * \return the y-value
     */
    ibex::Interval operator[](double t);
    ibex::Interval operator[](double t) const;

    /**
     * \brief Return the y-value for the given bounded time, refered by intv_t.
     * 
     * \param intv_t the given bounded time
     * \return the y-value
     */
    ibex::Interval operator[](ibex::Interval intv_t);
    ibex::Interval operator[](ibex::Interval intv_t) const;

    /**
     * \brief Return the union-value over the all domain.
     *
     * \return an interval [y1,y2]
     */
    const ibex::Interval& getY() const;

    /**
     * \brief Set the y-value intv_y for the slice represented by index.
     *
     * \param intv_y the y-value to be set
     * \param index slice's number, between 0 and (size - 1)
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
     * \brief Return the half sub-tube [t0,tf/2[.
     *
     * \return a pointer to the first part of the tube
     */
    const Tube* getFirstSubTube() const;

    /**
     * \brief Return the half sub-tube ]tf/2,tf].
     *
     * \return a pointer to the second part of the tube
     */
    const Tube* getSecondSubTube() const;

    /**
     * \brief Return the primitive tube of this.
     *
     * \return a tube defined over the same domain of this
     */
    Tube primitive();

    /**
     * \brief Return the primitive tube of this (with initial condition).
     *
     * \param initial_value the initial condition for computation
     * \return a tube defined over the same domain of this
     */
    Tube primitive(const ibex::Interval& initial_value);

    /**
     * \brief Return the precise bounded-integral over the domain represented by [0,t].
     *
     * \param t the upper bound of the domain
     * \return an interval [s1,s2]
     */
    ibex::Interval timeIntegration(double t);

    /**
     * \brief Return the precise bounded-integral over the domain represented by [0,[t]].
     *
     * \param t the bounded upper bound of the domain
     * \return an interval [i1,i2]
     */
    ibex::Interval timeIntegration(const ibex::Interval& t);

    /**
     * \brief Return the precise bounded-integral over the domain represented by [0,[t]] for f- and f+.
     *
     * \param t the bounded upper bound of the domain
     * \return a pair of integrals <[s1,s2],[S1,S2]>
     *   [s1,s2] corresponds to the bounded integral of f- over [0,[t]]
     *   [S1,S2] corresponds to the bounded integral of f+ over [0,[t]]
     */
    std::pair<ibex::Interval,ibex::Interval> partialTimeIntegration(const ibex::Interval& t);

    /**
     * \brief Return the precise bounded-integral over the domain represented by [[t1],[t2]].
     *
     * \param t1 the bounded lower bound of the domain
     * \param t2 the bounded upper bound of the domain
     * \return an interval [i1,i2]
     */
    ibex::Interval timeIntegration(const ibex::Interval& t1, const ibex::Interval& t2);

    /**
     * \brief Return the precise bounded-integral over the domain represented by [[t1],[t2]] for f- and f+.
     *
     * \param t1 the bounded lower bound of the domain
     * \param t2 the bounded upper bound of the domain
     * \return a pair of integrals <[s1,s2],[S1,S2]>
     *   [s1,s2] corresponds to the bounded integral of f- over [[t1],[t2]]
     *   [S1,S2] corresponds to the bounded integral of f+ over [[t1],[t2]]
     */
    std::pair<ibex::Interval,ibex::Interval> partialTimeIntegration(const ibex::Interval& t1, const ibex::Interval& t2);

    /**
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param index slice's number, between 0 and (size - 1)
     * \return true if a contraction has been done, false otherwise
     */
    bool intersect(const ibex::Interval& intv_y, int index);

    /**
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param t the input
     * \return true if a contraction has been done, false otherwise
     */
    bool intersect(const ibex::Interval& intv_y, double t);

    /**
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \return true if a contraction has been done, false otherwise
     */
    bool intersect(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);

    /**
     * \brief Return enclosed bounds of tube's y-values over the domain represented by intv_t.
     *
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \return a pair of intervals representing all tube's minima and maxima: [[lb_min, lb_max], [ub_min, ub_max]]
     */
    const std::pair<ibex::Interval,ibex::Interval> getEnclosedBounds(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;

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

     * \param precision sets the decimal precision to be used to format floating-point values on output
     *        Default value: 0, meaning a default display without the use of setprecision
     */
    void print(int precision = 0) const;

    /**
     * \brief Stream out tube x.
     */
    friend std::ostream& operator<<(std::ostream& str, const Tube& x);


    /** Contractors **/
    
        /**
         * \brief To contract tube in forward (from the past to the future)
         * 
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcFwd(Tube* derivative_tube);
    
        /**
         * \brief To contract tube in backward (from the future to the past)
         * 
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcBwd(Tube* derivative_tube);
    
        /**
         * \brief To contract tube in forward/backward
         * 
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcFwdBwd(Tube* derivative_tube);
    
        /**
         * \brief To be defined...
         */
        bool ctcIn(Tube* derivative_tube, ibex::Interval& y, ibex::Interval& t);

  //protected:

    /**
     * \brief Create a tube over a domain defined by a vector of intervals.
     *
     * Values are set by default to Interval::EMPTY_SET.
     * It is necessary to update data's structure when values are created/updated.
     * Boolean bool_update can be set to false to save time when subtubes are
     * created; in such a case, a recursive update is necessary on the parent tube.
     *
     * \param vector_dt an interval of slices domains
     * \param default_value default y-values
     * \param bool_update to force the update of tube's data structure
     */
    Tube(std::vector<ibex::Interval> vector_dt,
         const ibex::Interval& default_value,
         bool bool_update);

    /**
     * \brief Create a tube over a domain defined by a vector of intervals.
     *
     * Values are set by default to Interval::EMPTY_SET.
     *
     * \param vector_slices an interval of slices domains
     * \param default_value default y-values
     */
    void createFromSlicesVector(std::vector<ibex::Interval> vector_slices, const ibex::Interval& default_value);

    /**
     * \brief Set the y-value intv_y for the slice represented by index.
     *
     * \param intv_y the y-value to be set
     * \param index slice's number, between 0 and (size - 1)
     * \param bool_update to force the update of tube's data structure
     */
    void setY(const ibex::Interval& intv_y, int index, bool bool_update);

    /**
     * \brief Set the y-value intv_y for the slice containing the given input t.
     *
     * \param intv_y the y-value to be set
     * \param t the input
     * \param bool_update to force the update of tube's data structure
     */
    void setY(const ibex::Interval& intv_y, double t, bool bool_update);

    /**
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param intv_t the interval input
     * \param bool_update to force the update of tube's data structure
     */
    void setY(const ibex::Interval& intv_y, const ibex::Interval& intv_t, bool bool_update);

    /**
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param index slice's number, between 0 and (size - 1)
     * \param bool_update to force the update of tube's data structure
     * \return true if a contraction has been done, false otherwise
     */
    bool intersect(const ibex::Interval& intv_y, int index, bool bool_update);

    /**
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param t the input
     * \param bool_update to force the update of tube's data structure
     * \return true if a contraction has been done, false otherwise
     */
    bool intersect(const ibex::Interval& intv_y, double t, bool bool_update);

    /**
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \param bool_update to force the update of tube's data structure
     * \return true if a contraction has been done, false otherwise
     */
    bool intersect(const ibex::Interval& intv_y, const ibex::Interval& intv_t, bool bool_update);

    /**
     * \brief Return all the nodes of the tree (not only the leafs)
     *
     * The root is also included in the vector.
     * 
     * \param v_nodes a vector containing pointers to the nodes
     */
    void getTubeNodes(std::vector<Tube*> &v_nodes);
    void getTubeNodes(std::vector<const Tube*> &v_nodes) const; // the 'const' version

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
     * \brief Perform the union on the considered node only
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
     * \brief Perform the intersection on the considered node only
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
     */
    void update();

    /**
     * \brief Update tube's data structure (focusing on branches containing t_focus).
     *
     * \param t_focus if specified, it means an update is required for the given input
     */
    void updateFromInput(double t_focus);

    /**
     * \brief Update tube's data structure (focusing on branches containing index_focus).
     *
     * \param index_focus if specified, it means an update is required for the given index
     */
    void updateFromIndex(int index_focus);

    void buildPartialPrimitive(bool build_from_leafs = false);
    std::pair<ibex::Interval,ibex::Interval> getPartialPrimitiveValue(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);

    
    // Class variables:

    double m_dt;
    int m_slices_number;
    bool m_build_primitive_needed;
    Tube *m_first_subtube, *m_second_subtube;
    ibex::Interval m_intv_t, m_intv_y;
    std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
    std::pair<ibex::Interval,ibex::Interval> m_partial_primitive;
};

#endif