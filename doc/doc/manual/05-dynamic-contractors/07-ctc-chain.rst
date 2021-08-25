.. _sec-manual-ctcchain:

.. warning::
  
  The writing of this page is in progress. Several changes are currently performed on the library.
  A new stable version of Codac will be released in the coming weeks.

**********************************************
CtcChain: :math:`\dot{x_1}=x_2,~\dot{x_2}=x_3`
**********************************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}x_{(3)}\stackrel{\int}{\rightarrow}x_{(2)}\stackrel{\int}{\rightarrow}x_{(1)}\end{array}\right. \longrightarrow \mathcal{C}_{\iint}\big([x_1](\cdot),[x_2](\cdot),[x_3](\cdot)\big)

  .. tabs::

    .. code-tab:: py

      ctc.chain.contract(x1, x2, x3)

    .. code-tab:: c++

      ctc::chain.contract(x1, x2, x3);


.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classcodac_1_1_ctc_chain.html>`_.