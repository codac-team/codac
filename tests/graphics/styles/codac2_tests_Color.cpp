/**
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Color.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("Color")
{
    {
        // Red

        std::array<float, 3> d_rgb{255., 0., 0.};
        std::array<float, 4> d_rgba{255., 0., 0., 255.};
        std::array<float, 3> d_hsv{0., 100., 100.};
        std::array<float, 4> d_hsva{0., 100., 100., 100.};

        vector<Color> v{
            Color(d_rgb, Model::RGB),
            Color(d_rgba, Model::RGB),
            Color(d_hsv, Model::HSV),
            Color(d_hsva, Model::HSV),
            Color("#FF0000")};

        for (const auto &c : v)
        {
            CHECK(Approx(c.rgb().vec(),1.) == Color({255., 0., 0.}).vec());
            CHECK(Approx(c.rgb().vec(),1.) == Color({255., 0., 0., 255.}).vec());
            CHECK(Approx(c.hsv().vec(),1.) == Color({0., 100., 100.}, Model::HSV).vec());
            CHECK(Approx(c.hsv().vec(),1.) == Color({0., 100., 100., 100.}, Model::HSV).vec());
        }
    }

    {
        // Pink full opacity

        float a = 255.;
        std::array<float, 3> d_rgb{229., 128., 255.};
        std::array<float, 4> d_rgba{229., 128., 255., a};
        std::array<float, 3> d_hsv{288., 50., 100.};
        std::array<float, 4> d_hsva{288., 50., 100., 100.};

        vector<Color> v{
            Color(d_rgb, Model::RGB),
            Color(d_rgba, Model::RGB),
            Color(d_hsv, Model::HSV),
            Color(d_hsva, Model::HSV),
            Color("#e580ff")};

        for (const auto &c : v)
        {
            CHECK(Approx(c.rgb().vec(),1.) == Color({229., 128., 255.}).vec());
            CHECK(Approx(c.rgb().vec(),1.) == Color({229., 128., 255., a}).vec());
            CHECK(Approx(c.hsv().vec(),1.) == Color({288., 50., 100.}, Model::HSV).vec());
            CHECK(Approx(c.hsv().vec(),1.) == Color({288., 50., 100., 100.}, Model::HSV).vec());
        }
    }

      {
        // Pink 40% opacity

        float a = 0.4*255.;
        std::array<float,4> d_rgba { 229.,128.,255.,a };
        std::array<float,4> d_hsva { 288.,50.,100.,40. };
        
        vector<Color> v {
          Color(d_rgba, Model::RGB),
          Color(d_hsva, Model::HSV),
          Color("#e580ff66")
        };

        for(const auto& c : v)
        {
          CHECK(Approx(c.rgb().vec(),1.) == Color({229.,128.,255.,a}).vec());
          CHECK(Approx(c.hsv().vec(),1.) == Color({288.,50.,100.,40.},Model::HSV).vec());
        }
      }
}