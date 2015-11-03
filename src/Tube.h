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
    Tube(const ibex::Interval &intv_t, unsigned int slices_number);

    /**
     * \brief Create a copy of the given tube tu
     *
     * \param tu the tube to be copied
     */
    Tube(Tube* tu);

    /**
     * \brief Delete this tube
     */
    ~Tube();

    /**
     * \brief Return tube's size
     *
     * \return the number of slices defining the tube
     */
    int size() const;

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
    double index2input(int index) const;

    /**
     * \brief Return the domain of the tube
     *
     * \return an interval [t0,tf]
     */
    const ibex::Interval& getT();

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
     * \brief Return the value of the slice represented by index
     *
     * \param index slice's number, between 0 and (size - 1)
     * \return an interval [y1,y2]
     */
    const ibex::Interval& getY(int index);

    /**
     * \brief Return the value of the slice containing the given input
     *
     * \param t the input
     * \return an interval [y1,y2]
     */
    const ibex::Interval& getY(double t);

    /**
     * \brief Return the union-value over the domain represented by intv_t
     *
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \return an interval [y1,y2]
     */
    ibex::Interval getY(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);

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
    Tube* getFirstSubTube() const;

    /**
     * \brief Return the half sub-tube [t0,tf/2[
     *
     * \return the second part of the tube
     */
    Tube* getSecondSubTube() const;

    /**
     * \brief Return the integral over the domain represented by intv_t
     *
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \return an interval [s1,s2]
     */
    ibex::Interval getIntegral(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);

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
    std::pair<ibex::Interval,ibex::Interval> getEnclosedBounds() const;

    /**
     * \brief Return enclosed bounds of the tube over the domain represented by intv_t
     *
     * \param intv_t the interval input, Interval::ALL_REALS by default
     * \return a pair of intervals representing all tube'smaxima and minima
     */
    std::pair<ibex::Interval,ibex::Interval> getEnclosedBounds(const ibex::Interval& intv_t) const;

    /**
     * \brief Display tube's values
     */
    void print() const;
    friend std::ostream& operator<<(std::ostream&, const Tube&);

  protected:

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
    
    int m_slices_number;
    Tube *m_first_subtube, *m_second_subtube;
    ibex::Interval m_intv_t, m_intv_y;
    ibex::Interval m_intv_integral;
    std::pair<ibex::Interval,ibex::Interval> m_enclosed_bounds;
};

#endif