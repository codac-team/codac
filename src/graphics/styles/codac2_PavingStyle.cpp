/** 
 *  codac2_PavingStyle.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_PavingStyle.h"

using namespace std;
using namespace codac2;


PavingStyle::PavingStyle()
{ }

PavingStyle::PavingStyle(const StyleProperties& _boundary, const StyleProperties& _outside, const StyleProperties& _inside)
  : boundary(_boundary), outside(_outside), inside(_inside)
{ }

PavingStyle PavingStyle::blue_green()
{
  StyleProperties boundary;
  boundary.stroke_color = Color::dark_gray();
  boundary.fill_color = Color::yellow();
  boundary.layer = "boundary";

  StyleProperties outside;
  outside.stroke_color = Color::dark_gray();
  outside.fill_color = Color::light_blue();
  outside.layer = "outside";

  StyleProperties inside;
  inside.stroke_color = Color::dark_gray();
  inside.fill_color = Color::green();
  inside.layer = "inside";

  return { boundary, outside, inside };
}

PavingStyle PavingStyle::blue_white()
{
  StyleProperties boundary;
  boundary.stroke_color = Color::dark_orange();
  boundary.fill_color = Color::orange();
  boundary.layer = "boundary";

  StyleProperties outside;
  outside.stroke_color = Color({0,158,115}, Model::RGB);
  outside.fill_color = Color({86,180,233}, Model::RGB);
  outside.layer = "outside";

  StyleProperties inside;
  inside.stroke_color = Color::gray();
  inside.fill_color = Color::white();
  inside.layer = "inside";

  return { boundary, outside, inside };
}

PavingStyle PavingStyle::blue_pink()
{
  StyleProperties boundary;
  boundary.stroke_color = Color({153,119,0}, Model::RGB);
  boundary.fill_color = Color({238,204,102}, Model::RGB);
  boundary.layer = "boundary";

  StyleProperties outside;
  outside.stroke_color = Color({0,68,136}, Model::RGB);
  outside.fill_color = Color({102,153,204}, Model::RGB);
  outside.layer = "outside";

  StyleProperties inside;
  inside.stroke_color = Color({153,68,85}, Model::RGB);
  inside.fill_color = Color({238,153,170}, Model::RGB);
  inside.layer = "inside";

  return { boundary, outside, inside };
}

PavingStyle PavingStyle::black_white()
{
  StyleProperties boundary;
  boundary.stroke_color = Color::dark_gray();
  boundary.fill_color = Color::gray();
  boundary.layer = "boundary";

  StyleProperties outside;
  outside.stroke_color = Color::dark_gray();
  outside.fill_color = Color::black();
  outside.layer = "outside";

  StyleProperties inside;
  inside.stroke_color = Color::white();
  inside.fill_color = Color::light_gray();
  inside.layer = "inside";

  return { boundary, outside, inside };
}