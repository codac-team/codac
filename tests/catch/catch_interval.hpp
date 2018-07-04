/* ============================================================================
 *  Catch extended to Ibex
 *
 *  This code is used for unit-tests purposes and based on the Catch library.
 *  https://github.com/philsquared/Catch
 *
 *  The Approx classes enable the floating point tolerance comparison of
 *  Intervals objects and pairs of Intervals.
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "tubex_Point.h"
#include "tubex_Polygon.h"

namespace Catch
{
  namespace Detail
  {
    #define DEFAULT_EPSILON std::numeric_limits<double>::epsilon()*100

    class ApproxIntv
    {
      public:
        explicit ApproxIntv(ibex::Interval value) :
            m_epsilon(DEFAULT_EPSILON),
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

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxIntv(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        double m_epsilon;
        ibex::Interval m_value;
    };

    class ApproxVector
    {
      public:
        explicit ApproxVector(ibex::Vector value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        friend bool operator ==(ibex::Vector lhs, ApproxVector const& rhs)
        {
          double e = rhs.m_epsilon;

          if(lhs != rhs.m_value)
            return false;

          for(int i = 0 ; i < lhs.size() ; i++)
            if(fabs(lhs[i] - rhs.m_value[i]) >= e)
              return false;

          return true;
        }

        friend bool operator ==(ApproxVector const& lhs, ibex::Vector rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(ibex::Vector lhs, ApproxVector const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxVector const& lhs, ibex::Vector rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxVector(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        double m_epsilon;
        ibex::Vector m_value = ibex::Vector(1);
    };

    class ApproxIntvPair
    {
      public:
        explicit ApproxIntvPair(std::pair<ibex::Interval,ibex::Interval> value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        friend bool operator ==(std::pair<ibex::Interval,ibex::Interval> lhs, ApproxIntvPair const& rhs)
        {
          double e = rhs.m_epsilon;
          return lhs == rhs.m_value ||
                 (ApproxIntv(lhs.first) == rhs.m_value.first && ApproxIntv(lhs.second) == rhs.m_value.second);
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

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxIntvPair(" << Catch::toString(m_value.first) << "," << Catch::toString(m_value.second) << ")";
          return oss.str();
        }

      private:
        double m_epsilon;
        std::pair<ibex::Interval,ibex::Interval> m_value;
    };

    class ApproxPoint
    {
      public:
        explicit ApproxPoint(tubex::Point value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        friend bool operator ==(tubex::Point lhs, ApproxPoint const& rhs)
        {
          return lhs == rhs.m_value ||
            lhs.t() == ApproxIntv(rhs.m_value.t()) && lhs.x() == ApproxIntv(rhs.m_value.x());
        }

        friend bool operator ==(ApproxPoint const& lhs, tubex::Point rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(tubex::Point lhs, ApproxPoint const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxPoint const& lhs, tubex::Point rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxPoint(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        double m_epsilon;
        tubex::Point m_value;
    };

    class ApproxPolygon
    {
      public:
        explicit ApproxPolygon(tubex::Polygon value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        friend bool operator ==(tubex::Polygon lhs, ApproxPolygon const& rhs)
        {
          if(lhs == rhs.m_value)
            return true;

          int n = lhs.getVertices().size();
          if(n != rhs.m_value.getVertices().size())
            return false;

          int i; // looking for first same elements
          for(i = 0 ; i < n ; i++)
            if(lhs.getVertices()[0] == ApproxPoint(rhs.m_value.getVertices()[i]))
              break;

          int way = 1;
          if(n > 1)
            way = (lhs.getVertices()[1] == ApproxPoint(rhs.m_value.getVertices()[(i+1) % n])) ? 1 : -1;

          for(int j = 0 ; j < n ; j++)
          {
            if(lhs.getVertices()[j] != ApproxPoint(rhs.m_value.getVertices()[(i+way*j+n) % n]))
              return false;
          }

          return true;
        }

        friend bool operator ==(ApproxPolygon const& lhs, tubex::Polygon rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(tubex::Polygon lhs, ApproxPolygon const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxPolygon const& lhs, tubex::Polygon rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxPolygon(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        double m_epsilon;
        tubex::Polygon m_value;
    };
  }

  template<>
  inline std::string toString<Detail::ApproxIntv>(Detail::ApproxIntv const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxVector>(Detail::ApproxVector const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxIntvPair>(Detail::ApproxIntvPair const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxPoint>(Detail::ApproxPoint const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxPolygon>(Detail::ApproxPolygon const& value)
  {
    return value.toString();
  }
}