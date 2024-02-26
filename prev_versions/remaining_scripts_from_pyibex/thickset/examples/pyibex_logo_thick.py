from pyibex import IntervalVector, Interval
from pyibex_thickset import *

from pyibex_thickset.image import *
from scipy.ndimage import *
import numpy as np
from vibes import vibes

# load into grayscale image
# a = imread("out_norm.png",flatten=True).astype(np.uint64)
a = imread("logo.png",flatten=True).astype(np.uint64).T

img_out = np.zeros(a.shape, dtype=np.uint64)
img_out[ a == 0 ] = 1
img_out[ a == 255 ] = 1
img_in = np.zeros(a.shape, dtype=np.uint64)
img_in[ a == 255 ] = 1

# compute Integral image
img_out = img_out.cumsum(0).cumsum(1)
img_in  = img_in.cumsum(0).cumsum(1)

test = ThickGeoImage(img_in, img_out,0,0,1,1)

X0 = IntervalVector([0,0]) | IntervalVector([a.shape[0], a.shape[1]])

P = ThickPaving(X0, test, 10, opInter)
Pin = ThickPaving(X0, test.img_in, 10, opInter)
Pout = ThickPaving(X0, test.img_out, 10, opInter)

vibes.beginDrawing()
vibes.newFigure("Test")
vibes.setFigureProperties({'x':0,'y':0, 'width':500, 'height':500})
P.visit(ToVibes("Test"))
#
vibes.newFigure("Test in")
vibes.setFigureProperties({'x':500,'y':0, 'width':500, 'height':500})
Pin.visit(ToVibes("Test in"))

vibes.newFigure("Test out")
vibes.setFigureProperties({'x':1000,'y':0, 'width':500, 'height':500})
Pout.visit(ToVibes("Test out"))


vibes.endDrawing()
