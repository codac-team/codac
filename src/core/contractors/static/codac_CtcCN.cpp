//
// Created by julien-damers on 19/11/2021.
//

#include "codac_CtcCN.h"

using namespace ibex;
using namespace codac;
using namespace std;

namespace codac
{
    CtcCN::CtcCN(ContractorNetwork *cn, IntervalVectorVar *box)
            : Ctc(box->size()), m_cn(cn), m_box(box)
    {

    }

    void CtcCN::contract(ibex::IntervalVector &x)
    {
        assert(x.size() == m_box->size() && "the box you are trying to contract does not match the size of the symbolic box");
        m_cn->reset_interm_vars();
        m_cn->contract({
                               {*m_box, x} // the "box" var is associated with "x"
                               // for this contraction procedure
                       });
    }

}


