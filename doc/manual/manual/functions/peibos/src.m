import py.codac4matlab.*

currentFilePath = mfilename('fullpath');
[currentDir, ~, ~] = fileparts(currentFilePath);
cd(currentDir);

%%

% [peibos-1-beg]
% Definition of f
y_2d=VectorVar(2);
a=1.4;
b=0.3;
f_2d = AnalyticFunction({y_2d},vec(y_2d(2)+1-a*sqr(y_2d(1)),b*y_2d(1)));

% Definition of the gnomonic atlas
X_2d = VectorVar(1);
psi0_2d = AnalyticFunction({X_2d},vec(sin(X_2d(1)*PI/4.),cos(X_2d(1)*PI/4.)));
id_2d = OctaSym(int64([1,2]));
s = OctaSym(int64([-2,1]));

% Call to PEIBOS
v_par_2d = PEIBOS(f_2d,psi0_2d,{id_2d,s,s*s,s.invert()},0.2);
% [peibos-1-end]

% [peibos-2-beg]
% Definition of f
y_3d = VectorVar(3);
f_3d = AnalyticFunction({y_3d},vec(sqr(y_3d(1))-sqr(y_3d(2))+y_3d(1),2*y_3d(1)*y_3d(2)+y_3d(2),y_3d(3)));

% Definition of the gnomonic atlas
X_3d = VectorVar(2);
psi0_3d = AnalyticFunction({X_3d},vec(1/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2))),X_3d(1)/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2))),X_3d(2)/sqrt(1+sqr(X_3d(1))+sqr(X_3d(2)))));
id_3d = OctaSym(int64([1,2,3]));
s1 = OctaSym(int64([-2, 1, 3]));
s2 = OctaSym(int64([3, 2, -1]));

% Call to PEIBOS
v_par_3d = PEIBOS(f_3d,psi0_3d,{id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()},0.2);
% [peibos-2-end]