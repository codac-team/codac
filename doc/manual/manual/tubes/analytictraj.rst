.. _sec-trajectories-analytictraj:

Analytic trajectories
=====================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

``AnalyticTraj`` represents a trajectory defined by a function of time rather than by stored samples. It combines a temporal domain and an ``AnalyticFunction`` with one scalar input.

Construction
------------

Define a ``ScalarVar`` for time, create an ``AnalyticFunction`` and pass the domain before the function to ``AnalyticTraj``.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-analytic-create-beg]
      :end-before: [traj-analytic-create-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-analytic-create-beg]
      :end-before: [traj-analytic-create-end]
      :dedent: 4

Evaluation
----------

``x(t)`` evaluates the function at a given time. ``x([a,b])`` computes an interval enclosure over the time interval using, by default, the intersection of the natural and centered forms to reduce overestimation. If the centered form is unavailable, only the natural evaluation is used.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-analytic-eval-beg]
      :end-before: [traj-analytic-eval-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-analytic-eval-beg]
      :end-before: [traj-analytic-eval-end]
      :dedent: 4

Sampling
--------

``sampled(dt)`` constructs a ``SampledTraj`` at regular instants, including the final time. The resulting interpolation is generally different from the original analytic function between samples.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-analytic-sampled-beg]
      :end-before: [traj-analytic-sampled-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-analytic-sampled-beg]
      :end-before: [traj-analytic-sampled-end]
      :dedent: 4

Other operations
----------------

``tdomain()``, ``codomain()``, ``size()`` and ``truncate_tdomain()`` provide the usual trajectory properties. ``sampled_as(reference)`` evaluates at the reference timestamps. ``as_function()`` embeds the trajectory into an analytic expression.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-analytic-advanced-beg]
      :end-before: [traj-analytic-advanced-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-analytic-advanced-beg]
      :end-before: [traj-analytic-advanced-end]
      :dedent: 4

For numerical integration with ``primitive(dt)``, see :doc:`trajectory_operations`. An analytic trajectory may also initialize a :doc:`slicedtube`. Its symbolic expression is not serialized directly. See :doc:`trajectory_io`.
