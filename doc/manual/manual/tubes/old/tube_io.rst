.. _sec-domains-tubes-io:

Tube input, output and visualization
====================================

[même commentaire que précédemment, je doute de la pertinence de cette page, il vaudrait mieux transférer vers les sections dédiées]

From time series
----------------

A ``SlicedTube`` can enclose the interpolation of a ``SampledTraj`` on a selected ``TDomain``. Inflate it by a justified uncertainty bound when the tube is intended to enclose an unknown signal, rather than just the interpolant.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-parity-input-beg]
      :end-before: [tube-parity-input-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-parity-input-beg]
      :end-before: [tube-parity-input-end]
      :dedent: 4

Inspecting and saving tubes
---------------------------
[retirer cette section. On fera un jour les fonctions de serialization, mais pas maintenant. On retire]

For inspection, slice bounds can be written to a text file. The example below exports the temporal supports, codomains and gate indicators. This is not a native tube serialization format.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-parity-export-beg]
      :end-before: [tube-parity-export-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-parity-export-beg]
      :end-before: [tube-parity-export-end]
      :dedent: 4

The current binary ``serialize`` API supports ``SampledTraj`` but not direct serialization of ``SlicedTube`` or its shared ``TDomain``. Saving ``tube.mid()`` stores only an approximate trajectory, not the tube. See :doc:`trajectory_io` and :ref:`sec-tools-serialization`.

Visualization
-------------

Use ``Figure2D.plot_tube`` to draw interval enclosures over time, or ``Figure2D.plot_trajectory`` for a nominal signal. See :doc:`../visualization/functions`.
