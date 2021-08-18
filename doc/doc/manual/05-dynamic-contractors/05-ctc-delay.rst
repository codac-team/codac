.. _sec-manual-ctcdelay:

.. warning::
  
  The writing of this page is in progress. Several changes are currently performed on the library.
  A new stable version of Codac will be released in the coming weeks.

*****************************
CtcDelay: :math:`x(t)=y(t+a)`
*****************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}x(t)=y(t+a)\end{array}\right. \longrightarrow \mathcal{C}_{\textrm{delay}}\big([a],[x](\cdot),[y](\cdot)\big)

  .. tabs::

    .. code-tab:: py

      ctc.delay.contract(a, x, y)

    .. code-tab:: c++

      ctc::delay.contract(a, x, y);


.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classcodac_1_1_ctc_delay.html>`_.