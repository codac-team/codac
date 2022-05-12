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

Example incoming
