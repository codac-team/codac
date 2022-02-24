from pyibex.image import *
from pyibex import Interval, IntervalVector, SepCtcPair, pySIVIA
from vibes import vibes
import os
import scipy.misc
import scipy.ndimage as ndimage
import numpy as np

ain = np.zeros((200, 200), dtype=np.int64)
ain[75:125, 50:125] = 1
ain[25:100, 25:50] = 1

# over approximation of the set X
aout = np.zeros((200, 200), dtype=np.int64)
aout[74:126, 49:126] = 1
aout[24:101, 24:51] = 1

img_out = aout.cumsum(0).cumsum(1)
img_in = (1 - ain).cumsum(0).cumsum(1)


img = 127*(aout+ain).T
scipy.misc.imsave('test.png', img.astype(np.uint8))



ctcOut = CtcRaster(img_out, -5, 5, 0.1, -0.1)
ctcIn = CtcRaster(img_in, -5, 5, 0.1, -0.1)
sep = SepCtcPair(ctcIn, ctcOut)


vibes.beginDrawing()
vibes.newFigure('CtcImage')
X0 = IntervalVector(2, [-15,15])
# X0 = IntervalVector([[4, 10], [-6, -4]])
# X0 = IntervalVector([[-2.47, -2.17], [-0.5, 0.17]])
# X0 = IntervalVector([[-2.47, -2.17], [-0.1, 0.17]])
# X0_res = IntervalVector([[-2.47, -2.17], [-0.1, 0])
# X0 = IntervalVector([ [2.05, 2.15],  [-0.08, -0.02] ])
# X0 = IntervalVector([ [2.05, 2.15],  [-0.08, -0.0] ])
#
vibes.drawRaster(os.path.abspath('test.png'), -5,5, 0.1, -0.1)
# vibes.drawBox(X0[0][0], X0[0][1], X0[1][0], X0[1][1], '[#FF000055]')
# self.ctcOut.contract(X0)
# vibes.drawBox(X0[0][0], X0[0][1], X0[1][0], X0[1][1], '[#0000FF55]')

pySIVIA(X0, sep, 0.2, color_out='[#00FF00AA]', color_maybe="[#00FFFF55]", color_in="[#FF000055]")
# pySIVIA(X0, sep, 0.05, use_patch=True)


vibes.axisEqual()
