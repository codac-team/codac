/* ============================================================================
 *  Catch extended to Ibex
 *
 *  This code is used for unit-tests purposes and based on the Catch library.
 *  https://github.com/philsquared/Catch
 *
 *  The Approx classes enable the floating point tolerance comparison of
 *  Intervals objects and pairs of Intervals.
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "ibex.h"


class ApproxIntv
{
  public:
    explicit ApproxIntv(ibex::Interval value) :
        m_epsilon(std::numeric_limits<float>::epsilon()*10),
        m_value(value)
    {}

    friend bool operator ==(ibex::Interval lhs, ApproxIntv const& rhs)
    {
      double e = rhs.m_epsilon;
      return lhs == rhs.m_value ||
             (fabs(lhs.lb() - rhs.m_value.lb()) < e && fabs(lhs.ub() - rhs.m_value.ub()) < e);
    }

    friend bool operator ==(ApproxIntv const& lhs, ibex::Interval rhs)
    {
      return operator ==(rhs, lhs);
    }

    friend bool operator !=(ibex::Interval lhs, ApproxIntv const& rhs)
    {
      return !operator ==(lhs, rhs);
    }

    friend bool operator !=(ApproxIntv const& lhs, ibex::Interval rhs)
    {
      return !operator ==(rhs, lhs);
    }

  private:
    double m_epsilon;
    ibex::Interval m_value;
};


class ApproxIntvPair
{
  public:
    explicit ApproxIntvPair(std::pair<ibex::Interval,ibex::Interval> value) :
        m_epsilon(std::numeric_limits<float>::epsilon()*10),
        m_value(value)
    {}

    friend bool operator ==(std::pair<ibex::Interval,ibex::Interval> lhs, ApproxIntvPair const& rhs)
    {
      double e = rhs.m_epsilon;
      return lhs == rhs.m_value ||
             (fabs(lhs.first.lb() - rhs.m_value.first.lb()) < e &&
              fabs(lhs.first.ub() - rhs.m_value.first.ub()) < e &&
              fabs(lhs.second.lb() - rhs.m_value.second.lb()) < e &&
              fabs(lhs.second.ub() - rhs.m_value.second.ub()) < e);
    }

    friend bool operator ==(ApproxIntvPair const& lhs, std::pair<ibex::Interval,ibex::Interval> rhs)
    {
      return operator ==(rhs, lhs);
    }

    friend bool operator !=(std::pair<ibex::Interval,ibex::Interval> lhs, ApproxIntvPair const& rhs)
    {
      return !operator ==(lhs, rhs);
    }

    friend bool operator !=(ApproxIntvPair const& lhs, std::pair<ibex::Interval,ibex::Interval> rhs)
    {
      return !operator ==(rhs, lhs);
    }

  private:
    double m_epsilon;
    std::pair<ibex::Interval,ibex::Interval> m_value;
};