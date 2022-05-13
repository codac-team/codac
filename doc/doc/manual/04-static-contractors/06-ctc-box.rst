.. _sec-manual-ctcbox:

********************************************************************
CtcBox: :math:`\mathcal{C}_{box}`
********************************************************************

The :math:`\mathcal{C}_{box}` is contracting IntervalVector around a support box of :math:`\mathbb{R}^n`. This contractor is based on the inverse of a canonical injection between the provided box and :math:`\mathbb{R}^n`.

.. contents::


Definition
----------

.. important::
    
  .. math::

    \iota^{-1}_\mathbf{b} : \mathbb{R}^n \hookrightarrow \mathbf{b} : x \mapsto x \longrightarrow \mathcal{C}_{box}\big([\mathbf{b}]\big)

  .. tabs::

    .. code-tab:: py

      c = CtcBox(b)
      c.contract(x)

    .. code-tab:: c++
      
      CtcBox c(b);
      c.contract(x);

.. todo::

  | For now, this contractor is only available for two contractors, but this Cartesian product could be extended to the Cartesian product of :math:`n` contractors.
  | Todo: implement the :math:`n`-d version of :math:`\mathcal{C}_\times`.

.. rubric:: Optimality

This contractor is optimal as it is based on intersection between boxes which is optimal and does not add any pessimism.

Example
-------

Let consider a box :math:`\mathbf{b} = ((1, 2), (3, 4))` around which we want to contract boxes.

.. tabs::

  .. code-tab:: py

    b = IntervalVector([[1, 2], [3, 4]])
    ctc_box = CtcBox(b)

    x1 = IntervalVector([[0, 3], [2, 5]])
    x2 = IntervalVector([[1.5, 3.5], [3.5, 6.5]])

    ctc_box.contract(x1)
    ctc_box.contract(x2)

  .. code-tab:: c++

    IntervalVector b{{1, 2}, {3, 4}};
    CtcBox ctc_box(b);

    IntervalVector x1{{0, 3}, {2, 5}};
    IntervalVector x2{{1.5, 3.5}, {3.5, 6.5}};

    ctc_box.contract(x1);
    ctc_box.contract(x2);

.. from codac import *
.. 
.. b = IntervalVector([[1, 2], [3, 4]])
.. ctc_box = CtcBox(b)
..
.. x1 = IntervalVector([[0, 3], [2, 5]])
.. x2 = IntervalVector([[1.5, 3.5], [3.5, 6.5]])
..
.. beginDrawing()
.. 
.. fig = VIBesFigMap("Map")
.. fig.set_properties(50, 50, 500, 500)
.. 
.. fig.draw_box(b, "#475B96")
..
.. fig.draw_box(x1, "#475B96")
.. ctc_box.contract(x1)
.. fig.draw_box(x1, "#475B96")
.. 
.. fig.draw_box(x2, "#475B96")
.. ctc_box.contract(x2)
.. fig.draw_box(x2, "#475B96")
.. 
.. fig.show()
.. fig.axis_limits(fig.view_box(), True, 0.1)
.. 
.. endDrawing()