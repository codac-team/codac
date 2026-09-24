/** 
 *  \file codac2_Rerun_common.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <cctype>
#include "codac2_Color.h"
#include "codac2_StyleProperties.h"
#include "codac2_Vector.h"

#ifdef CODAC_WITH_RERUN
#include <rerun.hpp>

namespace codac2
{
  inline rerun::Color to_rerun_color(const Color& c)
  {
    const auto& rgb = c.rgb();
    uint8_t r = static_cast<uint8_t>(std::clamp(rgb[0], 0.0f, 255.0f));
    uint8_t g = static_cast<uint8_t>(std::clamp(rgb[1], 0.0f, 255.0f));
    uint8_t b = static_cast<uint8_t>(std::clamp(rgb[2], 0.0f, 255.0f));
    uint8_t a = static_cast<uint8_t>(std::clamp(rgb[3], 0.0f, 255.0f));
    return rerun::Color(r, g, b, a);
  }

  inline rerun::components::AlbedoFactor to_rerun_albedo(const Color& c)
  {
    const auto& rgb = c.rgb();
    uint8_t r = static_cast<uint8_t>(std::clamp(rgb[0], 0.0f, 255.0f));
    uint8_t g = static_cast<uint8_t>(std::clamp(rgb[1], 0.0f, 255.0f));
    uint8_t b = static_cast<uint8_t>(std::clamp(rgb[2], 0.0f, 255.0f));
    uint8_t a = static_cast<uint8_t>(std::clamp(rgb[3], 0.0f, 255.0f));
    return rerun::components::AlbedoFactor(rerun::encodings::Rgba32(r, g, b, a));
  }

  inline std::string sanitize_rerun_path(const std::string& name)
  {
    std::string res;
    res.reserve(name.size());
    for(char ch : name)
    {
      if(std::isspace(static_cast<unsigned char>(ch)) || ch == '/' || ch == '\\' || ch == ':' || ch == '*' || ch == '?' || ch == '"' || ch == '<' || ch == '>' || ch == '|' || ch == '`' || ch == '$' || ch == '@')
        res.push_back('_');
      else
        res.push_back(ch);
    }
    if(res.empty())
      res = "item";
    return res;
  }

  inline std::string make_entity_path(const std::string& fig_name, const std::string& layer,
                                      const std::string& type_name, size_t id)
  {
    std::string l = (layer.empty() || layer == "alpha") ? "default" : layer;
    return sanitize_rerun_path(fig_name) + "/" + sanitize_rerun_path(l) + "/" + sanitize_rerun_path(type_name) + "_" + std::to_string(id);
  }

  // 2D Rotation and translation helper
  inline rerun::Position2D transform_2d(double x_raw, double y_raw,
                                       double cx, double cy,
                                       double cos_th, double sin_th, double scale)
  {
    double x_scaled = x_raw * scale;
    double y_scaled = y_raw * scale;
    double rx = cx + x_scaled * cos_th - y_scaled * sin_th;
    double ry = cy + x_scaled * sin_th + y_scaled * cos_th;
    return rerun::Position2D(static_cast<float>(rx), static_cast<float>(ry));
  }
}
#endif
