Tubes
=====

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

A tube :math:`[x](\cdot)` represents the set of trajectories satisfying :math:`x(t)\in[x](t)` at every time. Its codomain is typically an interval or a box, and contractors can reduce these enclosures using observations and dynamical constraints without discarding feasible trajectories.

A ``SlicedTube`` represents a tube over a temporal partition :class:`~codac.TDomain`. Each time element is a :class:`~codac.TSlice`, with an associated :class:`~codac.Slice` for each tube. Several tubes may share one temporal partition, so refining it also updates the structure of the other tubes.

The principal codomains are ``Interval``, ``IntervalVector`` and ``IntervalMatrix``. They represent scalar, vector and matrix sliced tubes. C++ also supports compatible custom codomains.

.. figure:: ./tube_slices.png
  :width: 70%

  A sliced tube :math:`[x](\cdot)`, represented by interval enclosures over a temporal partition.

See :doc:`tdomain` for the shared temporal partition, :doc:`slice` for gates and :doc:`slicedtube` for construction, evaluation and operations. Practical examples are given in :doc:`tube_examples`.
