.. tubex-lib documentation master file, created by
   sphinx-quickstart on Sun Nov 20 11:16:37 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Tubex library
=============

Introduction
-------------

*Tubex* is a C++ library providing tools to guarantee computations over sets of trajectories. 

In the field of *constraint programming*, problems are defined thanks to a constraint network involving variables submitted to rules and known to belong to given domains. When dealing with dynamical systems, differential constraints have to be considered.
This library is a proposal to solve such problem. A *tube* is a domain for a trajectory variable (*e.g.* a signal evolving with time) for which several numerical, algebraic, non-linear, differential constraints will apply.

Functions called *contractors* are available to reduce these sets of trajectories given several constraints.

Computations stands on interval analysis and are, therefore, performed in a guaranteed way.
Tubex is compatible with the `IBEX library <http://www.ibex-lib.org/>`_, itself relying on reliable numerical libraries such as `Filib <http://www2.math.uni-wuppertal.de/~xsc/software/filib.html>`_.

.. figure:: ../img/tube_slices_small.png

Licence
-------

This software is under `GNU Lesser General Public License <https://www.gnu.org/copyleft/lgpl.html>`_.

Documentation
-------------

.. toctree::
   :maxdepth: 2

   theory
   installation
   how_to
   graphics
   examples/index

Development
-----------

For recent improvements and activities, see the `Github Tubex repository <https://github.com/SimonRohou/tubex-lib>`_.

A Python version (`pyIbex <http://www.ensta-bretagne.fr/desrochers/pyibex>`_ wrapping) is planned.