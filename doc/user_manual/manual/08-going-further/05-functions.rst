.. _sec-manual-functions:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*********
Functions
*********

.. contents::

Mathematical functions (that are not related to programming functions) are defined with `variables`.  These temporal functions are interesting for inter-temporal evaluations such as integral computations, delays, time uncertainties, and so on. This will be detailed thereafter.

The system variable :math:`t` is a predefined variable of ``Function`` objects.

An extended interval calculator
-------------------------------

Tubex allows you to declare symbolically a temporal function and to perform interval computations with it. For example:

.. tabs::

  .. code-tab:: c++

    Function f(x, y, "sin(x)+exp(t*y)");

  .. code-tab:: py

    # todo

defines the mathematical object :math:`\left(\begin{array}{c}x(t)\\y(t)\end{array}\right) \mapsto \sin\big(x(t)\big)+\exp\big(t\cdot y(t)\big)`.