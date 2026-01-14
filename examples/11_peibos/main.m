import py.codac4matlab.*

currentFilePath = mfilename('fullpath');
[currentDir, ~, ~] = fileparts(currentFilePath);
cd(currentDir);

%% 
% 2D example of the PEIBOS algorithm

y_2d=VectorVar(2);
a=1.4;
b=0.3;

f_2d = AnalyticFunction({y_2d},vec(y_2d(2)+1-a*sqr(y_2d(1)),b*y_2d(1)));

X_2d = VectorVar(1);
psi0_2d = AnalyticFunction({X_2d},vec(sin(X_2d(1)*PI/4.),cos(X_2d(1)*PI/4.)));

id_2d = OctaSym(int64([1,2]));
s = OctaSym(int64([-2,1]));

v_par_2d = PEIBOS(f_2d,psi0_2d,{id_2d,s,s*s,s.invert()},0.2,Vector([-0.2,0.]),true);

figure_2d = Figure2D("Henon Map",GraphicOutput().VIBES);
figure_2d.set_window_properties(Vector([25,50]),Vector([500,500]));
figure_2d.set_axes(axis(1,Interval([-1.4,2.2])), axis(2,Interval([-0.4,0.3])));

for i = 1:length(v_par_2d)
    p = v_par_2d{i};
    figure_2d.draw_parallelepiped(p, StyleProperties({Color().green(),Color().green(0.5)}));
    figure_2d.draw_box(p.box(), StyleProperties(Color().blue()));
    vertices = p.vertices();
    for j = 1:length(vertices)
        vertice = vertices{j};  % Access Python list element
        figure_2d.draw_point(vertice, StyleProperties({Color().red(),Color().red(0.5)}));
    end
end

%% 
%  3D example of the PEIBOS algorithm

y_3d = VectorVar(3);
f_3d = AnalyticFunction({y_3d},vec(sqr(y_3d(1))-sqr(y_3d(2))+y_3d(1),2*y_3d(1)*y_3d(2)+y_3d(2),y_3d(3)));

X_3d = VectorVar(2);
psi0_3d = AnalyticFunction({X_3d},vec(1/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2))),X_3d(1)/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2))),X_3d(2)/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2)))));

id_3d = OctaSym(int64([1,2,3]));
s1 = OctaSym(int64([-2, 1, 3]));
s2 = OctaSym(int64([3, 2, -1]));

v_par_3d = PEIBOS(f_3d,psi0_3d,{id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()},0.2,true);

figure_3d = Figure3D("Conform");
figure_3d.draw_axes();

figure_3d_proj = Figure2D("Conformd projected",GraphicOutput().VIBES);
figure_3d_proj.set_window_properties(Vector([25,600]),Vector([500,500]));
figure_3d_proj.set_axes(axis(1,Interval([-1.5,2.5])), axis(2,Interval([-2.0,2.0])));

for i = 1:length(v_par_3d)
    p = v_par_3d{i};
    figure_3d.draw_parallelepiped(p, StyleProperties(Color().green(0.5)));
    figure_3d_proj.draw_zonotope(p.proj(int64([1,2])), StyleProperties({Color().black(),Color().green(0.5)}));
end

%% 
% nD example of the PEIBOS algorithm

y_nd = VectorVar(3);
rot_matrix_1 = Matrix([[1,0,0; 0,1/sqrt(2.0).mid(),-1/sqrt(2.0).mid(); 0,1/sqrt(2.0).mid(),+1/sqrt(2.0).mid()]]);
rot_matrix_2 = Matrix([[1/sqrt(2.0).mid(),-1/sqrt(2.0).mid(),0; 1/sqrt(2.0).mid(),1/sqrt(2.0).mid(),0; 0,0,1]]);
g_nd = AnalyticFunction({y_nd}, vec(y_nd(1)/sqrt(sqr(y_nd(1))+sqr(y_nd(2))+sqr(y_nd(3))), y_nd(2)/sqrt(sqr(y_nd(1))+sqr(y_nd(2))+sqr(y_nd(3))), y_nd(3)/sqrt(sqr(y_nd(1))+sqr(y_nd(2))+sqr(y_nd(3)))));
f_nd = AnalyticFunction({y_nd}, rot_matrix_1 * rot_matrix_2 * g_nd(y_nd));

X_nd = VectorVar(1);
psi0_nd = AnalyticFunction({X_nd},vec(X_nd(1),1,1));

id_nd = OctaSym(int64([1, 2, 3]));
s1_nd = OctaSym(int64([-2, 1, 3]));
s2_nd = OctaSym(int64([3, 2, -1]));

figure_3d_nd = Figure3D("Cube on Sphere matlab");
figure_3d_nd.draw_axes(0.5);

figure_2d_nd_xy = Figure2D("XY Plane",GraphicOutput().VIBES);
figure_2d_nd_xy.set_window_properties(Vector([575,50]),Vector([500,500]));
figure_2d_nd_xy.set_axes(axis(1,Interval([-1.,1.])), axis(2,Interval([-1.,1.])));

figure_2d_nd_zy = Figure2D("ZY Plane",GraphicOutput().VIBES);
figure_2d_nd_zy.set_window_properties(Vector([1125,50]),Vector([500,500]));
figure_2d_nd_zy.set_axes(axis(1,Interval([-1.,1.])), axis(2,Interval([-1.,1.])));

v_par_nd = PEIBOS(f_nd,psi0_nd,{id_nd,s1_nd,s1_nd*s1_nd,s1_nd.invert(), s2_nd,s2_nd*s1_nd,s2_nd*s1_nd*s1_nd,s2_nd*s1_nd.invert(),s2_nd*s2_nd,s2_nd*s2_nd*s1_nd,s2_nd*s2_nd*s1_nd*s1_nd,s2_nd*s2_nd*s1_nd.invert()},0.1,true);

for i = 1:length(v_par_nd)
    p = v_par_nd{i};
    figure_3d_nd.draw_parallelepiped(p, StyleProperties(Color().green(0.5)));
    figure_2d_nd_xy.draw_zonotope(p.proj(int64([1,2])), StyleProperties({Color().black(),Color().green(0.2)}));
    figure_2d_nd_zy.draw_zonotope(p.proj(int64([2,3])), StyleProperties({Color().black(),Color().green(0.2)}));
end
