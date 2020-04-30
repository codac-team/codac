.. _sec-manual-ctcfunction:

************************************
CtcFunction: :math:`f(\mathbf{x})=0`
************************************

Lot of constraints can be expressed under the form :math:`f(\mathbf{x})=0` with :math:`f` an analytic function possibly non-linear. The goal is to estimate the set of feasible vectors :math:`\mathbf{x}` of a domain :math:`[\mathbf{x}]` that satisfy this constraint.
A dedicated contractor can be built from :math:`f` in order to contract boxes.

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}y_i=x(t_i)\\\dot{x}(\cdot)=v(\cdot)\end{array}\right\} \longrightarrow \mathcal{C}_{\textrm{eval}}\big([t_i],[y_i],[x](\cdot),[v](\cdot)\big)

  .. tabs::

    .. code-tab:: c++

      ctc::eval.contract(t, y, x, v);

    .. code-tab:: py

      # todo


Example
-------

Let us come back to :ref:`the Lissajous example<sec-manual-vardyn-lissajous>` introduced to present the use of trajectories.
Our Lissajous trajectory is defined by

.. math::

  \left(\begin{array}{c}x_1(t)\\x_2(t)\end{array}\right) \mapsto \left(\begin{array}{c}2\cos(t)\\\sin(2t)\end{array}\right)


.. figure:: ../02-variables/img/lissajous.png
