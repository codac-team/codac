.. _sec-manual-ctccartprod:

*********************************
CtcCartProd: :math:`\mathcal{C}_{\mathrm{X}_1\times \mathrm{X}_2}`
*********************************

The :math:`\mathcal{C}_{\mathrm{X}_1\times \mathrm{X}_2}` contractor is the cartesian product of two other contractors :math:`\mathcal{C}_{\mathrm{X}_1}` and :math:`\mathcal{C}_{\mathrm{X}_2}`. 

.. contents::


Definition
----------

.. important::
    
  .. math::

    \mathcal{C}_{\mathrm{X}_1\times\mathrm{X}_2}\left([\mathbf{x}] \right) = \left(\mathcal{C}_{\mathrm{X}_1}\left(\Pi_1\right) \times \mathcal{C}_{\mathrm{X}_2}\left(\Pi_2\right)\right) [\mathbf{x}]

  .. tabs::

    .. code-tab:: py

      todo

    .. code-tab:: c++
      
      CtcCardProd C(C1, C2);
      C.contract(x);

.. todo::

  | For now, this contractor is only available for two contractors, but this cartesian product could be extended to the cartesian product of n contractors.
  | Todo: implement the *n*-d version of :math:`\mathcal{C}_\times`.

.. rubric:: Optimality

This contractor is optimal as it is based on the optimality on other contractors.

Example
-------

Example incoming