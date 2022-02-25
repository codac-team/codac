# Verifying that the former module pyibex 
# has not been loaded, which could cause conflicts
import sys
if 'pyibex' in sys.modules:
  print('\nWarning: pyIbex has been merged into Codac, please do not use it anymore.')
  print('Conflicts may occur between the two libraries.\n')

from codac.core import *
from .version import __version__

# Predefined contractor objects

class ctc:

  delay = CtcDelay()
  deriv = CtcDeriv()
  eval = CtcEval()
  dist = CtcDist()
  polar = CtcPolar()