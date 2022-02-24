import pyibex
from pyibex import IntervalVector, Interval, LargestFirst
from vibes import vibes
# from pyibex.thickset import *
from collections import deque
from operator import itemgetter
import math
# import os
import struct
from functools import reduce
# from parameters import siviaParams, drawAxis

siviaParams = {'use_patch': True,
               'color_out': 'b[#00AAFFAA]',
               'color_in': '#FF0000[#BB00FFAA]',
               'color_maybe': '[y]'}

# siviaParams = {'use_patch': True,
#                'color_out': 'b[#00AAFF55]',
#                'color_in': '#FF0000[#BB00FF55]',
#                'color_maybe': '[#33003355]'}


class pySetNode:
    """Set Node."""
    YES=0
    NO=1
    MAYBE=2
    def __init__(self, Xin, Xout=None):
        """Constructor."""
        self.xin = IntervalVector(Xin)
        self.xout = IntervalVector(Xin) if (Xout is None) else Xout
        self.left = None
        self.right = None
        self.X0 = None
        self.X = None

    def isLeaf(self):
        return (self.left is None and self.right is None)

    def bisect(self, bisector):
        if self.isLeaf():
            X = self.xin & self.xout
            X1, X2 = bisector.bisect(X)
            self.left = pySetNode(X1)
            self.right = pySetNode(X2)
            return True
        return False

    def bisect_around(self, bisector, XX):
        X = self.xin & self.xout
        LX = X.diff(XX)
        if(len(LX) == 0):
            print("[len(LX) == 0]", X, XX)
            exit()
        if len(LX) == 1:
            X1, X2 = XX, LX[0]
        else:
            X1, X2 = bisector.bisect(X)
            if not (XX.is_subset(X1) or XX.is_subset(X2)):
                VX1 = reduce(lambda x, y: x*y, (XX & X1).diam(), 1)
                VX2 = reduce(lambda x, y: x*y, (XX & X2).diam(), 1)
                if VX1 > VX2:
                    X1 = X1 | XX
                    X2 = None if (X & X1).is_empty() else X.diff(X1)[0]
                else:
                    X1 = X2 | XX
                    X2 = None if (X & X1).is_empty() else X.diff(X1)[0]
            elif XX.is_subset(X2):
                X1, X2 = X2, X1
            else:
                print("ERROR !!!")
                # exit(-1)
        old_left, old_right = self.left, self.right
        if X2 is not None:
            self.left = pySetNode(X1)
            self.right = pySetNode(X2)
            self.left.left = old_left
            self.left.right = old_right
        else:
            pass
        return True

    def bisect_max(self, bisector, xin_old, xout_old):
        X = self.xin & self.xout
        XX = self.left.xin | self.left.xout | self.right.xin | self.right.xout
        if X == XX:
            return
        if X.is_subset(XX):
            return
        if not XX.is_subset(X):
            XX &= X
        return self.bisect_around(bisector, XX)



    def clear(self):
        if self.left is not None:
            self.left = None
        if self.right is not None:
            self.right = None

    def is_empty(self):
        return (self.xin & self.xout).is_empty()

    def getX(self):
        if self.X is None:
            self.X = self.xin & self.xout
        return self.X

    def getX0(self):
        if self.X0 is None:
            self.X0 = self.xin | self.xout
        return self.X0

    def isInside(self, x):
        if x not in self.getX():
            if x in self.xin:
                return False
            elif x in self.xout:
                return True
            else:
                return None
                # print("ERROR")
                # exit(-1)
        else:
            if x in self.left.getX0():
                return self.left.isInside(x)
            elif x in self.right.getX0():
                return self.right.isInside(x)


    def write(self, stream):
        size = self.xin.size()
        is_leaf = self.isLeaf()
        fmt = "<i" + "d"*4*size + "?"
        Lin, Lout = [], []
        for b_in, b_out in zip(self.xin, self.xout):
            Lin += [b_in[0], b_in[1]]
            Lout += [b_out[0], b_out[1]]

        data = struct.pack(fmt, size, *Lin, *Lout, is_leaf)
        stream.write(data)
        if not is_leaf:
            self.left.write(stream)
            self.right.write(stream)

    @classmethod
    def read(cls, stream):
        size, = struct.unpack("<i", stream.read(struct.calcsize("<i")))
        fmt = "d"*2*size
        bounds_in = struct.unpack(fmt, stream.read(struct.calcsize(fmt)))
        bounds_out = struct.unpack(fmt, stream.read(struct.calcsize(fmt)))
        is_leaf, = struct.unpack("?", stream.read(struct.calcsize("?")))

        bounds_in = [list(bounds_in[2*i:2*(i+1)]) for i in range(size)]
        bounds_out = [list(bounds_out[2*i:2*(i+1)]) for i in range(size)]
        node = pySetNode(IntervalVector(bounds_in), IntervalVector(bounds_out))
        if not is_leaf:
            node.left = cls.read(stream)
            node.right = cls.read(stream)
        return node

    def draw(self, color_in='#FF0000[#BB00FFAA]', color_out='b[#00AAFFAA]',
             color_maybe='[y]'):
        """Draw."""
        X0 = self.xin | self.xout
        vibes.drawBoxDiff(X0, self.xin, siviaParams["color_in"])
        vibes.drawBoxDiff(X0, self.xout, siviaParams["color_out"])
        if (not self.isLeaf()):
            self.left.draw()
            self.right.draw()
        else:
            X = self.xin & self.xout
            vibes.drawBox(X[0][0], X[0][1], X[1][0], X[1][1], siviaParams["color_maybe"])

    def removeNode(self):
        bleft, bright = self.left.is_empty(), self.right.is_empty()
        if bleft and bright:
            self.left, self.right = None, None
        elif (not bleft) and bright:
            self.right = self.left.right
            self.left = self.left.left
        elif (not bright) and bleft:
            self.left = self.right.left
            self.right = self.right.right
        else:
            print("ERROR !!!")
            exit(-1)

    def reunite(self):
        """Merge empty leaf to make a minimal subpaving."""
        if(self.isLeaf()):
            return
        self.left.reunite()
        self.right.reunite()
        X0 = self.xin | self.xout
        Lin = X0.diff(self.xin)
        Lout = X0.diff(self.xout)
        self.xin = self.left.xin | self.right.xin
        for b in Lout:
            self.xin |= b

        self.xout = self.left.xout | self.right.xout
        for b in Lin:
            self.xout |= b

        if self.left.is_empty() or self.right.is_empty():
            self.removeNode()
        else:
            X = self.xin & self.xout
            self.right.xin &= X
            self.right.xout &= X
            self.left.xin &= X
            self.left.xout &= X

    def __str__(self):
        return "xin : %s xout : %s, %d %d" % (self.xin, self.xout, (self.left is None), (self.right is None))

    def __repr__(self):
        return str(self.xin) + " " + str(self.xout)


class pySepPaving(pyibex.Sep):
    def __init__(self, X0=None, sep=None, eps=1, bsc=LargestFirst(1e-6)):
        if X0 is not None:
            pyibex.Sep.__init__(self, X0.size())
            self.root = pySetNode(X0)
        self.bsc = bsc
        if sep is not None:
            self.sivia(sep, eps)

    @classmethod
    def fromFile(cls, filename):
        with open(filename, 'rb') as stream:
            S = pySepPaving()
            S.root = pySetNode.read(stream)
        return S
        # self.bsc = LargestFirst(1e-6)

    def separate(self, xin, xout):
        X0 = xin | xout
        X = xin & xout
        stack = deque([self.root])
        k = 0
        xborder = IntervalVector.empty(xin.size())
        while len(stack) > 0:
            n = stack.popleft()
            k = k+1
            nX = (n.xin & n.xout)

            if n.isLeaf():
                xborder |= (X & nX)
            else:
                Xleft = n.left.getX() & X
                Xright = n.right.getX() & X
                if not Xleft.is_empty() and not Xleft.is_subset(xborder):
                    stack.append(n.left)
                if not Xright.is_empty() and not Xright.is_subset(xborder):
                    stack.append(n.right)

        xin &= xborder
        xout &= xborder
        for b in X0.diff(xborder):
            isInside = self.root.isInside(b.mid())
            if isInside is True:
                xout |= b
            elif isInside is False:
                xin |= b
            else:
                xin |= b
                xout |= b
        print(k)

    def sivia(self, sep, eps):
        stack = deque([self.root])
        k = 0
        while len(stack) > 0:
            n = stack.popleft()
            k = k+1
            X0 = (n.xin | n.xout)
            # x_in, x_out = n.xin, n.xout
            sep.separate(n.xin, n.xout)

            # n.xin, n.xout = x_in, x_out
            X = (n.xin & n.xout)

            # print(n, len(stack), X)
            # vibes.drawBoxDiff(X0, n.xin, 'r[magenta]')
            # vibes.drawBoxDiff(X0, n.xout, 'b[cyan]')

            if (not X.is_empty()) and X.max_diam() > eps:
                if n.isLeaf():
                    n.bisect(self.bsc)
                else:
                    XX = n.left.getX() | n.right.getX()


                stack.extendleft([n.left, n.right])
            # else:
                # vibes.drawBox(X[0][0], X[0][1], X[1][0], X[1][1], '[y]')

    def sivia_ops(self, sep, eps, union=True):
        stack = deque([self.root])
        k = 0
        # vibes.clearFigure()
        # vibes.newGroup("tmp")
        while len(stack) > 0:
            n = stack.pop()
            k = k+1
            # vibes.clearGroup("tmp")
            X0 = (n.xin | n.xout)
            # vibes.drawBox(X0[0][0], X0[0][1], X0[1][0], X0[1][1], '[g]', group="tmp")


            xin, xout = IntervalVector(X0), IntervalVector(X0)
            sep.separate(xin, xout)
            nxin_old, nxout_old = n.xin.copy(), n.xout.copy()
            if union is True:
                n.xin &= xin
                n.xout |= xout
            else :
                n.xin |= xin
                n.xout &= xout

            X = (n.xin & n.xout)
            # vibes.drawBox(X[0][0], X[0][1], X[1][0], X[1][1], '[orange]', group="tmp")

            # print(n, len(stack), X)
            # vibes.drawBoxDiff(X0, n.xin, 'r[magenta]')
            # vibes.drawBoxDiff(X0, n.xout, 'b[cyan]')

            if (not X.is_empty()) and X.max_diam() > eps:
                if n.isLeaf():
                    n.bisect(self.bsc)
                else:
                    n.bisect_max(self.bsc, nxin_old, nxout_old)

                    # XX = n.left.xin | n.left.xout | n.right.xin | n.left.xout
                    # if X.is_subset(XX) or XX.is_subset(X):
                    #     n.bisect_max(self.bsc, nxin_old, nxout_old)
                    # else:
                    #     if (XX & X).is_empty():
                    #         n.left = pySetNode(XX)
                    #         n.right = pySetNode(XX)
                    #     else:
                    #         XX1 = n.left.xin | n.left.xout
                    #         XX2 = n.right.xin | n.left.xout
                    #         print(XX1 & X, XX2 & X)
                    #         continue
                n.left.xout &= nxout_old
                n.right.xout &= nxout_old
                n.left.xin &= nxin_old
                n.right.xin &= nxin_old
                if n.left.xout.is_flat():
                    n.left.xout.set_empty()
                if n.left.xin.is_flat():
                    n.left.xin.set_empty()
                if n.right.xout.is_flat():
                    n.right.xout.set_empty()
                if n.right.xin.is_flat():
                    n.right.xin.set_empty()

                stack.extendleft([n.left, n.right])
            elif X.is_empty() or X.max_diam() < eps:
                n.left, n.right = None, None
            # else:
                # vibes.drawBox(X[0][0], X[0][1], X[1][0], X[1][1], '[y]')
if __name__ == '__main__':

    from pyibex import Function, SepFwdBwd

    funcs_dict = { "SepMini": Function("x", "y", "(x - 1/2.)^2 + abs(2*y) - 1/4."),
                   "SepNonMini" : Function("x", "y", "x^2 + abs(2*y) - x"),
            }
    funcs_dict = { "SepNonMini" : Function("x", "y", "x^2 + abs(2*y) - x") }

    # funcs_dict = { "SepNonMini" : Function("x", "y", "x^2 + y^2 + x - x + x^3 - x^3") }

    vibes.beginDrawing()
    X0 = IntervalVector(2, [-6, 6])
    for name, func in funcs_dict.items():
        vibes.newFigure(name)
        sep = SepFwdBwd(func, [3, 5])
        vibes.setFigureSize(600, 600)
        # pySIVIA(X0, sep, 0.1, use_patch=True, color_out='#888888[#DDDDDD]',
        # color_in='#888888[#444444]', color_maybe='#AAAAAA[w]')
        # pySIVIA(X0, sep, 0.1, **siviaParams)
        # pySIVIA(X0, sep, 0.1, use_patch=True)
        S = pySepPaving(X0, sep, 0.1)
        S.root.reunite()
        vibes.newFigure("Reunite")
        S.root.draw()

        with open("tets.paving", "wb") as fid:
            S.root.write(fid)

        S2 = pySepPaving.fromFile("tets.paving")
        vibes.newFigure("Reunite", x=500)
        S2.root.draw()
        vibes.axisEqual()

        # S.Reunite()
        # S.visit(SepToVibes(name, color_map=CMap_color3))
        # S.save(name+".paving")
        # break
        # vibes.showAxis(False)
        # drawAxis(-6, 6, -6, 6, step=2, scale=0.4, eps=0.1)
        # vibes.axisEqual()
        # vibes.saveImage(os.path.abspath('images/'+name+".svg"))

    vibes.endDrawing()
