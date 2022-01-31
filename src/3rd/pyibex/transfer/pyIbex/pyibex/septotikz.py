from pyibex import IntervalVector, Interval
from pyibex.paving import SepVisitor
from vibes import vibes
import time

preambule = """\\documentclass{article}
\\usepackage{tikz}
\\usepackage{mytikzpatterns}
% \\usetikzlibrary{arrows, arrows.meta}

\\usepackage[active,tightpage]{preview}
\\PreviewEnvironment{tikzpicture}
\\setlength\PreviewBorder{5pt}

\\begin{document}
\\begin{tikzpicture}
"""

closing = """\\end{scope}
\\end{tikzpicture}
\\end{document}
"""


axisDeg = """\begin{axis}[
    scale only axis,
    width=10cm,
    height=10cm,
    xmin=-11,
    xmax=11,
    ymin=-11,
    ymax=11,
    % grid=major,
    % inner axis line style={=>, very thick, red},
    xlabel=$x$,
    ylabel=$y$,
    axis y line=left,
    axis x line=bottom,
    axis on top=true,
    every axis x label/.style={
    at={(ticklabel* cs:1.)},
    anchor=south,
    },
    every axis y label/.style={
        at={(ticklabel* cs:1.)},
        anchor=west,
    },
    xticklabel style={anchor=south},
    yticklabel style={anchor=west}
]"""

TikzColorMap = {
  "IN": "magenta, fill=red!50",
  "OUT": "cyan, fill=blue!50",
  "MAYBE": "orange, fill=yellow",
  # str(MAYBE_IN): "yellow",
  # str(MAYBE_OUT) :"yellow",
  # str(UNK) :  "yellow",
  # str(EMPTY) : "green"
}

class SepToTikz(SepVisitor):
    def __init__(self, outputFilename, CMap_color=TikzColorMap):
        SepVisitor.__init__(self)
        self.f = open(outputFilename, 'w')
        self.color_map = CMap_color
        print("write Paving into ",outputFilename)
        # self.x0, self.y0 = BB[0][0], BB[1][0];

    def __del__(self):
        self.f.write(closing)
        self.f.close()

    def drawBox(self, X, color=''):
        if X.size() == 1:
            s = "\draw[%s] (%f, %f) rectangle (%f, %f);\n" % (color, X[0][0], 0, X[0][1], 1)
        elif X.size() == 2:
            s = "\draw[%s] (%f, %f) rectangle (%f, %f);\n" % (color, X[0][0], X[1][0], X[0][1], X[1][1])
        else:
            return
        self.f.write(s)

    def drawBoxDiff(self, X0, X, color=''):
        if X0 == X:
            return
        if X.is_empty():
            self.drawBox(X, color)
            return

        L = X0.diff(X)
        if len(L) > 1:
            s = "\draw[%s] (%f, %f) rectangle (%f, %f);\n" % (color, X[0][0],0, X[0][1], 1)
            s = "\draw[even odd rule,pattern=crosshatch, %s] " % color
            s += " (%f,%f) rectangle (%f,%f) " % (X[0][0], X[1][0], X[0][1], X[1][1])
            s += " (%f,%f) rectangle (%f,%f);\n" % (X0[0][0], X0[1][0], X0[0][1], X0[1][1])
            self.f.write(s)
        else:
            self.drawBox(L[0], color)

    def visit_leaf(self, boxIn, boxOut):
        X0 = boxIn | boxOut
        X = boxIn & boxOut
        # print(X0, boxIn, boxOut)
        self.drawBoxDiff(X0, boxIn, "IN")
        self.drawBoxDiff(X0, boxOut, "OUT")
        if not X.is_empty():
            self.drawBox(X, "MAYBE")
    # time.sleep(0.5)


    def post_visit(self, paving):
        pass
        # self.f.write(post)


    def visit_node(self, boxIn, boxOut):
        X0 = boxIn | boxOut
        self.drawBoxDiff(X0, boxIn, "IN")
        self.drawBoxDiff(X0, boxOut, "OUT")

    def pre_visit(self, paving):

        self.f.write(preambule)
        # write style
        for k, v in self.color_map.items():
            key = str(k)
            self.f.write("\\tikzset{%s/.style={%s}}\n" % (key, v))
        # get paving size
        diam = paving.getBoundingBox().max_diam()
        self.f.write('\\pgfmathsetmacro{{\\ratio}}{{{:g}}}\n'.format(1+diam*0.05))
        self.f.write("\\tikzset{AXIS/.style={line width=\\ratio, -{Latex[scale=0.8*\\ratio]} }}\n")
        self.f.write("\\tikzset{tickMarks/.style={line width={0.5*\\ratio}}}\n")
        self.f.write("\\begin{scope}[every node/.append style={scale=\\ratio}]\n")

    def drawPolygon(self, P, color='', cycle=True):
        s = "\draw[{}]".format(color)
        for pt in P:
            s += ' ( {:g}, {:g}) -- '.format(pt[0], pt[1])
        if cycle is True:
            s += 'cycle;\n'
        else:
            s = s[:-4]+';\n'
        self.f.write(s)

    def drawLine(self, P, color=''):
        self.drawPolygon(P, color, cycle=False)

    def drawCircle(self, cx, cy, r, color):
        s = "\\draw[{}] ({:g},{:g}) circle ({:g}cm);\n".format(color, cx, cy, r)
        self.f.write(s)

    def drawVehicle(self, x, y, th, scale, color):
        s = "\\begin{{scope}}[shift={{({:g},{:g})}}, rotate={:g}, scale={:g}]\n".format(x, y, th, scale)
        s += "\t\\draw[{}] (-1, -0.5) -- (1, 0) -- (-1, 0.5) -- cycle;\n".format(color)
        s += "\\end{scope}\n"
        self.f.write(s)

    def addAxisAuto(self, X, step=2, offset=0, outside=True):
        s ="\n%%%%% DRAW AXIS %%%%%\n"
        xlb, xub, ylb, yub = X[0][0], X[0][1], X[1][0], X[1][1]
        s += "\pgfmathsetmacro{{\\width}}{{{:g}}}\n".format(X.max_diam()*0.005);
        # s += "\pgfmathsetmacro{{\\scale}}{{{:g}}}\n".format(X.max_diam()*0.005);
        s += "\draw[AXIS] ({:g}, {:g}) -- ({:g}, {:g});\n".format(xlb, ylb, xub, ylb)
        s += "\draw[AXIS] ({:g}, {:g}) -- ({:g}, {:g});\n".format(xlb, ylb, xlb, yub)
        s += "\\foreach \\x in {{{:g},{:g},...,{:g}}}{{\n".format(xlb+offset, xlb+offset+step, xub)
        if outside is True:
            s += "\t\draw[tickMarks] (\\x, {{{:g}+\\width}}) -- (\\x, {{{:g}-\\width}}) ".format(ylb, ylb)
            s += "node[anchor=north] {\\x};\n}"
        else :
            s += "\t\draw[tickMarks] (\\x, {{{:g}-\\width}}) -- (\\x, {{{:g}+\\width}}) ".format(ylb, ylb)
            s += "node[anchor=south] {\\x};\n}"

        s += "\\foreach \\x in {{{:g},{:g},...,{:g}}}{{\n".format(ylb+offset, ylb+offset+step, yub)
        if outside is True:
            s += "\t\draw[tickMarks] ({{{:g}+\\width}}, \\x ) -- ({{{:g}-\\width}}, \\x) ".format(xlb, xlb)
            s += "node[anchor=east] {\\x};\n}"
        else :
            s += "\t\draw[tickMarks] ({{{:g}-\\width}}, \\x ) -- ({{{:g}+\\width}}, \\x) ".format(xlb, xlb)
            s += "node[anchor=west] {\\x};\n}"
        s +="\n%%%%% DRAW AXIS %%%%%\n"
        # print(s)
        self.f.write(s)

    # def addAxis(dim, step):
