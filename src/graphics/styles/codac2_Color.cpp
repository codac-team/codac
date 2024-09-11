/** 
 *  codac2_Color.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Color.h"

using namespace std;
using namespace codac2;

string Color::to_hex_str(const string& prefix) const
{
  if(hex_str.empty())
  {
    hex_str = prefix;
   
    char char_r[255];
    sprintf(char_r, "%.2X", (int)(r * 255));
    hex_str.append(char_r);
    char char_g[255];
    sprintf(char_g, "%.2X", (int)(g * 255));
    hex_str.append(char_g);
    char char_b[255];
    sprintf(char_b, "%.2X", (int)(b * 255));
    hex_str.append(char_b);

    if(alpha != 1.)
    {
      char char_alpha[255];
      sprintf(char_alpha, "%.2X", (int)(alpha * 255));
      hex_str.append(char_alpha);
    }
  }

  return hex_str;
}