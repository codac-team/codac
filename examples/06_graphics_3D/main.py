# The generated .obj files can be visualized on https://3dviewer.net

from codac import *
import math

x = VectorVar(3)
f = AnalyticFunction([x], [
  -sqr(x[2])+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
  2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
])

ctc = CtcInverse(f, [0,0])
p_ctc = pave([[0,2],[2,4],[0,10]], ctc, 0.02)
fig_ctc=Figure3D("Paving contractor")
fig_ctc.draw_paving(p_ctc)

sep_ellipsoid1=SepInverse (AnalyticFunction([x], 0.5*sqr(x[0])+x[0]*x[1]+x[0]*x[2]+2*sqr(x[1])+2*sqr(x[2])),Interval(0.4,1))
sep_ellipsoid2=SepInverse (AnalyticFunction([x], 3*sqr(x[0])+x[0]*x[1]+x[0]*x[2]+sqr(x[1])+sqr(x[2])),Interval(0.,1))
p_sep = pave([[-1.5,1.5],[-1.5,1.5],[-1.5,1.5]], sep_ellipsoid1&sep_ellipsoid2, 0.1)
fig_sep = Figure3D("Paving separator")
fig_sep.draw_axes(0.4)
fig_sep.draw_paving(p_sep)

fig_examples = Figure3D("3D examples")
fig_examples.draw_axes(1.0)
fig_examples.draw_triangle([1,0,0],[0,1,0],[0,0,1],StyleProperties(Color.dark_green(0.5),"triangle"))
fig_examples.draw_triangle([2,0,0],
                        Matrix([[2,0,0],[-1,0,0],[0,1,1]]),
                            [1,0,0],[0,1,0],[0,0,1],Color.purple(0.5))
fig_examples.draw_sphere([0,0,2],Matrix([[-1,0,0],[0,1,1],[0,0,-1]]),
                            StyleProperties(Color.yellow(0.6),"sphere"))
fig_examples.draw_arrow([0,2,0],Matrix([[-1,0,0],[0,1,1],[0,0,-1]]),Color.red(1.0))
fig_examples.draw_car([-1,0,0],0.3*Matrix.eye(3,3),
                        StyleProperties(Color.green(0.8),"car"))
fig_examples.draw_plane([3,0,0],0.5*Matrix.eye(3,3),True,
                        StyleProperties(Color.dark_gray(0.8),"plane"))

def f(phi,psi):
    return Vector([(1-math.cos(phi))*math.sin(phi),
                   (1-math.cos(2*phi))*math.cos(phi)*math.cos(psi),
                   (1-math.cos(phi))*math.cos(phi)*math.sin(psi)])

fig_examples.draw_surface([0,-2,0], 0.5*Matrix.eye(3,3),
                    [0,2*PI],0.05*PI,[0,2*PI],0.05*PI,
                    f,StyleProperties(Color.red(0.6),"example_surface"))

# to flush the files
del fig_ctc
del fig_sep
del fig_examples
