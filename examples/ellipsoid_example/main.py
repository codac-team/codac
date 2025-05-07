from codac import *

# ----------------------------------------------------------
# linear and nonlinear mappings
# ----------------------------------------------------------

fig1 = Figure2D('Linear and nonlinear mappings', GraphicOutput.VIBES)
fig1.set_axes(axis(0, [-0.1, 1.3]), axis(1, [-1.2, 0.2]))
fig1.set_window_properties([0, 100], [500, 500])

# initial ellipsoid
mu = Vector([1., 0.])
G = Matrix([[0.05, 0.0],
          [0.,   0.05]])
e1 = Ellipsoid(mu, G)
fig1.draw_ellipsoid(e1, [Color.red(), Color.red(0.3)])
print('Initial ellipsoid e1 (red):', e1)

# discrete nonlinear pendulum system
x = VectorVar(2)
h = AnalyticFunction ([x], vec(x[0] + 0.1 * x[1], -0.2 * sin(x[0]) + 0.9 * x[1]))

# linear mapping
N = 10
e2 = Ellipsoid(e1)
for i in range(0,N):
    A = h.diff(e2.mu).mid()
    b = Vector(h.eval(e2.mu).mid() - A * e2.mu)
    e2 = unreliable_linear_mapping(e2, A, b)
    fig1.draw_ellipsoid(e2, [Color.green(), Color.green(0.3)])

print('\nLinear Mapping - Image ellipsoid e2 (green):', e2)

# nonlinear mapping
e3 = Ellipsoid(e1)
for i in range(0,N):
    e3 = nonlinear_mapping(e3, h)
    fig1.draw_ellipsoid(e3, [Color.blue(), Color.blue(0.3)])

print('\nNon Linear Mapping - Image ellipsoid e3 (blue):', e3)

# particle cloud (draw the evolution of 200 points in the ellipsoid)
Np = 200
for i in range(0,Np):
    x0 = e1.rand()
    fig1.draw_box(IntervalVector(x0).inflate(0.0001), [Color.black(), Color.black(0.3)])
    for j in range(0,N):
        x0 = h.eval(x0).mid()
        fig1.draw_box(IntervalVector(x0).inflate(0.0001), [Color.black(), Color.black(0.3)])


# ----------------------------------------------------------
# ellipsoid projections
# ----------------------------------------------------------

mu4 = Vector([1., 0., 0.])
G4 = Matrix([[1.,  0.5, 0.],
           [0.5, 2.,  0.2],
           [0.,  0.2, 3.]])
e4 = Ellipsoid(mu4, G4)

G5 = 0.7 * G4
e5 = Ellipsoid(mu4, G5)

G6 = Matrix([[2., 0.,  0.5],
           [0., 1.,  0.2],
           [0., 0.2, 3.]])
e6 = Ellipsoid(mu4, G6)

fig2 = Figure2D('Projected ellipsoid xy', GraphicOutput.VIBES)
fig3 = Figure2D('Projected ellipsoid yz', GraphicOutput.VIBES)
fig4 = Figure2D('Projected ellipsoid xz', GraphicOutput.VIBES)

fig2.set_window_properties([700, 100], [500, 500])
fig3.set_window_properties([1200, 100], [500, 500])
fig4.set_window_properties([0, 600], [500, 500])

fig2.set_axes(axis(0, [-3, 3]), axis(1, [-3, 3]))
fig3.set_axes(axis(1, [-3, 3]), axis(2, [-3, 3]))
fig4.set_axes(axis(0, [-3, 3]), axis(2, [-3, 3]))

fig2.draw_ellipsoid(e4, [Color.blue(), Color.blue(0.3)])
fig3.draw_ellipsoid(e4, [Color.blue(), Color.blue(0.3)])
fig4.draw_ellipsoid(e4, [Color.blue(), Color.blue(0.3)])

fig2.draw_ellipsoid(e5, [Color.red(), Color.red(0.3)])
fig3.draw_ellipsoid(e5, [Color.red(), Color.red(0.3)])
fig4.draw_ellipsoid(e5, [Color.red(), Color.red(0.3)])

fig2.draw_ellipsoid(e6, [Color.green(), Color.green(0.3)])
fig3.draw_ellipsoid(e6, [Color.green(), Color.green(0.3)])
fig4.draw_ellipsoid(e6, [Color.green(), Color.green(0.3)])

# particle cloud (draw the evolution of 200 points in the ellipsoid e5)
for i in range(0,Np):
    x5 = IntervalVector(e5.rand())
    x5.inflate(0.001)
    fig2.draw_box(x5, [Color.black(), Color.black(0.3)])
    fig3.draw_box(x5, [Color.black(), Color.black(0.3)])
    fig4.draw_box(x5, [Color.black(), Color.black(0.3)])

# ----------------------------------------------------------
# inclusion tests
# ----------------------------------------------------------

print('\nInclusion test e5 in e4: ', e5.is_concentric_subset(e4))

print('\nclusion test e4 in e5: ', e4.is_concentric_subset(e5))

print('\nclusion test e4 in e6: ', e6.is_concentric_subset(e4))

print('\nclusion test e5 in e6: ', e5.is_concentric_subset(e6))

# TODO add the non inclusion test in python

# ----------------------------------------------------------
# guaranteed and non guaranteed linear mappings
# ----------------------------------------------------------
A = Matrix([[0.9, 0.5],
          [-.5, 1.1]])
b = Vector([0.1, 0.2])

e7 = unreliable_linear_mapping(e1, A, b)
e8 = linear_mapping(e1, A, b)

print('\nLinear Mapping - Image ellipsoid e7:', e7)

print('\nLinear Mapping Guaranteed - Image ellipsoid e8:', e8)

print('\nDifference between e7 and e8:', 'mu diff norm is ', (e7.mu - e8.mu).norm(), 'G diff norm is ', (e7.G - e8.G).norm())

# ----------------------------------------------------------
# singular case for nonlinear mapping
# ----------------------------------------------------------

fig5 = Figure2D('singular mappings and degenerated ellipsoids', GraphicOutput.VIBES)
fig5.set_axes(axis(0, [-0.5, 2]), axis(1, [-1.5, 1.]))
fig5.set_window_properties([700, 600], [500, 500])

e9 = Ellipsoid(Vector([0., 0.5]), Matrix([[0.25, 0.],
                                        [0.,   0.]]))
e10 = Ellipsoid(Vector([0., -0.5]), Matrix([[0.25, 0.],
                                          [0.,   0.25]]))

fig5.draw_ellipsoid(e9, [Color.blue(), Color.red(0.3)])
fig5.draw_ellipsoid(e10, [Color.red(), Color.red(0.3)])

h2 = AnalyticFunction([x], vec(x[0] + 0.5 * x[1] + 0.75, -0.5 * sin(x[0]) + 0.9 * x[1] + 0.1*0.5))
h3 = AnalyticFunction([x], vec(x[0] + 0.5 * x[1] + 1.25, x[0] + 0.5 * x[1]-0.25))

e11 = nonlinear_mapping(e9, h2)
e12 = nonlinear_mapping(e10, h3)

fig5.draw_ellipsoid(e11, [Color.green(), Color.green(0.3)])
fig5.draw_ellipsoid(e12, [Color.green(), Color.green(0.3)])

print('\nDegenerate ellipsoid e9 (blue):\n', e9)
print('\nImage of degenerated ellipsoid e11 (green):\n', e11)
print('\nNon-degenerate ellipsoid e10 (red):\n', e10)
print('\nImage of singular mapping e12 (green):\n', e12)

# particle cloud (draw the evolution of 200 points in the ellipsoid)
for i in range(0,Np):
    x0 = e9.rand()
    fig5.draw_box(IntervalVector(x0).inflate(0.0001), [Color.black(), Color.black(0.3)])
    x0 = h2.eval(x0).mid()
    fig5.draw_box(IntervalVector(x0).inflate(0.0001), [Color.black(), Color.black(0.3)])

    x0 = e10.rand()
    fig5.draw_box(IntervalVector(x0).inflate(0.0001), [Color.black(), Color.black(0.3)])
    x0 = h3.eval(x0).mid()
    fig5.draw_box(IntervalVector(x0).inflate(0.0001), [Color.black(), Color.black(0.3)])

# ----------------------------------------------------------
# stability analysis
# ----------------------------------------------------------

# pendulum example
h4 = AnalyticFunction([x], vec(x[0] + 0.5 * x[1] , x[1] + 0.5 * (-x[1]-sin(x[0]))))
e13 = Ellipsoid(Vector(2), Matrix(2,2))
e13_out = Ellipsoid(Vector(2), Matrix(2,2))
alpha_max = 1

# todo if stability_analysis(h4,alpha_max, e13, e13_out) == BoolInterval.TRUE:
# todo     print('\nStability analysis: the system is stable')
# todo     print('Ellipsoidal domain of attraction e13 (red):')
# todo     print(e13)
# todo     print('Outter enclosure e13_out of the Image of e13 by h4 (green):')
# todo     print(e13_out)
# todo 
# todo else:
# todo     print('\nStability analysis: the method is not able to conclude')
# todo 
# todo fig6 = Figure2D('Stability analysis - pendulum example', GraphicOutput.VIBES)
# todo fig6.set_axes(axis(0, [-0.1, 0.1]), axis(1, [-0.1, 0.1]))
# todo fig6.set_window_properties([1200, 600], [500, 500])
# todo fig6.draw_ellipsoid(e13, [Color.red(), Color.red(0.3)])
# todo fig6.draw_ellipsoid(e13_out, [Color.green(), Color.green(0.3)])

#    # high dimensional rov example
#    VectorVar m(6)
#    double T = 0.1
#    double s = 10
#    double kpd = 0.1
#    double kdd = 0.1
#    double kpp = 0.2
#    double kdp = 0.7
#    double dd = 5
#
#    AnalyticFunction h5[
#            [m], vec(m[0] + T * m[3] ,
#                     m[1] + T * m[4] ,
#                     m[2] + T* (m[6]-m[5]),
#                     m[3] + T*s*atan(-kpd*m[0]-kdd*m[3]),
#                     m[4] + T*s*atan(-kpd*m[1]-kdd*m[4]),
#                     m[5] + T*s*atan(kpp/2*m[2]-kdp*m[5])/(m[0]+dd),
#                     m[6] + T*s*atan(-kpp/2*m[2]-kdp*m[6])/(m[1]+dd))]