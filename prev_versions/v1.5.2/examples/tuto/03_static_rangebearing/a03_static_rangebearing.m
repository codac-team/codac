% Codac - Examples
% Static range-bearing localization
% ----------------------------------------------------------------------------

import py.codac.*

% =================== 0. Parameters, truth and data ====================

% Truth (unknown pose)
x_truth = [0,0,pi/6]; % (x,y,heading)

% Creating random map of landmarks
map_area = IntervalVector(int32(2), [-8,8]);
v_map = DataLoader().generate_landmarks_boxes(map_area, int32(1));

% The following function generates a set of [range]x[bearing] values
v_obs = DataLoader().generate_static_observations(py.list(x_truth), v_map, false);

% Adding uncertainties on the measurements
for i=1:length(v_obs) % for each observation:
  v_obs{i}.getitem(int32(0)).inflate(0.3); % range
  v_obs{i}.getitem(int32(1)).inflate(0.1); % bearing
end


% =============== 1. Defining domains for our variables ================

x = IntervalVector(int32(2)); % unknown position
heading = Interval(x_truth(3)).inflate(0.01); % measured heading


% =========== 2. Defining contractors to deal with equations ===========

ctc_plus = CtcFunction(Function('a', 'b', 'c', 'a+b-c')); % a+b=c
ctc_minus = CtcFunction(Function('a', 'b', 'c', 'a-b-c')); % a-b=c
% We also use the predefined contractor CtcPolar(), no need to build it


% =============== 3. Adding the contractors to a network ===============

cn = ContractorNetwork();

for i=1:length(v_obs)

  % Intermediate variables
  alpha = cn.create_interm_var(Interval());
  d = cn.create_interm_var(IntervalVector(int32(2)));

  cn.add(ctc_plus, py.list({v_obs{i}.getitem(int32(1)), heading, alpha}));
  cn.add(ctc_minus, py.list({v_map{i}, x, d}));
  cn.add(CtcPolar(), py.list({d, v_obs{i}.getitem(int32(0)), alpha}));
end


% ======================= 4. Solving the problem =======================

cn.contract();


% ============================ 5. Graphics =============================

beginDrawing();

fig = VIBesFigMap('Map');
fig.set_properties(int32(50), int32(50), int32(600), int32(600));

for i=1:length(v_map)
  iv = v_map{i};
  fig.add_beacon(iv.mid(), 0.2);
end

for i=1:length(v_obs)
  y = v_obs{i};
  fig.draw_pie(x_truth(1), x_truth(2), y.getitem(int32(0)).union(Interval(0)), heading+y.getitem(int32(1)), 'lightGray');
  fig.draw_pie(x_truth(1), x_truth(2), y.getitem(int32(0)), heading+y.getitem(int32(1)), 'gray');
end

fig.draw_vehicle(py.list(x_truth),0.5);
fig.draw_box(x); % estimated position
fig.show();

endDrawing();


% Checking if this example still works:
assert(x.contains(py.list(x_truth(1:2))))
