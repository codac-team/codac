from codac import *

if __name__=="__main__":
  y = VectorVar(3)
  f = AnalyticFunction([y],[sqr(y[0])-sqr(y[1])+y[0],2*y[0]*y[1]+y[1],y[2]])

  X = VectorVar(2)
  psi0 = AnalyticFunction([X],[1/sqrt(1+sqr(X[0])+sqr(X[1])),X[0]/sqrt(1+sqr(X[0])+sqr(X[1])),X[1]/sqrt(1+sqr(X[0])+sqr(X[1]))])

  generators = [[1, 2, 3], [-2, 1, 3], [3, 2, -1]]

  epsilon = 0.1

  figure_3d = Figure3D("Conform")

  v_par = PEIBOS(f,psi0,generators,epsilon)

  for par in v_par:
    figure_3d.draw_parallelepiped(par.z,par.A,Color.green(0.5))
