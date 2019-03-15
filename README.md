# [Tubex library](http://simon-rohou.fr/research/tubex-lib) [![Build Status](https://travis-ci.org/SimonRohou/tubex-lib.svg)](https://travis-ci.org/SimonRohou/tubex-lib)

Tubex is a C++ library providing tools for computations over sets of trajectories.
It has been designed to deal with **dynamical systems** defined by **non-linear differential equations** and involving constraints such as time uncertainties or **delays**.

The building blocks of this library are *tubes*, that represent domains for **trajectory** variables (*e.g.* signals evolving with time). When a trajectory comes with uncertainties (due to numerical approximations or errors coming from sensors), a reliable enclosure of it can be made thanks to a tube.

A tube is defined as an interval of two trajectories. Its bounds delineate the **uncertainties** of a dynamical system, thus defining a solution set.
Then, any operation related to the trajectory will be made on its reliable enclosure, which allows **guaranteed outputs**. These computations stand on interval analysis, a well suited tool that reliably propagates uncertainties.

**Mobile robotics** has been the initial motivation of this project: the mathematical tools provided in Tubex come together with robotic applications.

Computations stands on the [IBEX library](http://www.ibex-lib.org/) that provides reliable tools for static systems.

**See the [official page](http://simon-rohou.fr/research/tubex-lib)**.

![Image of Tube](https://cdn.rawgit.com/SimonRohou/tubex-lib/master/doc/img/tube_slices_small.png)



Related publications
--------------------

* [Guaranteed computation of robot trajectories](http://simon-rohou.fr/research/tubint/tubint_paper.pdf)<br />*Robotics and Autonomous Systems*, 93:76-84<br />Rohou, S., Jaulin, L., Mihaylova, L., Le Bars, F., and Veres, S. M. (2017)

* [Reliable non-linear state estimation involving time uncertainties](http://simon-rohou.fr/research/tubeval/tubeval_paper.pdf)<br />*Automatica*, 93:379-388<br />Rohou, S., Jaulin, L., Mihaylova, L., Le Bars, F., and Veres, S. M. (2017)

* [Proving the existence of loops in robot trajectories](http://simon-rohou.fr/research/loopproof/loopproof_paper.pdf)<br />*The International Journal of Robotics Research*, 37(12):1500-1516<br />Rohou, S., Franek, P., Aubry, C., and Jaulin, L. (2018)



Documentation
-------------

* [User guide](http://simon-rohou.fr/research/tubex-lib)
* [API technical documentation](http://simon-rohou.fr/research/tubex-lib/api)



Licence and development
-----------------------

This software is under [GNU Lesser General Public License](https://www.gnu.org/copyleft/lgpl.html).

For recent improvements and activities, see the [Github Tubex repository](https://github.com/SimonRohou/tubex-lib).
A Python version ([pyIbex](http://www.ensta-bretagne.fr/desrochers/pyibex) wrapping) is planned.