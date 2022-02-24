/** 
 *  \file
 *  ConnectedSubset class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou, Peter Franek
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CONNECTEDSUBSET_H__
#define __CODAC_CONNECTEDSUBSET_H__

#include <functional>
#include "codac_IntervalMatrix.h"
#include "codac_Set.h"

namespace codac
{
  class Paving;

  /**
   * \class ConnectedSubset
   * \brief Multi-dimensional paving representation of a connected subset
   *
   * A connected subset is a topological space that cannot be represented
   * as the union of two or more disjoint non-empty open subsets.
   *
   * This object gathers a set of Paving leaves items that represent a subset of
   * a paving structure.
   */
  class ConnectedSubset : public Set
  {
    public:

      /// \name Basics
      /// @{

      /**
       * \brief Creates a connected subset
       *
       * \param v_subset_items set of Paving leaves this object will be made of
       */
      ConnectedSubset(const std::vector<const Paving*>& v_subset_items);

      /**
       * \brief ConnectedSubset destructor
       */
      ~ConnectedSubset();

      /**
       * \brief Tests if this subset is strictly included in its paving structure
       *
       * \return `true` in case of strict inclusion
       */
      bool is_strictly_included_in_paving() const;

      /**
       * \brief Returns a const pointer to the paving structure
       *
       * \return a const pointer to the Paving object
       */
      const Paving* get_paving() const;

      /**
       * \brief Returns the set of Paving leaves this subset is made of
       *
       * \return a vector of Paving leaves items
       */
      const std::vector<const Paving*>& get_items() const; // items of type k

      /**
       * \brief Returns the set of Paving boxes this subset is made of
       *
       * \return a vector of boxes
       */
      std::vector<IntervalVector> get_boxes() const; // items of type k

      /**
       * \brief Returns the boundary boxes of this subset
       *
       * Note that the returned boxes are degenerated as they represent
       * subpaving's edges.
       *
       * \param value_boundary value of the items representing the boundary of the subset
       * \param value_out value of the items representing no-solution boxes in the paving
       * \return a vector of boxes
       */
      std::vector<IntervalVector> get_boundary(SetValue value_boundary = SetValue::UNKNOWN, SetValue value_out = SetValue::OUT) const; // items of type k-1

      /**
       * \brief Returns the boxed hull of each connected subset into a vector of boxes
       * 
       * \param v_subsets vector of connected subsets
       * \return a vector of boxes
       */
      static const std::vector<IntervalVector> get_boxed_hulls(const std::vector<ConnectedSubset> v_subsets);

      /// @}
      /// \name Methods related to topological degree
      /// @{

      /**
       * \brief Tests if an unknown function \f$\mathbf{f}^*\f$ enclosed by 
       *        \f$[\mathbf{f}]\f$ vanishes at least once on this subset
       *
       * This method stands on topological degree computations. This has been the object
       * of the publication:
       *
       *     Proving the existence of loops in robot trajectories
       *     S. Rohou, P. Franek, C. Aubry, L. Jaulin
       *     The International Journal of Robotics Research, 2018
       *
       * \note If this method returns `false`, it does not mean a zero cannot exist on this
       *       subset. It only corresponds to a case of undecidability.
       *
       * \param f the inclusion function \f$[\mathbf{f}]:\mathbb{IR}^2\to\mathbb{IR}^2\f$
       * \return `true` in case of at least one zero proven on this subset, `false` in case of undecidability
       */
      bool zero_proven(const std::function<IntervalVector(const IntervalVector&)>& f);

      /**
       * \brief Counts the number of zeros of an uncertain function \f$\mathbf{f}^*\f$
       *        enclosed by \f$[\mathbf{f}]\f$ on this subset
       *
       * This method stands on topological degree computations. This has been the object
       * of the publication:
       *
       *     Proving the existence of loops in robot trajectories
       *     S. Rohou, P. Franek, C. Aubry, L. Jaulin
       *     The International Journal of Robotics Research, 2018
       *
       * \param f the inclusion function \f$[\mathbf{f}]:\mathbb{IR}^2\to\mathbb{IR}^2\f$
       * \param Jf Jacobian matrix \f$[\mathbf{J_f}]\f$ of the unknown function \f$\mathbf{f}^*\f$
       * \param precision The subset may be made of wide boxes \f$[\mathbf{t}]_k\f$ that will
       *                  result in an over-approximation of the \f$[\mathbf{J_f}]([\mathbf{t}]_k)\f$.
       *                  A bisection of the \f$[\mathbf{t}]_k\f$ may be applied on order to deal with
       *                  smaller boxes, thus reducing the pessimism of the Jacobian evaluation and thus
       *                  the chances of concluding about the number of zeros. This parameter is the
       *                  precision limit of this auto-refinement.
       * \return the number of zeros, or -1 in case of undecidability
       */
      int zeros_number(const std::function<IntervalVector(const IntervalVector&)>& f, const std::function<IntervalMatrix(const IntervalVector&)>& Jf, float precision);

      /// @}

    protected:

      /// \name Protected methods for topological degree computation
      /// @{

      /**
       * \brief Computes the topological degree related to \f$\mathbf{f}\f$
       *
       * \param f the inclusion function \f$[\mathbf{f}]:\mathbb{IR}^2\to\mathbb{IR}^2\f$
       * \return degree number
       */
      int topological_degree(const std::function<IntervalVector(const IntervalVector&)>& f);

      /**
       * \brief Returns `true` if all items in v_s are positive
       *
       * \param v_s vector of integers
       * \return `true` if all positive
       */
      bool all_positive_signs(const std::vector<int>& v_s) const;

      /**
       * \brief Returns orientation of vector b
       *
       * \param b
       * \param parent_coface
       * \param orientation 
       * \return orientation integer
       */
      int orientation(const IntervalVector& b, const IntervalVector& parent_coface, int orientation) const;

      /**
       * \brief Computes local degree related to \f$\mathbf{f}\f$
       *
       * \param f the inclusion function \f$[\mathbf{f}]:\mathbb{IR}^2\to\mathbb{IR}^2\f$
       * \param b
       * \param common_cocoface
       * \return local degree number
       */
      int compute_local_degree(const std::function<IntervalVector(const IntervalVector&)>& f, const IntervalVector& b, const IntervalVector& common_cocoface) const;
      
      /**
       * \brief Returns a vector of signs represented as integers
       *
       * \param f
       * \param b
       * \param common_cocoface
       * \return vector of integers
       */
      std::vector<int> sign_vector(const std::function<IntervalVector(const IntervalVector&)>& f, const IntervalVector& b, const IntervalVector& common_cocoface) const;
      
      /**
       * \brief Returns a vector of cofaces related to \f$[\mathbf{b}]\f$
       *
       * \param b
       * \return vector of cofaces boxes
       */
      std::vector<IntervalVector> get_cofaces(const IntervalVector& b) const;

      /**
       * \brief Returns a vector of cofaces related to \f$[\mathbf{b}]\f$, common_cocoface
       *
       * \param b
       * \param common_cocoface
       * \return vector of cofaces boxes
       */
      std::vector<IntervalVector> get_cofaces(const IntervalVector& b, const IntervalVector& common_cocoface) const;

      /**
       * \brief Returns the dimension of the box \f$[\mathbf{b}]\f$
       *
       * \param b
       * \return integer dimension
       */
      int box_dimension(const IntervalVector& b) const;

      /**
       * \brief Tests whether the Jacobian is non-singular or not
       *
       * \param Jf Jacobian matrix function
       * \param precision of the bisected method used in case of ambiguity on the already existing paving structure
       * \return `true` if the Jacobian is non-singular
       */
      bool non_singular_jacobian(const std::function<IntervalMatrix(const IntervalVector&)>& Jf, float precision);

      /// @}

    protected:

      std::vector<const Paving*> m_v_subset_items; //!< set of leaves items
  };
}

#endif