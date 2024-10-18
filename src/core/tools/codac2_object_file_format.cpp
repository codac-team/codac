/** 
 *  codac2_object_file_format.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <fstream>
#include "codac2_object_file_format.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  void export_to_ObjectFileFormat(const list<IntervalVector>& l, const string& file_name, const string& color)
  {
    ofstream of;
    of.open("./" + file_name);
    size_t n = l.size();

    // First line (optional): the letters OFF to mark the file type
    of << "OFF" << endl;

    // Second line: the number of vertices, number of faces, and number of edges
    of << 8*n << " " << 6*n << " " << 12*n << endl;

    // List of vertices: X, Y and Z coordinates
    for(const auto& x : l)
    {
      assert_release(x.size() == 3);
      // 8 vertices
      of << x[0].lb() << " " << x[1].lb() << " " << x[2].lb() << endl;
      of << x[0].lb() << " " << x[1].lb() << " " << x[2].ub() << endl;
      of << x[0].lb() << " " << x[1].ub() << " " << x[2].lb() << endl;
      of << x[0].lb() << " " << x[1].ub() << " " << x[2].ub() << endl;
      of << x[0].ub() << " " << x[1].lb() << " " << x[2].lb() << endl;
      of << x[0].ub() << " " << x[1].lb() << " " << x[2].ub() << endl;
      of << x[0].ub() << " " << x[1].ub() << " " << x[2].lb() << endl;
      of << x[0].ub() << " " << x[1].ub() << " " << x[2].ub() << endl;
    }

    // List of faces: number of vertices, followed by the indexes of the composing vertices, in order (indexed from zero)
    for(size_t j = 0 ; j < l.size() ; j++)
    {
      size_t i = j*8;
      // 6 faces
      of << 4 << " " << (i+0) << " " << (i+2) << " " << (i+3) << " " << (i+1) << " #" << color << endl;
      of << 4 << " " << (i+2) << " " << (i+6) << " " << (i+7) << " " << (i+3) << " #" << color << endl;
      of << 4 << " " << (i+0) << " " << (i+4) << " " << (i+5) << " " << (i+1) << " #" << color << endl;
      of << 4 << " " << (i+4) << " " << (i+6) << " " << (i+7) << " " << (i+5) << " #" << color << endl;
      of << 4 << " " << (i+0) << " " << (i+4) << " " << (i+6) << " " << (i+2) << " #" << color << endl;
      of << 4 << " " << (i+1) << " " << (i+5) << " " << (i+7) << " " << (i+3) << " #" << color << endl;
    }

    of.close();
  }
}