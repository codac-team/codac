.. _sec-manual-functions:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently performed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*********
Functions
*********

.. contents::

Mathematical functions (that are not related to programming functions) are defined with `variables`.


An extended interval calculator
-------------------------------

Tubex allows you to declare symbolically a function and to perform interval computations with it. For example:

.. tabs::

  .. code-tab:: py

    f = TFunction(x, y, "sin(x)+exp(t*y)")

  .. code-tab:: c++

    TFunction f(x, y, "sin(x)+exp(t*y)");

defines the mathematical object :math:`\left(\begin{array}{c}x(t)\\y(t)\end{array}\right) \mapsto \sin\big(x(t)\big)+\exp\big(t\cdot y(t)\big)`.


In Tubex we distinct two kinds of functions:

* *static* functions, such as :math:`\mathbf{x}\mapsto\sin(x_1)+\cos(\exp(x_2))`, implemented by ``Function`` objects;
* *temporal* functions that are time related. These *t*-functions are made for inter-temporal evaluations such as integral computations, delays, time uncertainties, and so on. The system variable :math:`t` is a predefined variable of ``TFunction`` objects.


