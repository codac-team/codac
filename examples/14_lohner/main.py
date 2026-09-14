from codac import *

x0 = IntervalVector([Interval(1,1),Interval(1,1)])
x0.inflate(0.1)

ta = create_tdomain([0.,10.0],0.2)
a = SlicedTube_IntervalVector(ta, IntervalVector(2))
a.set(x0,0.0)

tb =create_tdomain([0.,10.0],0.01)
b = SlicedTube_IntervalVector(tb, IntervalVector(2))
b.set(x0,0.0)

x = VectorVar(2)
f = AnalyticFunction([x], [-x[0],-sin(x[1])])
ctc_lohner = CtcLohner(f)

ctc_lohner.contract(a)
ctc_lohner.contract(b)

fig1 = Figure2D("Lohner_1",GraphicOutput.VIBES|GraphicOutput.IPE)
fig2 = Figure2D("Lohner_2",GraphicOutput.VIBES|GraphicOutput.IPE)

fig1.plot_tube(a[0])
fig1.plot_tube(b[0],StyleProperties([Color.blue(),Color.blue()]))

fig2.plot_tube(a[1])
fig2.plot_tube(b[1],StyleProperties([Color.blue(),Color.blue()]))

fig1.set_window_properties([100,50],[1000,400])
fig2.set_window_properties([100,500],[1000,400])