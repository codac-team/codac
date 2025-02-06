from codac import *
from codac.unsupported import *
from math import pi
from vibes import vibes

class SepDist(SepFwdBwd):

  def __init__(self, c, intv_r):
    SepFwdBwd.__init__(self,
      Function('x[2]', 'sqrt(sqr(x[0]-'+str(c[0])+')+sqr(x[1]-'+str(c[1])+'))'),
      intv_r)

class Sep2dRot(SepInverse):

  def __init__(self, sep, a):
    SepInverse.__init__(self, sep,
      Function('x[2]', '(x[0]*cos('+str(a)+')-x[1]*sin('+str(a)+');'+
                        'x[0]*sin('+str(a)+')+x[1]*cos('+str(a)+'))'))

class SepTranslate(SepInverse):

  def __init__(self, sep, c):
    SepInverse.__init__(self, sep,
      Function('x[2]', '(x[0]-'+str(c[0])+';x[1]-'+str(c[1])+')'))


a1,a2 = 0, 2*pi/3
c1 = [cos(a1)*3.9,sin(a1)*3.9]
c2 = [cos(a1)*5.3,sin(a1)*5.3]
c3 = [cos(a2)*3.9,sin(a2)*3.9]

s_legs = SepDist(c1, Interval(0,4.8))
s_legs &= SepDist(c2, Interval(3,oo))
s_ = SepPolarXY(Interval(4.8,6), Interval(a1).inflate(0.5))
s_legs &= SepNot(Sep2dRot(SepTranslate(s_,c3), a1))
s_legs &= SepNot(SepDist(c3, Interval(0,5)))

s_heart = SepDist(c3, Interval(0,4.8))

for i in range(2,5,2):
  s_legs |= Sep2dRot(s_legs,i*pi/3)
  s_heart &= Sep2dRot(s_heart,i*pi/3)

s_triskel = s_heart | s_legs


x = IntervalVector([[-10,10],[-10,10]])

vibes.beginDrawing()
vibes.newFigure("Triskelion")
SIVIA(x, s_triskel, 0.05, fig_name="Triskelion")
vibes.setFigureProperties({"x":100, "y":100, "width":900, "height":900})
endDrawing()