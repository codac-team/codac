import pyibex
from pyibex import Ctc, Interval, IntervalVector, Function, BoolInterval
from pyibex.geometry import CtcPolar
from codac.tube import *
from .version import __version__

# Predefined contractor objects

class ctc:

  delay = CtcDelay()
  deriv = CtcDeriv()
  eval = CtcEval()
  dist = CtcDist()
  polar = CtcPolar()

oo = pyibex.oo