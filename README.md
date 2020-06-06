# [Tubex library](http://simon-rohou.fr/research/tubex-lib) [![Build Status](https://travis-ci.org/SimonRohou/tubex-lib.svg)](https://travis-ci.org/SimonRohou/tubex-lib)


Tubex is a library providing tools for **constraint programming** over reals and trajectories. It has many applications in **state estimation** or **robot localization**.

| **What is constraint programming?**
| In this paradigm, users concentrate on the properties of a solution to be found (*e.g.* the pose of a robot, the location of a landmark) by stating **constraints on the variables**. Then, a solver performs constraint propagation on the variables and provides a **reliable** set of feasible solutions corresponding to the considered problem. In this approach, the user concentrates on *what* is the problem instead of *how* to solve it, thus leaving the computer dealing with the *how*. The strength of this declarative paradigm lies in its **simpleness**, as it allows one to describe a complex problem without requiring the knowledge of resolution tools coming with specific parameters to choose.

| **What about mobile robotics?**
| In the field of robotics, complex problems such as **non-linear state estimation**, **delays**, **SLAM** or **kidnapped robot problems** can be solved in a very few steps by using constraint programming. Even though the Tubex library is not meant to target only robotics problems, the design of its interface has been largely influenced by the needs of the above class of applications. Tubex provides solutions to deal with these problems, that are usually hardly solvable by conventional methods such as particle approaches or Kalman filters. 

In a nutshell, Tubex is a high-level **constraint programming framework** providing tools to easily solve a wide range of robotic problems.

**See the [official page](http://simon-rohou.fr/research/tubex-lib)**.

![Image of Tube](https://cdn.rawgit.com/SimonRohou/tubex-lib/master/doc/user_manual/img/tube_slices_small.png)


Related publications
--------------------

* [Guaranteed computation of robot trajectories](http://simon-rohou.fr/research/tubint/tubint_paper.pdf)<br />*Robotics and Autonomous Systems*, 93:76-84 (2017)<br />Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres

* [Reliable non-linear state estimation involving time uncertainties](http://simon-rohou.fr/research/tubeval/tubeval_paper.pdf)<br />*Automatica*, 93:379-388 (2017)<br />Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres

* [Proving the existence of loops in robot trajectories](http://simon-rohou.fr/research/loopproof/loopproof_paper.pdf)<br />*The International Journal of Robotics Research*, 37(12):1500-1516 (2018)<br />Simon Rohou, Peter Franek, Clément Aubry, Luc Jaulin

* [[book] Reliable robot localization: a constraint-programming approach over dynamical systems](http://iste.co.uk/book.php?id=1553)<br />*ISTE Ltd*, *Wiley* (2019)<br />Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres


Documentation
-------------

* [User guide](http://simon-rohou.fr/research/tubex-lib)
* [API technical documentation](http://simon-rohou.fr/research/tubex-lib/doxygen/html/annotated.html)


License and development
-----------------------

This software is under [GNU Lesser General Public License](https://www.gnu.org/copyleft/lgpl.html).

A Python version ([pyIbex](http://www.ensta-bretagne.fr/desrochers/pyibex) wrapping) is planned.