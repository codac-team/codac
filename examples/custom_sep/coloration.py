from codac import *
import math


class DumbCtc(Ctc):

  def __init__(self):
    Ctc.__init__(self, 2)

  def contract(self, x):
    return x


x = VectorVar(2)
q = VectorVar(5)

e = AnalyticFunction([x,q],
  sqr(((x[0]-q[2])*cos(q[4])+(x[1]-q[3])*sin(q[4]))/q[0])
  + sqr(((x[0]-q[2])*sin(q[4])-(x[1]-q[3])*cos(q[4]))/q[1]))

e1 = AnalyticFunction([x], e(x,Vector([3,1,-1,0,math.pi/3.])))
e2 = AnalyticFunction([x], e(x,Vector([2,0.5,-1,0,math.pi/3.])))
s = SepCtcPair(DumbCtc(), CtcInverse(e1,[1]) | CtcInverse(e2,[1]))

#draw_while_paving([[-4,3],[-3,3]], s, 0.1)


p = pave([[-4,2],[-3,3]], s, 0.1)
cs = p.connected_subsets(PavingInOut.outer_complem)
print(str(len(cs)) + " connected subsets")


def coloration(n):
  boxes_to_color = cs[1].boxes()
  h = n.hull()
  u = n.unknown()

  for bi in boxes_to_color:
    
    b = bi & h
    if not b.is_empty():

      if n.boxes()[0].is_empty():
        n.set_boxes(tuple([n.boxes()[1], n.boxes()[0]]))
        return False

      lbi = []
      for bj in boxes_to_color:
        b = bj & h
        if not b.is_empty():
          lbi.append(b)

      prev_x_in = IntervalVector.empty(2)
      x_in = h

      while x_in != prev_x_in:

        prev_x_in.init(x_in)

        for li in lbi:
          d = h.diff(li)
          if d:
            hi = IntervalVector.empty(2)
            for di in d:
              hi |= x_in & di
            x_in &= hi

      n.set_boxes(tuple([hull(lbi)|u, x_in]))
      return True

  return True


p.tree().visit(coloration)
DefaultFigure.set_window_properties([50,50],[800,800])
DefaultFigure.set_axes(axis(0,[-4,2]), axis(1,[-3,3]))
DefaultFigure.draw_paving(p)