%  Codac tests
% ----------------------------------------------------------------------------
%  \date       2026
%  \author     Simon Rohou
%  \copyright  Copyright 2026 Codac Team
%  \license    GNU Lesser General Public License (LGPL)

import py.codac4matlab.*

% [ctceval-scalar-beg]
td = create_tdomain(Interval(-PI,PI/2),0.01,true);
s = ScalarVar();
v = SlicedTube(td, AnalyticFunction({s},cos(s)+Interval(-0.1,0.1)));
x = v.primitive(Interval(-0.1,0.1));

t = Interval(-0.5,0.3);
z = Interval(0.3,1.1);
ctc_eval = CtcEval();
ctc_eval.contract(t,z,x,v);
% [ctceval-scalar-end]

assert(~logical(x.is_empty()));
assert(logical(t.is_subset(Interval(-0.5,0.3))));
assert(logical(z.is_subset(Interval(0.3,1.1))));
assert(double(t.lb()) > -0.5);
assert(double(z.ub()) < 1.1);
ref = sin(0.25)+0.1+0.03*0.25;
assert(logical(t.contains(0.25)));
assert(logical(z.contains(ref)));

% [ctceval-propagation-beg]
ctc_deriv = CtcDeriv();
ctc_deriv.contract(x,v);
% [ctceval-propagation-end]

assert(~logical(x.is_empty()));
xt = x(0.25);
assert(logical(xt.contains(ref)));


td = create_tdomain(Interval(-PI,PI/2),0.01,true);
s = ScalarVar();
v = SlicedTube(td, AnalyticFunction({s},cos(s)+Interval(-0.1,0.1)));
x = v.primitive(Interval(-0.1,0.1));

% [ctceval-multi-beg]
t = {Interval(-0.5,0.3),Interval(-0.6,0.8),Interval(-2.3,-2.2)};
z = {Interval(0.3,1.1),Interval(-0.5,-0.4),Interval(-0.8,-0.7)};

ctc_eval = CtcEval();
ctc_deriv = CtcDeriv();
primitives = v.partial_primitives();

for iter = 1:2
  for i = 1:numel(t)
    ctc_eval.contract(t{i},z{i},x,v,primitives); % Optional primitives argument for faster integration
  end
  ctc_deriv.contract(x,v);
end
% [ctceval-multi-end]

assert(~logical(x.is_empty()));
ref_times = [0.25,-0.55,-2.25];
for i = 1:numel(t)
  ti = ref_times(i);
  zi = sin(ti)+0.1+0.03*ti;
  assert(logical(t{i}.contains(ti)));
  assert(logical(z{i}.contains(zi)));
  xt = x(ti);
  assert(logical(xt.contains(zi)));
end


% [ctceval-localization-beg]
td = create_tdomain(Interval(0,5),0.01,true);
s = ScalarVar();
v = SlicedTube(td, AnalyticFunction({s},vec(-2*sin(s),2*cos(2*s))));
v.inflate(0.01);

x = SlicedTube(td, IntervalVector(2));
t = Interval(4.3,4.4);
z = IntervalVector({{-0.73,-0.69},{0.64,0.68}});

ctc_eval = CtcEval();
ctc_eval.contract(t,z,x,v);
ctc_deriv = CtcDeriv();
ctc_deriv.contract(x,v);
% [ctceval-localization-end]

assert(~logical(x.is_empty()));
assert(~logical(x.is_unbounded()));
assert(t == Interval(4.3,4.4));
assert(z == IntervalVector({{-0.73,-0.69},{0.64,0.68}}));
xt = x(0.);
assert(logical(xt.contains(Vector({2.,0.}))));
xt = x(4.35);
assert(logical(xt.contains(Vector({2*cos(4.35),sin(8.7)}))));

% [ctceval-inversion-beg]
y = SlicedTube(td, AnalyticFunction({s},vec(2*cos(s),sin(2*s))));
y.inflate(0.05);
t = td.t0_tf();
z = IntervalVector({{-1,0},{0.4,1.2}});

ctc_eval.contract(t,z,y,v);
% [ctceval-inversion-end]

assert(~logical(y.is_empty()));
assert(logical(t.is_subset(Interval(4.,4.7))));
assert(logical(t.contains(4.35)));
assert(logical(z.is_subset(IntervalVector({{-1,0},{0.4,1.2}}))));
z1 = z(1);
z2 = z(2);
assert(double(z1.ub()) < 0.);
assert(double(z2.ub()) < 1.2);
assert(logical(z.contains(Vector({2*cos(4.35),sin(8.7)}))));
