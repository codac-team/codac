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
  CtcFwdBwd ctc(f, Interval(-oo,0));
  SepFwdBwd sep(f, Interval(-oo,0));
  IntervalVector X0({{-10,10},{-10,10}});

  const SetColorMap custom_colormap = {
    {SetValue::IN,"#E2884F[#E2C54F]"},
    {SetValue::OUT,"#009E73[#56B4E9]"},
    {SetValue::UNKNOWN,"#D55E00[#CC79A7]"}};

  string figname;
  beginDrawing();

  figname = "Sep regular";
  newFigure(figname); cout << figname << endl;
  SIVIA(X0, sep, 0.1, true, true, figname, false, custom_colormap);
  setFigureProperties(vibesParams("x",10, "y",10, "width",500, "height",500));
  axisAuto();

  figname = "Sep diff";
  newFigure(figname); cout << figname << endl;
  SIVIA(X0, sep, 0.1, false, true, figname, false, custom_colormap);
  setFigureProperties(vibesParams("x",510, "y",10, "width",500, "height",500));
  axisAuto();

  figname = "Ctc regular";
  newFigure(figname); cout << figname << endl;
  SIVIA(X0, ctc, 0.1, true, true, figname, false, custom_colormap);
  setFigureProperties(vibesParams("x",10, "y",510, "width",500, "height",500));
  axisAuto();

  figname = "Ctc diff";
  newFigure(figname); cout << figname << endl;
  SIVIA(X0, ctc, 0.1, false, true, figname, false, custom_colormap);
  setFigureProperties(vibesParams("x",510, "y",510, "width",500, "height",500));
  axisAuto();

  endDrawing();

  return EXIT_SUCCESS;
}