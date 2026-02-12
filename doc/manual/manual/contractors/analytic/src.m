%  Codac tests
% ----------------------------------------------------------------------------
%  \date       2026
%  \author     Maël Godard, Simon Rohou
%  \copyright  Copyright 2026 Codac Team
%  \license    GNU Lesser General Public License (LGPL)

import py.codac4matlab.*

% [ctcinv-1-beg]
% Example of Himmelblau's function
a = 11.0; 
b = 7.0;
x = VectorVar(2);
f = AnalyticFunction({x}, sqr(sqr(x(1))+x(2)-a)+sqr(x(1)+sqr(x(2))-b));
c = CtcInverse(f,50.0);
% [ctcinv-1-end]

% [ctcinv-2-beg]
z = IntervalVector({{0.0,3.5},{0.0,1.0}});
DefaultFigure().draw_box(z, StyleProperties({Color().blue(),Color().blue(.1)})); % prior to contraction
z = c.contract(z);
DefaultFigure().draw_box(z, StyleProperties({Color().blue(),Color().white()})); % after one CtcInverse contraction
% z == [ [1.84, 3.5] ; [0, 1] ]

% Combining CtcInverse with a CtcFixpoint:
cfix = CtcFixpoint(c);
z = c.contract(z);
DefaultFigure().draw_box(z, Color().red()); % after a fixed point contraction
% z == [ [1.84, 2.483] ; [0, 1] ]
% [ctcinv-2-end]

% [ctcinv-3-beg]
DefaultFigure().pave(IntervalVector({{-6,6},{-6,6}}), c, 1e-2);
% [ctcinv-3-end]

% [ctcinv-4-beg]
s = SepInverse(f, Interval(0,50));
DefaultFigure().pave(IntervalVector({{-6,6},{-6,6}}), s, 1e-2);
% [ctcinv-4-end]

% [ctcinv-5-beg]
cu = CtcUnion(CtcUnion(CtcInverse(f,50), CtcInverse(f,150)), CtcInverse(f,250));
DefaultFigure().pave(IntervalVector({{-6,6},{-6,6}}), cu, 1e-2)
% [ctcinv-5-end]

% [ctcinv-6-beg]
x = VectorVar(2);
f = AnalyticFunction({x}, x(1));

% Enforce the first component not in [0,1]
c = CtcInverseNotIn(f, Interval(0,1));

y = IntervalVector({{0.5,3},{-1,1}});
c.contract(y); % [[1,3],[-1,1]]
% Only the first component is constrained by the not-in condition
% [ctcinv-6-end]

assert(y==IntervalVector({{1,3},{-1,1}}));

% [ctcinv-7-beg]
x = VectorVar(2);
f = AnalyticFunction({x}, x(1)-x(2));
c = CtcInverse(f, 0);
assert(c.fnc().input_size() == 2);
assert(c.fnc().output_size() == 1);
% [ctcinv-7-end]