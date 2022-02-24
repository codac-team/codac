/** 
 *  ConnectedSubset class, methods related to topological degree computations
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou, Peter Franek
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include <iostream>
#include "codac_IntervalMatrix.h"
#include "codac_ConnectedSubset.h"
#include "codac_Paving.h"

using namespace std;
using namespace ibex;

namespace codac
{
  bool ConnectedSubset::zero_proven(const function<IntervalVector(const IntervalVector&)>& f)
  {
    return topological_degree(f) != 0;
  }

  int ConnectedSubset::zeros_number(const function<IntervalVector(const IntervalVector&)>& f, const function<IntervalMatrix(const IntervalVector&)>& Jf, float precision)
  {
    int degree = topological_degree(f);

    if(non_singular_jacobian(Jf, precision))
      return abs(degree);

    else
      return -1;
  }

  int ConnectedSubset::topological_degree(const function<IntervalVector(const IntervalVector&)>& f)
  {
    if(!is_strictly_included_in_paving())
    {
      // Unable to compute the degree on paving's edges
      return 0;
    }

    assert(box().size() == 2 && "unhandled dimension case");

    vector<IntervalVector> v_bi = get_boundary();
    assert(v_bi.size() >= 4 && "wrong boundaries");

    int degree = 0;

    for(const auto& b : v_bi)
    {
      assert(box_dimension(b) == 1 && "wrong dimension");

      vector<IntervalVector> v_cofaces = get_cofaces(b);
      int b_orientation = orientation(b, v_cofaces[0], 1);
      int local_deg = compute_local_degree(f, b, v_cofaces[0]);
      degree += b_orientation * local_deg;
    }

    return degree;
  }

  bool ConnectedSubset::all_positive_signs(const vector<int>& v_s) const
  {
    for(const auto& s : v_s)
      if(s != 1)
        return false;
    return true;
  }

  int ConnectedSubset::orientation(const IntervalVector& b, const IntervalVector& parent_coface, int orientation) const
  {
    assert(b.size() == 2 && parent_coface.size() == 2 && "unhandled dimension case");

    int j = 0;
    for(int i = 0 ; i < parent_coface.size() ; i++)
    {
      if(parent_coface[i].is_degenerated())
        continue;
      j++;

      if(b[i].is_degenerated() && !parent_coface[i].is_degenerated())
      {
        if(b[i].mid() == parent_coface[i].lb())
          return orientation * pow(-1,j);
        else if(b[i].mid() == parent_coface[i].ub())
          return orientation * pow(-1,j+1);
        else
          assert("unhandled case (1)");
      }
    }

    assert("unhandled case (2)");
    return 0;
  }

  int ConnectedSubset::compute_local_degree(const function<IntervalVector(const IntervalVector&)>& f, const IntervalVector& b, const IntervalVector& common_cocoface) const
  {
    assert(b.size() == 2 && common_cocoface.size() == 2 && "unhandled dimension case");

    size_t n = b.size();
    unsigned short k = box_dimension(b);

    vector<int> v_s = sign_vector(f, b, common_cocoface);

    assert(!(n-k-1 < 0 || n-k-1 >= v_s.size()));

    if(v_s[n-k-1] != 1) // we only care if the first positions are +
      return 0;

    if(k == 0) // points
      return all_positive_signs(v_s) ? 1 : 0;

    // Normally, at this step, for a 2D application, k should be equal to 1
    assert(k == 1);

    int sum = 0, j = 0;
    for(int i = 0 ; i < b.size() ; i++)
    {
      if(b[i].is_degenerated())
        continue;
      j++;

      // \partial_i^-
      IntervalVector box_face_l(b);
      box_face_l[i] = box_face_l[i].lb();
      sum += -1 * compute_local_degree(f, box_face_l, common_cocoface);

      // \partial_i^+
      IntervalVector box_face_u(b);
      box_face_u[i] = box_face_u[i].ub();
      sum += 1 * compute_local_degree(f, box_face_u, common_cocoface);
    }

    assert(j == 1);
    return sum;
  }

  vector<int> ConnectedSubset::sign_vector(const function<IntervalVector(const IntervalVector&)>& f, const IntervalVector& b, const IntervalVector& common_cocoface) const
  {
    assert(b.size() == 2 && common_cocoface.size() == 2 && "unhandled dimension case");

    vector<int> v_s;
    int dim = box_dimension(b);

    if(dim == 2)
    {
      IntervalVector c = f(b);
      for(const auto& ci : c)
        v_s.push_back(ci.contains(0.) ? 0 : (ci.mid() > 0. ? 1 : -1));
    }

    else
    {
      for(int i = 0 ; i < b.size() ; i++) // for now, m=n
        v_s.push_back(0);

      vector<IntervalVector> v_cofaces = get_cofaces(b, common_cocoface);
      for(const auto& coface : v_cofaces)
      {
        assert(box_dimension(coface) == dim+1 && "unhandled dimension case");
        vector<int> v_sj = sign_vector(f, coface, common_cocoface);

        for(size_t i = 0 ; i < v_sj.size() ; i++)
          if(v_sj[i] != 0)
          {
            assert(!(v_s[i] != 0 && v_s[i] != v_sj[i]) && "unknown case?");
            v_s[i] = v_sj[i];
          }
      }
    }

    return v_s;
  }

  vector<IntervalVector> ConnectedSubset::get_cofaces(const IntervalVector& b) const
  {
    assert(b.size() == 2 && "unhandled dimension case");
    IntervalVector cocoface(2, Interval::ALL_REALS);
    return get_cofaces(b, cocoface);
  }

  vector<IntervalVector> ConnectedSubset::get_cofaces(const IntervalVector& b, const IntervalVector& common_cocoface) const
  {
    assert(b.size() == 2 && common_cocoface.size() == 2 && "unhandled dimension case");

    int dim = box_dimension(b);
    vector<IntervalVector> v_cofaces;

    switch(dim)
    {
      case 0: // vertex, point
        {
          vector<IntervalVector> v_bi = get_boundary();

          int p = 0;
          for(const auto& bi : v_bi)
            if(bi.intersects(b) && (common_cocoface & bi).max_diam() != 0.)
              p++;

          if(p == 2) // Case of two subsets joined by a single point
          {
            for(const auto& bi : v_bi)
              if(bi.intersects(b) && (common_cocoface & bi).max_diam() != 0.)
                v_cofaces.push_back(bi);
          }

          else
          {
            for(const auto& bi : v_bi)
              if(bi.intersects(b))
                v_cofaces.push_back(bi);
          }

          assert(v_cofaces.size() == 2 && "nb of cofaces for a point should be 2");
        }
        break;

      case 1: // edge
        {
          vector<const Paving*> v_subpavings;
          get_paving()->get_pavings_intersecting(SetValue::OUT, b, v_subpavings, true);
          assert(v_subpavings.size() == 1 && "nb of cofaces for an edge should be 1");
          v_cofaces.push_back(v_subpavings[0]->box());
        }
        break;

      default:
        assert("unhandled dimension case");
    }

    return v_cofaces;
  }

  int ConnectedSubset::box_dimension(const IntervalVector& b) const
  {
    int k = b.size();
    for(int i = 0 ; i < b.size() ; i++)
      if(b[i].is_degenerated())
        k--;
    return k;
  }

  bool ConnectedSubset::non_singular_jacobian(const function<IntervalMatrix(const IntervalVector&)>& Jf, float precision)
  {
    assert(precision > 0.);
    
    // Inside the subpaving:
    list<Paving*> l;
    for(const auto& subset_item : m_v_subset_items)
      l.push_back((Paving*)subset_item);

    while(!l.empty())
    {
      Paving* top = l.front(); l.pop_front();
      IntervalMatrix v = Jf(top->box());

      assert(v.nb_cols() == 2 && v.nb_rows() == 2 && "unhandled matrix dim case");

      Interval det = v[0][0] * v[1][1] - v[0][1] * v[1][0];
      
      if(det.contains(0.)) // not non-singular
      {
        if(top->box().max_diam() < precision || precision == -1)
          return false;

        // Trying to bisect up to a given precision.
        // This is a feature to overcome uncertainties encountered 
        // when dealing with wide [t] boxes.
        top->bisect(0.5);

        l.push_back((Paving*)top->get_first_subpaving());
        l.push_back((Paving*)top->get_second_subpaving());
      }
    }

    return true;
  }
}