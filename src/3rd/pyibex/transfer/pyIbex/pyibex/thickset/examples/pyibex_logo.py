from pyibex import IntervalVector, Interval, pySIVIA

from pyibex_thickset import *

from pyibex_thickset.image import *
from scipy.ndimage import *
import numpy as np
from vibes import vibes
import sys
a = imread("out_norm.png",flatten=True).astype(np.uint64)
# a = imread("logo.png",flatten=True).astype(np.uint64).T
a[a == 0] = 255
a[a == 128] = 0
a[a == 255] = 1

a = a.cumsum(0).cumsum(1)
# load into grayscale image


test = GeoImage(a,0,0,1,1)

X0 = IntervalVector([1,1]) | IntervalVector([a.shape[0], a.shape[1]])

P = ThickPaving(X0.inflate(100), UNK)
P.Sivia(test, 20, opInter )


vibes.beginDrawing()
vibes.newFigure("Test")
vibes.setFigureProperties({'x':0,'y':0, 'width':500, 'height':500})
P.visit(ToVibes(200))
# pySIVIA(X0, test, 10)

vibes.endDrawing()
