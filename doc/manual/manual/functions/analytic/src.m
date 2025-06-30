import py.codac4matlab.*

% [1-beg]
x1 = ScalarVar();
x2 = ScalarVar();
v = VectorVar(3);

% Example of scalar function: from R to R
f1 = AnalyticFunction({x1}, x1*cos(x1));

% Example of vectorial function: from R to R²
f2 = AnalyticFunction({x1}, vec(x1*cos(x1), x1*sin(x1)));

% Example of vectorial function: from R³ to R²
f3 = AnalyticFunction({v}, Interval(-1,1)*v);

% Example of multivariate vectorial function: from R×R to R³
f4 = AnalyticFunction({x1,x2}, vec(x1+x2, Interval(0,1)*exp(x1), x2^(1+x1)));
% [1-end]


% [2-beg]
% Example of function: from R³ to R²
w = v(1)*v;
f5 = AnalyticFunction({v}, w.subvector(2,3));

% Example of scalar function: from R^(2x2) to R
M = MatrixVar(2,2);
f6 = AnalyticFunction({M}, M(1,1)*M(2,2)-M(2,1)*M(1,2));
% [2-end]


% [3-beg]
function f = create_f()
  import py.codac4matlab.*
  x = ScalarVar();
  f = AnalyticFunction({x}, x*cos(x));
end

f = create_f(); % x is no longer useful here
% [3-end]


% [4-beg]
f = AnalyticFunction({x1}, x1*cos(x1));
g = AnalyticFunction({x1}, vec(f(2*x1), x1*sin(x1)));
gx1 = g(x1);
h = AnalyticFunction({x1}, gx1(1)) % output is 2*x*cos(2*x);
% [4-end]


% [4b-beg]
a = ScalarVar();
v = VectorVar(2);

Rot = AnalyticFunction({a}, mat(vec(cos(a),sin(a)),vec(-sin(a),cos(a))));
f = AnalyticFunction({v}, Rot(PI/4)*v);

y = f.eval(Vector({1,1})); % y == [ [-2.22045e-16, 4.4409e-16] ; [1.41421, 1.41422] ]
% [4b-end]


% [5-beg]
x1 = ScalarVar(); % scalar argument
f1 = AnalyticFunction({x1}, x1*cos(x1));

y1 = f1.eval(0.);
y1 = f1.eval(PI);
y1 = f1.eval(Interval(0,1));

x2 = ScalarVar();
f2 = AnalyticFunction({x1,x2}, x1^x2); % example of multivariate function

y2 = f2.eval(Interval(2,3), 2);

v = VectorVar(3); % vector arguments
w = VectorVar(3);
f3 = AnalyticFunction({v,w}, v-w); % example of vectorial function

y3 = f3.eval(Vector({5,4,3}), IntervalVector({{3,oo},{2},{1,2}}));
% [5-end]


% [6-beg]
f1.eval(EvalMode_NATURAL, Interval(0,1));
f1.eval(EvalMode_CENTERED, Interval(0,1));

f1.eval(Interval(0,1));
% which is equivalent to: f1.eval(EvalMode_NATURAL | EvalMode_CENTERED, Interval(0,1)) in Python
% [6-end]


% [7-beg]
x1 = ScalarVar();
f1 = AnalyticFunction({x1}, x1*cos(x1));
J1 = f1.diff(Interval(0,PI/2));
% J1 = intv. matrix 1x1: [[ [-(PI/2),1] ]]

x2 = ScalarVar();
f2 = AnalyticFunction({x1,x2}, x1^x2); % example of multivariate function
J2 = f2.diff(2.,Interval(2,3));
% J2 = intv. matrix 1x2: [[ [4,12], [2.77258,5.54518] ]]

v = VectorVar(3);
f3 = AnalyticFunction({v}, vec( ... % vectorial function
  v(1)-(v(2)^2), ...
  Interval(-1,0)*v(3) ...
));
J3 = f3.diff(Vector({5,8,10}));
% J3 = intv. matrix 2x3: [[ 1,-16,0 ],[ 0,0,[-1,0] ]]
% [7-end]


f = f3
% [8-beg]
n = f.input_size();
m = f.output_size();
% [8-end]


% [9-beg]
% Forward evaluation
y = CosOp().fwd(Interval(0,PI/2)); % y = [0,1]

% Backward evaluation
x = Interval(0,PI); % prior value of [x]
CosOp().bwd(Interval(0,0.5), x); % [x] is contracted to [PI/3,PI/2]
% [9-end]
