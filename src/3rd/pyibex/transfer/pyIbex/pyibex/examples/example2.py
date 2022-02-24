#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : example2.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : May 28, 2015
#============================================================================

#example 2, Custom contractor

"""
Write a custom contractor for the equation
(x - x0)^2 + (y - y0)^2 \in R^2

This example uses:
    - Interval and IntervalVector
    - Ctc
    - pySIVIA
"""
from pyibex import *
from vibes import *

class myCtc(Ctc):
	def __init__(self, x0, y0, R):
		Ctc.__init__(self, 2)
		self.x0 = Interval(x0)
		self.y0 = Interval(y0)
		self.R = R

	def contract(self, X):
		a1 = X[0] - Interval(self.x0)
		a2 = X[1] - Interval(self.y0)

		a3 = sqr(a1)
		a4 = sqr(a2)

		a5 = a3 + a4
		a5 &= sqr(self.R)

		bwd_add(a5, a3, a4)
		bwd_sqr(a4, a2)
		bwd_sqr(a3, a1)

		bwd_sub(a1, X[0], (self.x0))
		bwd_sub(a2, X[1], (self.y0))


# instanciate out new contractor
ctc = myCtc(1, 2, Interval(1, 2))
# set the inital box
box = IntervalVector(2, [-10, 10])

# init vibes display and resize the new figure
vibes.beginDrawing()
vibes.newFigure("Example 2")
vibes.setFigureSize(500,500)

# run the paver
pySIVIA(box, ctc, 0.3)

vibes.endDrawing()
