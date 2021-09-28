#!/usr/bin/env python

import unittest
from pyibex import Interval, IntervalVector, Function
from codac import *
import codac as codac

class TestCN(unittest.TestCase):

  def test_CN_simple(self):

    ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c"))
    a = Interval(0,1)
    b = Interval(-1,1)
    c = Interval(1.5,2)

    cn = ContractorNetwork()
    cn.add(ctc_plus, [a, b, c]) 
    cn.contract()

    self.assertEqual(a, Interval(0.5,1))
    self.assertEqual(b, Interval(0.5,1))
    self.assertEqual(c, Interval(1.5,2))

    self.assertEqual(cn.nb_dom(), 3)
    self.assertEqual(cn.nb_ctc(), 1)


  def test_simple_static_case(self):

    ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c")) # algebraic constraint a+b=c

    a = IntervalVector(2, [0,1])
    b = IntervalVector(2, [-1,1])
    c = IntervalVector(2, [1.5,2])

    cn = ContractorNetwork()
    cn.add(ctc_plus, [a[0], b[0], c[0]]) 
    cn.contract()

    self.assertEqual(a[0], Interval(0.5,1))
    self.assertEqual(b[0], Interval(0.5,1))
    self.assertEqual(c[0], Interval(1.5,2))

    self.assertEqual(cn.nb_dom(), 3)
    self.assertEqual(cn.nb_ctc(), 1)


  def test_dependencies_on_vector_components(self):

    ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c")) # algebraic constraint a+b=c

    a = IntervalVector(2, [0,1])
    b = IntervalVector(2, [-1,1])
    c = IntervalVector(2, [1.5,2])
    d = IntervalVector(2, [0.,0.])
    e = IntervalVectore(2)

    cn = ContractorNetwork()
    # Contractors are added to the graph, 
    # the first one contracts the vector sets, the second one then contracts the components intervals,
    # and so it should trigger again the first one since components have changed.
    cn.add(ctc_plus, [b, d, e])
    cn.add(ctc_plus, [b, d, e])
    cn.add(ctc_plus, [b, d, e])
    cn.add(ctc_plus, [a[0], b[0], c[0]])
    cn.add(ctc_plus, [a[0], b[0], c[0]])
    cn.add(ctc_plus, [a[0], b[0], c[0]])
    cn.add(ctc_plus, [a[0], b[0], c[0]])
    cn.contract()

    self.assertEqual(a[0], Interval(0.5,1))
    self.assertEqual(b[0], Interval(0.5,1))
    self.assertEqual(c[0], Interval(1.5,2))
    self.assertEqual(d[0], Interval(0))
    self.assertEqual(e[0], Interval(0.5,1))

    # Before setting names (some vectors not added entirely):
    self.assertEqual(cn.nb_dom(), 3*3+2)
    # vector items contractors + ctc_plus in array mode + ctc_plus on scalar values
    self.assertEqual(cn.nb_ctc(), 3+2+1)

    cn.set_name(a, "a")
    cn.set_name(b, "b")
    cn.set_name(c, "c")
    cn.set_name(d, "d")
    cn.set_name(e, "e")
    cn.set_name(ctc_plus, "+")
    cn.print_dot_graph("cn_vector_dependencies")

    self.assertEqual(cn.nb_dom(), 3*5)
    # vector items contractors + ctc_plus in array mode + ctc_plus on scalar values
    self.assertEqual(cn.nb_ctc(), 5+2+1)


  def test_dependencies_on_vector_components(self):

    dt = 5
    domain = Interval(0.,20.)
    x = Tube(domain, dt, Interval(-10.,10.))
    v = Tube(domain, dt, Interval(0.))

    ctc_deriv = CtcDeriv()

    cn = ContractorNetwork()
    cn.add(ctc_deriv, [x, v])
    cn.add(ctc_deriv, [x, v]) # redundant contractor that should not be added
    cn.contract()
    cn.contract()

    self.assertEqual(v.codomain(), Interval(0.))
    self.assertEqual(x.codomain(), Interval(-10.,10.))
    self.assertEqual(x.nb_slices(), 4)
    self.assertEqual(v.nb_slices(), 4)
    self.assertEqual(cn.nb_ctc(), 12)
    self.assertEqual(cn.nb_dom(), 10)

    ctc_eval = CtcEval()
    t1 = Interval(5.)
    z = Interval(2.)

    cn.add(ctc_eval, [t1, z, x, v])
    cn.add(ctc_eval, [t1, z, x, v])
    cn.add(ctc_eval, [t1, z, x, v])
    cn.add(ctc_eval, [t1, z, x, v]) # redundant contractor that should not be added
    cn.contract()

    cn.set_name(t1, "t_1")
    cn.set_name(z, "z")
    cn.set_name(x, "x")
    cn.set_name(v, "v")
    cn.print_dot_graph("cn_observation")

    self.assertEqual(v.codomain(), Interval(0.))
    self.assertEqual(x.codomain(), Interval(2.))
    self.assertEqual(cn.nb_ctc(), 13)
    self.assertEqual(cn.nb_dom(), 12)


  def test_dependencies_on_vector_components(self):

    dt = 5.
    domain = Interval(0.,20.)
    x = Tube(domain, dt, Interval(-10.,10.))
    v = Tube(domain, dt, Interval(0.))

    ctc_deriv = CtcDeriv()
    ctc_f = CtcFunction(Function("x", "xdot", "xdot+sin(x)"))

    cn = ContractorNetwork()
    cn.add(ctc_deriv, [x, v])
    cn.add(ctc_f, [x, v])

    self.assertEqual(v.codomain(), Interval(0.))
    self.assertEqual(x.codomain(), Interval(-10.,10.))
    self.assertEqual(x.nb_slices(), 4)
    self.assertEqual(v.nb_slices(), 4)
    self.assertEqual(cn.nb_ctc(), 16)
    self.assertEqual(cn.nb_dom(), 10)

    cn.set_fixedpoint_ratio(0.8)
    cn.contract()


  def test_dependencies_on_vector_components_1(self):

    x = Interval(0,1)
    y = Interval(-2,3)
    a = Interval(1,20)
    vx = IntervalVector(2,x)
    vy = IntervalVector(2,y)
    va = IntervalVector(2,a)
    vec = [4,0.5]
    
    ctc_add = CtcFunction(Function("b", "c", "a", "b+c-a"))
    ctc_cos = CtcFunction(Function("b", "c", "a", "b+c-a"))
    ctc_minus = CtcFunction(Function("b", "c", "b-c-0"))

    cn = ContractorNetwork()

    cn.add(ctc_add, [x,y,a])
    cn.add(ctc_add, [x,y,a])
    cn.contract()

    self.assertEqual(x, Interval(0,1))
    self.assertEqual(y, Interval(0,3))
    self.assertEqual(a, Interval(1,4))
    self.assertEqual(cn.nb_dom(), 3)
    self.assertEqual(cn.nb_ctc(), 1)


  def test_dependencies_on_vector_components_2(self):

    x = Interval(0,1)
    y = Interval(-2,3)
    a = Interval(1,20)
    vx = IntervalVector(2,x)
    vy = IntervalVector(2,y)
    va = IntervalVector(2,a)
    vec = [4,0.5]
    
    ctc_add = CtcFunction(Function("b", "c", "a", "b+c-a"))
    ctc_cos = CtcFunction(Function("b", "c", "a", "b+c-a"))

    cn = ContractorNetwork()
    cn.add(ctc_add, [vx,vy,va])
    cn.add(ctc_add, [vx[0],vy[0],va[0]])
    cn.contract()

    self.assertEqual(vx, IntervalVector(2,[0,1]))
    self.assertEqual(vy, IntervalVector(2,[0,3]))
    self.assertEqual(va, IntervalVector(2,[1,4]))

    self.assertEqual(cn.nb_dom(), 3*3)
    self.assertEqual(cn.nb_ctc(), 3+2)

    cn.set_name(va, "a")
    cn.set_name(vx, "x")
    cn.set_name(vy, "y")
    cn.print_dot_graph("cn_function")


  def test_CtcFunction_on_heterogeneous_variables(self):

    x = IntervalVector([[0,1],[-2,3]])
    a = Interval(1,20)
    
    ctc_add = CtcFunction(Function("b[2]", "a", "b[0]+b[1]-a"))

    cn = ContractorNetwork()
    cn.add(ctc_add, [x,a])
    cn.contract()

    self.assertEqual(x[0], Interval(0,1))
    self.assertEqual(x[1], Interval(0,3))
    self.assertEqual(a, Interval(1,4))


  def test_subvector(self):

    x = IntervalVector([[0,1],[-2,3],[1,20]])

    ctc_add = CtcFunction(Function("b", "c", "a", "b+c-a"))

    cn = ContractorNetwork()
    sub_x = cn.subvector(x,1,2)

    self.assertEqual(sub_x[0], Interval(-2,3))
    self.assertEqual(sub_x[1], Interval(1,20))

    cn.add(ctc_add, [x[0],x[1],x[2]])
    cn.contract()

    self.assertEqual(x[0], Interval(0,1))
    self.assertEqual(x[1], Interval(0,3))
    self.assertEqual(x[2], Interval(1,4))

    self.assertEqual(sub_x[0], Interval(0,3))
    self.assertEqual(sub_x[1], Interval(1,4))

    sub_x[0] = Interval(1,2)
    cn.trigger_all_contractors()
    cn.contract()
    self.assertEqual(x[1], Interval(1,2))


  def test_singleton_variables_double_or_Vector_1(self):

    x = Interval(0,1)
    y = Interval(-2,3)
    a = Interval(1,20)

    double_y = 1.
    vector_y = [2, 1.]
    ivx = IntervalVector(2,x)
    ivy = IntervalVector(2,y)
    iva = IntervalVector(2,a)
    
    ctc_add = CtcFunction(Function("b", "c", "a", "b+c-a"))

    cn = ContractorNetwork()

    cn.add(ctc_add, [x,double_y,a])
    cn.add(ctc_add, [x,double_y,a]) # redundant adding
    cn.add(ctc_add, [x,double_y,a]) # redundant adding
    cn.contract()

    self.assertEqual(x, Interval(0,1))
    self.assertEqual(double_y, 1.)
    self.assertEqual(a, Interval(1,2))
    #self.assertEqual(cn.nb_dom(), 3) # todo: not able to get reference from a py::float_
    #self.assertEqual(cn.nb_ctc(), 1) # todo: not able to get reference from a py::float_: adds several contractors


  def test_singleton_variables_double_or_Vector_2(self):

    x = Interval(0,1)
    y = Interval(-2,3)
    a = Interval(1,20)

    vector_y = [2, 1.]
    ivx = IntervalVector(2,x)
    iva = IntervalVector(2,a)
    
    ctc_add = CtcFunction(Function("b", "c", "a", "b+c-a"))

    cn = ContractorNetwork()

    cn.add(ctc_add, [ivx,vector_y,iva])
    cn.add(ctc_add, [ivx,vector_y,iva]) # redundant adding
    cn.add(ctc_add, [ivx,vector_y,iva]) # redundant adding
    cn.contract()

    self.assertEqual(ivx, IntervalVector(2,[0,1]))
    self.assertEqual(vector_y, [2,1.])
    self.assertEqual(iva, IntervalVector([[2,3],[1,2]]))
    #self.assertEqual(cn.nb_dom(), 3*3) # todo: not able to get reference from a py::list
    #self.assertEqual(cn.nb_ctc(), 3+2) # todo: not able to get reference from a py::list

    cn.set_name(vector_y, "y")
    cn.set_name(ivx, "x")
    cn.set_name(iva, "a")
    cn.set_name(ctc_add, "+")
    cn.print_dot_graph("cn_singleton")


  def test_heterogeneous_variables_with_CtcFunction_1(self):

    dt = 0.1
    tdomain = Interval(0.,10.)

    x = Interval(2,3)
    a = Tube(tdomain, dt, Interval(4.,5.))
    b = Tube(tdomain, dt, Interval(5.,9.))

    ctc_add = CtcFunction(Function("x", "a", "b", "x+a-b"))

    cn = ContractorNetwork()
    cn.add(ctc_add, [x,a,b])
    cn.contract()

    self.assertEqual(x, Interval(2.,3.))
    self.assertEqual(a.codomain(), Interval(4.,5.))
    self.assertEqual(b.codomain(), Interval(6.,8.))


  def test_heterogeneous_variables_with_CtcFunction_2(self):

    dt = 0.1
    tdomain = Interval(0.,10.)

    x = Interval(-1.,3.)
    a = Tube(tdomain, dt, Interval(6.,7.))
    b = Tube(tdomain, dt, Interval(7.))

    ctc_add = CtcFunction(Function("x", "a", "b", "x+a-b"))

    cn = ContractorNetwork()
    cn.add(ctc_add, [x,a,b])
    cn.contract()

    cn.set_name(x, "x")
    cn.set_name(a, "a")
    cn.set_name(b, "b")

    self.assertEqual(x, Interval(0.,1.))
    self.assertEqual(a.codomain(), Interval(6.,7.))
    self.assertEqual(b.codomain(), Interval(7.))


  def test_heterogeneous_variables_with_CtcFunction_3(self):

    dt = 0.1
    tdomain = Interval(0.,10.)

    x = Interval(2.,2.5)
    a = Tube(tdomain, dt, Interval(0.,10.))
    b = Tube(tdomain, dt, Interval(7.))

    ctc_add = CtcFunction(Function("x", "a", "b", "x+a-b"))

    cn = ContractorNetwork()
    cn.add(ctc_add, [x,a,b])
    cn.contract()

    self.assertEqual(x, Interval(2.,2.5))
    self.assertEqual(a.codomain(), Interval(4.5,5.))
    self.assertEqual(b.codomain(), Interval(7.))


  def test_heterogeneous_variables_with_CtcFunction_4(self):

    dt = 0.1
    tdomain = Interval(0.,10.)

    x = Interval(2.,2.5)
    a = Tube(tdomain, dt, TFunction("cos(t)"))
    b = Tube(tdomain, dt)

    ctc_add = CtcFunction(Function("x", "a", "b", "x+a-b"))

    cn = ContractorNetwork()
    cn.add(ctc_add, [x,a,b])
    cn.contract()
    
    result = Tube(tdomain, dt, TFunction("cos(t)"))
    result += x

    self.assertEqual(x, Interval(2.,2.5))
    self.assertEqual(b, result)


  def test_adding_continuous_data(self):

    dt = 1.
    tdomain = Interval(0.,5.)

    x = Tube(tdomain, dt)
    v = Tube(tdomain, dt)
    x.set(Interval(0.), 0.) # todo: allow to write x.set(0., 0.) ?

    ctc_deriv = CtcDeriv()
    cn = ContractorNetwork()
    cn.add(ctc_deriv, [x,v])

    cn.set_name(x, "x")
    cn.set_name(v, "v")
    cn.print_dot_graph("cn_adddata")

    cn.contract()

    self.assertTrue((x.codomain() == Interval.ALL_REALS \
      or x.codomain(), Interval(-99999, 99999))) # todo: remove this
    self.assertTrue((v.codomain() == Interval.ALL_REALS \
      or x.codomain(), Interval(-99999, 99999))) # todo: remove this
    self.assertEqual(cn.nb_ctc_in_stack(), 0)

    # Adding data
    cn.add_data(v, 0., Interval(0.))
    self.assertEqual(cn.nb_ctc_in_stack(), 0)
    cn.add_data(v, 0.3, Interval(0.))
    self.assertEqual(cn.nb_ctc_in_stack(), 0)
    cn.add_data(v, 0.4, Interval(0.))
    cn.add_data(v, 0.5, Interval(0.))
    # cn.add_data(x, 0.5, Interval(0.5))) # should fail
    self.assertEqual(cn.nb_ctc_in_stack(), 0)
    cn.add_data(v, 0.99, Interval(0.))
    self.assertEqual(cn.nb_ctc_in_stack(), 0)
    cn.add_data(v, 1.3, Interval(0.))
    self.assertEqual(cn.nb_ctc_in_stack(), 3)
    cn.add_data(v, 1.5, Interval(0.))
    self.assertEqual(cn.nb_ctc_in_stack(), 3)
    cn.add_data(v, 4.5, Interval(-3.)) # accross two slices
    self.assertEqual(cn.nb_ctc_in_stack(), 9)
    cn.add_data(v, 5.5, Interval(1.)) # after tf
    self.assertEqual(cn.nb_ctc_in_stack(), 10)

    self.assertEqual(v(0), Interval(0.))
    self.assertEqual(v(1), Interval(-0.5,0.))
    self.assertEqual(v(2), Interval(-1.5,-0.5))
    self.assertEqual(v(3), Interval(-2.5,-1.5))
    self.assertEqual(v(4), Interval(-3.,-1.))


  def test_create_interm_var_tube(self):

    dt = 0.1
    tdomain = Interval(0.,10.)
    x = Tube(tdomain, dt)
    x.set(Interval(0.), 0.)

    self.assertEqual(x.codomain(), Interval())

    cn = ContractorNetwork()
    ctc_deriv = CtcDeriv()

    v_local = Tube(tdomain, dt, TFunction("0"))
    #v_inside = cn.create_interm_var(v_local)
    #cn.add(ctc_deriv, [x,v_inside])

    #cn.contract()
    #self.assertEqual(x.codomain(), Interval(0))

  
  def test_cn_variables(self):

    cn = ContractorNetwork()
    a = IntervalVar()

    ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c"))
    a1 = Interval(0,1)
    b1 = Interval(-1,1)
    a2 = Interval(-1,1)
    b2 = Interval(0,1)
    c = Interval(1.5,2);

    cn.add(ctc_plus, [a, b1, c])

    cn.contract({
      a: a1
    })

    self.assertEqual(a1, Interval(0.5,1))
    self.assertEqual(b1, Interval(0.5,1))
    self.assertEqual(c, Interval(1.5,2))

    a1 |= Interval(0,1)
    b1 |= Interval(-1,1)

    cn.contract({
      a: a1
    })

    self.assertEqual(a1, Interval(0.5,1))
    self.assertEqual(b1, Interval(0.5,1))
    self.assertEqual(c, Interval(1.5,2))

    cn.contract({
      a: a2,
      b1: b2
    })

    self.assertEqual(a1, Interval(0.5,1))
    self.assertEqual(b1, Interval(0.5,1))
    self.assertEqual(a2, Interval(0.5,1))
    self.assertEqual(b2, Interval(0.5,1))
    self.assertEqual(c, Interval(1.5,2))

    self.assertEqual(cn.nb_dom(), 3)
    self.assertEqual(cn.nb_ctc(), 1)

  def test_cn_heterogeneous_variables(self):

    a = IntervalVar()
    x = IntervalVectorVar(2)
    
    ctc_add = CtcFunction(Function("b[2]", "c", "b[0]+b[1]-c"))

    cn = ContractorNetwork()
    cn.add(ctc_add, [x,a])

    x1 = IntervalVector([[0,1],[-2,3]])
    a1 = Interval(1,20)
    cn.contract({
      a: a1,
      x: x1
    })

    self.assertEqual(x1[0], Interval(0,1))
    self.assertEqual(x1[1], Interval(0,3))
    self.assertEqual(a1, Interval(1,4))

    x2 = IntervalVector([[10,10.5],[22,99]])

    a2 = Interval(32,33)
    cn.contract({
      x: x2,
      a: a2
    })

    #self.assertEqual(a, Interval())
    #self.assertEqual(x, IntervalVector(2))

    self.assertEqual(x1[0], Interval(0,1)) # remain unchanged
    self.assertEqual(x1[1], Interval(0,3)) # remain unchanged
    self.assertEqual(a1, Interval(1,4))    # remain unchanged
    self.assertEqual(x2[0], Interval(10,10.5))
    self.assertEqual(x2[1], Interval(22,23))
    self.assertEqual(a2, Interval(32,33))


if __name__ ==  '__main__':
  unittest.main()