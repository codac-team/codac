/** 
 *  \file codac2_ColorMap.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <cmath>
#include "codac2_Color.h"
#include "codac2_Interval.h"
#include"codac2_assert.h"

namespace codac2
{
  /**
   * \struct ColorMap
   * \brief Represents a set of RGB or HSV values
   * 
   * This class is used to represent a set of RGB or HSV values to create a color map. It inherits from std::map<float,Color>
   * 
   * Once a ColorMap is created, Colors can be added to it as for a std::map.
   */
  struct ColorMap : public std::map<float,Color>
  {
    protected:
      Model m; //RGB or HSV

    public:
      
      /**
       * \brief Constructor from a model
       * 
       * \param m_ Color model (RGB or HSV). It defines the interpolation space
       */
      explicit ColorMap(Model m_ = Model::RGB);

      /**
       * \brief Getter for the color model
       * 
       * \return The color model (RGB or HSV)
       */
      const Model& model() const { return m; }

      /**
       * \brief Getter for the color at a given value
       * 
       * \param r Value between 0 and 1
       */
      Color color (float r) const;

      /**
       * \brief Haxby color map
       * 
       * Haxby color map
       */
      static ColorMap haxby(float alpha=1.)
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({39.,90.,211.,255.f*alpha});
        cmap[1]=Color({40.,123.,245.,255.f*alpha});
        cmap[2]=Color({45.,155.,253.,255.f*alpha});
        cmap[3]=Color({73.,209.,255.,255.f*alpha});
        cmap[4]=Color({100.,230.,254.,255.f*alpha});
        cmap[5]=Color({118.,235.,226.,255.f*alpha});
        cmap[6]=Color({135.,236.,187.,255.f*alpha});
        cmap[7]=Color({194.,252.,165.,255.f*alpha});
        cmap[8]=Color({217.,251.,151.,255.f*alpha});
        cmap[9]=Color({233.,241.,131.,255.f*alpha});
        cmap[10]=Color({252.,201.,96.,255.f*alpha});
        cmap[11]=Color({255.,184.,84.,255.f*alpha});
        cmap[12]=Color({255.,170.,75.,255.f*alpha});
        cmap[13]=Color({255.,167.,83.,255.f*alpha});
        cmap[14]=Color({255.,200.,158.,255.f*alpha});
        cmap[15]=Color({255.,233.,217.,255.f*alpha});
        return cmap;
      }

      /**
       * \brief Basic color map
       * 
       * Default color map
       */
      static ColorMap basic(float alpha=1.) // Can't use default as name
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({10.,0.,121.,255.f*alpha});
        cmap[1]=Color({40.,0.,150.,255.f*alpha});
        cmap[2]=Color({20.,5.,175.,255.f*alpha});
        cmap[3]=Color({0.,10.,200.,255.f*alpha});
        cmap[4]=Color({0.,25.,212.,255.f*alpha});
        cmap[5]=Color({0.,40.,224.,255.f*alpha});
        cmap[6]=Color({26.,102.,240.,255.f*alpha});
        cmap[7]=Color({13.,129.,248.,255.f*alpha});
        cmap[8]=Color({25.,175.,255.,255.f*alpha});
        cmap[9]=Color({50.,190.,255.,255.f*alpha});
        cmap[10]=Color({68.,202.,255.,255.f*alpha});
        cmap[11]=Color({97.,225.,240.,255.f*alpha});
        cmap[12]=Color({106.,235.,225.,255.f*alpha});
        cmap[13]=Color({124.,235.,200.,255.f*alpha});
        cmap[14]=Color({138.,236.,174.,255.f*alpha});
        cmap[15]=Color({172.,245.,168.,255.f*alpha});
        cmap[16]=Color({205.,255.,162.,255.f*alpha});
        cmap[17]=Color({223.,245.,141.,255.f*alpha});
        cmap[18]=Color({240.,236.,121.,255.f*alpha});
        cmap[19]=Color({247.,215.,104.,255.f*alpha});
        cmap[20]=Color({255.,189.,87.,255.f*alpha});
        cmap[21]=Color({255.,160.,69.,255.f*alpha});
        cmap[22]=Color({244.,117.,75.,255.f*alpha});
        cmap[23]=Color({238.,80.,78.,255.f*alpha});
        cmap[24]=Color({255.,90.,90.,255.f*alpha});
        cmap[25]=Color({255.,124.,124.,255.f*alpha});
        cmap[26]=Color({255.,158.,158.,255.f*alpha});
        cmap[27]=Color({245.,179.,174.,255.f*alpha});
        cmap[28]=Color({255.,196.,196.,255.f*alpha});
        cmap[29]=Color({255.,215.,215.,255.f*alpha});
        cmap[30]=Color({255.,235.,235.,255.f*alpha});
        cmap[31]=Color({255.,254.,253.,255.f*alpha});
        return cmap;
      }

      /**
       * \brief Blue tube color map
       * 
       * Default color map for tubes
       */
      static ColorMap blue_tube(float alpha=1.)
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({76.,110.,127.,255.f*alpha});
        cmap[1]=Color({136.,197.,228.,255.f*alpha});
        return cmap;
      }

      /**
       * \brief Red tube color map
       * 
       * Color map for tubes
       */
      static ColorMap red_tube(float alpha=1.)
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({169.,55.,0.,255.f*alpha});
        cmap[1]=Color({241.,140.,54.,255.f*alpha});
        return cmap;
      }

      /**
       * \brief Rainbow color map
       * 
       * Rainbow color map
       */
      static ColorMap rainbow(float alpha=1.)
      {
        ColorMap cmap( Model::HSV );
        int i = 0;
        for(int h = 300 ; h > 0 ; h-=10)
        {
          cmap[i]=Color({(float)h,100.,100.,100.f*alpha},Model::HSV);
          i++;
        }
        return cmap;
      }
  };
}