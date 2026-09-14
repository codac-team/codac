/** 
 *  \file codac2_threading.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <thread>



namespace codac2
{
  int max_threads();
  void set_nb_threads(int n);
  int nb_threads();
  
  namespace threading
  {
    inline int threads_used = 1;
  }
}