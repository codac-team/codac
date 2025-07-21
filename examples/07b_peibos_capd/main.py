from codac import *
# 2D example of the PEIBOS algorithm

vectorField_2d_wrap = IMapWrapper ("var:x1,x2;fun:x2,(1-sqr(x1))*x2-x1;")
tf_2d = 2.0

X_2d = VectorVar(1)
psi0_2d = AnalyticFunction([X_2d],[cos(X_2d[0]*PI/4.-PI/2),sin(X_2d[0]*PI/4.-PI/2)])

generators_2d = [[1, 2], [-2, 1]]

v_par_2d = PEIBOS (vectorField_2d_wrap, tf_2d, psi0_2d, generate_symmetries(generators_2d, psi0_2d), 0.05, True)

output = Figure2D("Van der Pol", GraphicOutput.VIBES)
output.set_window_properties([50,100],[800,800])
output.set_axes(axis(0,[-3,3]), axis(1,[-3,3]))

for p in v_par_2d:
    output.draw_parallelepiped(p.z, p.A, [Color.green(), Color.green(0.5)])

# 3D example of the PEIBOS algorithm

vectorField_3d_wrap = IMapWrapper ("par:sigma,rho,beta;var:x1,x2,x3;fun:sigma*(x2-x1),rho*x1-x2-x1*x3,-beta*x3+x1*x2;")
vectorField_3d_wrap.setParameter("sigma",10.)
vectorField_3d_wrap.setParameter("rho",28.)
vectorField_3d_wrap.setParameter("beta",8./3.)

tf_3d = 0.05

X_3d = VectorVar(2)
psi0_3d = AnalyticFunction([X_3d],[1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))])

generators_3d = [[1, 2, 3], [-2, 1, 3], [3, 2, -1]]

v_par_3d = PEIBOS (vectorField_3d_wrap, tf_3d, psi0_3d, generate_symmetries(generators_3d, psi0_3d), 0.2, True)

figure_3d_lorenz = Figure3D("Lorenz")

for p in v_par_3d:
    figure_3d_lorenz.draw_parallelepiped(p.z, p.A, Color.green(0.5))

# Multiple discrete times

vectorField_discrete_wrap = IMapWrapper ("par:l,g;var:t,w;fun:w,-sin(t)*g/l - 0.5*w;")
vectorField_discrete_wrap.setParameter("l",Interval(2.))
vectorField_discrete_wrap.setParameter("g",Interval(10.))

tf_discrete = 10.
dt_discrete = tf_discrete / 200.

X_discrete = VectorVar(1)
psi0_discrete = AnalyticFunction([X_discrete],[0.01*cos(X_discrete[0]*PI/4.0),0.01*sin(X_discrete[0]*PI/4.0)])

generators_discrete = [[1, 2], [-2, 1]]

v_par_discrete = PEIBOS (vectorField_discrete_wrap, tf_discrete, dt_discrete, psi0_discrete, generate_symmetries(generators_discrete, psi0_discrete), 0.2, [-PI/2.0,0.0], True)

output_discrete = Figure2D("Pendulum", GraphicOutput.VIBES | GraphicOutput.IPE)
output_discrete.set_window_properties([1000,100],[800,800])
output_discrete.set_axes(axis(0,[-2,1.5]), axis(1,[-2,3]))

for t, v_par in v_par_discrete.items():
    for p in v_par:
        output_discrete.draw_parallelepiped(p.z, p.A, [ColorMap.rainbow().color(t / tf_discrete), ColorMap.rainbow_05().color(t / tf_discrete)])