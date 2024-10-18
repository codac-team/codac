% Codac - Examples
% Dynamic range-bearing localization
% ----------------------------------------------------------------------------

import py.codac.*


% =================== 0. Parameters, truth and data ====================

dt = 0.05;                                      % timestep for tubes accuracy
tdomain = Interval(0,3);                        % temporal limits [t_0,t_f]=[0,3]

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

% Creating random map of landmarks
map_area = IntervalVector(int32(2), [-8,8]);
v_map = DataLoader().generate_landmarks_boxes(map_area, int32(30));

% The following function generates a set of [range]x[bearing] values
v_obs = DataLoader().generate_observations(x_truth, v_map, int32(10));

% Adding uncertainties on the measurements
for i=1:length(v_obs) % for each observation:
  obs = v_obs{i};
  obs.getitem(int32(1)).inflate(0.3); % range
  obs.getitem(int32(2)).inflate(0.1); % bearing
end

% =============== 1. Defining domains for our variables ================

x = TubeVector(tdomain, dt, int32(4));                    % 4d tube for state vectors
v = TubeVector(tdomain, dt, int32(4));                    % 4d tube for derivatives of the states
u = TubeVector(tdomain, dt, int32(2));                    % 2d tube for inputs of the system

x.setitem(int32(2), Tube(measured_psi, dt).inflate(0.01));       % measured_psi is a set of measurements
x.setitem(int32(3), Tube(measured_speed, dt).inflate(0.01));


% =========== 2. Defining contractors to deal with equations ===========

ctc_f = CtcFunction(Function('v[4]', 'x[4]', 'u[2]', ...
                             '(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])'));
ctc_plus = CtcFunction(Function('a', 'b', 'c', 'a+b-c')); % a+b=c
ctc_minus = CtcFunction(Function('a', 'b', 'c', 'a-b-c')); % a-b=c
% We also use the predefined contractors CtcPolar(), CtcEval(), no need to build them


% =============== 3. Adding the contractors to a network ===============

cn = ContractorNetwork();   % creating a network
cn.add(ctc_f, py.list({v, x, u}));   % adding the f constraint

for i=1:length(v_obs) % we add the observ. constraint for each range-only measurement
  y = v_obs{i};

  % Intermediate variables
  alpha = cn.create_interm_var(Interval()); % absolute angle robot-landmark
  d = cn.create_interm_var(IntervalVector(int32(2))); % dist robot-landmark
  p = cn.create_interm_var(IntervalVector(int32(4))); % state at t_i

  cn.add(ctc_plus, py.list({y.getitem(int32(2)), p.getitem(int32(2)), alpha}));
  cn.add(ctc_minus, py.list({cn.subvector(y,int32(3),int32(4)), cn.subvector(p,int32(0),int32(1)), d}));
  cn.add(CtcPolar(), py.list({d, y.getitem(int32(1)), alpha}));
  cn.add(CtcEval(), py.list({y.getitem(int32(0)), p, x, v}));
end


% ======================= 4. Solving the problem =======================

cn.contract(true);


% ============================ 5. Graphics =============================

beginDrawing();
fig = VIBesFigMap('fig');
fig.set_properties(int32(50), int32(50), int32(900), int32(550));
fig.add_trajectory(x_truth, 'xtruth', int32(0), int32(1), int32(2));
fig.add_tube(x, 'x', int32(0), int32(1));
fig.smooth_tube_drawing(true);

for i=1:length(v_map)
  b = v_map{i};
  fig.add_beacon(b.mid(), 0.2); % drawing beacons
end

for i=1:length(v_obs)
  y = v_obs{i};
  t_obs = y.getitem(int32(0)).mid();
  t_state = x_truth(t_obs);
  fig.draw_pie(t_state{1}, t_state{2}, y.getitem(int32(1)).union(Interval(0.01)), t_state{3} + y.getitem(int32(2)), 'lightGray'); % drawing range-bearing measurements
  fig.draw_pie(t_state{1}, t_state{2}, y.getitem(int32(1)), t_state{3} + y.getitem(int32(2)), 'darkGray'); % drawing range-bearing measurements
  fig.draw_vehicle(t_obs, x_truth, 0.7);
end

fig.show(double(0));
endDrawing();


% Checking if this example still works:
assert(x.contains(x_truth) == py.codac.core.BoolInterval(int32(2)));
