.. _sec-manual-introduction:

############
Introduction
############

Tubex is a library providing tools for constraint programming over reals and trajectories.
It has many applications in state estimation or robot localization.

.. contents::

.. include:: intro_constraint_programming.rst

------------------------------------------------------


*****************
The Tubex library
*****************

The API of Tubex can be broken down into three layers:

- an extended tube calculator
- a catalog of contractors for dynamical systems and mobile robotics
- a system solver

Each usage corresponds to a different layer and each layer is built on top of the previous one. This structure is similar to the one of IBEX, but dedicated to dynamical systems and robotic applications.

| Note that the last layer is the result of very recent developments around networks of contractors. It aims at simplifying the use of the library while increasing the performances of the contractions.
| Another parallel work is conducted on a complementary approach. See `the Contredo project <https://github.com/ibex-team/tubex-solve>`_.

Tubex has been designed by robotic researchers but provides a generic solver that has broader applications in guaranteed integration or parameter estimation.

------------------------------------------------------


****************************
What about the IBEX library?
****************************

.. Figure:: ../../img/logo_ibex.jpg
  :align: left

The `IBEX library <http://www.ibex-lib.org>`_ is a C++ software for constraint processing over real numbers.
As for Tubex, it stands on Constraint Programming but focuses on static contexts, providing reliable algorithms for handling non-linear constraints.

It also proposes various tools such as the *IbexSolve* and *IbexOpt* plugins that are dedicated to system solving and optimization, and come both with a default black-box solver and global optimizer for immediate usage.

Tubex is built upon IBEX and uses the elementary components of this library such as interval objects, arithmetic implementations, or contractors for static constraints. More precisely, Tubex extends the *contractor programming* framework of IBEX to the dynamical case, introduces trajectories and tubes objects, and provides another kind of solver for heterogeneous systems made of both static and dynamical constraints.

If the use of IBEX is transparent in several robotic applications presented in this manual, it is still possible to build complex static contractors with IBEX and use them in Tubex. Hence, IBEX can be used as a **powerful contractor factory** for static systems.


------------------------------------------------------

********************************
Using IBEX in Python with pyIbex
********************************

.. Figure:: ../../img/logo_pyibex.jpg
  :align: left

A python API has been developed for IBEX: the `pyIbex library <http://benensta.github.io/pyIbex/>`_.
It makes available in Python many C++ functionalities of IBEX that are used in robotic applications such as contractors and separators. It aims at combining the flexibility of Python with the speed of C++ algorithms.

Tubex is available in both C++ and Python. It stands on pyIbex for the use of IBEX objects such as intervals and static contractors.