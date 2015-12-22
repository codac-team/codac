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

    /**
     * \brief Return the number of the slice containing the given input t.
     *
     * \param t the input
     * \return an identifier representing the slice in the current tube
     */
    int input2index(double t) const;

    /**
     * \brief Return an input value of the slice represented by the given index.
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

    /**
     * \brief Return the domain of the slice containing the given input.
     *
     * \param t the input
     * \return an interval [t0,tf]
     */
    const ibex::Interval& getT(double t);

    /**
     * \brief Return the y-value of the i^th slice, refered by index.
     * 
     * \param index slice's number, between 0 and (size - 1)
     * \return the y-value
     */
    const ibex::Interval& operator[](int index);

    /**
     * \brief Return the y-value for the given time, refered by t.
     * 
     * \param t the given time
     * \return the y-value
     */
    const ibex::Interval& operator[](double t);

    /**
     * \brief Return the y-value for the given bounded time, refered by intv_t.
     * 
     * \param intv_t the given bounded time
     * \return the y-value
     */
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
    const Tube primitive();

    /**
     * \brief Return the primitive tube of this (with initial condition).
     *
     * \param initial_value the initial condition for computation
     * \return a tube defined over the same domain of this
     */
    const Tube primitive(const ibex::Interval& initial_value);

    /**
     * \brief Return the bounded-integral over the domain represented by [[t1],[t2]].
     *
     * The primitive has to be given by argument for efficiency reasons (pre-calculated values).
     * The primitive can be computed with the primitive() method.
     *
     * \param primitive the tube primitive of this
     * \param t1 the lower interval-bound input
     * \param t2 the upper interval-bound input
     * \return an interval [s1,s2]
     */
    ibex::Interval timeIntegration(const Tube &primitive, const ibex::Interval& t1, const ibex::Interval& t2) const;

    /**
     * \brief Return an inner and outer approximation of the bounded-integral over the domain represented by [[t1],[t2]].
     *
     * The primitive has to be given by argument for efficiency reasons (pre-calculated values).
     * The primitive can be computed with the primitive() method.
     *
     * \param primitive the tube primitive of this
     * \param t1 the lower interval-bound input
     * \param t2 the upper interval-bound input
     * \return a pair of intervals [s1,s2],[S1,S2]
     */
    std::pair<ibex::Interval,ibex::Interval> partialTimeIntegration(const Tube &primitive, const ibex::Interval& t1, const ibex::Interval& t2) const;

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
     */
    void print() const;

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
        bool ctcFwd(const Tube& derivative_tube);
    
        /**
         * \brief To contract tube in backward (from the future to the past)
         * 
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcBwd(const Tube& derivative_tube);
    
        /**
         * \brief To contract tube in forward/backward
         * 
         * \param derivative_tube the derivative of this
         * \return true if a contraction has been done, false otherwise
         */
        bool ctcFwdBwd(const Tube& derivative_tube);

  protected:

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
     * \brief Intersect and update tube's values with the given y-value intv_y.
     *
     * \param intv_y the y-value to intersect
     * \param index slice's number, between 0 and (size - 1)
     * \param bool_update to force the update of tube's data structure
     * \return true if a contraction has been done, false otherwise
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
     * \brief Set this Tube to the intersection of itself and another: tube x.
     *
     * Note: both tubes have to be similar (same domain, same number of slices).
     * 
     * \param x the other tube
     * \param bool_update to force the update of tube's data structure
     * \return a reference to this
     */
    Tube& intersectWith(const Tube& x, bool bool_update);

    /**
     * \brief Set this Tube to the hull of itself and another: tube x.
     *
     * Note: both tubes have to be similar (same domain, same number of slices).
     * 
     * \param x the other tube
     * \param bool_update to force the update of tube's data structure
     * \return a reference to this
     */
    Tube& unionWith(const Tube& x, bool bool_update);

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
    
    // Class variables:

    double m_dt;
    int m_slices_number;
    Tube *m_first_subtube, *m_second_subtube;
    ibex::Interval m_intv_t, m_intv_y;
    std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
};

#endif