// File temporarily extracted from pyibex repo (Benoît Desrochers)
// todo: make a clean dependency

//============================================================================
//                                P Y I B E X
// Author      : Mohamed Saad Ibn Seddik, Benoit Desrochers
//       Inspired from the work of Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPL v3
// Created     : May 24, 2015
// Last Update : Jun 29, 2017
//============================================================================

#ifndef __IBEX_CTC_POLAR_H__
#define __IBEX_CTC_POLAR_H__

#include "ibex_IntervalVector.h"
#include "ibex_Ctc.h"

using ibex::Interval;
using ibex::IntervalVector;
using ibex::Ctc;



using namespace std;

namespace pyibex {

/**
 * \ingroup geometry
 * Minimal contractor for the polar constraint:
 *  x        = rho*cos(theta)
 *  y        = rho*sin(theta)
 *  theta    = angle(x,y)
 *  sqr(rho) = sqr(x)+sqr(y)
 */
class CtcPolar : public Ctc {

public:
  CtcPolar();

  /**
   * @brief Contract the box
   * @details Contract the box according to the polar constraints
   *
   * @param box Box of Intervals to contract. box=([x],[y],[rho],[theta])
   */
  virtual void contract(IntervalVector& box);

  /**
   * @brief Contract a the given intervals.
   *
   * @param x Interval x
   * @param y Interval y
   * @param rho Interval rho
   * @param theta Interval theta
   */
  void contract(Interval &x, Interval& y, Interval& rho, Interval& theta);

  /**
   * \brief Return polar coordinate from Cartesian ones
   *
   * \param x x coordinate
   * \param y y coordinate
   * \return 2d polar coordinates
   */
  IntervalVector RTfromXY(Interval x, Interval y);

  /**
   * \brief Delete this.
   */
  ~CtcPolar();

protected:
};


/**
 * \ingroup geometry
 * Minimal contractor for the polar constraint:
 *  x        = rho*cos(theta)
 *  y        = rho*sin(theta)
 * where rho and theta are not contracted
 */
class CtcPolarXY : public Ctc {
public:
  CtcPolarXY(const Interval &rho, const Interval& theta): Ctc(2), rho(rho), theta(theta){}

  /**
   * @brief Contract the box
   * @details Contract the box according to the polar constraints
   *
   * @param box Box of Intervals to contract. box=([x],[y])
   */
  virtual void contract(IntervalVector& box);
private:
  Interval rho, theta;
};

class CtcPolarXY_landmark : public Ctc {
public:
  /**
   * \brief Polar contractor for the position XY with respect to a landmarks
   * @param rho: measured range to the landmark
   * @param theta: measured angle to the landmarks
   * @param mx : x coordinate of the landmark
   * @param my : y coordinate of the landmark
   **/
  CtcPolarXY_landmark(const Interval &rho, const Interval& theta,
                      const Interval &mx, const Interval &my):
    Ctc(2), rho(rho), theta(theta), mx(mx), my(my){}
  /**
   * @brief Contract the box
   * @details Contract the box according to the polar constraints
   *
   * @param box Box of Intervals to contract. box=([x],[y])
   */
  virtual void contract(IntervalVector& box);

private:
  Interval rho, theta, mx, my;
};


class CtcPolarXYT_landmark : public Ctc {
public:
  /**
   * \brief Polar contractor for the position XY with respect to a landmarks
   * @param rho: measured range to the landmark
   * @param theta: measured angle to the landmarks
   * @param mx : x coordinate of the landmark
   * @param my : y coordinate of the landmark
   **/
  CtcPolarXYT_landmark(const Interval &rho, const Interval& theta,
                      const Interval &mx, const Interval &my):
    Ctc(3), rho(rho), theta(theta), mx(mx), my(my){}
  /**
   * @brief Contract the box
   * @details Contract the box according to the polar constraints
   *
   * @param box Box of Intervals to contract. box=([x],[y])
   */
  virtual void contract(IntervalVector& box);

private:
  Interval rho, theta, mx, my;
};

} // end namespace

#endif // __IBEX_CTC_POLAR_H__
