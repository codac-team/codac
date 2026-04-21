clear all;

import py.codac4matlab.*

% [C-q4-beg]
function obs = g(t,x,M)
    obs = {}; % several landmarks can be seen at some ti
    scope_range = py.codac4matlab.Interval(0,10);
    scope_angle = py.codac4matlab.Interval(-py.codac4matlab.PI/4,py.codac4matlab.PI/4);

    for i = 1:numel(M)
        mi = M{i};
        r = py.codac4matlab.sqrt(py.codac4matlab.sqr(mi(1)-x(1)) + py.codac4matlab.sqr(mi(2)-x(2)));
        a = py.codac4matlab.atan2(mi(2)-x(2),mi(1)-x(1)) - x(3);

        % If the landmark is seen by the robot:
        if scope_range.is_superset(r) && scope_angle.is_superset(a)
            obs{end+1} =py.codac4matlab.cart_prod(t,r,a);
        end
    end
end
% [C-q4-end]

% [C-q2-beg]
srand(); % initialize the random seed (from C++)
N = 50; % number of landmarks
X = IntervalVector({{-40,40},{-40,40}}); % landmarks distribution zone

M={}; % creating the landmarks
for i = 1:N
    M{i} = IntervalVector(X.rand()).inflate(0.2);
end

fig = Figure2D("Robot simulation", GraphicOutput().VIBES);
fig.set_axes(axis(1, X(1).inflate(10),"x_1"),axis(2, X(2).inflate(10),"x_2")).auto_scale();

for i = 1:numel(M) % displaying the landmarks
  fig.draw_box(M{i}, StyleProperties({Color().dark_green(),Color().green()}))
end
% [C-q2-end]


% [C-q3-beg]
wpts={}; % creating random waypoints for simulating the robot trajectory
X = IntervalVector({{-35,35},{-35,35}}); % robot evolution zone
for i = 1:5 % 5 waypoints
  wpts{i} = X.rand();
end

s = RobotSimulator();
s.w_max = 0.2; % maximum turning speed
u = SampledTraj_Vector(); % the simulator will return the inputs (not used)
x_truth = s.simulate(Vector({0,0,0,0}), 0.01, wpts, u); % initial state (will be supposed unknown) and simulation time step
% [C-q3-end]


% [C-q5-beg]
prev_t = 0.;
time_between_obs = 3.;
obs = {};

t = double(x_truth.tdomain().lb());

tend = x_truth.tdomain().ub();
while t < tend
    if t-prev_t > time_between_obs
        x_truth_t = x_truth(t);
        obs_ti = g(t,x_truth_t,M); % computing the observation vector

        for i = 1:numel(obs_ti)
            yi = obs_ti{i};
            prev_t = yi(1).mid();
            fig.draw_pie(x_truth_t.subvector(1,2), yi(2).union(0), x_truth_t(3)+yi(3), Color().light_gray());
            fig.draw_pie(x_truth_t.subvector(1,2), yi(2),          x_truth_t(3)+yi(3), Color().red());

            obs{end+1} = yi;
        end
    end
    t = t + 0.01; % for performance, it is advised to increment by steps of 0.1 instead
end
% [C-q5-end]


% [C-q6-beg]
x_ = VectorVar(4);
% Positions are not known, but headings (x3) and velocities (x4) are bounded..
h = AnalyticFunction({x_},vec(Interval(-oo,oo),Interval(-oo,oo), x_(3) + 0.02*Interval(-1,1), x_(4) + 0.02*Interval(-1,1)));

tdomain = create_tdomain(x_truth.tdomain(),0.05, true);
% The tube x is created from the interval evaluation of the actual trajectory
x = h.tube_eval(SlicedTube(tdomain,x_truth));
% The tube v is created as a four-dimensional tube of infinite values
v = SlicedTube(tdomain, IntervalVector(4));
% [C-q6-end]



[x1,x2,x3] = deal(VectorVar(2), VectorVar(4), VectorVar(2));
f_minus = AnalyticFunction({x1,x2,x3},vec(x1(1)-x2(1)-x3(1), x1(2)-x2(2)-x3(2)));
ctc_minus = CtcInverse(f_minus, Vector([0,0]));

[s1,s2,s3] = deal(ScalarVar(),ScalarVar(),ScalarVar());
f_plus = AnalyticFunction({s1,s2,s3}, s1+s2-s3);
ctc_plus = CtcInverse(f_plus, Interval(0,0));

% [C-q7-beg]
ctc_deriv = CtcDeriv();

[x_f,v_f] = deal(VectorVar(4),VectorVar(4));
f = AnalyticFunction({x_f,v_f},vec(v_f(1)-x_f(4)*cos(x_f(3)), v_f(2)-x_f(4)*sin(x_f(3))));

ctc_f = CtcInverse(f, Vector([0,0]));
% + other contractors from previous lessons:
% ctc_plus, ctc_minus, ctc_polar, ctc_constell
% [C-q7-end]

ctc_polar = CtcPolar();
ctc_constell = MyCtc(M);

% [C-q8-beg]
function [xi,yi,mi,ai,si] = ctc_one_obs(xi,yi,mi,ai,si,ctc_plus,ctc_polar,ctc_minus,ctc_constell)

    res_ctc_minus = ctc_minus.contract(py.codac4matlab.cart_prod(mi,xi,si)); % The result is a 8D IntervalVector
    [mi,xi,si] = deal(res_ctc_minus.subvector(1,2),res_ctc_minus.subvector(3,6),res_ctc_minus.subvector(7,8));

    res_ctc_plus = ctc_plus.contract(py.codac4matlab.cart_prod(xi(3), yi(3), ai)); % The result is a 3D IntervalVector
    xi.set_item(3,res_ctc_plus(1));
    yi.set_item(3,res_ctc_plus(2));
    ai = res_ctc_plus(3);

    res_ctc_polar = ctc_polar.contract(py.codac4matlab.cart_prod(si(1),si(2),yi(2),ai)); % The result is a 4D IntervalVector
    si.set_item(1,res_ctc_polar(1));
    si.set_item(2,res_ctc_polar(2));
    yi.set_item(2,res_ctc_polar(3));
    ai = res_ctc_polar(4);

    mi = ctc_constell.contract(mi);
end

function [x,y,m,a,s] = fixpoint_ctc_one_obs(x,y,m,a,s,ctc_plus,ctc_polar,ctc_minus,ctc_constell)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        [x,y,m,a,s] = ctc_one_obs(x,y,m,a,s,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
        vol = 0.;
        [x_vol,y_vol,m_vol,a_vol,s_vol] = deal(x.volume(), y.volume(), m.volume(), a.volume(), s.volume());
        if x_vol~=inf
            vol = vol + x_vol;
        end
        if y_vol~=inf
            vol = vol + y_vol;
        end
        if m_vol~=inf
            vol = vol + m_vol;
        end
        if a_vol~=inf
            vol = vol + a_vol;
        end
        if s_vol~=inf
            vol = vol + s_vol;
        end
        if x.is_empty() || y.is_empty() || m.is_empty() || a.is_empty() || s.is_empty()
            break
        end
    end
end

function [x,v] = ctc_all_obs(x,v,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell,ctc_f,ctc_deriv)
    for i = 1:numel(obs)
        yi = obs{i};
        xi = x(yi(1));
        ai = py.codac4matlab.Interval();
        si = py.codac4matlab.IntervalVector(2);
        mi = py.codac4matlab.IntervalVector(2);
        [xi,yi,mi,ai,si] = fixpoint_ctc_one_obs(xi,yi,mi,ai,si,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
        x.set(xi,yi(1));
    end

    res_ctc_f = ctc_f.contract(x,v);
    x = res_ctc_f{1};
    v = res_ctc_f{2};

    res_ctc_deriv = ctc_deriv.contract(x,v);
    x = res_ctc_deriv{1};
    v = res_ctc_deriv{2};

end

function x = fixpoint_ctc_all_obs(x,v,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell,ctc_f,ctc_deriv)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        [x,v] = ctc_all_obs(x,v,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell,ctc_f,ctc_deriv);
        vol = x.volume();
        if x.is_empty()
            break
        end
    end
end

fixpoint_ctc_all_obs(x,v,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell,ctc_f,ctc_deriv);
 %[C-q8-end]

x

% [C-q9-beg]
fig.draw_tube(x);
fig.draw_trajectory(x_truth);
xf = x_truth(tend);
fig.draw_tank(xf, 2., StyleProperties({Color().black(),Color().yellow()}));
fig.draw_pie(xf, Interval(0,10), xf(3)+Interval(-PI/4.,PI/4), Color().dark_gray());
% [C-q9-end]