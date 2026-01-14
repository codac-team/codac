import py.codac4matlab.*

currentFilePath = mfilename('fullpath');
[currentDir, ~, ~] = fileparts(currentFilePath);
cd(currentDir);

%% 
% 2D case

% [1-beg]
X_2d = ScalarVar();
f_2d = AnalyticFunction({X_2d},vec(X_2d,sqr(X_2d)));
% [1-end]

% [2-beg]
dx_2d = 0.2;
x0_2d = -2.0;

while x0_2d<2.0
    X0_2d = Interval(x0_2d,x0_2d+dx_2d);
    p = f_2d.parallelepiped_eval(X0_2d);
    DefaultFigure().draw_parallelepiped(p,StyleProperties(Color().dark_green()));
    
    x0_2d = x0_2d + dx_2d;
end
% [2-end]

DefaultFigure().set_axes(axis(1,Interval(-2.5,2.5)),axis(2,Interval(-0.5,4.5)));
DefaultFigure().set_window_properties(Vector([50,50]),Vector([500,500]))

%% 
% 3D case

% [3-beg]
X_3d = VectorVar(2);
f_3d = AnalyticFunction({X_3d},vec(X_3d(1),X_3d(2),sqr(X_3d(1))+sqr(X_3d(2))));
% [3-end]

% [4-beg]
fig_3d = Figure3D("3D");
dx_3d = 0.2;
x0_3d = -2.0;

while x0_3d<=2.0
    X0_3d = Interval(x0_3d,x0_3d+dx_3d);
    y0_3d = -2.0;
    
    while y0_3d<=2.0
        Y0_3d = Interval(y0_3d,y0_3d+dx_3d);
        b = f_3d.eval(IntervalVector({X0_3d,Y0_3d}));
        p = f_3d.parallelepiped_eval(IntervalVector({X0_3d,Y0_3d}));
        
        fig_3d.draw_box(b, StyleProperties(Color().blue(0.3),"box"));
        fig_3d.draw_parallelepiped(p, StyleProperties(Color().green(0.3),"parallelepiped"));

        y0_3d = y0_3d + dx_3d;
    end

    x0_3d = x0_3d + dx_3d;
end
% [4-end]

%% 
% Thick case

% [5-beg]
X_if = ScalarVar();
f_if = AnalyticFunction({X_if},vec(X_if,Interval(1.1,1.2)*sqr(X_if)));
% [5-end]

% [6-beg]
f_lb = AnalyticTraj(AnalyticFunction({X_if},1.1*sqr(X_if)),Interval(-2.0,2.0));
f_ub = AnalyticTraj(AnalyticFunction({X_if},1.2*sqr(X_if)),Interval(-2.0,2.0));

DefaultFigure().plot_trajectory(f_lb.sampled(0.01));
DefaultFigure().plot_trajectory(f_ub.sampled(0.01));
% [6-end]

% [7-beg]
dx_if = 0.1;
x0_if = -2.0;

while x0_if<=2.0
    X0_if = Interval(x0_if,x0_if+dx_if);
    p = f_if.parallelepiped_eval(X0_if);
    DefaultFigure().draw_parallelepiped(p,StyleProperties(Color().dark_green()));

    x0_if = x0_if + dx_if;
end
% [7-end]
