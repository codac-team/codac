from pyibex import IntervalVector, Interval
from pyibex.paving import SepVisitor
from vibes import vibes
import time

# CMap_BW = {
#   str(IN): "darkGray[w]",
#   str(OUT): "darkGray[k]",
#   str(MAYBE): "darkGray[lightGray]",
#   str(MAYBE_IN): "darkGray[lightGray]",
#   str(MAYBE_OUT): "darkGray[lightGray]",
#   str(UNK):  "darkGray[lightGray]",
#   str(EMPTY): "[g]"
# }

CMap_color = {
  "IN": "[red]",
  "OUT": "[blue]",
  "MAYBE": "[orange]",
  "MAYBE_IN": "[magenta]",
  "MAYBE_OUT": "[cyan]",
  "UNK":  "[y]",
  "EMPTY": "[g]"
}

CMap_color = {
  "IN": "[#FF000022]",
  "OUT": "[#0000FF22]",
  "MAYBE": "[orange]",
  "MAYBE_IN": "[magenta]",
  "MAYBE_OUT": "[cyan]",
  "UNK":  "[y]",
  "EMPTY": "[g]"
}

# CMap_color2 = {
#
#   str(IN): "mangenta[red]",
#   str(OUT): "#000099[blue]",
#   str(MAYBE): "transparent[orange]",
#   str(MAYBE_IN): "[y]",
#   str(MAYBE_OUT): "[y]",
#   str(UNK):  "[y]",
#   str(EMPTY): "[g]"
# }
#
# CMap_color3 = {
#   str(IN): "[red]",
#   str(OUT): "[blue]",
#   str(MAYBE): "[orange]",
#   str(MAYBE_IN): "[y]",
#   str(MAYBE_OUT): "[y]",
#   str(UNK):  "[y]",
#   str(EMPTY): "[g]"
# }


def drawBoxDiff(X0, X, color, use_patch=False, **kwargs):
    if X0 == X:
        return
    if not X.is_empty():
        if use_patch is True:
            drawBoxDiff(X0, X, color, **kwargs)
            # vibes.drawBoxDiff([ X0[0].lb(), X0[0].ub(), X0[1].lb(), X0[1].ub()],
            #  [X[0].lb(), X[0].ub(), X[1].lb(), X[1].ub()], color)
        else:
            for b in X0.diff(X):
                vibes.drawBox(
                  b[0].lb(), b[0].ub(), b[1].lb(), b[1].ub(),
                  color, **kwargs)
    else:
        vibes.drawBox(
            X0[0].lb(), X0[0].ub(), X0[1].lb(), X0[1].ub(),
            color, **kwargs)


class SepToVibes(SepVisitor):
    def __init__(self, figureName=None, color_map=CMap_color, clearFigure=True):
        """ ToVibes visitor object
        Args:
          max (float): bound of the windows centered in (0,0)
          figureName (string): If None create a new figure else select the existing one
          BW (bool): Use black and white color map
        """
        SepVisitor.__init__(self)
        self.figureName = str(figureName)
        if figureName is not None:
            vibes.selectFigure(figureName)
            if clearFigure is True:
                vibes.clearFigure()
        self.color_map = color_map

    def visit_leaf(self, boxIn, boxOut):
        X0 = boxIn | boxOut
        X = boxIn & boxOut
        # print(X0, boxIn, boxOut)
        drawBoxDiff(X0, boxIn, self.color_map["IN"])
        drawBoxDiff(X0, boxOut, self.color_map["OUT"])
        if not X.is_empty():
            for i in range(X.size()):
                if X[i].diam() < 1e-8:
                    X[i].inflate(0.05)
            vibes.drawBox(X[0][0], X[0][1], X[1][0], X[1][1], self.color_map["MAYBE"])
    # time.sleep(0.5)

    def pre_visit(self, paving):
        if(self.figureName is not None):
            vibes.selectFigure(self.figureName)
    # self.frame = paving.X0

    def visit_node(self, boxIn, boxOut):

        X0 = boxIn | boxOut
        # print(X0, boxIn, boxOut)
        drawBoxDiff(X0, boxIn, self.color_map["IN"])
        drawBoxDiff(X0, boxOut, self.color_map["OUT"])

    def post_visit(self, paving):
        vibes.axisEqual()
