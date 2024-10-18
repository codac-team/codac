% Codac - Examples
% Dynamic range-bearing localization
% ----------------------------------------------------------------------------

import py.codac.*

% =========== CREATING DATA ===========

dt = 0.05;
iteration_dt = 0.2;
tdomain = Interval(0,15); % [t0,tf]

% Initial pose x0=(0,0,2)
x0 = [0, 0, 2];

% System input
u = Trajectory(tdomain, TFunction('3*(sin(t)^2)+t/100'), dt);

% Noise
i_n = Interval(-0.03,0.03); % the noises are known to be bounded by i_n

n_u = RandTrajectory(tdomain, dt, i_n); % input noise
n_theta = RandTrajectory(tdomain, dt, i_n); % heading noise

% Actual trajectories (state + derivative)
v_truth = TrajectoryVector(int32(3));
x_truth = TrajectoryVector(int32(3));
v_truth.setitem(int32(2), u + n_u);
x_truth.setitem(int32(2), v_truth.getitem(int32(2)).primitive() + x0(3));
v_truth.setitem(int32(0), 10*cos(x_truth.getitem(int32(2))));
v_truth.setitem(int32(1), 10*sin(x_truth.getitem(int32(2))));
x_truth.setitem(int32(0), v_truth.getitem(int32(0)).primitive() + x0(1));
x_truth.setitem(int32(1), v_truth.getitem(int32(1)).primitive() + x0(2));

% Bounded trajectories (dead reckoning)
v = TubeVector(tdomain, dt, int32(3));
x = TubeVector(tdomain, dt, int32(3));
v.setitem(int32(2), Tube(u, dt).inflate(i_n.rad())); % command u with bounded uncertainties
x.setitem(int32(2), Tube(x_truth.getitem(int32(2))+n_theta, dt).inflate(i_n.rad())); % heading measurement with bounded uncertainties
v.setitem(int32(0), 10*cos(x.getitem(int32(2))));
v.setitem(int32(1), 10*sin(x.getitem(int32(2))));
x = v.primitive()+IntervalVector(x0); % dead reckoning

% Set of landmarks
v_m = { py.list([6,12]), py.list([-2,-5]), py.list([-3,20]), py.list([3,4]) };

% =========== GRAPHICS ===========

beginDrawing();

fig_map = VIBesFigMap('slam');
fig_map.set_properties(int32(50), int32(50), int32(1200), int32(600));
fig_map.add_tube(x, 'x', int32(0), int32(1));
fig_map.add_trajectory(x_truth, 'truth', int32(0), int32(1), 'white');
fig_map.smooth_tube_drawing(true);
fig_map.add_landmarks(py.list(v_m), single(0.4));
fig_map.show(double(1));

% =========== CONTRACTOR NETWORK ===========

v_m_boxes = cell(size(v_m));
for i=1:length(v_m)
  v_m_boxes(i) = {IntervalVector(int32(2))};
end

% Contractor Network:

cn = ContractorNetwork();

t = tdomain.lb();
prev_t_obs = t;

while t < tdomain.ub()

  if t-prev_t_obs > 2*dt % new observation each 2*delta
    
    % Creating new observation to a random landmark

    landmark_id = randi([1 length(v_m)]); % a random landmark is perceived

    xt = double(x_truth(t));
    pos_x = [xt(1), xt(2)];
    pos_b = double(v_m{landmark_id});

    yi = Interval(sqrt((pos_x(1)-pos_b(1))^2+(pos_x(2)-pos_b(2))^2));
    yi.inflate(0.03); % adding range bounded uncertainty

    prev_t_obs = t;

    % Adding related observation constraints to the network

    % Alias (for ease of reading)
    b = v_m_boxes{landmark_id};

    % Intermediate variables
    ti = Interval(t);
    xi = IntervalVector(int32(3));

    % Contractors
    cn.add(CtcEval(), py.list({ti, xi, x, v}));
    cn.add(CtcDist(), py.list({xi.getitem(int32(0)), xi.getitem(int32(1)), b.getitem(int32(0)), b.getitem(int32(1)), yi}));

  end
  
  contraction_dt = cn.contract_during(iteration_dt);  
  if iteration_dt>contraction_dt
    pause(iteration_dt-contraction_dt); % iteration delay
  end

  % Display the current slice x
  fig_map.draw_box(x(t).subvector(int32(0),int32(1)));

  t = t + dt;

end

cn.contract(true); % lets the solver run the remaining contractions

fig_map.show();
for i=1:length(v_m_boxes)
  b = v_m_boxes{i};
  fig_map.draw_box(b);
end

endDrawing();
