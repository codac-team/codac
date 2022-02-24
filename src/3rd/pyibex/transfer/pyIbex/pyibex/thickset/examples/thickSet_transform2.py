from pyibex.thickset import *
from pyibex_thickset.image import *
from pyibex import Interval, IntervalVector
from vibes import vibes

import numpy as np

import time


class Timer(object):
    """ A quick tic-toc timer

    Credit: http://stackoverflow.com/questions/5849800/tic-toc-functions-analog-in-python
    """

    def __init__(self, name=None, verbose=True):
        self.name = name
        self.verbose = verbose
        self.elapsed = None

    def __enter__(self):
        self.tstart = time.time()
        return self

    def __exit__(self, type, value, traceback):
        self.elapsed = time.time() - self.tstart
        if self.verbose:
            if self.name:
                print('[%s]' % self.name, end=" " )
            print('Elapsed: %s' % (self.elapsed) )


X0 = 3*IntervalVector(2, [-10,10])
# X0 = IntervalVector([[3.5, 3.6], [1.9, 2]])
# X0 = IntervalVector([[-5.8  , -5.6], [3.3, 3.5]])
# X0 = IntervalVector([[-5.4, -4.8], [3, 3.4]])
# X0 = IntervalVector([[-6, -4], [4, 6]])
# X0 = IntervalVector([[-7, -6], [2, 3]])
# X0 = IntervalVector([[-2, -1.6], [1.2, 1.4]])
# X0 = IntervalVector([ [-1.2, -0.7], [0.1, 0.6]])
# X0 = IntervalVector([ [-2.5, -1.25], [0, 1.25]])
# test = ThickDisk(Interval(0), Interval(0), Interval(0,4), Interval(0,4))
# X0 = IntervalVector(2, [-5,5])
a = np.zeros((500,500))
a[150:350, 150:350] = 1
a[100:400, :] = 1
a = a.cumsum(0).cumsum(1)

b = np.zeros((500,500))
b[100:400, 100:400] = 1
b[100:400, :] = 1
b = b.cumsum(0).cumsum(1)
# im = GeoImage(a, -5,5,0.02, -0.02)
im = ThickGeoImage(a, b, -5,5,0.02, -0.02, OUT)
TEST2 = lambda x : im.intersects(ThickBox(x+[1,1], x+[2 ,1]) )
# TEST2 = lambda x : im.intersects(ThickBox(x+[0,0], x+[0 ,0]) )

TEST3 = lambda x : im.img_in.test_bb(ThickBox(x+[1,1], x+[1 ,2]) )
# P = ThickPaving(X0, im, 0.01, opInter)

with Timer("TEST2"):
  P = ThickPaving(X0, TEST2, 0.5, opInter)
  # P = ThickPaving(X0, im, 0.1, opInter)
print("avant :", P.size, end='')

# with Timer("TEST3"):
  # P = ThickPaving(X0, GeoImageTranslate(im, IntervalVector([[1,4], [1,1]])), 0.5, opInter)
# P.Reunite()
print("-->  :", P.size, end='')

# P2 = ThickPaving(X0, TEST2, 0.1, opInter)
# P.Reunite()
# test2 = ThickDisk(Interval(1), Interval(1), Interval(0,2), Interval(0,10))
# P.Sivia(test2, 0.1, opAnd)
# T  = ThickTranslateInPaving(P, IntervalVector(2, [1,4]))
# P2 = ThickPaving(X0, T, 0.1, opInter)

vibes.beginDrawing()
#
P.visit(ToVibes("test", clearFigure=True))
vibes.drawBox(-2, 2, -2, 2 , 'g')
vibes.drawBox(-3, 3, -3, 3 , 'g')
vibes.drawBox(-5, 5, -5, 5 , 'r')

# P2.visit(ToVibes(10000, "test2"))
# print(P.size, P2.size)
vibes.endDrawing()
