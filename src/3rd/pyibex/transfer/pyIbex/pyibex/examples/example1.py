#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : example1.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : May 28, 2015
#============================================================================

#example 1, simple sivia

from pyibex import *
from vibes import *

f = Function('x', 'y', 'x*cos(x-y)+y')

sep = SepFwdBwd(f, CmpOp.LEQ)

box = IntervalVector(2, [-10, 10])

vibes.beginDrawing()

pySIVIA(box, sep, 0.3)

vibes.endDrawing()
