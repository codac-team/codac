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
      static Color none() { return Color({255,255,255, 0.}); };

      #define DEFINE_COLOR(NAME, R, G, B) \
        static Color NAME(float alpha = 1.) { \
            return Color({(float)(R), (float)(G), (float)(B), (float)(alpha * 255.)}); \
        }

      // Predefined colors
      
      DEFINE_COLOR(black,         0,   0,   0)
      DEFINE_COLOR(white,         255, 255, 255)

      DEFINE_COLOR(light_gray,    217, 217, 217)
      DEFINE_COLOR(gray,          180, 180, 180)
      DEFINE_COLOR(dark_gray,     112, 112, 112)

      DEFINE_COLOR(light_green,   184, 233, 118)
      DEFINE_COLOR(green,         144, 242, 0)
      DEFINE_COLOR(dark_green,    94,  158, 0)

      DEFINE_COLOR(light_blue,    75,  207, 250)
      DEFINE_COLOR(blue,          45,  152, 218)
      DEFINE_COLOR(dark_blue,     34,  112, 147)

      DEFINE_COLOR(light_cyan,    129, 236, 236)
      DEFINE_COLOR(cyan,          109, 200, 200)
      DEFINE_COLOR(dark_cyan,     82,  151, 151)

      DEFINE_COLOR(light_yellow,  255, 250, 101)
      DEFINE_COLOR(yellow,        255, 211, 42)
      DEFINE_COLOR(dark_yellow,   225, 177, 44)

      DEFINE_COLOR(light_orange,  253, 150, 68)
      DEFINE_COLOR(orange,        255, 159, 26)
      DEFINE_COLOR(dark_orange,   214, 134, 22)

      DEFINE_COLOR(light_red,     231, 127, 103)
      DEFINE_COLOR(red,           209, 59,  0)
      DEFINE_COLOR(dark_red,      179, 57,  57) 

      DEFINE_COLOR(light_brown,   208, 151, 71)
      DEFINE_COLOR(brown,         151, 109, 52)
      DEFINE_COLOR(dark_brown,    99,  72,  34)

      DEFINE_COLOR(light_purple,  205, 132, 241)
      DEFINE_COLOR(purple,        154, 0,   170)
      DEFINE_COLOR(dark_purple,   108, 0,   119)

      DEFINE_COLOR(light_pink,    253, 167, 223)
      DEFINE_COLOR(pink,          243, 104, 224)
      DEFINE_COLOR(dark_pink,     185, 79,  171)
  };
}