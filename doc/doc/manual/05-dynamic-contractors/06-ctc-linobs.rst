.. _sec-manual-ctclinobs:

.. warning::
  
  The writing of this page is in progress. Several changes are currently performed on the library.
  A new stable version of Codac will be released in the coming weeks.

**************************************************
CtcLinobs: :math:`\dot{\mathbf{x}}=\mathbf{Ax+Bu}`
**************************************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}\dot{\mathbf{x}}=\mathbf{Ax+Bu}\end{array}\right. \longrightarrow \mathcal{C}_{\textrm{linobs}}\big([\mathbf{x}](\cdot),[u](\cdot)\big)

  .. tabs::

    .. code-tab:: py

      ctc_linobs = CtcLinobs(A, b) # A: matrix object, b: vector object
      ctc_linobs.contract(x, u)

    .. code-tab:: c++

      CtcLinobs ctc_linobs(A, b); // A: matrix object, b: vector object
      ctc_linobs.contract(x, u);


.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classcodac_1_1_ctc_linobs.html>`_.