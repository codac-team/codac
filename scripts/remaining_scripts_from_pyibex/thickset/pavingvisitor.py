from pyibex import IntervalVector, Interval
from pyibex.thickset import PavingVisitor, IN, OUT, MAYBE_IN, MAYBE_OUT, UNK, MAYBE, EMPTY
from vibes import vibes

CMap_BW = {
  str(IN) : "darkGray[w]",
  str(OUT): "darkGray[k]",
  str(MAYBE): "darkGray[lightGray]",
  str(MAYBE_IN): "darkGray[lightGray]",
  str(MAYBE_OUT) :"darkGray[lightGray]",
  str(UNK) :  "darkGray[lightGray]",
  str(EMPTY) : "[g]"
}

CMap_color = {
  str(IN) : "[red]",
  str(OUT): "[blue]",
  str(MAYBE): "[orange]",
  str(MAYBE_IN): "[magenta]",
  str(MAYBE_OUT) :"[cyan]",
  str(UNK) :  "[y]",
  str(EMPTY) : "[g]"
}

CMap_color2 = {

  str(IN) : "mangenta[red]",
  str(OUT): "#000099[blue]",
  str(MAYBE): "transparent[orange]",
  str(MAYBE_IN): "[y]",
  str(MAYBE_OUT) :"[y]",
  str(UNK) :  "[y]",
  str(EMPTY) : "[g]"
}

CMap_color3 = {
  str(IN) : "[red]",
  str(OUT): "[blue]",
  str(MAYBE): "[orange]",
  str(MAYBE_IN): "[y]",
  str(MAYBE_OUT) :"[y]",
  str(UNK) :  "[y]",
  str(EMPTY) : "[g]"
}


CMap_color_alpha = {
  str(IN) : "r[#FF000077]",
  str(OUT): "b[#0000FF77]",
  str(MAYBE): "orange[#FFA50077]",
  str(MAYBE_IN): "[#FFFF0077]",
  str(MAYBE_OUT) :"[#FFFF0077]",
  str(UNK) :  "[#FFFF0077]",
  str(EMPTY) : "[g]"
}

class ToVibes(PavingVisitor):
  def __init__(self, figureName=None, color_map=CMap_color, clearFigure=True):
    """ ToVibes visitor object
    Args:
      max (float): bound of the windows centered in (0,0)
      figureName (string): If None create a new figure else select the existing one
      BW (bool): Use black and white color map
    """
    PavingVisitor.__init__(self)
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
  def visit_leaf(self, box, status):
    framebox= box & self.frame;
    if str(status) not in self.color_map:
      return
    color=self.color_map[str(status)]#.get(str(status), '[gray]')
    # Plot the box with Vibes
    if framebox.size() == 1:
      vibes.drawBox(framebox[0].lb(), framebox[0].ub(), 0, 1  , color);
    elif framebox.size() == 2:
      vibes.drawBox(framebox[0].lb(), framebox[0].ub(), framebox[1].lb(), framebox[1].ub(), color);


  def pre_visit(self, paving):
    if(self.figureName != None):
      vibes.selectFigure(self.figureName)
    self.frame = paving.X0

  def visit_node(self, box):
    pass


  def post_visit(self, paving):
    vibes.axisEqual()

TikzColorMap = {
  str(IN) : "red",
  str(OUT): "blue",
  str(MAYBE): "orange",
  str(MAYBE_IN): "yellow",
  str(MAYBE_OUT) :"yellow",
  str(UNK) :  "yellow",
  str(EMPTY) : "green"
}

class Extractor(PavingVisitor):
	def __init__(self, val):
		PavingVisitor.__init__(self)
		self.L = []
		self.val = val

	def visit_leaf(self, box, status):
		if status in self.val:
			self.L.append(box)


#
class ToTikz(PavingVisitor):
  def __init__(self, outputFilename, CMap_color=TikzColorMap):
    PavingVisitor.__init__(self)
    self.f = open(outputFilename, 'w')
    self.color_map = CMap_color
    # self.x0, self.y0 = BB[0][0], BB[1][0];

  def __del__(self):
    self.f.close()

  def drawBox(self, X, color=''):
    if X.size() == 1:
      s = "\draw[%s] (%f, %f) rectangle (%f, %f);\n"%(color, X[0][0],0, X[0][1], 1)
    elif X.size() == 2:
      s = "\draw[%s] (%f, %f) rectangle (%f, %f);\n"%(color, X[0][0],X[1][0], X[0][1], X[1][1])
    else:
      return

    self.f.write(s)

  def pre_visit(self, paving):
    X0 = paving.X0
    # write style
    for k,v in self.color_map.items():
      key = str(k).split('.')[1]
      self.f.write("\\tikzset{%s/.style={fill=%s}}\n"%(key, v))
    self.drawBox(X0)

  def visit_leaf(self, box, status):
    X = box# - [self.x0, self.y0]
    color = str(status).split('.')[1]
    # color=self.color_map.get(str(status), '')
    self.drawBox(X, color)
