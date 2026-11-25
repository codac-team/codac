% Codac manual example
import py.codac4matlab.*

td = create_tdomain(Interval(0,100), 0.5, true);
x = SlicedTube(td, Interval(1,2));

% [slicedtube-partial-primitives-beg]
p = x.partial_primitives();

y1 = x.integral(20,p);                     % integral from 0 to 20
y2 = x.integral(10,20,p);                  % integral from 10 to 20
q = x.partial_integral(Interval(10,20),p); % partial integrals from 0 to an uncertain time in [10,20]
% [slicedtube-partial-primitives-end]

assert(double(y1.lb()) == 20 && double(y1.ub()) == 40);
assert(double(y2.lb()) == 10 && double(y2.ub()) == 20);
