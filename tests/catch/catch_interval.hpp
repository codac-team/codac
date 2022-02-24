/* ============================================================================
 *  Catch extended to Codac
 *
 *  This code is used for unit-tests purposes and based on the Catch library.
 *  https://github.com/philsquared/Catch
 *
 *  The Approx classes enable the floating point tolerance comparison of
 *  codac::Intervals objects and pairs of codac::Intervals.
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#ifndef __CODAC_CATCH_H__
#define __CODAC_CATCH_H__

#include "catch.hpp"
#include "codac_Vector.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_Point.h"
#include "codac_ConvexPolygon.h"
#include "codac_TubeVector.h"
#include "codac_Slice.h"

namespace Catch
{
  namespace Detail
  {
    #define DEFAULT_EPSILON std::numeric_limits<double>::epsilon()*100

    class ApproxIntv
    {
      public:
        explicit ApproxIntv(codac::Interval value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        explicit ApproxIntv(codac::Interval value, double epsilon) :
            m_epsilon(epsilon),
            m_value(value)
        {}

        friend bool operator ==(codac::Interval lhs, ApproxIntv const& rhs)
        {
          double e = rhs.m_epsilon;
          return lhs == rhs.m_value ||
                 (fabs(lhs.lb() - rhs.m_value.lb()) < e && fabs(lhs.ub() - rhs.m_value.ub()) < e);
        }

        friend bool operator ==(ApproxIntv const& lhs, codac::Interval rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::Interval lhs, ApproxIntv const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxIntv const& lhs, codac::Interval rhs)
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
        codac::Interval m_value;
    };

    class ApproxIntvVector
    {
      public:
        explicit ApproxIntvVector(codac::IntervalVector value) :
            m_value(value)
        {}

        friend bool operator ==(codac::IntervalVector lhs, ApproxIntvVector const& rhs)
        {
          if(lhs.size() != rhs.m_value.size())
            return false;
          for(int i = 0 ; i < rhs.m_value.size() ; i++)
            if(lhs[i] != ApproxIntv(rhs.m_value[i]))
              return false;
          return true;
        }

        friend bool operator ==(ApproxIntvVector const& lhs, codac::IntervalVector rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::IntervalVector lhs, ApproxIntvVector const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxIntvVector const& lhs, codac::IntervalVector rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxIntvVector(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        codac::IntervalVector m_value;
    };

    class ApproxSlice
    {
      public:
        explicit ApproxSlice(codac::Slice value) :
            m_value(value)
        {}

        friend bool operator ==(codac::Slice lhs, ApproxSlice const& rhs)
        {
          return lhs.tdomain() == ApproxIntv(rhs.m_value.tdomain()) &&
                 lhs.codomain() == ApproxIntvVector(rhs.m_value.codomain()) &&
                 lhs.input_gate() == ApproxIntvVector(rhs.m_value.input_gate()) &&
                 lhs.output_gate() == ApproxIntvVector(rhs.m_value.output_gate());
        }

        friend bool operator ==(ApproxSlice const& lhs, codac::Slice rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::Slice lhs, ApproxSlice const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxSlice const& lhs, codac::Slice rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxSlice(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        codac::Slice m_value;
    };

    class ApproxTube
    {
      public:
        explicit ApproxTube(codac::Tube value) :
            m_value(value)
        {}

        friend bool operator ==(codac::Tube lhs, ApproxTube const& rhs)
        {
          if(lhs.nb_slices() != rhs.m_value.nb_slices())
            return false;

          const codac::Slice *s = lhs.first_slice(), *s_x = rhs.m_value.first_slice();
          while(s != nullptr)
          {
            if(*s != ApproxSlice(*s_x)) // todo
              return false;
            s = s->next_slice();
            s_x = s_x->next_slice();
          }

          return true;
        }

        friend bool operator ==(ApproxTube const& lhs, codac::Tube rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::Tube lhs, ApproxTube const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxTube const& lhs, codac::Tube rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxTube(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        codac::Tube m_value;
    };

    class ApproxTubeVector
    {
      public:
        explicit ApproxTubeVector(codac::TubeVector value) :
            m_value(value)
        {}

        friend bool operator ==(codac::TubeVector lhs, ApproxTubeVector const& rhs)
        {
          if((lhs.nb_slices() != rhs.m_value.nb_slices()) || (lhs.size() != rhs.m_value.size()))
            return false;
          for(int i = 0 ; i < lhs.size() ; i++)
            if(lhs[i] != ApproxTube(rhs.m_value[i]))
              return false;
          return true;
        }

        friend bool operator ==(ApproxTubeVector const& lhs, codac::TubeVector rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::TubeVector lhs, ApproxTubeVector const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxTubeVector const& lhs, codac::TubeVector rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxTubeVector(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        codac::TubeVector m_value;
    };

    class ApproxVector
    {
      public:
        explicit ApproxVector(codac::Vector value, double epsilon = 100.*DEFAULT_EPSILON) :
            m_epsilon(epsilon),
            m_value(value)
        {}

        friend bool operator ==(codac::Vector lhs, ApproxVector const& rhs)
        {
          double e = rhs.m_epsilon;

          if(lhs == rhs.m_value)
            return true;

          for(int i = 0 ; i < lhs.size() ; i++)
            if(fabs(lhs[i] - rhs.m_value[i]) >= e)
              return false;

          return true;
        }

        friend bool operator ==(ApproxVector const& lhs, codac::Vector rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::Vector lhs, ApproxVector const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxVector const& lhs, codac::Vector rhs)
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
        codac::Vector m_value = codac::Vector(1);
    };

    class ApproxIntvPair
    {
      public:
        explicit ApproxIntvPair(std::pair<codac::Interval,codac::Interval> value) :
            m_value(value)
        {}

        friend bool operator ==(std::pair<codac::Interval,codac::Interval> lhs, ApproxIntvPair const& rhs)
        {
          return lhs == rhs.m_value ||
                 (ApproxIntv(lhs.first) == rhs.m_value.first && ApproxIntv(lhs.second) == rhs.m_value.second);
        }

        friend bool operator ==(ApproxIntvPair const& lhs, std::pair<codac::Interval,codac::Interval> rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(std::pair<codac::Interval,codac::Interval> lhs, ApproxIntvPair const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxIntvPair const& lhs, std::pair<codac::Interval,codac::Interval> rhs)
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
        std::pair<codac::Interval,codac::Interval> m_value;
    };

    class ApproxPoint
    {
      public:
        explicit ApproxPoint(codac::Point value) :
            m_value(value)
        {
          m_value.inflate(100.*DEFAULT_EPSILON);
        }

        friend bool operator ==(codac::Point lhs, ApproxPoint const& rhs)
        {
          return lhs == rhs.m_value ||
            (lhs.x().intersects(rhs.m_value.x()) && lhs.y().intersects(rhs.m_value.y()));
        }

        friend bool operator ==(ApproxPoint const& lhs, codac::Point rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::Point lhs, ApproxPoint const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxPoint const& lhs, codac::Point rhs)
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
        codac::Point m_value;
    };

    class ApproxConvexPolygon
    {
      public:
        explicit ApproxConvexPolygon(codac::ConvexPolygon value, double epsilon = 100.*DEFAULT_EPSILON) :
            m_epsilon(epsilon),
            m_value(value)
        {

        }

        const codac::ConvexPolygon& polygon() const
        {
          return m_value;
        }

        friend bool operator ==(codac::ConvexPolygon lhs, ApproxConvexPolygon const& rhs)
        {
          if(lhs == rhs.m_value)
            return true;

          size_t n = lhs.vertices().size();
          if(n != rhs.m_value.vertices().size())
            return false;

          bool found = false;
          size_t i; // looking for first same elements
          for(i = 0 ; i < n ; i++)
            if(lhs.vertices()[0] == ApproxVector(rhs.m_value.vertices()[i], rhs.m_epsilon))
            {
              found = true;
              break;
            }

          if(!found)
            return false; // no common first element

          int way = 1;
          if(n > 1)
            way = (lhs.vertices()[1] == ApproxVector(rhs.m_value.vertices()[(i+1) % n], rhs.m_epsilon)) ? 1 : -1;

          for(size_t j = 0 ; j < n ; j++)
            if(lhs.vertices()[j] != ApproxVector(rhs.m_value.vertices()[(i+way*j+n) % n], rhs.m_epsilon))
              return false;

          return true;
        }

        friend bool operator ==(ApproxConvexPolygon const& lhs, codac::ConvexPolygon rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(codac::ConvexPolygon lhs, ApproxConvexPolygon const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxConvexPolygon const& lhs, codac::ConvexPolygon rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "ApproxConvexPolygon(" << Catch::toString(m_value) << ")";
          return oss.str();
        }

      private:
        double m_epsilon;
        codac::ConvexPolygon m_value;
    };
  }

  template<>
  inline std::string toString<Detail::ApproxIntv>(Detail::ApproxIntv const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxIntvVector>(Detail::ApproxIntvVector const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxTubeVector>(Detail::ApproxTubeVector const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxSlice>(Detail::ApproxSlice const& value)
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
  inline std::string toString<Detail::ApproxConvexPolygon>(Detail::ApproxConvexPolygon const& value)
  {
    return value.toString();
  }
}

#endif