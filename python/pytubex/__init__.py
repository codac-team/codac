from pyibex import Interval, IntervalVector
from pyibex.geometry import CtcPolar
from pytubex.tube import *

# Predefined contractor objects

class ctc:

  deriv = CtcDeriv()
  eval = CtcEval()
  dist = CtcDist()
  polar = CtcPolar()