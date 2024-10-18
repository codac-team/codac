.. _sec-manual-ctccartprod:

********************************************************************
CtcCartProd: :math:`\mathcal{C}_{\mathcal{C}_1\times \mathcal{C}_2}`
********************************************************************

The :math:`\mathcal{C}_{\mathcal{C}_1\times \mathcal{C}_2}` contractor is the Cartesian product of two other contractors :math:`\mathcal{C}_{\mathcal{C}_1}` and :math:`\mathcal{C}_{\mathcal{C}_2}`. This contractor is useful in the case where the state vector of the system is composed of two parts subject to independent constraints.

.. contents::


Definition
----------

.. important::
    
  .. math::

    \mathcal{C}_{\mathcal{C}_1\times\mathcal{C}_2}\left([\mathbf{x}]\right) = \left(\mathcal{C}_1\left(\Pi_1\right) \times \mathcal{C}_2\left(\Pi_2\right)\right)\left([\mathbf{x}]\right)

  .. tabs::

    .. code-tab:: py

      c = CtcCartProd([c1, c2])
      c.contract(x)

    .. code-tab:: c++
      
      CtcCartProd c(c1, c2);
      c.contract(x);

.. todo::

  | For now, this contractor is only available for two contractors, but this Cartesian product could be extended to the Cartesian product of :math:`n` contractors.
  | Todo: implement the :math:`n`-d version of :math:`\mathcal{C}_\times`.

.. rubric:: Optimality

This contractor is optimal as it is based on the optimality of other contractors.

Example
-------

Example incoming

.. from codac import *
.. 
.. x1 = [ Interval(1.25, 3), Interval(4, 5.75), Interval(8.25, 9.5) ]
.. x2 = [ Interval(2.25, 4.75), Interval(6.25, 8) ]
.. 
.. C1 = CtcUnion(x1)
.. C2 = CtcUnion(x2)
..
.. C = CtcCartProd([C1, C2])
.. 
.. x = [ IntervalVector([[4.5, 9], [5.75, 8.5]]),
..       IntervalVector([[0.5, 3.5], [3, 5]]) ]
.. 
.. beginDrawing()
.. 
.. fig = VIBesFigMap("Map")
.. fig.set_properties(50, 50, 500, 500)
.. 
.. for box in in x:
..   fig.draw_box(box, "#475B96")
..   C.contract(box)
..   fig.draw_box(box, "#475B96[#1A80FF55]")
.. 
.. fig.show()
.. fig.axis_limits(fig.view_box(), True, 0.1)
.. 
.. endDrawing()