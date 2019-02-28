.. tubex-lib documentation master file

Tubex library
=============


Introduction
------------

Tubex is a C++ library providing tools for computations over sets of trajectories.
It has been designed to deal with **dynamical systems** defined by **non-linear differential equations** and involving constraints such as time uncertainties or **delays**.

The building blocks of this library are *tubes*, that represent domains for **trajectory** variables (*e.g.* signals evolving with time). When a trajectory :math:`x(\cdot)` comes with uncertainties (due to numerical approximations or errors coming from sensors), a reliable enclosure of it can be made thanks to a tube :math:`[x](\cdot)`.

A tube is defined as an interval of two trajectories. Its bounds delineate the **uncertainties** of a dynamical system, thus defining a solution set.
Then, any operation related to :math:`x(\cdot)` will be made on its reliable enclosure :math:`[x](\cdot)`, which allows **guaranteed outputs**. These computations stand on interval analysis, a well suited tool that reliably propagates uncertainties.

**Mobile robotics** has been the initial motivation of this project: the mathematical tools provided in Tubex come together with robotic applications.

Computations stands on the `IBEX library <http://www.ibex-lib.org/>`_ that provides reliable tools for static systems.

.. figure:: ../img/tube_slices_small.png



Related publications
--------------------

- `Guaranteed computation of robot trajectories <http://simon-rohou.fr/research/tubint/tubint_paper.pdf>`_, *Robotics and Autonomous Systems*, 93:76-84, Rohou, S., Jaulin, L., Mihaylova, L., Le Bars, F., and Veres, S. M. (2017)

- `Reliable non-linear state estimation involving time uncertainties <http://simon-rohou.fr/research/tubeval/tubeval_paper.pdf>`_, *Automatica*, 93:379-388, Rohou, S., Jaulin, L., Mihaylova, L., Le Bars, F., and Veres, S. M. (2017)

- `Proving the existence of loops in robot trajectories <http://simon-rohou.fr/research/loopproof/loopproof_paper.pdf>`_, *The International Journal of Robotics Research*, Rohou, S., Franek, P., Aubry, C., and Jaulin, L. (2018)



Documentation
-------------

.. toctree::
   :maxdepth: 1

   theory
   installation
   how_to
   graphics
   examples/index

- `API technical documentation <./doxygen/html/annotated.html>`_ 



Licence and development
-----------------------

This software is under `GNU Lesser General Public License <https://www.gnu.org/copyleft/lgpl.html>`_.

For recent improvements and activities, see the `Github Tubex repository <https://github.com/SimonRohou/tubex-lib>`_.
A Python version (`pyIbex <http://www.ensta-bretagne.fr/desrochers/pyibex>`_ wrapping) is planned.