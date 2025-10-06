/** 
 *  \file codac2_drawwhilepaving.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ctc.h"
#include "codac2_Sep.h"
#include "codac2_Figure2D.h"

namespace codac2
{
  template<typename...>
  inline constexpr bool always_false = false;

  /**
   * \brief Deprecated!
   */
  template<typename T = void>
  inline void draw_while_paving(const IntervalVector& x0, std::shared_ptr<const CtcBase<IntervalVector>> c, double eps, std::shared_ptr<Figure2D> fig = nullptr)
  {
    static_assert(always_false<T>, "\n\n\
      draw_while_paving(..) is deprecated,\n \
      please replace by DefaultFigure::pave(..) (or any Figure2D object).\n");
  }

  /**
   * \brief Deprecated!
   */
  template<typename T = void>
  inline void draw_while_paving(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, std::shared_ptr<Figure2D> fig = nullptr)
  {
    static_assert(always_false<T>, "\n\n\
      draw_while_paving(..) is deprecated,\n \
      please replace by DefaultFigure::pave(..) (or any Figure2D object).\n");
  }

  /**
   * \brief Deprecated!
   */
  template<typename T = void>
  inline void draw_while_paving(const IntervalVector& x0, std::shared_ptr<const SepBase> s, double eps, std::shared_ptr<Figure2D> fig = nullptr)
  {
    static_assert(always_false<T>, "\n\n\
      draw_while_paving(..) is deprecated,\n \
      please replace by DefaultFigure::pave(..) (or any Figure2D object).\n");
  }

  /**
   * \brief Deprecated!
   */
  template<typename T = void>
  inline void draw_while_paving(const IntervalVector& x0, const SepBase& s, double eps, std::shared_ptr<Figure2D> fig = nullptr)
  {

    static_assert(always_false<T>, "\n\n\
      draw_while_paving(..) is deprecated,\n \
      please replace by DefaultFigure::pave(..) (or any Figure2D object).\n");
  }
}