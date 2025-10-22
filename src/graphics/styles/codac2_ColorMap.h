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
      static ColorMap haxby()
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({39.,90.,211.});
        cmap[1]=Color({40.,123.,245.});
        cmap[2]=Color({45.,155.,253.});
        cmap[3]=Color({73.,209.,255.});
        cmap[4]=Color({100.,230.,254.});
        cmap[5]=Color({118.,235.,226.});
        cmap[6]=Color({135.,236.,187.});
        cmap[7]=Color({194.,252.,165.});
        cmap[8]=Color({217.,251.,151.});
        cmap[9]=Color({233.,241.,131.});
        cmap[10]=Color({252.,201.,96.});
        cmap[11]=Color({255.,184.,84.});
        cmap[12]=Color({255.,170.,75.});
        cmap[13]=Color({255.,167.,83.});
        cmap[14]=Color({255.,200.,158.});
        cmap[15]=Color({255.,233.,217.});
        return cmap;
      }

      /**
       * \brief Basic color map
       * 
       * Default color map
       */
      static ColorMap basic() // Can't use default as name
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({10.,0.,121.});
        cmap[1]=Color({40.,0.,150.});
        cmap[2]=Color({20.,5.,175.});
        cmap[3]=Color({0.,10.,200.});
        cmap[4]=Color({0.,25.,212.});
        cmap[5]=Color({0.,40.,224.});
        cmap[6]=Color({26.,102.,240.});
        cmap[7]=Color({13.,129.,248.});
        cmap[8]=Color({25.,175.,255.});
        cmap[9]=Color({50.,190.,255.});
        cmap[10]=Color({68.,202.,255.});
        cmap[11]=Color({97.,225.,240.});
        cmap[12]=Color({106.,235.,225.});
        cmap[13]=Color({124.,235.,200.});
        cmap[14]=Color({138.,236.,174.});
        cmap[15]=Color({172.,245.,168.});
        cmap[16]=Color({205.,255.,162.});
        cmap[17]=Color({223.,245.,141.});
        cmap[18]=Color({240.,236.,121.});
        cmap[19]=Color({247.,215.,104.});
        cmap[20]=Color({255.,189.,87.});
        cmap[21]=Color({255.,160.,69.});
        cmap[22]=Color({244.,117.,75.});
        cmap[23]=Color({238.,80.,78.});
        cmap[24]=Color({255.,90.,90.});
        cmap[25]=Color({255.,124.,124.});
        cmap[26]=Color({255.,158.,158.});
        cmap[27]=Color({245.,179.,174.});
        cmap[28]=Color({255.,196.,196.});
        cmap[29]=Color({255.,215.,215.});
        cmap[30]=Color({255.,235.,235.});
        cmap[31]=Color({255.,254.,253.});
        return cmap;
      }

      /**
       * \brief Blue tube color map
       * 
       * Default color map for tubes
       */
      static ColorMap blue_tube()
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({76.,110.,127.});
        cmap[1]=Color({136.,197.,228.});
        return cmap;
      }

      /**
       * \brief Red tube color map
       * 
       * Color map for tubes
       */
      static ColorMap red_tube()
      {
        ColorMap cmap( Model::RGB );
        cmap[0]=Color({169.,55.,0.});
        cmap[1]=Color({241.,140.,54.});
        return cmap;
      }

      /**
       * \brief Rainbow color map
       * 
       * Rainbow color map
       */
      static ColorMap rainbow()
      {
        ColorMap cmap( Model::HSV );
        int i = 0;
        for(int h = 300 ; h > 0 ; h-=10)
        {
          cmap[i]=Color({(float)h,100.,100.},Model::HSV);
          i++;
        }
        return cmap;
      }
  };
}