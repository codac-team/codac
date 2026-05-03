from codac import *
import numpy as np
import random
import math

def draw_polytope(fig,P,st):
  facets3D = P.vertices_3Dfacets()
  center = Vector([0,0,0])
  transfo = Matrix.eye(3,3)
  for vec in facets3D:
    fig.draw_polygon(center,transfo,vec,st)

radius = 10.0

def dis():
  return random.uniform(-1,1)

def random_point():
  a = dis()*radius
  b = dis()*math.sqrt(radius*radius-a*a)
  c = math.sqrt(radius*radius-a*a-b*b)
  if dis()<0.0:
    c = -c
  return IntervalVector([[a-0.2,a+0.2],[b-0.2,b+0.2],[c-0.2,c+0.2]])

if __name__=="__main__":
  fig = Figure3D("3D hull")
  nbvertices=20
  vertices= []
  scale = 0.1*Matrix.eye(3,3)
  for i in range(0,nbvertices):
    point = random_point()
    vertices.append(point)
    fig.draw_box(point,StyleProperties(Color.black()))
  pol = Polytope(vertices)
  draw_polytope(fig,pol,StyleProperties(Color.dark_blue(0.5),"hull"))
  
  del(fig) # flush the file
