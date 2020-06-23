.. _sec-manual-ctcdist:

*********************************
CtcDist: :math:`\sqrt{x^2+y^2}=d`
*********************************

The :math:`\mathcal{C}_{\textrm{dist}}` contractor is mainly used in robotics to express the distance constraint :math:`d` between two vectors :math:`\mathbf{a}` and :math:`\mathbf{b}`. Its implementation currently stands on the :math:`\mathcal{C}_{\mathbf{f}}` contractor :ref:`previously presented <sec-manual-ctcfunction>`.

.. contents::


Definition
----------

.. important::
    
  .. math::

    \sqrt{(a_1-b_1)^2+(a_2-b_2)^2}=d \longrightarrow \mathcal{C}_{\textrm{dist}}\big([\mathbf{a}],[\mathbf{b}],[d]\big)

  .. tabs::

    .. code-tab:: py

      ctc.dist.contract(a, b, d)

    .. code-tab:: c++

      ctc::dist.contract(a, b, d);

.. todo::

  | For now, this contractor is only available for :math:`[\mathbf{a}]\in\mathbb{IR}^2`, :math:`[\mathbf{b}]\in\mathbb{IR}^2`.
  | Todo: implement the *n*-d version of :math:`\mathcal{C}_{\textrm{dist}}`.

.. rubric:: Optimality

This contractor is optimal.


Example
-------

Suppose that we want to estimate the distance :math:`d` between a vector :math:`\mathbf{x}=(0,0)^\intercal` and several vectors :math:`\mathbf{b}^1`, :math:`\mathbf{b}^2`, :math:`\mathbf{b}^3`.

We define domains (intervals and boxes):

* :math:`[d]=[7,8]`
* :math:`[\mathbf{x}]=[0,0]^2`
* :math:`[\mathbf{b}^1]=[1.5,2.5]\times[4,11]`
* :math:`[\mathbf{b}^2]=[3,4]\times[4,6.5]`
* :math:`[\mathbf{b}^3]=[5,7]\times[5.5,8]`

.. tabs::

  .. code-tab:: py

    d = Interval(7,8)
    x = IntervalVector(2,Interval(0))
    b1 = IntervalVector([[1.5,2.5],[4,11]])
    b2 = IntervalVector([[3,4],[4,6.5]])
    b3 = IntervalVector([[5,7],[5.5,8]])

  .. code-tab:: c++

    Interval d(7.,8.);
    IntervalVector x(2,0.);
    IntervalVector b1{{1.5,2.5},{4,11}};
    IntervalVector b2{{3,4},{4,6.5}};
    IntervalVector b3{{5,7},{5.5,8}};

Several calls to :math:`\mathcal{C}_{\textrm{dist}}` will allow the contraction of both the :math:`[\mathbf{b}^i]` and :math:`[d]`. Because domains are involved in several contractions, an iterative contraction loop is necessary in order to reach a consistency state (in our case, two iterations are sufficient):

.. tabs::

  .. code-tab:: py

    ctc_dist = CtcDist()

    for i in range(0,2): # iterative contractions
      ctc_dist.contract(x, b1, d)
      ctc_dist.contract(x, b2, d)
      ctc_dist.contract(x, b3, d)

    # note that we could also use directly the ctc.dist object already available

  .. code-tab:: c++

    CtcDist ctc_dist;

    for(int i = 0 ; i < 2 ; i++) // iterative contractions
    {
      ctc_dist.contract(x, b1, d);
      ctc_dist.contract(x, b2, d);
      ctc_dist.contract(x, b3, d);
    }

    // note that we could also use directly the ctc::dist object already available

.. figure:: img/CtcDist.png

  Illustration of several contracted boxes with the above ``ctc_dist`` contractor. The blue boxes have been contracted as well as the ring.

.. #include <tubex.h>
.. #include <tubex-rob.h>
.. 
.. using namespace std;
.. using namespace tubex;
.. using namespace ibex;
.. 
.. int main()
.. {
..   Interval d(7.,8.);
..   IntervalVector x(2,0.);
..   IntervalVector b1{{1.5,2.5},{4,11}};
..   IntervalVector b2{{3,4},{4,6.5}};
..   IntervalVector b3{{5,7},{5.5,8}};
.. 
..   CtcDist ctc_dist;
.. 
..   vibes::beginDrawing();
.. 
..   VIBesFigMap fig("Map");
..   fig.set_properties(50, 50, 500, 500);
..   fig.add_beacon(Beacon(x), 0.2);
.. 
..   fig.draw_box(b1, "#475B96");
..   fig.draw_box(b2, "#475B96");
..   fig.draw_box(b3, "#475B96");
..   fig.draw_circle(x[0].mid(), x[1].mid(), d.lb());
..   fig.draw_circle(x[0].mid(), x[1].mid(), d.ub());
.. 
..   for(int i = 0 ; i < 2 ; i++)
..   {
..     ctc_dist.contract(x, b1, d);
..     ctc_dist.contract(x, b2, d);
..     ctc_dist.contract(x, b3, d);
..   }
.. 
..   fig.draw_box(b1, "#475B96[#1A80FF55]");
..   fig.draw_box(b2, "#475B96[#1A80FF55]");
..   fig.draw_box(b3, "#475B96[#1A80FF55]");
..   fig.draw_circle(x[0].mid(), x[1].mid(), d.lb(), "#475B96");
..   fig.draw_circle(x[0].mid(), x[1].mid(), d.ub(), "#475B96");
.. 
..   fig.axis_limits(0.5, 8., 4., 11., true, 0.02);
.. 
..   vibes::endDrawing();
.. }


.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classtubex_1_1_ctc_dist.html>`_.