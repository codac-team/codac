import py.codac4matlab.*

currentFilePath = mfilename('fullpath');
[currentDir, ~, ~] = fileparts(currentFilePath);
cd(currentDir);

% 2D example of the PEIBOS algorithm

vectorField_2d_wrap = IMapWrapper ("var:x1,x2;fun:x2,(1-sqr(x1))*x2-x1;");
tf_2d = 2.0;

X_2d = VectorVar(1);
psi0_2d = AnalyticFunction({X_2d},vec(cos(X_2d(1)*PI/4.-PI/2),sin(X_2d(1)*PI/4.-PI/2)));

generators_2d = int64([1,2;-2,1]);

v_par_2d = PEIBOS (vectorField_2d_wrap, tf_2d, psi0_2d, generators_2d, 0.05, true);

output = Figure2D("Van der Pol",GraphicOutput().VIBES);
output.set_window_properties(Vector([25,50]),Vector([500,500]));
output.set_axes(axis(1,Interval([-3.,3.])), axis(2,Interval([-3.,3.])));

for i = 1:length(v_par_2d)
    par = v_par_2d{i};
    output.draw_parallelepiped(par.z, par.A, StyleProperties({Color().green(),Color().green(0.5)}));
end

% 3D example of the PEIBOS algorithm

vectorField_3d_wrap = IMapWrapper ("par:sigma,rho,beta;var:x1,x2,x3;fun:sigma*(x2-x1),rho*x1-x2-x1*x3,-beta*x3+x1*x2;");
vectorField_3d_wrap.setParameter("sigma",10.);
vectorField_3d_wrap.setParameter("rho", 28.);
vectorField_3d_wrap.setParameter("beta", 8/3);
tf_3d = 0.05;

X_3d = VectorVar(2);
psi0_3d = AnalyticFunction({X_3d},vec(1/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2))),X_3d(1)/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2))),X_3d(2)/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2)))));

generators_3d = int64([1, 2, 3; -2, 1, 3; 3, 2, -1]);

v_par_3d = PEIBOS (vectorField_3d_wrap, tf_3d, psi0_3d, generators_3d, 0.2, true);

figure_3d_lorenz = Figure3D ("Lorenz matlab");

for i = 1:length(v_par_3d)
    par = v_par_3d{i};
    figure_3d_lorenz.draw_parallelepiped(par.z, par.A, StyleProperties(Color().green(0.5)));
end

% Multiple discrete times

vectorField_discrete_wrap = IMapWrapper ("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;");
vectorField_discrete_wrap.setParameter("l", Interval(2.));
vectorField_discrete_wrap.setParameter("g", Interval(10.));

tf_discrete = 10.;
dt_discrete = tf_discrete / 200.;

X_discrete = VectorVar(1);
psi0_discrete = AnalyticFunction({X_discrete},vec(0.01*cos(X_discrete(1)*PI/4.0),0.01*sin(X_discrete(1)*PI/4.0)));

generators_discrete = int64([1,2;-2,1]);

v_par_discrete = PEIBOS (vectorField_discrete_wrap, tf_discrete, dt_discrete, psi0_discrete, generators_discrete, 0.2, Vector([-PI/2.0,0.0]), true);

output_discrete = Figure2D("Pendulum", GraphicOutput().VIBES);
output_discrete.set_window_properties(Vector([1000,100]), Vector([800,800]));
output_discrete.set_axes(axis(1, Interval([-2.0,1.5])), axis(2, Interval([-2.,3.])));

k = v_par_discrete.keys();

pyKeys = py.list(v_par_discrete.keys());
matKeys = cell(pyKeys);

pyValues = py.list(v_par_discrete.values());
matValues = cell(pyValues);

% If keys are strings, convert each to MATLAB char array
for i = 1:numel(matKeys)
    t = matKeys{i};
    v_par = matValues{i};
    for j = 1:length(v_par)
        p = v_par{j};
        output_discrete.draw_parallelepiped(p.z, p.A, StyleProperties({ColorMap().rainbow().color(t/tf_discrete),ColorMap().rainbow_05().color(t/tf_discrete)}));
    end
end
