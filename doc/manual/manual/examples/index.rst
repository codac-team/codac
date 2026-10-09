.. _sec-examples:

Examples
========

The `examples <https://github.com/codac-team/codac/tree/codac2/examples>`_
directory of the repository holds around twenty complete, self-contained
programs. They are worth reading as a complement to this manual: each one is a
short program that does one thing, and most of them exist in C++, in Python and
sometimes in MATLAB, so they double as a translation table between the three
interfaces.

They are also continuously checked. Configuring the project with
``-D TEST_EXAMPLES=ON`` compiles every one of them against the library being
built and registers each as a CTest integration test, so an example that stops
working is caught at the same time as a failing unit test. See
:ref:`sec-dev-info-examples`.


Building and running them
-------------------------

Every C++ example ships a standalone ``CMakeLists.txt``, written exactly the way
a project of your own would be (see :ref:`sec-start-cpp-project`). Against an
installed Codac:

.. code-block:: bash

  cd $HOME/codac/examples/01_batman
  mkdir build ; cd build
  cmake -DCMAKE_BUILD_TYPE=Release ..
  cmake --build .
  ./codac_example

The Python examples need no build step:

.. code-block:: bash

  cd $HOME/codac/examples/03_sivia
  python main.py

.. admonition:: Graphical outputs

  Most examples draw something. Those producing a 2D figure need
  :ref:`the VIBes viewer <sec-graphics-vibes>` to be running beforehand, and
  write an ``.ipe`` file as well, readable with :ref:`the IPE editor
  <sec-graphics-ipe>`. Those producing a 3D figure write an ``.obj`` file, which
  can be visualized on `3dviewer.net <https://3dviewer.net>`_ or with any 3D
  model viewer. See :ref:`sec-graphics`.


Graphics
--------

``00_graphics``
^^^^^^^^^^^^^^^

Three programs covering the drawing API: ``graphic_examples``
(C++/Python/MATLAB) goes through a wide variety of drawing functions, colors and
styles; ``graphic_animation`` (C++/Python) redraws a figure in a loop, clearing
it between frames; ``graphic_colors`` (C++/Python) displays the predefined
colors of the library.

These are the sources the :ref:`sec-graphics-2d-example` page points at.
The reference pages for what they use are :ref:`sec-graphics-functions` and
:ref:`sec-graphics-colors`.

``06_graphics_3D``
^^^^^^^^^^^^^^^^^^

The 3D counterpart, in C++, Python and MATLAB, covering the drawing functions of
``Figure3D``. As the source says:

  The generated ``.obj`` files can be visualized on https://3dviewer.net

See :ref:`sec-graphics-3d` and :ref:`sec-graphics-3d-example`.


Set inversion and paving
------------------------

``01_batman`` *(C++)*
^^^^^^^^^^^^^^^^^^^^^

The set inversion that produces the Batman logo. This is the example
:ref:`the C++ installation page <sec-install-cpp>` suggests running first to
check an installation.

It is also, at the moment, the only place where the set-membership functions are
shown at work: half of the logo is described as a ``SetFunction`` combining
``inverse()`` of four ``AnalyticFunction`` objects with ``&``, ``|`` and
``not``, the other half is its image by the symmetry ``OctaSym({-1,2})``, and the
separator of the whole is obtained with ``create_sep()``. See
:ref:`sec-tools-octasym`.

``03_sivia`` *(C++/Python/MATLAB)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The shortest possible set inversion, and a good first program to read: five lines
to characterize :math:`\{\mathbf{x} \mid x_1^2\sin(x_1^2+x_2^2)-x_2^2 \geqslant
0\}` with the ``sivia`` function, then draw the resulting paving. See
:ref:`sec-intro-pavings`.

``02_centered_form`` *(C++/Python/MATLAB)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The equation system of the home page of this manual, solved with
``CtcInverse``. From the source:

  Example from the publication:
  https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

The directory holds two variants that its own ``CMakeLists.txt`` does not build,
and that are meant to be compiled by hand: ``main_rump.cpp``, from
https://www.tuhh.de/ti3/rump/intlab/demos/html/dglobal.html#1, and
``main_evans.cpp``. ``main_parabolas`` (C++/Python) applies the same centered
form to a family of parabolas.

``07_centered_2D`` and ``08_centered_3D`` *(C++)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Representation of the centered form with zonotopes, in the plane (on Fermat's
spiral) and in space (on a torus-like surface). See :ref:`sec-zonotope` and
:ref:`sec-functions-parallelepiped-eval`.

``16_visibility`` *(C++/Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Paving of ``SepVisible``: from an observation point and a list of obstacle
segments, the set of points that can be seen. See :ref:`sec-ctc-geom-ctcvisible`.

``13_qinter`` *(C++/Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Robust localization from three distance measurements, one of which may be an
outlier, using ``SepQInter``. From the source:

  | Example from:
  | Robust Localisation Using Separators
  | Luc Jaulin and Benoît Desrochers

.. note::

  The meaning of the parameter ``q`` changed in version 2.1.1: it is now the
  number of constraints that are *allowed to be violated*, where it used to be
  the number that had to be satisfied. See :ref:`sec-dev-changelog`.

``custom_sep`` *(Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^

Three short scripts showing how to extend the catalog from Python rather than
from C++, by deriving from the ``Sep`` and ``Ctc`` base classes.
``custom_sep.py`` defines a separator as the complement of another one --
returning a ``BoxPair`` with the inner and outer boxes permuted; ``custom_ctc.py``
does the same for a contractor.

``coloration.py`` goes further and is the one example that shows what a paving is
made of: a custom contractor is paired with two inverse contractors in a
``SepCtcPair``, the resulting paving is queried for its connected subsets, and its
tree is then walked with a visitor that recolors each node in place.


Dynamical systems
-----------------

``14_lohner`` *(C++/Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Guaranteed integration of :math:`\dot{\mathbf{x}} = (-x_1, -\sin(x_2))` with
``CtcLohner``, on two tubes built on time partitions of different samplings, which
shows the effect of the sampling on the width of the result. See
:ref:`sec-ctc-dynamic-ctclohner` and :ref:`sec-domains-tubes`.

``05_capd_solver`` *(C++, needs CAPD)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Integration through the CAPD interface. From the source:

  | Author: Maël Godard
  | Adapted from CAPD examples

See :ref:`sec-extensions-capd`.

``10_lie_groups`` *(C++, needs CAPD)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

From the source:

  | Codac example - Integration using Lie symmetries
  |
  | Author: Simon Rohou (2025), from the thesis of Julien Damers
  |
  | Reference: *Lie symmetries applied to interval integration*, Julien Damers,
    Luc Jaulin, Simon Rohou. Automatica, Volume 144, October 2022

``09_robot_simu`` *(C++/Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Simulation of a robot following a list of waypoints with ``RobotSimulator``,
producing the state and input trajectories, then drawing the tank and its path.

``04_explored_area`` *(C++/Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

From the source:

  Example: A sampled trajectory, ``sampled_f`` (composed of time-stamped
  positions with linear interpolation between them), is first obtained by
  discretizing an analytical expression (the function ``f``, which represents a
  Lissajous curve) and then appending an additional position. This trajectory is
  subsequently used in another analytical expression (function ``h``). The
  projection of an inverse separator is then employed to validate the result.


Wrapping and enclosures
-----------------------

``11_peibos`` *(C++/Python/MATLAB)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The PEIBOS algorithm in 2D (on the Hénon map) and in 3D, run on several threads
with ``set_nb_threads(max_threads())``. See :ref:`sec-functions-peibos` and
:ref:`sec-tools-threading`.

``12_peibos_capd`` *(C++, needs CAPD)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

PEIBOS applied to the flow of a differential equation integrated with CAPD. See
:ref:`sec-extensions-capd-peibos`.

``ellipsoid_example`` *(C++/Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The figures of :ref:`sec-ellipsoids`, in one program: linear and nonlinear
mappings of an ellipsoid, projections of a 3D ellipsoid onto the three planes,
the singular and degenerate cases, and a stability analysis on a pendulum.


Symbolic computation
--------------------

``15_sympy`` *(C++/Python)*
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Partial differentiation of an ``AnalyticFunction`` through SymPy, with
``sympy_partial_diff``. See :ref:`sec-extensions-sympy`.
