/** 
 *  CtcLinobs class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcLinobs.h"
#include "tubex_Domain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  /*CtcLinobs::CtcLinobs(const Matrix& A, const Vector& b) : Ctc()
  {
    assert(A.nb_cols() == 2 && A.nb_rows() == 2);
    assert(b.size() == 2);

  }*/

  CtcLinobs::CtcLinobs(IntervalMatrix (*exp_At)(const Matrix& A, const Interval& t), const Vector& b)
    : Ctc(), m_exp_At(exp_At)
  {
    assert(b.size() == 2);

  }

  void CtcLinobs::contract(vector<Domain*>& v_domains)
  {
    assert(v_domains.size() == 2);
    assert(v_domains[0]->type() == Domain::Type::TUBE_VECTOR && v_domains[0]->type() == Domain::Type::TUBE);

    contract(v_domains[0]->tube_vector(), v_domains[1]->tube());
  }

  void CtcLinobs::contract(TubeVector& x, const Tube& u)
  {
    assert(Tube::same_slicing(x[0], u));
    assert(Tube::same_slicing(x[1], u));

  }
}