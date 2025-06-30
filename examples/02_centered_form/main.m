import py.codac4matlab.*

x = VectorVar(3);
f = AnalyticFunction({x}, vec( ...
  -sqr(x(3))+2*x(3)*sin(x(3)*x(1))+cos(x(3)*x(2)), ...
  2*x(3)*cos(x(3)*x(1))-sin(x(3)*x(2)) ...
));

ctc = CtcInverse(f, IntervalVector({0,0}));
draw_while_paving(IntervalVector({{0,2},{2,4},{0,10}}), ctc, 0.004);