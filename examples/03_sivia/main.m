import py.codac4matlab.*

x = VectorVar(2);
f = AnalyticFunction({x}, sqr(x(1))*sin(sqr(x(1))+sqr(x(2)))-sqr(x(2)));
p = sivia(IntervalVector({{-5,5},{-4,4}}), f, Interval(0,oo), 1e-2, true);

fig = Figure2D("My figure", GraphicOutput().VIBES);
fig.draw_paving(p);
fig.set_window_properties(Vector({75,75}),Vector({700,500}));
h = p.tree().hull();
fig.set_axes(axis(1,h(1)), axis(2,h(2)));

% Or...
% DefaultFigure().draw_paving(p);