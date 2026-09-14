/** 
 *  codac2_threading.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_threading.h"

using namespace std;
using namespace codac2;


namespace codac2
{
  int max_threads()
  {
    return std::thread::hardware_concurrency(); 
  }
  
  void set_nb_threads(int n)
  {
    threading::threads_used = (n > 1 ? std::min(n,max_threads()) : 1);
  }

  int nb_threads()
  {
    return threading::threads_used;
  }
}