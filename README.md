# tubex-lib [![Build Status](https://travis-ci.org/SimonRohou/tube-lib.svg)](https://travis-ci.org/SimonRohou/tube-lib)

*Tubex* is a C++ library providing tools to guarantee computations over sets of trajectories. 

In the field of *constraint programming*, problems are defined thanks to a constraint network involving variables submitted to constraints and known to belong to given domains. When dealing with dynamical systems, differential constraints have to be considered.
This library is a proposal to solve such problem. A *tube* is a domain for a trajectory variable (*e.g.* a signal evolving with time) for which several numerical, algebraic, non-linear, differential constraints will apply.

Functions called *contractors* are available to reduce these sets of trajectories given several constraints.

Computations stands on interval analysis and are, therefore, performed in a guaranteed way.
Tubex stands on the [IBEX library](http://www.ibex-lib.org), itself relying on reliable numerical libraries such as [Filib](http://www2.math.uni-wuppertal.de/~xsc/software/filib.html).

Note: a Python version ([pyIbex](http://www.ensta-bretagne.fr/desrochers/pyibex) wrapping) is planned.


![Image of Tube](http://rawgit.com/SimonRohou/tubex-lib/master/doc/img/tube_slices.png)