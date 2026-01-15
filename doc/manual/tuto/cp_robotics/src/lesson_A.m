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

test = Interval(1,2) | Interval (3,4)

% [A-q5-beg]
% DefaultFigure().draw_pie(x_truth.subvector(1,2), y(1),     x_truth(3)+y(2), Color().red());
% DefaultFigure().draw_pie(x_truth.subvector(1,2), y(1)|Interval(0,0), x_truth(3)+y(2), Color().light_gray());
% [A-q5-end]