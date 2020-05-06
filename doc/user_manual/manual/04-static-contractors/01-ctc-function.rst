.. _sec-manual-ctcfunction:

**************************************************************
Generic CtcFunction: :math:`\mathbf{f}(\mathbf{x})=\mathbf{0}`
**************************************************************

Lot of constraints can be expressed under the form :math:`\mathbf{f}(\mathbf{x})=\mathbf{0}` with :math:`\mathbf{f}` an analytic function possibly non-linear. The goal is to estimate the set of feasible vectors :math:`\mathbf{x}` of a domain :math:`[\mathbf{x}]` that satisfy this constraint.
A dedicated contractor can be built from :math:`\mathbf{f}` in order to contract boxes.

.. contents::


Definition
----------

.. important::
    
  .. math::

    \mathbf{f}(\mathbf{x})=\mathbf{0} \longrightarrow \mathcal{C}_{\mathbf{f}}\big([\mathbf{x}]\big)

  .. tabs::

    .. code-tab:: c++

      CtcFunction ctc_f(Function f("<var1>", "<var2...>", "<expr>"));
      ctc_f.contract(x);

    .. code-tab:: py

      # todo

.. note::

  .. Figure:: ../../img/ibex_logo.jpg
    :align: right
  .. Figure:: ../../img/pyibex_logo.jpg
    :align: right
  
  This contractor originates from the `IBEX library <http://www.ibex-lib.org>`_. It is briefly presented here for the sake of consistency. For more information, please refer to the `IBEX documentation <http://www.ibex-lib.org/doc/contractor.html>`_ for C++ use or to the `pyIbex manual <http://benensta.github.io/pyIbex/sphinx/api.html#ctcfwdbwd>`_ for Python usage.


Example
-------

Let us come back to :ref:`the Lissajous example<sec-manual-vardyn-lissajous>` introduced to present the use of trajectories.
Our Lissajous trajectory is defined by

.. math::

  \left(\begin{array}{c}x_1(t)\\x_2(t)\end{array}\right) \mapsto \left(\begin{array}{c}2\cos(t)\\\sin(2t)\end{array}\right)


.. figure:: ../02-variables/img/lissajous.png
