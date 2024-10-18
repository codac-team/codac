import pyibex
from pyibex import IntervalVector, LargestFirst
from collections import deque
try:
    from vibes import vibes
    vibes_available = True
except ImportError:
    vibes_available = False


def pySIVIA(X0, ops, epsilon, **kwargs):
    """
    Execute a SIVIA and generate a paving.

    Parameters
    ----------
        X0: IntervalVector
            Initial box

        ops: ctc or sep
            Operator must be a contractor or a separatot

        epsilon: double
            Accuracy of the sub-Paving.
            Use to end the algorithm.

        color_out : string, optional
            color used to draw boxes which are outide the solution set
            string need to be in vibes format ex: k[r] of #RRGGBBAA[#RRGGBBAA]
            Only used when vibes is installed.
            default value : k[r]

        color_in : string, optional
            color used to draw boxes which are inside the solution set
            string need to be in vibes format ex: k[r] of #RRGGBBAA[#RRGGBBAA]
            Only used when vibes is installed.
            default value : k[r]

        color_maybe : string, optional
            color used to draw boxes which are smaller than eps
            string need to be in vibes format ex: k[r] of #RRGGBBAA[#RRGGBBAA]
            Only used when vibes is installed.
            default value : k[r]

        figure_name: string or None, optional
            Name of the figure on which boxes are drawn
            if None use the current figure
            default value : None

        draw_boxes: boolean, optional
            if True, boxes removed by contractor are displayed
            if vibes is not installed, draw_boxes is always equal to false
            default value False

        display_stats: boolean, optional
            if True, display the number of call of the contractor,
                time needed by the sivia
            and the total number of boxes generated.
            default value: False

        save_result: boolean, optinnal
            default value : True
            Save removed boxes with a list

        use_patch : boolean, optional
            if true draw the difference of boxes with a patch
            natural : compute the difference of two boxes and draw each
                complementary part separatly
            patch : compute the polygon which represents the difference of the
                two boxes
            default patch

    Return
    ------
        return lists of boxes,  outter boxes, inner boxes and mayBe boxes
        Lists are empty if save_boxes is False
    """

    if issubclass(ops.__class__, pyibex.Ctc):
        return __pySIVIA_ctc(X0, ops, epsilon, **kwargs)
    elif issubclass(ops.__class__, pyibex.Sep):
        return __pySIVIA_sep(X0, ops, epsilon, **kwargs)


def drawBoxDiff(X0, X, color, use_patch=False, **kwargs):
    if X0 == X:
        return
    if not X.is_empty():
        if use_patch is True:
            vibes.drawBoxDiff(X0, X, color, **kwargs)
            # vibes.drawBoxDiff([ X0[0].lb(), X0[0].ub(), X0[1].lb(), X0[1].ub()],
            #  [X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()], color)
        else:
            for b in X0.diff(X):
                vibes.drawBox(b[0].lb(), b[0].ub(), b[1].lb(), b[1].ub(), color, **kwargs)
    else:
        vibes.drawBox(X0[0].lb(), X0[0].ub(), X0[1].lb(), X0[1].ub(), color, **kwargs)


def __pySIVIA_ctc(X0, ctc, epsilon, color_out='k[b]', color_maybe='k[y]', draw_boxes=True, save_result=True, **kwargs):

    stack = deque([IntervalVector(X0)])
    res_y, res_out = [], []
    lf = LargestFirst(epsilon/2.0)
    k = 0

    while len(stack) > 0:
        k = k+1
        X = stack.popleft()
        X0 = IntervalVector(X)

        # X =  __contract_and_extract(X, ctc, res_out, color_out)
        ctc.contract(X)
        if (draw_boxes is True and vibes_available is True):
            drawBoxDiff(X0, X, color_out, **kwargs)

        if save_result is True:
            res_out += X0.diff(X)

        if (X.is_empty()):
            continue
        if(X.max_diam() < epsilon):
            if draw_boxes is True:
                vibes.drawBox(
                    X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub(),
                    color_maybe)
            if save_result is True:
                res_y.append(X)
        elif (X.is_empty() is False):
            (X1, X2) = lf.bisect(X)
            stack.append(X1)
            stack.append(X2)

    print('number of contraction %d / number of boxes %d'%(k,len(res_out)+len(res_y)))

    return (res_out, res_y)


def __pySIVIA_sep(X0, sep, epsilon, color_in='k[r]', color_out='k[b]', color_maybe='k[y]', draw_boxes=True, save_result=True, **kwargs):

    stack = deque([IntervalVector(X0)])
    res_y, res_in, res_out = [], [], []
    lf = LargestFirst(epsilon/2.0)
    k = 0
    while len(stack) > 0:
        X = stack.popleft()
        k = k+1
        x_in, x_out = map(IntervalVector, (X, )*2)
        sep.separate(x_in, x_out)

        if (draw_boxes is True and vibes_available is True):
            drawBoxDiff(X, x_in, color_in, **kwargs)
            drawBoxDiff(X, x_out, color_out, **kwargs)
        if save_result is True:
            res_in += X.diff(x_in)
            res_out += X.diff(x_out)

        X = x_in & x_out

        if (X.is_empty()):
            continue
        if(X.max_diam() < epsilon):
            if draw_boxes is True:
                vibes.drawBox(
                    X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub(),
                    color_maybe)
            res_y.append(X)
        elif (X.is_empty() is False):
            (X1, X2) = lf.bisect(X)
            stack.append(X1)
            stack.append(X2)


    print('number of separation %d / number of boxes %d'%(k,len(res_in)+len(res_out)+len(res_y)))

    return (res_in , res_out, res_y)


if __name__ == '__main__':
    from pyibex import *
    f = Function('x', 'y', 'x^2 + y^2')
    ctc = CtcFwdBwd(f, sqr(Interval(3,4)))
    sep = SepFwdBwd(f, sqr(Interval(3,4)))
    params = {'color_in': 'r[g]', 'color_out':'b[#AACC00]', 'color_maybe':'k[y]', 'use_patch': False}
    box = IntervalVector(2, [-5, 5])
    vibes.beginDrawing()
    pySIVIA(box, ctc, 0.1, draw_boxes=True, save_result=False, **params)
    vibes.newFigure('test2')
    pySIVIA(box, sep, 0.1, draw_boxes=True, save_result=False, **params)
    vibes.endDrawing()
