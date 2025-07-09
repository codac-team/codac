from codac import *
import numpy as np

def draw_zonotope (p, i, j, figure, style):
  A_cropped = Matrix(2, p.A.cols())
  for index in range(p.A.cols()):
    A_cropped[0, index] = p.A[i, index]
    A_cropped[1, index] = p.A[j, index]
  z_cropped = Vector([p.z[i], p.z[j]])

  figure.draw_zonotope(z_cropped, A_cropped, style)

if __name__=="__main__":

  # 2D example of the PEIBOS algorithm

  y_2d = VectorVar(2)
  a,b = 1.4,0.3
  f_2d = AnalyticFunction([y_2d],[y_2d[1]+1-a*sqr(y_2d[0]),b*y_2d[0]])

  X_2d = VectorVar(1)
  psi0_2d = AnalyticFunction([X_2d],[cos(X_2d[0]*PI/4.-PI/2),sin(X_2d[0]*PI/4.-PI/2)])
  
  generators_2d = [[1, 2], [-2, 1]]

  v_par_2d = PEIBOS(f_2d,psi0_2d,generators_2d,0.2,[-0.2,0.])
  
  figure_2d = Figure2D("Henon Map", GraphicOutput.VIBES)
  figure_2d.set_window_properties([25,50],[500,500])
  figure_2d.set_axes(axis(0,[-1.4,2.2]), axis(1,[-0.4,0.3]))

  for par in v_par_2d:
    figure_2d.draw_parallelepiped(par.z,par.A,[Color.green(),Color.green(0.5)])
    figure_2d.draw_box(par.bounding_box(), [Color.blue()])
    for vertice in par.vertices():
      figure_2d.draw_point(vertice, [Color.red(), Color.red(0.5)])

  # 3D example of the PEIBOS algorithm

  y_3d = VectorVar(3)
  f_3d = AnalyticFunction([y_3d],[sqr(y_3d[0])-sqr(y_3d[1])+y_3d[0],2*y_3d[0]*y_3d[1]+y_3d[1],y_3d[2]])

  X_3d = VectorVar(2)
  psi0_3d = AnalyticFunction([X_3d],[1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))])

  generators_3d = [[1, 2, 3], [-2, 1, 3], [3, 2, -1]]

  figure_3d = Figure3D("Conform")
  figure_3d.draw_axes()

  figure_3d_proj = Figure2D("Conform projected", GraphicOutput.VIBES)
  figure_3d_proj.set_window_properties([25,600],[500,500])
  figure_3d_proj.set_axes(axis(0,[-1.5,2.5]), axis(1,[-2,2]))

  v_par_3d = PEIBOS(f_3d,psi0_3d,generators_3d,0.2)

  for par in v_par_3d:
    figure_3d.draw_parallelepiped(par.z,par.A,Color.green(0.5))
    draw_zonotope(par, 0, 1, figure_3d_proj, [Color.black(),Color.green(0.2)])

  # nD example of the PEIBOS algorithm

  y_nd = VectorVar(3)
  rot_matrix_1 = Matrix([[1,0,0],[0,1/np.sqrt(2.0),-1/np.sqrt(2.0)],[0,1/np.sqrt(2.0),+1/np.sqrt(2.0)]])
  rot_matrix_2 = Matrix([[1/np.sqrt(2.0),-1/np.sqrt(2.0),0],[1/np.sqrt(2.0),1/np.sqrt(2.0),0],[0,0,1]])
  g_nd = AnalyticFunction([y_nd], [y_nd[0]/sqrt(sqr(y_nd[0])+sqr(y_nd[1])+sqr(y_nd[2])), y_nd[1]/sqrt(sqr(y_nd[0])+sqr(y_nd[1])+sqr(y_nd[2])), y_nd[2]/sqrt(sqr(y_nd[0])+sqr(y_nd[1])+sqr(y_nd[2]))])
  f_nd = AnalyticFunction([y_nd], rot_matrix_1 * rot_matrix_2 * g_nd(y_nd))

  X_nd = VectorVar(1)
  psi0_nd = AnalyticFunction([X_nd],[X_nd[0],1,1])

  generators_nd = [[1, 2, 3], [-2, 1, 3], [3, 2, -1], [1, -2, -3]]

  figure_3d_nd = Figure3D("Cube on Sphere")
  figure_3d_nd.draw_axes(0.5)

  figure_2d_nd_xy = Figure2D("XY Plane", GraphicOutput.VIBES)
  figure_2d_nd_xy.set_window_properties([575,50],[500,500])
  figure_2d_nd_xy.set_axes(axis(0,[-1., 1.]), axis(1,[-1., 1.]))

  figure_2d_nd_zy = Figure2D("ZY Plane", GraphicOutput.VIBES)
  figure_2d_nd_zy.set_window_properties([1125,50],[500,500])
  figure_2d_nd_zy.set_axes(axis(0,[-1., 1.]), axis(1,[-1., 1.]))

  v_par_nd = PEIBOS(f_nd,psi0_nd,generators_nd,0.1)

  for p in v_par_nd:
    figure_3d_nd.draw_parallelepiped(p.z, p.A, Color.green(0.5))
    draw_zonotope(p, 0, 1, figure_2d_nd_xy, [Color.black(), Color.green(0.2)])
    draw_zonotope(p, 2, 1, figure_2d_nd_zy, [Color.black(), Color.green(0.2)])
    for vertice in p.vertices():
      figure_2d_nd_xy.draw_point([vertice[0],vertice[1]], [Color.red(), Color.red(0.5)])
      figure_2d_nd_zy.draw_point([vertice[2],vertice[1]], [Color.red(), Color.red(0.5)])
