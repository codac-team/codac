from pyibex.thickset import *
from pyibex import Interval, IntervalVector
from vibes import vibes


def ex_circle():
  t = ThickDisk(0,0,2)
  X0 = IntervalVector(2, [-5,5])
  # P = ThickPaving(X0, lambda x: opNot(t.test(x)), 0.05)
  P = ThickPaving(X0, t, 0.05)

  P.visit(ToVibes(1000, "Disk"))

  t2 = ThickTranslateInPaving(P, IntervalVector([[0,0], [0,1]]))
  X1 = IntervalVector([[4.6,4.7], [2.8,2.9]])
  vibes.drawBox(X1[0][0], X1[0][1], X1[1][0], X1[1][1], 'y')
  vibes.drawBox(X1[0][0]+1, X1[0][1]+1, X1[1][0]+1, X1[1][1]+1, 'g')
  vibes.drawBox(X1[0][0]+1, X1[0][1]+1, X1[1][0]+1, X1[1][1]+1, 'g')
  vibes.drawBox(5, 5, X0[1][0]+1, X0[1][1]+1, 'orange')
  vibes.axisAuto()
  P2 = ThickPaving(X0.inflate(10), t2, 0.1)
  P2.visit(ToVibes(1000, "Disk translated"))
  vibes.setFigurePos(500,10)




if __name__ == '__main__':
  vibes.beginDrawing()
  ex_circle()
  vibes.endDrawing()
