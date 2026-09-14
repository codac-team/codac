import py.codac4matlab.*

% [B-q3-beg]
M = {IntervalVector([1.5,2.5]),IntervalVector([3,1]), IntervalVector([2,2]), IntervalVector([2.5,3]), IntervalVector([3.5,2]), IntervalVector([4,1]), IntervalVector([1.5,0.5])};

for i = 1:numel(M)
    M{i}.inflate(0.05);
end

a1 = IntervalVector({{1.25,3.},{1.6,2.75}});
a2 = IntervalVector({{2,3.5},{0.6,1.2}});
a3 = IntervalVector({{1.1,3.25},{0.2,1.4}});

ctc_constell = MyCtc(M);

a1 = ctc_constell.contract(a1)
a2 = ctc_constell.contract(a2)
a3 = ctc_constell.contract(a3)
% [B-q3-end]

% [B-q4-beg]
x_truth = Vector([2,1,PI/6]);
% [B-q4-end]

% [B-q5-beg]
DefaultFigure().draw_tank(x_truth, 0.4, StyleProperties({Color().black(),Color().yellow()}))
for i = 1:numel(M)
  DefaultFigure().draw_box(M{i}, StyleProperties({Color().dark_green(),Color().green()}))
end

DefaultFigure().set_axes(axis(1,Interval([1.,4.5])), axis(2,Interval([0.,3.5])));
% [B-q5-end]

% [B-q6-beg]
function p = g(x, mi)
    r = py.codac4matlab.sqrt(py.codac4matlab.sqr(mi(1)-x(1))+py.codac4matlab.sqr(mi(2)-x(2)));
    b = py.codac4matlab.atan2(mi(2)-x(2),mi(1)-x(1)) - x(3);
    p = py.codac4matlab.IntervalVector({r,b}).inflate(0.02);
end
% [B-q6-end]

% [B-q7-beg]
obs = {};
for i = 1:numel(M)
    obs{end+1} = cart_prod(g(x_truth, M{i}), M{i});
    % We append the position of the landmark to the measurement
    % yi = {range}×{bearing}×{2d position}
end

for i = 1:numel(obs)
    DefaultFigure().draw_pie(x_truth.subvector(1,2),obs{i}(1),         x_truth(3)+obs{i}(2),Color().red());
    DefaultFigure().draw_pie(x_truth.subvector(1,2),obs{i}(1).union(0),x_truth(3)+obs{i}(2),Color().gray());
end
% [B-q7-end]

ctc_polar = CtcPolar();

[x1,x2,x3] = deal(VectorVar(2), VectorVar(3), VectorVar(2));
f_minus = AnalyticFunction({x1,x2,x3},vec(x1(1)-x2(1)-x3(1), x1(2)-x2(2)-x3(2)));
ctc_minus = CtcInverse(f_minus, Vector([0,0]));

[s1,s2,s3] = deal(ScalarVar(),ScalarVar(),ScalarVar());
f_plus = AnalyticFunction({s1,s2,s3}, s1+s2-s3);
ctc_plus = CtcInverse(f_plus, Interval(0,0));

% [B-q7b-beg]
function [x,yi,mi,ai,di] = ctc_one_obs(x,yi,mi,ai,di,ctc_plus,ctc_polar,ctc_minus)
    res_ctc_minus = ctc_minus.contract(py.codac4matlab.cart_prod(mi,x,di)); % The result is a 7D IntervalVector
    [mi,x,di] = deal(res_ctc_minus.subvector(1,2),res_ctc_minus.subvector(3,5),res_ctc_minus.subvector(6,7));

    res_ctc_plus = ctc_plus.contract(py.codac4matlab.cart_prod(x(3), yi(2), ai)); % The result is a 3D IntervalVector
    x.set_item(3,res_ctc_plus(1));
    yi.set_item(2,res_ctc_plus(2));
    ai = res_ctc_plus(3);
    
    res_ctc_polar = ctc_polar.contract(py.codac4matlab.cart_prod(di(1),di(2),yi(1),ai)); % The result is a 4D IntervalVector
    di.set_item(1,res_ctc_polar(1));
    di.set_item(2,res_ctc_polar(2));
    yi.set_item(1,res_ctc_polar(3));
    ai = res_ctc_polar(4);
end

function [x,y,m,a,d] = fixpoint_ctc_one_obs(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        [x,y,m,a,d] = ctc_one_obs(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus);
        vol = x.volume() + y.volume() + m.volume() + a.volume() + d.volume();
    end
end

function x = ctc_all_obs(x,obs,ctc_plus,ctc_polar,ctc_minus)
    for i = 1:numel(obs)
        yi = obs{i};
        ai = py.codac4matlab.Interval();
        di = py.codac4matlab.IntervalVector(2);
        mi = yi.subvector(3,4);
        [x,yi,mi,ai,di] = fixpoint_ctc_one_obs(x,yi,mi,ai,di,ctc_plus,ctc_polar,ctc_minus);
    end
end

function x = fixpoint_ctc_all_obs(x,obs,ctc_plus,ctc_polar,ctc_minus)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        x = ctc_all_obs(x,obs,ctc_plus,ctc_polar,ctc_minus);
        vol = x.volume();
    end
end

x = cart_prod(IntervalVector(2),x_truth(3));
x = fixpoint_ctc_all_obs(x,obs,ctc_plus,ctc_polar,ctc_minus);
% [B-q7b-end]

% [B-q8-beg]
x
DefaultFigure().draw_box(x)
% [B-q8-end]

% [B-q9-beg]
function [x,yi,mi,ai,di] = ctc_one_obs_datasso(x,yi,mi,ai,di,ctc_plus,ctc_polar,ctc_minus,ctc_constell)
    res_ctc_minus = ctc_minus.contract(py.codac4matlab.cart_prod(mi,x,di)); % The result is a 7D IntervalVector
    [mi,x,di] = deal(res_ctc_minus.subvector(1,2),res_ctc_minus.subvector(3,5),res_ctc_minus.subvector(6,7));

    res_ctc_plus = ctc_plus.contract(py.codac4matlab.cart_prod(x(3), yi(2), ai)); % The result is a 3D IntervalVector
    x.set_item(3,res_ctc_plus(1));
    yi.set_item(2,res_ctc_plus(2));
    ai = res_ctc_plus(3);
    
    res_ctc_polar = ctc_polar.contract(py.codac4matlab.cart_prod(di(1),di(2),yi(1),ai)); % The result is a 4D IntervalVector
    di.set_item(1,res_ctc_polar(1));
    di.set_item(2,res_ctc_polar(2));
    yi.set_item(1,res_ctc_polar(3));
    ai = res_ctc_polar(4);
    % ==== Added contractor ====
    mi = ctc_constell.contract(mi);
    % ==========================
end

function [x,y,m,a,d] = fixpoint_ctc_one_obs_datasso(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus,ctc_constell)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        [x,y,m,a,d] = ctc_one_obs_datasso(x,y,m,a,d,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
        vol = x.volume() + y.volume() + m.volume() + a.volume() + d.volume();
    end
end

function x = ctc_all_obs_datasso(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell)
    for i = 1:numel(obs)
        yi = obs{i};
        ai = py.codac4matlab.Interval();
        di = py.codac4matlab.IntervalVector(2);
        % ==== Changed domain ====
        mi = py.codac4matlab.IntervalVector(2); % the identity (position) of the landmark is not known
        % ========================
        [x,yi,mi,ai,di] = fixpoint_ctc_one_obs_datasso(x,yi,mi,ai,di,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
    end
end

function x = fixpoint_ctc_all_obs_datasso(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        x = ctc_all_obs_datasso(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
        vol = x.volume();
    end
end

x = cart_prod(IntervalVector(2),x_truth(3));
x = fixpoint_ctc_all_obs_datasso(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
% [B-q9-end]

function x = ctc_all_obs_datasso_mi(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell)
    for i = 1:numel(obs)
        yi = obs{i};
        ai = py.codac4matlab.Interval();
        di = py.codac4matlab.IntervalVector(2);
        % ==== Changed domain ====
        mi = py.codac4matlab.IntervalVector(2); % the identity (position) of the landmark is not known
        % ========================
        [x,yi,mi,ai,di] = fixpoint_ctc_one_obs_datasso(x,yi,mi,ai,di,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
        % [B-q10-beg]
        if mi.max_diam() < 0.11
            py.codac4matlab.DefaultFigure().draw_point(mi.mid());
        % [B-q10-end]
        end
    end
end

function x = fixpoint_ctc_all_obs_datasso_mi(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell)
    vol = -1.;
    prev_vol = -2.;

    while vol ~= prev_vol
        prev_vol = vol;
        x = ctc_all_obs_datasso_mi(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
        vol = x.volume();
    end
end

x = cart_prod(IntervalVector(2),x_truth(3));
x = fixpoint_ctc_all_obs_datasso_mi(x,obs,ctc_plus,ctc_polar,ctc_minus,ctc_constell);
