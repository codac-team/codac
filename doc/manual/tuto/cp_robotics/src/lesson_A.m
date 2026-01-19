import py.codac4matlab.*

% [A-q2-beg]
x_truth = Vector([2.,1.,PI/6.0]);
y_truth = Vector([6.,PI/6.0]);
m_truth = Vector([5.,6.2]);
% [A-q2-end]

% [A-q3-beg]
x = cart_prod(Interval(-oo,oo),Interval(-oo,oo),x_truth(3));
m = IntervalVector(m_truth).inflate(0.2);
y = IntervalVector(y_truth).inflate(Vector([0.3,0.1]));
% [A-q3-end]

DefaultFigure().draw_box(m,Color().red());
DefaultFigure().draw_tank(x_truth, 1, StyleProperties({Color().black(),Color().yellow()}));

% [A-q5-beg]
DefaultFigure().draw_pie(x_truth.subvector(1,2), y(1), x_truth(3)+y(2), Color().red());
DefaultFigure().draw_pie(x_truth.subvector(1,2), y(1).union(0), x_truth(3)+y(2), Color().light_gray());
% [A-q5-end]

% [A-q6-beg]
ctc_polar = CtcPolar();

x123 = VectorVar(7);
f_minus = AnalyticFunction({x123},vec(x123(1)-x123(3)-x123(6), x123(2)-x123(4)-x123(7)));
ctc_minus = CtcInverse(f_minus, Vector([0,0]));

s =VectorVar(3);
f_plus = AnalyticFunction({s}, s(1)+s(2)-s(3));
ctc_plus = CtcInverse(f_plus, Interval(0,0));
% [A-q6-end]

% [A-q7-beg]
a = Interval();
d = IntervalVector(2);
% [A-q7-end]

% [A-q8-beg]
% Either with a smart order of contractor calls:
res_ctc_plus = ctc_plus.contract(cart_prod(x(3), y(2), a)); % The result is a 3D IntervalVector
x.setitem(3,res_ctc_plus(1));
y.setitem(2,res_ctc_plus(2));
a = res_ctc_plus(3);

res_ctc_polar = ctc_polar.contract(cart_prod(d(1),d(2),y(1),a)); % The result is a 4D IntervalVector
d.setitem(1,res_ctc_polar(1));
d.setitem(2,res_ctc_polar(2));
y.setitem(1,res_ctc_polar(3));
a = res_ctc_polar(4);

res_ctc_minus = ctc_minus.contract(cart_prod(m,x,d)); % The result is a 7D IntervalVector
m = res_ctc_minus.subvector(1,2);
x = res_ctc_minus.subvector(3,5);
d = res_ctc_minus.subvector(6,7);
% [A-q8-end]

% [A-q9-beg]
x
DefaultFigure().draw_box(x) % does not display anything if unbounded
% [A-q9-end]