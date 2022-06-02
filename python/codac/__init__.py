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


# Deprecated functions from the former pyIbex library:

def pySIVIA(X0, ops, epsilon, figure_name='', draw_boxes=True, save_result=True, color_maybe='', color_out='', color_in=''):
  print('\nWarning: pySIVIA(..) is deprecated and has been replaced by SIVIA(..).')
  print('More information can be found with help(SIVIA).\n')
  print('Example of use:\n')
  print('  SIVIA(x0, sep, 0.05, display_result=True, fig_name="SIVIA", return_result=True, color_map={SetValue.IN:"k[r]", SetValue.OUT:"k[b]", SetValue.UNKNOWN:"k[y]"})\n')

  cmap = {}
  if color_in:
    cmap[SetValue.IN] = color_in
  if color_out:
    cmap[SetValue.OUT] = color_out
  if color_maybe:
    cmap[SetValue.UNKNOWN] = color_maybe

  return SIVIA(X0, ops, epsilon, display_result=draw_boxes, fig_name=figure_name, return_result=save_result, color_map=cmap)