% Codac - Examples
% Getting started: 2 minutes to Codac
% ----------------------------------------------------------------------------

import py.codac.*


% =================== 0. Parameters, truth and data ====================

dt = 0.01;                                % timestep for tubes accuracy
tdomain = Interval(0, 3);                 % temporal limits [t_0,t_f]=[0,3]

x = TubeVector(tdomain, dt, int32(4));    % 4d tube for state vectors
v = TubeVector(tdomain, dt, int32(4));    % 4d tube for derivatives of the states
u = TubeVector(tdomain, dt, int32(2));    % 2d tube for inputs of the system

x_truth = TrajectoryVector(tdomain, TFunction(['(' ...
  '10*cos(t)+t ;' ...
  '5*sin(2*t)+t ;' ...
  'atan2((10*cos(2*t)+1),(-10*sin(t)+1)) ;' ...
  'sqrt((-10*sin(t)+1)^2+(10*cos(2*t)+1)^2))'])); % actual trajectory

% Continuous measurements coming from the truth
measured_psi = x_truth.getitem(int32(2)).sample(dt).make_continuous();
measured_psi = measured_psi + RandTrajectory(tdomain, dt, Interval(-0.01,0.01)); % adding some noise
measured_speed = x_truth.getitem(int32(3)).sample(dt);
measured_speed = measured_speed + RandTrajectory(tdomain, dt, Interval(-0.01,0.01)); % adding some noise


% =============== 1. Defining domains for our variables ================

x.setitem(int32(2), Tube(measured_psi, dt).inflate(0.01));       % measured_psi is a set of measurements
x.setitem(int32(3), Tube(measured_speed, dt).inflate(0.01));

e_y = Interval(-0.1,0.1);
y = {Interval(1.9+e_y), Interval(3.6+e_y), ...
     Interval(2.8+e_y)};                                         % set of range-only observations
b = {[8,3],[0,5],[-2,1]};                                        % positions of the three 2d landmarks
t = [0.3, 1.5, 2.0];                                             % times of measurements


% =========== 2. Defining contractors to deal with equations ===========

ctc_f = CtcFunction(Function('v[4]', 'x[4]', 'u[2]', '(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])'));


% =============== 3. Adding the contractors to a network ===============

cn = ContractorNetwork();   % creating a network

cn.add(ctc_f, py.list({v, x, u}));   % adding the f constraint

for i=1:length(y) % we add the observ. constraint for each range-only measurement

  p = cn.create_interm_var(IntervalVector(int32(4))); % intermed. variable (state at t_i)

  % Distance constraint: relation between the state at t_i and the ith beacon position
  cn.add(CtcDist(), py.list({cn.subvector(p,int32(0),int32(1)), py.list(b{i}), y{i}}));

  % Eval constraint: relation between the state at t_i and all the states over [t_0,t_f]=
  cn.add(CtcEval(), py.list({t(i), p, x, v}));
end


% ======================= 4. Solving the problem =======================

cn.contract(true);


% ============================ 5. Graphics =============================

beginDrawing();
fig = VIBesFigMap('fig');
fig.set_properties(int32(50), int32(50), int32(900), int32(550));
fig.add_trajectory(x_truth, 'xtruth', int32(0), int32(1), 'white');
fig.add_tube(x, 'x', int32(0), int32(1));
fig.smooth_tube_drawing(true);

for i=1:3
  fig.add_beacon(py.list(b{i}), 0.2); % drawing beacons
  fig.draw_ring(b{i}(1), b{i}(2), y{i}, 'darkGray'); % drawing range-only measurements
  fig.draw_vehicle(t(i), x_truth, 0.7); % drawing robot position at t
end

fig.show(0);
endDrawing();


% Checking if this example still works:
assert(x.volume() < 5) % todo: x.contains(x_truth)
