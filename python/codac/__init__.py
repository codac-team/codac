from codac.core import *
from .version import __version__

# Predefined contractor objects

class ctc:

  delay = CtcDelay()
  deriv = CtcDeriv()
  eval = CtcEval()
  dist = CtcDist()
  polar = CtcPolar()