.. _sec-manual-ctcpicard:

.. warning::
  
  The writing of this page is in progress. Several changes are currently performed on the library.
  A new stable version of Codac will be released in the coming weeks.

************************************************************************
CtcPicard: :math:`\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t)\big)`
************************************************************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t)\big)\end{array}\right. \longrightarrow \mathcal{C}_{\textrm{picard}}\big([\mathbf{x}](\cdot)\big)

  .. tabs::

    .. code-tab:: py

      ctc_picard = CtcPicard(TFunction("<var1>", "<var2...>", "<exp>"))
      ctc_picard.contract(x)

    .. code-tab:: c++

      CtcPicard ctc_picard(TFunction("<var1>", "<var2...>", "<exp>"));
      ctc_picard.contract(x);



.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classcodac_1_1_ctc_picard.html>`_.