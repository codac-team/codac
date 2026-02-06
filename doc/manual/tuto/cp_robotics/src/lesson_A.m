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
DefaultFigure().draw_pie(x_truth.subvector(1,2), y(1),          x_truth(3)+y(2), Color().red());
DefaultFigure().draw_pie(x_truth.subvector(1,2), y(1).union(0), x_truth(3)+y(2), Color().light_gray());
% [A-q5-end]

% [A-q6-beg]
ctc_polar = CtcPolar();

[x1,x2,x3] = deal(VectorVar(2), VectorVar(3), VectorVar(2));
f_minus = AnalyticFunction({x1,x2,x3},vec(x1(1)-x2(1)-x3(1), x1(2)-x2(2)-x3(2)));
ctc_minus = CtcInverse(f_minus, Vector([0,0]));

[s1,s2,s3] = deal(ScalarVar(),ScalarVar(),ScalarVar());
f_plus = AnalyticFunction({s1,s2,s3}, s1+s2-s3);
ctc_plus = CtcInverse(f_plus, Interval(0,0));
% [A-q6-end]

% [A-q7-beg]
a = Interval();
d = IntervalVector(2);
% [A-q7-end]

% [A-q8-beg]
% Either with a smart order of contractor calls:

res_ctc_plus = ctc_plus.contract(cart_prod(x(3), y(2), a)); % The result is a 3D IntervalVector
x.set_item(3,res_ctc_plus(1));
y.set_item(2,res_ctc_plus(2));
a = res_ctc_plus(3);

res_ctc_polar = ctc_polar.contract(cart_prod(d(1),d(2),y(1),a)); % The result is a 4D IntervalVector
d.set_item(1,res_ctc_polar(1));
d.set_item(2,res_ctc_polar(2));
y.set_item(1,res_ctc_polar(3));
a = res_ctc_polar(4);

res_ctc_minus = ctc_minus.contract(cart_prod(m,x,d)); % The result is a 7D IntervalVector
m = res_ctc_minus.subvector(1,2);
x = res_ctc_minus.subvector(3,5);
d = res_ctc_minus.subvector(6,7);

% Or using a fixpoint method:
function [x,y,m,a,d] = contract(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus)
    res_ctc_plus = ctc_plus.contract(py.codac4matlab.cart_prod(x(3), y(2), a)); % The result is a 3D IntervalVector
    x.set_item(3,res_ctc_plus(1));
    y.set_item(2,res_ctc_plus(2));
    a = res_ctc_plus(3);

    res_ctc_polar = ctc_polar.contract(py.codac4matlab.cart_prod(d(1),d(2),y(1),a)); % The result is a 4D IntervalVector
    d.set_item(1,res_ctc_polar(1));
    d.set_item(2,res_ctc_polar(2));
    y.set_item(1,res_ctc_polar(3));
    a = res_ctc_polar(4);

    res_ctc_minus = ctc_minus.contract(py.codac4matlab.cart_prod(m,x,d)); % The result is a 7D IntervalVector
    m = res_ctc_minus.subvector(1,2);
    x = res_ctc_minus.subvector(3,5);
    d = res_ctc_minus.subvector(6,7);
end

function [x,y,m,a,d] = fixpoint(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        [x,y,m,a,d] = contract(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus);
        vol = x.volume() + y.volume() + m.volume() + a.volume() + d.volume();
    end
end

[x,y,m,a,d] = fixpoint(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus);
% [A-q8-end]

% [A-q9-beg]
x
DefaultFigure().draw_box(x); % does not display anything if unbounded
% [A-q9-end]