.. _sec-manual-ctcpolar:

**************************************************************
CtcPolar: :math:`\big(x=\rho\cos\theta~;y=\rho\sin\theta\big)`
**************************************************************

The :math:`\mathcal{C}_{\textrm{polar}}` contractor is mainly used in robotics to express a range-and-bearing constraint. It can be seen as an extension of the :ref:`previously presented <sec-manual-ctcdist>` :math:`\mathcal{C}_{\textrm{dist}}` enriched with the bearing information. However, its implementation does not stand on the :math:`\mathcal{C}_{\mathbf{f}}` contractor :ref:`presented in the beginning of this chapter <sec-manual-ctcfunction>`. The implementation of :math:`\mathcal{C}_{\textrm{polar}}` is minimal and comes from the `pyIbex library <http://benensta.github.io/pyIbex/>`_.

.. contents::

.. note::

  .. Figure:: ../../img/logo_pyibex.jpg
    :align: right
  
  This contractor directly comes from the `pyIbex library <http://benensta.github.io/pyIbex/>`_ and is temporarily provided here for ease of integration.


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}x=\rho\cos\theta\\y=\rho\sin\theta\end{array}\right\} \longrightarrow \mathcal{C}_{\textrm{polar}}\big([x],[y],[\rho],[\theta]\big)

  .. tabs::

    .. code-tab:: py

      ctc.polar.contract(x, y, rho, theta)

    .. code-tab:: c++

      #include <tubex-pyibex.h>
      ctc::polar.contract(x, y, rho, theta);


.. rubric:: Optimality

This contractor is optimal.


Example
-------

Suppose that we want to estimate the distance :math:`d^i` and angle :math:`\theta^i` (bearing) between a vector :math:`\mathbf{x}=(0,0)^\intercal` and a vector :math:`\mathbf{b}^i`, :math:`i\in\{1,2,3\}`.

We define domains (intervals and boxes):

* :math:`[\mathbf{x}]=[0,0]^2`
* :math:`[\mathbf{b}^1]=[1.5,2.5]\times[4,11]`, :math:`[d^1]=[7,8]`, :math:`[\theta^1]=[0.6,1.45]`
* :math:`[\mathbf{b}^2]=[3,4.5]\times[8,10.5]`, :math:`[d^2]=[9,10]`, :math:`[\theta^2]=[1.15,1.2]`
* :math:`[\mathbf{b}^3]=[5,6.5]\times[6.5,8]`, :math:`[d^3]=[10,11]`, :math:`[\theta^3]=[0.8,1]`

.. tabs::

  .. code-tab:: py

    x = IntervalVector(2,Interval(0))

    d = [Interval(7,8), Interval(9,10), Interval(10,11)]

    theta = [Interval(0.6,1.45), Interval(1.15,1.2), Interval(0.8,1)]

    b = [IntervalVector([[1.5,2.5],[4,11]]), \
         IntervalVector([[3,4.5],[8,10.5]]), \
         IntervalVector([[5,6.5],[6.5,8]])]

  .. code-tab:: c++

    IntervalVector x(2,0.);

    Interval d1(7.,8.), theta1(0.6,1.45);
    IntervalVector b1{{1.5,2.5},{4,11}};

    Interval d2(9.,10.), theta2(1.15,1.2);
    IntervalVector b2{{3,4.5},{8,10.5}};

    Interval d3(10.,11.), theta3(0.8,1.);
    IntervalVector b3{{5,6.5},{6.5,8}};


Calls to :math:`\mathcal{C}_{\textrm{polar}}` will allow the contraction of the :math:`[\mathbf{b}^i]`, :math:`[d^i]` and :math:`[\theta^i]`:

.. tabs::

  .. code-tab:: py

    ctc_polar = CtcPolar()

    for i in range(0,3):
      ctc_polar.contract(b[i][0], b[i][1], d[i], theta[i])
      ctc_polar.contract(b[i][0], b[i][1], d[i], theta[i])
      ctc_polar.contract(b[i][0], b[i][1], d[i], theta[i])

    # note that we could also use directly the ctc.polar object already available

  .. code-tab:: c++

    #include <tubex-pyibex.h>

    // ...

    pyibex::CtcPolar ctc_polar;

    ctc_polar.contract(b1[0], b1[1], d1, theta1);
    ctc_polar.contract(b2[0], b2[1], d2, theta2);
    ctc_polar.contract(b3[0], b3[1], d3, theta3);

    // note that we could also use directly the ctc::polar object already available


.. figure:: img/CtcPolar.png

  Illustration of several contracted boxes and pies with the above ``ctc_polar`` contractor. The blue boxes :math:`[\mathbf{b}^i]` have been contracted as well as the pies :math:`[d^i]\times[\theta^i]`.


.. note:: 

  .. Figure:: img/CtcPolar_nonoptimal.png
    :align: right
    :width: 150px

  | **Optimality**
  | We could have used the ``CtcFunction`` contractor to express the constraint, but the results would not have been optimal. This means that the resulting intervals would not perfectly size the set of feasible values. The figure on the right shows off the pessimism of such alternative.



.. from pyibex import *
.. from tubex_lib import *
.. 
.. x = IntervalVector(2,Interval(0.))
.. 
.. d1 = Interval(7.,8.)
.. theta1 = Interval(0.6,1.45)
.. b1 = IntervalVector([[1.5,2.5],[4,11]])
.. 
.. d2 = Interval(9.,10.)
.. theta2 = Interval(1.15,1.2)
.. b2 = IntervalVector([[3,4.5],[8,10.5]])
.. 
.. d3 = Interval(10.,11.)
.. theta3 = Interval(0.8,1.)
.. b3 = IntervalVector([[5,6.5],[6.5,8]])
.. 
.. # For highlighting non optimality:
.. yd1 = Interval(7.,8.)
.. ytheta1 = Interval(0.6,1.45)
.. yd2 = Interval(9.,10.)
.. ytheta2 = Interval(1.15,1.2)
.. yd3 = Interval(10.,11.)
.. ytheta3 = Interval(0.8,1.)
.. c1 = IntervalVector([[1.5,2.5],[4,11]])
.. c2 = IntervalVector([[3,4.5],[8,10.5]])
.. c3 = IntervalVector([[5,6.5],[6.5,8]])
.. ctc_g = CtcFunction(Function("y[2]", "m[2]",
..   "(y[0]*cos(y[1])-m[0] ; y[0]*sin(y[1])-m[1])"))
.. cn = ContractorNetwork()
.. cn.add(ctc_g, [yd1, ytheta1, c1])
.. cn.add(ctc_g, [yd2, ytheta2, c2])
.. cn.add(ctc_g, [yd3, ytheta3, c3])
.. cn.contract()
.. 
.. beginDrawing()
.. 
.. fig = VIBesFigMap("Map")
.. fig.set_properties(50, 50, 500, 500)
.. fig.add_beacon(x.mid(), 0.2)
.. 
.. fig.draw_box(b1, "#475B96")
.. fig.draw_box(b2, "#475B96")
.. fig.draw_box(b3, "#475B96")
.. fig.draw_pie(x[0].mid(), x[1].mid(), d1, theta1, "#C65B00")
.. fig.draw_pie(x[0].mid(), x[1].mid(), d2, theta2, "#C65B00")
.. fig.draw_pie(x[0].mid(), x[1].mid(), d3, theta3, "#C65B00")
.. 
.. ctc.polar.contract(b1[0], b1[1], d1, theta1)
.. ctc.polar.contract(b2[0], b2[1], d2, theta2)
.. ctc.polar.contract(b3[0], b3[1], d3, theta3)
.. 
.. fig.draw_box(b1, "#475B96[#1A80FF55]")
.. fig.draw_box(b2, "#475B96[#1A80FF55]")
.. fig.draw_box(b3, "#475B96[#1A80FF55]")
.. fig.draw_pie(x[0].mid(), x[1].mid(), d1, theta1, "#C65B00[#FF9A1A55]")
.. fig.draw_pie(x[0].mid(), x[1].mid(), d2, theta2, "#C65B00[#FF9A1A55]")
.. fig.draw_pie(x[0].mid(), x[1].mid(), d3, theta3, "#C65B00[#FF9A1A55]")
.. 
.. # For highlighting non optimality:
.. fig.draw_box(c1, "#475B96[#1A80FF55]")
.. fig.draw_box(c2, "#475B96[#1A80FF55]")
.. fig.draw_box(c3, "#475B96[#1A80FF55]")
.. fig.draw_pie(x[0].mid(), x[1].mid(), yd1, ytheta1, "#C65B00[#FF9A1A55]")
.. fig.draw_pie(x[0].mid(), x[1].mid(), yd2, ytheta2, "#C65B00[#FF9A1A55]")
.. fig.draw_pie(x[0].mid(), x[1].mid(), yd3, ytheta3, "#C65B00[#FF9A1A55]")
.. 
.. fig.axis_limits(0.5, 8., 4., 11., True, 0.02)
.. 
.. endDrawing()


Related content
---------------

.. |polar-pdf| replace:: **Download the paper**
.. _polar-pdf: https://www.ensta-bretagne.fr/jaulin/paper_polar.pdf

.. admonition:: Related publication
  
  | **A Minimal contractor for the Polar equation: application to robot localization**
  | B. Desrochers, L. Jaulin
  | *Engineering Applications of Artificial Intelligence*, 55(Supplement C):83–92, Oct. 2016.
  | |polar-pdf|_

.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classpyibex_1_1_ctc_polar.html>`_.