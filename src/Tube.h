/************************************************************/
/*    AUTH: Simon Rohou
/*    FILE: Tube.h
/*    PRJT: TubeIbex https://github.com/SimonRohou/TubeIbex
/*    DATE: 2015
/************************************************************/

#ifndef TubeIbex_HEADER
#define TubeIbex_HEADER

#include <utility>
#include <iostream>
#include "ibex/ibex.h"

class Tube
{
  public:

    /**
     * \brief Create a tube over the domain defined by the interval intv_t
     *
     * \param intv_t tube's domain
     * \param slices_number tube's sampling
     *
     * Values are set by default to empty sets.
     * Tube's sampling is defined by slices_number. Because tube's data structure 
     * is based on a perfect binary tree, slices_number has to be a power of 2.
     */
    Tube(const ibex::Interval &intv_t, double dt);
    Tube(std::vector<ibex::Interval> vector_dt, bool bool_update = true);

    /**
     * \brief Create a copy of the given tube tu
     *
     * \param tu the tube to be copied
     */
    Tube(const Tube& tu);

    /**
     * \brief Delete this tube
     */
    ~Tube();

    double volume() const;
    double dist(const Tube& tube) const;

    /**
     * \brief Return tube's size
     *
     * \return the number of slices defining the tube
     */
    int size() const;

    /**
     * \brief Return the width of a slice
     *
     * \return the width of a slice
     */
    double dt() const;

    /**
     * \brief Test if the tube is a box
     *
     * \return true if the tube is a slice, false otherwise
     */
    bool isSlice() const;

    /**
     * \brief Return the requested slice of the tube
     *
     * \param index slice's number, between 0 and (size - 1)
     * \return an object of class Tube corresponding to the slice indexed by index
     */
    Tube* getSlice(int index);

    /**
     * \brief Return the y-value of the ith slice
     *
     * A return a non-const reference to the i^th slice (i starts from 0)
     */
    const ibex::Interval& operator[](int index);
    const ibex::Interval& operator[](double t);
    ibex::Interval operator[](ibex::Interval intv_t) const;

    /**
     * \brief Return the number of the slice containing the given input
     *
     * \param t the input
     * \return an identifier representing the slice in the current tube
     */
    int input2index(double t) const;

    /**
     * \brief Return an input value of the slice represented by the given index
     *
     * \param index slice's number, between 0 and (size - 1)
     * \return an input in the slice
     */
    double index2input(int index);

    /**
     * \brief Return the domain of the tube
     *
     * \return an interval [t0,tf]
     */
    const ibex::Interval& getT() const;

    /**
     * \brief Return the domain of the slice represented by index
     *
     * \param index slice's number, between 0 and (size - 1)
     * \return an interval [t0,tf]
     */
    const ibex::Interval& getT(int index);

    /**
     * \brief Return the domain of the slice containing the given input
     *
     * \param t the input
     * \return an interval [t0,tf]
     */
    const ibex::Interval& getT(double t);

    /**
     * \brief Return the union-value over the all domain
     *
     * \return an interval [y1,y2]
     */
    const ibex::Interval& getY() const;

    /**
     * \brief Set the output value intv_y of the slice represented by index
     *
     * \param intv_y the output value to be set
     * \param index slice's number, between 0 and (size - 1)
     */
    void setY(const ibex::Interval& intv_y, int index);

    /**
     * \brief Set the output value intv_y of the slice containing the given input
     *
     * \param intv_y the output value to be set
     * \param t the input
     */
    void setY(const ibex::Interval& intv_y, double t);

    /**
     * \brief Set the output value intv_y over the domain represented by intv_t
     *
     * \param intv_y the output value to be set
     * \param intv_t the interval input, Interval::ALL_REALS by default
     */
    void setY(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);

    /**
     * \brief Return the half sub-tube ]tf/2,tf]
     *
     * \return the first part of the tube
     */
    const Tube* getFirstSubTube() const;

    /**
     * \brief Return the half sub-tube [t0,tf/2[
     *
     * \return the second part of the tube
     */
    const Tube* getSecondSubTube() const;

    /**
     * \brief Return the integral over the domain represented by [[t1],[t2]]
     *
     * \param t1 the lower interval-bound input
     * \param t2 the upper interval-bound input
     * \return an interval [s1,s2]
     */
    std::pair<ibex::Interval,ibex::Interval> partialTimeIntegration(const Tube &primitive, const ibex::Interval& t1, const ibex::Interval& t2) const;
    ibex::Interval timeIntegration(const Tube &primitive, const ibex::Interval& t1, const ibex::Interval& t2) const;

    /**
     * \brief Return the primitive tube of this
     *
     * \return a tube defined over the same domain of this
     */
    const Tube primitive();

    /**
     * \brief Return the primitive tube of this
     *
     * \return a tube defined over the same domain of this
     */
    const Tube primitive(const ibex::Interval& initial_value);

    /**
     * \brief Intersect and update tube's values with the given output intv_y
     *
     * \param intv_y the output value to intersect
     * \param index slice's number, between 0 and (size - 1)
     * \return true if a contraction has been done
     */
    bool intersect(const ibex::Interval& intv_y, int index);

    /**
     * \brief Intersect and update tube's values with the given output intv_y
     *
     * \param intv_y the output value to intersect
     * \param t the input
     * \return true if a contraction has been done
     */
    bool intersect(const ibex::Interval& intv_y, double t);

    /**
     * \brief Intersect and update tube's values with the given output intv_y
     *
     * \param intv_y the output value to intersect
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \param allow_update to allow the update of tube's data structure
     * \return true if a contraction has been done
     */
    bool intersect(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS, bool allow_update = true);

    /**
     * \brief Return enclosed bounds of the tube
     *
     * \return a pair of intervals representing all tube'smaxima and minima
     */
    const std::pair<const ibex::Interval&,const ibex::Interval&> getEnclosedBounds() const;

    /**
     * \brief Return enclosed bounds of the tube over the domain represented by intv_t
     *
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \return a pair of intervals representing all tube'smaxima and minima
     */
    const std::pair<ibex::Interval,ibex::Interval> getEnclosedBounds(const ibex::Interval& intv_t) const;
    const std::pair<ibex::Interval,ibex::Interval> getEnclosedBounds(const ibex::Interval& t1, const ibex::Interval& t2) const;

    /**
     * \brief Set this Tube to the hull of itself and another
     *
     * \return a reference to this
     */
    Tube& operator|=(const Tube& x);

    /**
     * \brief \brief Set *this to its intersection with x
     *
     * \return a reference to this
     */
    Tube& operator&=(const Tube& x);

    /**
     * \brief Display tube's values
     */
    void print() const;
    friend std::ostream& operator<<(std::ostream&, const Tube&);


    /** Contractors **/
    bool ctcFwd(const Tube& derivative_tube);
    bool ctcBwd(const Tube& derivative_tube);
    bool ctcFwdBwd(const Tube& derivative_tube);

  protected:

    void initFromSlicesVector(std::vector<ibex::Interval> vector_slices);

    /**
     * \brief Update tube's data structure 
     * 
     * Tube's structure is based on a perfect binary tree. When a
     * leaf is changed, the modification has to be propagated through
     * all upper branches.
     */
    void update();

    /**
     * \brief Update tube's data structure (focusing on branches containing t_focus)
     *
     * \param t_focus if specified, means an update is required for the given input
     */
    void updateFromInput(double t_focus);

    /**
     * \brief Update tube's data structure (focusing on branches containing index_focus)
     *
     * \param index_focus if specified, means an update is required for the given input
     */
    void updateFromIndex(int index_focus);
    
    double m_dt;
    int m_slices_number;
    Tube *m_first_subtube, *m_second_subtube;
    ibex::Interval m_intv_t, m_intv_y;
    std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
};

#endif