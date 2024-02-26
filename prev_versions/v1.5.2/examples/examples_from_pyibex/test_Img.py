# Codac - Examples
# Originated from the former pyIbex library (Benoît Desrochers)
# Author: Benoît Desrochers
# ----------------------------------------------------------------------------

import numpy as np
import scipy.ndimage as ndi

from codac import *
from codac.unsupported import *

import os

import matplotlib.pyplot as plt
# load image data
img_gray = ndi.imread('./test.png', 'L').T
img_out = np.zeros(img_gray.shape, dtype=np.uint64)
img_in = np.zeros(img_gray.shape, dtype=np.uint64)

img_out[img_gray >= 127 ] = 1
img_in[img_gray != 255] = 1

# compute summed tables
img_out = img_out.cumsum(0).cumsum(1)
img_in = img_in.cumsum(0).cumsum(1)

# create contractors with CtcRaster
# top left corner is at position (-5, 5)
# pixel size is 0.1 by -0.1
ctcOut = CtcRaster(img_out, -5, 5, 0.1, -0.1)
ctcIn = CtcRaster(img_in, -5, 5, 0.1, -0.1)
sep = SepCtcPair(ctcIn, ctcOut)


X0 = IntervalVector(2, [-15,15])

beginDrawing()
fig = VIBesFig('CtcImage')
fig.set_properties(x=100, y=100, width=600, height=600)
fig.axis_limits(X0)

#vibes.drawRaster(os.path.abspath('test.png'), -5,5, 0.1, -0.1)
SIVIA(X0, sep, 0.2)

endDrawing()