/** 
 *  \file codac2_TDomain.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include <memory>
#include "codac2_TSlice.h"

namespace codac2
{
  /**
   * \class TDomain
   * \brief Temporal partition associated with sliced tubes.
   *
   * A ``TDomain`` stores an ordered list of ``TSlice`` objects defining the
   * temporal decomposition of one or several sliced tubes over a time domain.
   *
   * Each element of the list is either:
   * - a non-degenerate temporal slice \f$[t_i,t_{i+1}]\f$, or
   * - a degenerate slice \f$[t_i,t_i]\f$, referred to as a *gate*.
   *
   * Gates are optional. When they are explicitly represented, the temporal
   * partition typically alternates between gates and non-degenerate slices.
   *
   * This object is shared by all ``SlicedTube`` instances defined on the same
   * temporal partition.
   */
  class TDomain : public std::list<TSlice>
  {
    protected:

      /**
       * \brief Creates a temporal domain made of a single ``TSlice``
       *
       * The resulting temporal domain contains only one slice over \f$[t_0,t_f]\f$.
       *
       * \param t0_tf temporal interval \f$[t_0,t_f]\f$
       */
      explicit TDomain(const Interval& t0_tf);

      /**
       * \brief Creates a sampled temporal domain over \f$[t_0,t_f]\f$
       *
       * The interval is subdivided with a nominal time step ``dt``.
       * If ``with_gates`` is set to ``true``, degenerate temporal slices are
       * inserted at sampling times.
       *
       * \param t0_tf bounded non-degenerate temporal interval \f$[t_0,t_f]\f$
       * \param dt sampling step
       * \param with_gates if set to ``true``, explicit gates are inserted
       */
      explicit TDomain(const Interval& t0_tf, double dt, bool with_gates = true);

    public:

      /**
       * \brief Returns the global temporal domain \f$[t_0,t_f]\f$
       *
       * \return hull of the first and last temporal bounds stored in this object
       */
      Interval t0_tf() const;

      /**
       * \brief Returns the number of temporal elements stored in this partition
       *
       * The count includes both non-degenerate slices and gates.
       *
       * \return number of ``TSlice`` objects
       */
      size_t nb_tslices() const;

      /**
       * \brief Returns the number of sliced tubes attached to this temporal domain
       *
       * \return number of tubes sharing this ``TDomain``
       */
      size_t nb_tubes() const;

      /**
       * \brief Tests whether all sampling gates are explicitly represented
       *
       * This method is intended to detect whether the temporal partition contains
       * explicit degenerate slices between consecutive non-degenerate slices.
       *
       * \return ``true`` if the temporal partition explicitly contains its gates,
       *         ``false`` otherwise
       */
      bool all_gates_defined() const;

      /**
       * \brief Returns a copy of the temporal slices as a ``std::vector``
       *
       * \return vector containing the ``TSlice`` objects of this temporal domain
       */
      std::vector<TSlice> tslices_vector() const;

      /**
       * \brief Returns the temporal slice containing \f$t\f$
       *
       * If \f$t\f$ coincides with an explicit gate, the iterator points to this
       * gate. If \f$t=t_f\f$, the iterator points to the last stored ``TSlice``
       * rather than ``end()``.
       *
       * \param t temporal value
       * \return iterator to the corresponding ``TSlice``; ``end()`` if \f$t\notin[t_0,t_f]\f$
       */
      std::list<TSlice>::iterator tslice(double t);

      /**
       * \brief Samples this temporal domain at time \f$t\f$
       *
       * If needed, the current temporal partition is refined so that \f$t\f$
       * becomes an explicit temporal boundary. If \f$t\f$ lies outside the
       * current temporal domain, the partition is extended accordingly.
       *
       * \param t temporal value at which the partition is refined
       * \param with_gate if set to ``true``, an explicit gate is created at \f$t\f$
       * \return iterator to the ``TSlice`` associated with \f$t\f$
       */
      std::list<TSlice>::iterator sample(double t, bool with_gate = false);

      /**
       * \brief Samples this temporal domain over a time interval
       *
       * The partition is refined repeatedly over the interval ``t0_tf`` using
       * the step ``dt``.
       *
       * \param t0_tf temporal interval to be sampled
       * \param dt sampling step
       * \param with_gates if set to ``true``, explicit gates are created
       */
      void sample(const Interval& t0_tf, double dt, bool with_gates = false);

      /**
       * \brief Removes all explicit gates from this temporal domain
       *
       * After this operation, only non-degenerate temporal slices remain.
       */
      void delete_gates();

      /**
       * \brief Stream output for a temporal domain
       *
       * The output contains the global temporal interval, the number of temporal
       * slices and the number of attached tubes.
       *
       * \param os output stream
       * \param x temporal domain to be displayed
       * \return reference to the output stream
       */
      friend std::ostream& operator<<(std::ostream& os, const TDomain& x);

      /**
       * \brief Tests whether two temporal domains share the same temporal partition
       *
       * Two temporal domains are considered identical if they contain the same
       * ordered sequence of ``TSlice`` objects.
       *
       * \param tdom1 first temporal domain
       * \param tdom2 second temporal domain
       * \return ``true`` if both temporal domains define the same partition,
       *         ``false`` otherwise
       */
      static bool are_same(const std::shared_ptr<const TDomain>& tdom1, const std::shared_ptr<const TDomain>& tdom2);

    protected:
      
      friend std::shared_ptr<TDomain> create_tdomain(const Interval&);
      friend std::shared_ptr<TDomain> create_tdomain(const Interval&,double,bool);
  };

  /**
   * \brief Creates a temporal domain made of a single slice
   *
   * This factory creates a ``TDomain`` containing a single ``TSlice`` over
   * ``t0_tf``.
   *
   * \param t0_tf temporal interval \f$[t_0,t_f]\f$
   * \return shared pointer to the created ``TDomain``
   */
  std::shared_ptr<TDomain> create_tdomain(const Interval& t0_tf = Interval(-oo,oo));
  
  /**
   * \brief Creates a sampled temporal domain
   *
   * This factory creates a temporal partition of ``t0_tf`` using a step ``dt``.
   * Optional explicit gates can be inserted at sampling times.
   *
   * \param t0_tf bounded non-degenerate temporal interval \f$[t_0,t_f]\f$
   * \param dt sampling step
   * \param with_gates if set to ``true``, explicit gates are inserted
   * \return shared pointer to the created ``TDomain``
   */
  std::shared_ptr<TDomain> create_tdomain(const Interval& t0_tf, double dt, bool with_gates = true);
}