.. _sec-manual-contractors:

#############################################
Catalog of contractors for static constraints
#############################################

The **domains** previously presented will be reduced by means of **contractors**: operators designed to eliminate unfeasible solutions according to **constraints**.
We distinct two categories of contractors: those that are time-related (and that involve tubes) and the static ones presented in this chapter.

Mathematically, a contractor :math:`\mathcal{C}` on a box :math:`[\mathbf{x}]\in\mathbb{IR}^n` is an operator :math:`\mathbb{IR}^{n}\to\mathbb{IR}^{n}` such that

.. math::

  \begin{equation}
    \begin{array}{lll}
      \mathcal{C}([\mathbf{x}])\subset[\mathbf{x}] &  & \text{(contractance)}\\
      {}[\mathbf{x}]\subset\left[\mathbf{y}\right]\text{ }\Rightarrow\text{}\mathcal{C}([\mathbf{x}])\subset\mathcal{C}([\mathbf{y}]). &  & \text{(monotonicity)}
  \end{array}
  \end{equation}

In Codac, a contractor is applied on a box by calling the ``.contract(…)`` method.

.. , or directly by using the ``()`` operator.


Static contractors
------------------

The following contractors apply on intervals and boxes:

.. toctree::

  01-ctc-function
  02-ctc-dist
  03-ctc-polar
  04-ctc-constell
  05-ctc-cart-prod
  06-ctc-box

See also: :ref:`codac-unsupported <sec-codac-unsupported>`.

------------------------------------------------------

.. rubric:: Combining contractors

Union, intersection, Cartesian product, or inversion of contractors is achievable. See for instance:

.. tabs::

  .. code-tab:: py

    from codac import *
    from vibes import vibes

    # Define some non-linear function
    f = Function('x', 'y', 'x*cos(x-y)+y')

    # Build the separator associated to the constraint f(x,y) < 0
    sep = SepFwdBwd(f, CmpOp.LEQ)

    # Example of Cartesian product of two contractors
    c3 = cart_prod([ctc_1, ctc_2])

  .. code-tab:: c++

    // todo




.. include:: ../04-static-contractors/00-ctc-basics.rst