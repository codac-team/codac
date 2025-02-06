#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou, Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *


class TestColor(unittest.TestCase):
    
    def test_Color(self):
        # Red

        d_rgb = [255, 0, 0]
        d_rgba = [255, 0, 0, 255]
        d_hsv = [0, 100, 100]
        d_hsva = [0, 100, 100, 100]


        colors = [
            Color(d_rgb, Model.RGB),
            Color(d_rgba, Model.RGB),
            Color(d_hsv, Model.HSV),
            Color(d_hsva, Model.HSV),
            Color("#FF0000")
        ]

        for c in colors:
            self.assertTrue(Approx(c.rgb().vec(), 1.0)==Color([255, 0, 0]).rgb().vec())
            self.assertTrue(Approx(c.rgb().vec(), 1.0)==Color([255, 0, 0, 255]).rgb().vec())
            self.assertTrue(Approx(c.hsv().vec(), 1.0)==Color([0, 100, 100],Model.HSV).hsv().vec())
            self.assertTrue(Approx(c.hsv().vec(), 1.0)==Color([0, 100, 100, 100],Model.HSV).hsv().vec())

        # Pink full opacity

        d_rgb = [229,128,255]
        d_rgba = [229,128,255,255]
        d_hsv = [288,50,100]
        d_hsva = [288,50,100,100]

        colors = [
            Color(d_rgb, Model.RGB),
            Color(d_rgba, Model.RGB),
            Color(d_hsv, Model.HSV),
            Color(d_hsva, Model.HSV),
            Color("#E580FF")
        ]

        for c in colors:
            self.assertTrue(Approx(c.rgb().vec(), 1.0)==Color([229,128,255]).rgb().vec())
            self.assertTrue(Approx(c.rgb().vec(), 1.0)==Color([229,128,255,255]).rgb().vec())
            self.assertTrue(Approx(c.hsv().vec(), 1.0)==Color([288,50,100],Model.HSV).hsv().vec())
            self.assertTrue(Approx(c.hsv().vec(), 1.0)==Color([288,50,100,100],Model.HSV).hsv().vec())

        # Pink 40% opacity

        a_rgb=102
        a_hsv=40
        d_rgba = [229,128,255,a_rgb]
        d_hsva = [288,50,100,a_hsv]


        colors = [
            Color(d_rgba, Model.RGB),
            Color(d_hsva, Model.HSV),
            Color("#E580FF66")
        ]
        for c in colors:
            self.assertTrue(Approx(c.rgb().vec(), 1.0)==Color([229,128,255,a_rgb]).rgb().vec())
            self.assertTrue(Approx(c.hsv().vec(), 1.0)==Color([288,50,100,a_hsv],Model.HSV).hsv().vec())


if __name__ ==  '__main__':
  unittest.main()