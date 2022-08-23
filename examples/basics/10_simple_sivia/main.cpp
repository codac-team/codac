/**
 *  Codac - Examples
 *  Using SIVIA
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example of SIVIA usage

 *  \date       2022
 *  \author     Julien Damers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;
using namespace vibes;

int main(int argc, char** argv)
{
    Function f("x", "y", "x*cos(x-y)+y");
    CtcFwdBwd ctc(f,Interval::NEG_REALS);
    SepFwdBwd sep(f,Interval::NEG_REALS);
    IntervalVector X0({{-10,10},{-10,10}});



    beginDrawing();

    cout << "sep regular" << endl;
    SIVIA(X0, sep, 0.1,true, true, "regular",false,{{SetValue::IN,"lightgrey[white]"},{SetValue::OUT,"#009E73[#56B4E9]"},{SetValue::UNKNOWN,"#D55E00[#CC79A7]"}});
    cout << "sep diff" << endl;
    SIVIA(X0, sep, 0.1,false, true, "diff");


    cout << "ctc regular" << endl;
    SIVIA(X0, ctc, 0.1, true, true,"ctc_r");
    cout << "ctc diff" << endl;
    SIVIA(X0, ctc, 0.1,false, true, "ctc_d");
    endDrawing();


}
