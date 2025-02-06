/** 
 *  \file codac2_Color.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <iomanip>
#include <math.h> 

#include"codac2_assert.h"
#include"codac2_Vector.h"

namespace codac2
{

  enum Model { RGB, HSV };

  struct Color : public std::array<float,4>
  {
    protected:
      Model m;

    public:

      // Constructors

      explicit Color();
      explicit Color(const std::array<float,3>& xyz, Model m_ = Model::RGB);
      explicit Color(const std::array<float,4>& xyza, Model m_ = Model::RGB);
      explicit Color(const std::initializer_list<float> xyza, Model m_ = Model::RGB);
      explicit Color(const std::string& hex_str);

      const Model& model() const { return m; }


      // other formats

      std::string hex_str() const;

      codac2::Vector vec() const;

      // Conversions

      Color rgb() const;
      Color hsv() const;

      // Overload flux operator

      friend std::ostream& operator<<(std::ostream& os, const Color& c)
      {
        if (c.m == Model::RGB)
          os << "RGB Color (" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << ")";
        else if (c.m == Model::HSV)
          os << "HSV Color (" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << ")";
        return os;
      }

      // Predefined colors

      static Color none()                      { return Color({255., 255., 255., 0.}); };
      static Color black(float alpha = 1.)      { return Color({0.,   0.,   0.,   (float) (alpha*255.)}); };
      static Color white(float alpha = 1.)      { return Color({255., 255., 255., (float) (alpha*255.)}); };
      static Color green(float alpha = 1.)      { return Color({144., 242., 0.,   (float) (alpha*255.)}); };
      static Color blue(float alpha = 1.)       { return Color({0.,   98.,  198., (float) (alpha*255.)}); };
      static Color cyan(float alpha = 1.)       { return Color({75.,  207., 250., (float) (alpha*255.)}); };
      static Color yellow(float alpha = 1.)     { return Color({255., 211., 42.,  (float) (alpha*255.)}); };
      static Color red(float alpha = 1.)        { return Color({209., 59.,  0.,   (float) (alpha*255.)}); };
      static Color dark_gray(float alpha = 1.)  { return Color({112., 112., 112., (float) (alpha*255.)}); };
      static Color purple(float alpha = 1.)     { return Color({154., 0.,   170., (float) (alpha*255.)}); };
      static Color dark_green(float alpha = 1.) { return Color({94.,  158., 0.,   (float) (alpha*255.)}); };
  };

  template <std::size_t N>
  static std::array<float, N> to_array(const std::initializer_list<float>& list) {
      assert(list.size() == N);
      std::array<float, N> arr;
      std::copy(list.begin(), list.end(), arr.begin());
      return arr;
  }
}