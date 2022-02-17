from pyibex import IntervalVector, Interval
from pyibex.thickset import SepVisitor
from pyibex.thickset import *
from vibes import vibes


CMap_BW = {
  str(IN): "darkGray[w]",
  str(OUT): "darkGray[k]",
  str(MAYBE): "darkGray[lightGray]",
  str(MAYBE_IN): "darkGray[lightGray]",
  str(MAYBE_OUT): "darkGray[lightGray]",
  str(UNK):  "darkGray[lightGray]",
  str(EMPTY): "[g]"
}

CMap_color = {
  str(IN): "[red]",
  str(OUT): "[blue]",
  str(MAYBE): "[orange]",
  str(MAYBE_IN): "[magenta]",
  str(MAYBE_OUT): "[cyan]",
  str(UNK):  "[y]",
  str(EMPTY): "[g]"
}

CMap_color2 = {

  str(IN): "mangenta[red]",
  str(OUT): "#000099[blue]",
  str(MAYBE): "transparent[orange]",
  str(MAYBE_IN): "[y]",
  str(MAYBE_OUT): "[y]",
  str(UNK):  "[y]",
  str(EMPTY): "[g]"
}

CMap_color3 = {
  str(IN): "[red]",
  str(OUT): "[blue]",
  str(MAYBE): "[orange]",
  str(MAYBE_IN): "[y]",
  str(MAYBE_OUT): "[y]",
  str(UNK):  "[y]",
  str(EMPTY): "[g]"
}


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
                vibes.drawBox(
                  b[0].lb(), b[0].ub(), b[1].lb(), b[1].ub(),
                  color, **kwargs)
    else:
        vibes.drawBox(
            X0[0].lb(), X0[0].ub(), X0[1].lb(), X0[1].ub(),
            color, **kwargs)

import time
class SepToVibes(SepVisitor):
  def __init__(self, figureName=None, color_map=CMap_color, clearFigure=True):
    """ ToVibes visitor object
    Args:
      figureName (string): If None create a new figure else select the existing one
      BW (bool): Use black and white color map
    """
    SepVisitor.__init__(self)
    # self.frame = IntervalVector(2, max*Interval(-1, 1))

    self.figureName = str(figureName)
    if figureName != None:
      vibes.selectFigure(figureName)
      vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 500, 'height': 500})
      if clearFigure == True:
        vibes.clearFigure()
    self.color_map = color_map

  """
  Function that will be called automatically on every boxes (leaves) of the set.
  """
  def visit_leaf(self, boxIn, boxOut):

    X0 = boxIn | boxOut
    X = boxIn & boxOut
    # print(X0, boxIn, boxOut)
    drawBoxDiff(X0, boxIn, self.color_map[str(IN)])
    drawBoxDiff(X0, boxOut, self.color_map[str(OUT)])
    if not X.is_empty():
      vibes.drawBox(X[0][0], X[0][1], X[1][0], X[1][1], self.color_map[str(MAYBE)])
    # time.sleep(0.5)

  def pre_visit(self, paving):
    if(self.figureName != None):
      vibes.selectFigure(self.figureName)
    # self.frame = paving.X0

  def visit_node(self, boxIn, boxOut):

    X0 = boxIn | boxOut
    # print(X0, boxIn, boxOut)
    drawBoxDiff(X0, boxIn, self.color_map[str(IN)])
    drawBoxDiff(X0, boxOut, self.color_map[str(OUT)])

  def post_visit(self, paving):
    vibes.axisEqual()
