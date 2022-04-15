.. _sec-usecases-setinv-separators:

.. warning::
  
  Several changes are currently performed on the library.
  This page may not be up to date.

**********
Separators
**********

A separator :math:`\mathcal{S}` is an operator that performs two complementary contractions. Associated with a set :math:`\mathbb{S}` and given a box :math:`[\mathbf{x}]`, the separator produces two sub-boxes :math:`[\mathbf{x}_ {in}]` and :math:`[\mathbf{x}_{out}]` that verify:

.. math::

  ([\mathbf{x}] \cap [\mathbf{x}_{in}]) \subset \mathbb{S} \\
  ([\mathbf{x}] \cap [\mathbf{x}_{out}]) \cap \mathbb{S} = \varnothing

A separator can also be viewed as a pair of contractors. For efficiency reasons, the ``.separate(…)`` method takes only two input-output arguments, :math:`[\mathbf{x}_{in}]` and :math:`[\mathbf{x}_{out}]`, each containing initially a copy of the box :math:`[\mathbf{x}]`.


.. tabs::

  .. code-tab:: py

    from codac import *

    # Define some non-linear function
    f = Function('x', 'y', 'x*cos(x-y)+y')

    # Build the separator associated to the constraint f(x,y) < 0
    sep = SepFwdBwd(f, CmpOp.LEQ)

    # Setup the initial box
    box = IntervalVector(2, [-10, 10])

    # Graphics
    beginDrawing()
    SIVIA(box, sep, 0.3)
    endDrawing()

  .. code-tab:: c++

    // todo