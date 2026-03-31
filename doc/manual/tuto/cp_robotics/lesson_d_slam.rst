.. _sec-tuto-cprob-lesson-d:

Lesson D: SLAM
==============

  Main authors: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

We propose to apply interval tools to solve a classical state estimation problem of Simultaneous Localization And Mapping (SLAM).

A tank robot :math:`\mathcal{R}`, whose state vector
:math:`\mathbf{x}\in\mathbb{R}^3` describes its position
:math:`(x_1,x_2)^\intercal` and its heading :math:`x_3`, is evolving among a set of
landmarks :math:`\mathbf{b}^k\in\mathbb{R}^2` with a constant speed :math:`v=10`. It is equipped with a compass for
measuring its heading :math:`x_3` with bounded uncertainty.

Formalism
---------

We are addressing the following state estimation problem:

.. math::
  :label: eq-state

  \left\{
  \begin{array}{lll}
  \dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),u(t)\big) &  & \textrm{(evolution equation)}\\
  d_i^k=g\big(\mathbf{x}(t_i),\mathbf{b}^k\big) &  & \textrm{(observation equation)}
  \end{array}
  \right.

The evolution of the state is given by:

.. math::
  :label: eq-f

  \dot{\mathbf{x}}=
  \mathbf{f}(\mathbf{x},u)=\left(
  \begin{array}{c}
  10\cos(x_3) \\
  10\sin(x_3) \\
  u
  \end{array}
  \right),

and the system input :math:`u` (rotation rate) is expressed as:

.. math::

  u(t) = 3\sin^2(t)+\frac{t}{100}.

The observation function :math:`g` is the distance function between a position
:math:`(x_1,x_2)^\intercal` and a landmark :math:`(b_1^k,b_2^k)^\intercal`.

.. admonition:: Exercise

  **D.1.** Update your current version of Codac. In Python, for instance:

  .. code-block:: bash

    pip install codac --upgrade --pre
    # Option --pre has to be set because Codac v2 is only available in pre-release

  To verify that you have the correct version, you can use:

  .. code-block:: bash

    python -m pip show codac

    > Name: codac
      Version: 2.0.0.dev27

  The required version for this tutorial is 2.0.0.dev27 or later.

The following instructions are given in Python, but feel free to use C++ or Matlab if you prefer.

Simulation
----------

The following code provides a simulation of the actual but unknown trajectory :math:`\mathbf{x}^*(\cdot)`, without uncertainties.
This code can be used as a starting point for your project.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src/lesson_D.py
      :language: py
      :start-after: [D-q1-beg]
      :end-before: [D-q1-end]
      :dedent: 0

  .. group-tab:: C++

    .. literalinclude:: src/lesson_D.cpp
      :language: c++
      :start-after: [D-q1-beg]
      :end-before: [D-q1-end]
      :dedent: 2

  .. group-tab:: Matlab

    .. literalinclude:: src/lesson_D.m
      :language: matlab
      :start-after: [D-q1-beg]
      :end-before: [D-q1-end]
      :dedent: 0

Using VIBes, you should visualize these states:

.. figure:: img/slam_0.png
  :width: 55%
  
  Unknown actual trajectory, to be estimated.

Deadreckoning
-------------

The robot knows its initial state: :math:`\mathbf{x}(0)=(0,0,0)^\intercal`.
Deadreckoning consists in estimating the robot’s subsequent positions without
exteroceptive measurements (*i.e.* without distances from the landmarks). In this
section, we will compute the set of feasible positions of :math:`\mathcal{R}`,
considering only heading measurements and the evolution function :math:`\mathbf{f}`.

.. admonition:: Exercise
  
  **D.2. Domains.**
  The set of feasible positions along time is a *tube* (interval of trajectories).
  We create a tube :math:`[\mathbf{x}](\cdot)` using:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q2-beg]
        :end-before: [D-q2-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q2-beg]
        :end-before: [D-q2-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q2-beg]
        :end-before: [D-q2-end]
        :dedent: 0

  where ``tdomain`` is the temporal discretization over :math:`[t_0,t_f]`,
  and ``dt`` is the discretization parameter. Here, ``x`` is a 3d tube.

  At this point, :math:`\forall t\in[t_0,t_f],~ [\mathbf{x}](t)=[-\infty,\infty]^3`:
  the states are completely unknown.

  This can be verified, for instance at :math:`t=0`, with:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q2b-beg]
        :end-before: [D-q2b-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q2b-beg]
        :end-before: [D-q2b-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q2b-beg]
        :end-before: [D-q2b-end]
        :dedent: 0

.. admonition:: Exercise

  **D.3. Heading measurements.**
  In practice, the headings :math:`x_3(\cdot)` are measured with some uncertainties known to be
  bounded in :math:`[-0.03,0.03]`. We set these bounded measurements in the last
  component of the tube vector :math:`[\mathbf{x}](\cdot)` with:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q3-beg]
        :end-before: [D-q3-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q3-beg]
        :end-before: [D-q3-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q3-beg]
        :end-before: [D-q3-end]
        :dedent: 0

.. admonition:: Exercise

  **D.4.  Initial condition.**
  The initial state :math:`\mathbf{x}(0)` (position and heading) is known, which
  can be implemented in the tube with:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q4-beg]
        :end-before: [D-q4-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q4-beg]
        :end-before: [D-q4-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q4-beg]
        :end-before: [D-q4-end]
        :dedent: 0

Now that a domain (a tube) has been defined for enclosing the estimates together
with their uncertainties, it remains to define contractors for narrowing their bounds.

In deadreckoning, only Eq. :eq:`eq-f` is considered:
:math:`\dot{\mathbf{x}}(\cdot)=\mathbf{f}(\mathbf{x}(\cdot))`. This can be processed with
two contractors, one for dealing with :math:`\mathbf{v}(\cdot)=\mathbf{f}(\mathbf{x}(\cdot))`,
and one for :math:`\dot{\mathbf{x}}(\cdot)=\mathbf{v}(\cdot)`.

The new tube :math:`[\mathbf{v}](\cdot)` will enclose the feasible derivatives of the
possible states in :math:`[\mathbf{x}](\cdot)`.

.. admonition:: Exercise

  **D.5. Derivatives of the state.** As for :math:`[\mathbf{x}](\cdot)`, create another 3D ``SlicedTube`` for
  :math:`[\mathbf{v}](\cdot)`, called ``v``.

.. admonition:: Exercise

  **D.6. Contractors.** Then, create a contractor for :math:`\mathbf{v}(\cdot)=\mathbf{f}(\mathbf{x}(\cdot))`. The associated constraint is expressed as an implicit form :math:`\mathbf{f}(\mathbf{x}(\cdot),\mathbf{v}(\cdot))=\mathbf{0}`.

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q6-beg]
        :end-before: [D-q6-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q6-beg]
        :end-before: [D-q6-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q6-beg]
        :end-before: [D-q6-end]
        :dedent: 0

  Create a contractor for :math:`\dot{\mathbf{x}}(t)=\mathbf{v}(t)`:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q6b-beg]
        :end-before: [D-q6b-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q6b-beg]
        :end-before: [D-q6b-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q6b-beg]
        :end-before: [D-q6b-end]
        :dedent: 0

We recall that contractors are algorithms for reducing sets of feasible values (intervals, boxes,
tubes..).

.. admonition:: Exercise

  **D.7. Deadreckoning estimation.** At this point, you can implement an algorithm for deadreckoning, by calling the previously defined contractors.
  
  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q7-beg]
        :end-before: [D-q7-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q7-beg]
        :end-before: [D-q7-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q7-beg]
        :end-before: [D-q7-end]
        :dedent: 0

.. admonition:: Exercise

  **D.8. Display.** Finally, the contracted tube :math:`[\mathbf{x}](t)` can be displayed with:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q8-beg]
        :end-before: [D-q8-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q8-beg]
        :end-before: [D-q8-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q8-beg]
        :end-before: [D-q8-end]
        :dedent: 0

You should obtain the following result:

.. figure:: img/slam_1.png
  :width: 55%

  In gray: tube :math:`[\mathbf{x}](t)` enclosing the estimated trajectories of the
  robot. The actual but unknown trajectory is depicted in black. With interval methods,
  the computations are guaranteed: the actual trajectory cannot be outside the tube.
  However, the tube may be large in case of poor localization, as is the case so far without state observations.

Range-only localization
-----------------------

The obtained tube grows with time, illustrating a significant drift of the robot.
We now rely on distances measured from known landmarks to reduce this drift.
This amounts to a non-linear state estimation problem: function :math:`g` in
System :eq:`eq-state` is a distance function. Non-linearities can be difficult
to solve with conventional methods.

.. admonition:: Exercise

  **D.9. Landmarks.** Define five landmarks with:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q9-beg]
        :end-before: [D-q9-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q9-beg]
        :end-before: [D-q9-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q9-beg]
        :end-before: [D-q9-end]
        :dedent: 0

.. admonition:: Exercise

  **D.10. Range observations.** In a loop, for each :math:`t_i\in\{0,1,\dots,15\}`, compute the distance measured
  from a random landmark:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q10-beg]
        :end-before: [D-q10-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q10-beg]
        :end-before: [D-q10-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q10-beg]
        :end-before: [D-q10-end]
        :dedent: 0

.. admonition:: Exercise

  **D.11. Measurement uncertainty.** The measurements come with some errors (not computed here) that are known to be
  bounded within :math:`[-0.03,0.03]`. We use intervals for enclosing the observations:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q11-beg]
        :end-before: [D-q11-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q11-beg]
        :end-before: [D-q11-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q11-beg]
        :end-before: [D-q11-end]
        :dedent: 0

As in the previous lessons, a fixpoint function can be used to manage the contractors automatically.
In another ``for`` loop, we will combine contractors using a fixpoint method, in order to improve the localization of the robot.

.. admonition:: Exercise

  **D.12. Observation contractors.** The state observations are added to the
  set of constraints by means of a new distance contractor linking the state to the position of a landmark:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q12-beg]
        :end-before: [D-q12-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q12-beg]
        :end-before: [D-q12-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q12-beg]
        :end-before: [D-q12-end]
        :dedent: 0

.. admonition:: Exercise

  **D.13. Range-only localization.** 
  The contractor network is now as follows:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q13-beg]
        :end-before: [D-q13-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q13-beg]
        :end-before: [D-q13-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q13-beg]
        :end-before: [D-q13-end]
        :dedent: 0

.. admonition:: Exercise

  **D.14. Display the contracted tube.** 

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_D.py
        :language: py
        :start-after: [D-q14-beg]
        :end-before: [D-q14-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_D.cpp
        :language: c++
        :start-after: [D-q14-beg]
        :end-before: [D-q14-end]
        :dedent: 2

    .. group-tab:: Matlab

      .. literalinclude:: src/lesson_D.m
        :language: matlab
        :start-after: [D-q14-beg]
        :end-before: [D-q14-end]
        :dedent: 0

You should obtain the following result:

.. figure:: img/slam_2.png
  :width: 55%

  In gray: the former tube without state observations. In blue, the new
  contracted tube, considering distance measurements from the landmarks.

.. _fig-deadrecko:

.. admonition:: Exercise

  | **D.15. Unknown initial condition?** 
  | *What if we have no knowledge about the initial position of the robot?*

  Try to remove the condition set in Exercise **D.4.**, and zoom towards the initial position.


Challenge: Simultaneous Localization and Mapping (SLAM)
-------------------------------------------------------

.. admonition:: Exercise

  **D.16. Range-only SLAM.**

  Adapt the script in order to deal with a SLAM problem, in which the positions of the
  landmarks are a priori unknown, but then estimated together with the positions of the robot.

  For this estimation, we will need more observations: we now assume that distance measurements are obtained every 0.1s.

  Note that in SLAM, the initial condition set in Exercise **D.4.** is mandatory.