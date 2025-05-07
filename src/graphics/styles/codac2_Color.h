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

  /**
   * \enum Model
   * \brief Color model (RGB or HSV)
   * 
   * This enum is used to specify the color model of a color, either RGB or HSV.
   */
  enum Model { RGB, HSV };

  /**
   * \struct Color
   * \brief Color structure, in RGBA or HSVA format
   * 
   * This class is used to represent a Color, either in RGBA or HSVA format. It inherits from std::array<float,4> 
   * and provides methods to convert between different color models.
   * 
   * For RGBA, the values are in the range [0,255].
   * 
   * For HSVA, the values are in the range [0,360] for hue and [0,100] for saturation, value and alpha.
   * 
   * Predefined Colors are also available.
   */
  struct Color : public std::array<float,4>
  {
    protected:
      Model m;

    public:

      // Constructors

      /**
       * \brief Default constructor
       * 
       * All values are set to 0
       */
      explicit Color();

      /**
       * \brief Constructor from an array of 3 floats
       * 
       * \param xyz Array of 3 floats (RGB or HSV values)
       * \param m_ Color model (RGB or HSV)
       */
      explicit Color(const std::array<float,3>& xyz, Model m_ = Model::RGB);

      /**
       * \brief Constructor from an array of 4 floats
       * 
       * \param xyza Array of 4 floats (RGBA or HSVA values)
       * \param m_ Color model (RGB or HSV)
       */
      explicit Color(const std::array<float,4>& xyza, Model m_ = Model::RGB);

      /**
       * \brief Constructor from an initializer list of floats
       * 
       * \param xyza Initializer list of floats (RGB, RGBA, HSV or HSVA values)
       * \param m_ Color model (RGB or HSV)
       */
      explicit Color(const std::initializer_list<float> xyza, Model m_ = Model::RGB);

      /**
       * \brief Constructor from a hex string
       * 
       * \param hex_str Hex string in html format (e.g. \#ff0000 or \#ff0000ff)
       */
      explicit Color(const std::string& hex_str);

      /**
       * \brief Getter for the color model
       * 
       * \return The color model (RGB or HSV)
       */
      const Model& model() const { return m; }


      // other formats

      /**
       * \brief Converts the color to a hex string in html format
       * 
       * \return The color as a hex string (e.g. \#ff0000 or \#ff0000ff)
       */
      std::string hex_str() const;


      /**
       * \brief Converts the color to a codac2::Vector
       * 
       * \return The color as a codac2::Vector
       */
      codac2::Vector vec() const;

      // Conversions

      /**
       * \brief Converts the color to RGB format
       * 
       * \return The Color in RGB format
       */
      Color rgb() const;

      /**
       * \brief Converts the color to HSV format
       * 
       * \return The Color in HSV format
       */
      Color hsv() const;

      // Overload flux operator

      /**
       * \brief Overload of the << operator for the Color class
       * 
       * \param os Output stream
       * \param c Color to output
       * 
       * \return The output stream
       */
      friend std::ostream& operator<<(std::ostream& os, const Color& c)
      {
        if (c.m == Model::RGB)
          os << "RGB Color (" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << ")";
        else if (c.m == Model::HSV)
          os << "HSV Color (" << c[0] << "," << c[1] << "," << c[2] << "," << c[3] << ")";
        return os;
      }

      // Predefined colors

      /**
       * \brief Empty color (transparent white)
       */
      static Color none()                      { return Color({255., 255., 255., 0.}); };

      /**
       * \brief Black color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color black(float alpha = 1.)      { return Color({0.,   0.,   0.,   (float) (alpha*255.)}); };

      /**
       * \brief White color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color white(float alpha = 1.)      { return Color({255., 255., 255., (float) (alpha*255.)}); };

      /**
       * \brief Green color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color green(float alpha = 1.)      { return Color({144., 242., 0.,   (float) (alpha*255.)}); };

      /**
       * \brief Blue color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color blue(float alpha = 1.)       { return Color({0.,   98.,  198., (float) (alpha*255.)}); };

      /**
       * \brief Cyan color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color cyan(float alpha = 1.)       { return Color({75.,  207., 250., (float) (alpha*255.)}); };

      /**
       * \brief Yellow color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color yellow(float alpha = 1.)     { return Color({255., 211., 42.,  (float) (alpha*255.)}); };

      /**
       * \brief Red color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color red(float alpha = 1.)        { return Color({209., 59.,  0.,   (float) (alpha*255.)}); };

      /**
       * \brief Dark gray color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color dark_gray(float alpha = 1.)  { return Color({112., 112., 112., (float) (alpha*255.)}); };

      /**
       * \brief Light gray color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
      static Color purple(float alpha = 1.)     { return Color({154., 0.,   170., (float) (alpha*255.)}); };

      /**
       * \brief Dark green color
       * 
       * \param alpha Alpha value between 0. (transparent) and 1. (opaque)
       */
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