from codac import *

# [A-q2-beg]
x_truth = Vector([2,1,PI/6])
y_truth = Vector([6,PI/6])
m_truth = Vector([5,6])
# [A-q2-end]

# [A-q3-beg]
x = cart_prod([-oo,oo],[-oo,oo],x_truth[2])
m = IntervalVector(m_truth).inflate(0.2)
y = IntervalVector(y_truth).inflate([0.3,0.1])
# [A-q3-end]

#fig = Figure2D("Robot simulation", GraphicOutput.VIBES | GraphicOutput.IPE)
DefaultFigure.draw_box(m, Color.red())
DefaultFigure.draw_tank(x_truth, size=1, style=[Color.black(),Color.yellow()])
# [A-q5-beg]
DefaultFigure.draw_pie(x_truth.subvector(0,1), y[0],     x_truth[2]+y[1], Color.red())
DefaultFigure.draw_pie(x_truth.subvector(0,1), y[0] | 0, x_truth[2]+y[1], Color.light_gray())
# [A-q5-end]

# fig.set_axes(
#   axis(0, X[0].inflate(10), "x_1"),
#   axis(1, X[1].inflate(10), "x_2")
# ).auto_scale()

# [A-q6-beg]
ctc_polar = CtcPolar()

x1,x2,x3 = VectorVar(2), VectorVar(3), VectorVar(2)
f_minus = AnalyticFunction([x1,x2,x3], [
    x1[0]-x2[0]-x3[0],
    x1[1]-x2[1]-x3[1]
])
ctc_minus = CtcInverse(f_minus, [0,0])

x1,x2,x3 = ScalarVar(), ScalarVar(), ScalarVar()
f_plus = AnalyticFunction([x1,x2,x3], x1+x2-x3)
ctc_plus = CtcInverse(f_plus, 0)
# [A-q6-end]

# [A-q7-beg]
a = Interval()
d = IntervalVector(2)
# [A-q7-end]

# [A-q8-beg]
# Either with a smart order of contractor calls:

x[2],y[1],a = ctc_plus.contract(x[2],y[1],a)
d[0],d[1],y[0],a = ctc_polar.contract(d[0],d[1],y[0],a)
m,x,d = ctc_minus.contract(m,x,d)

# Or using a fixpoint method:

def constraints(x,y,m,a,d):
  x[2],y[1],a = ctc_plus.contract(x[2],y[1],a)
  d[0],d[1],y[0],a = ctc_polar.contract(d[0],d[1],y[0],a)
  m,x,d = ctc_minus.contract(m,x,d)
  return x,y,m,a,d

x,y,m,a,d = fixpoint(constraints, x,y,m,a,d)
# [A-q8-end]

# [A-q9-beg]
print(x)
DefaultFigure.draw_box(x) # does not display anything if unbounded
# [A-q9-end]