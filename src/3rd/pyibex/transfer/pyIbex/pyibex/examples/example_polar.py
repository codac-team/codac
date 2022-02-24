from pyibex import *
from pyibex.geometry import SepPolarXY
import numpy as np
from vibes import vibes
sep1 = SepPolarXY(Interval(4,5), Interval(np.deg2rad(-570), np.deg2rad(-450)))

f1 = Function("x", 'y', '(x + 2; y - 2)')
f2 = Function("x", 'y', '(x - 2; y + 2)')
sep = SepTransform(sep1, f1, f2)

box = IntervalVector([[-10,10],[-10,10]])
vibes.beginDrawing()
pySIVIA(box, sep1, 0.1)
