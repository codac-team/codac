.. _sec-trajectories-io:

Trajectories serialization
==========================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

Importing CSV data
------------------

A ``SampledTraj`` can be filled from the following ``data.csv`` file:

.. literalinclude:: data.csv
  :language: text

Read its time/value columns and insert the samples using ``set(value,time)``. The examples locate ``data.csv`` alongside the source file.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-io-csv-beg]
      :end-before: [traj-io-csv-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-io-csv-beg]
      :end-before: [traj-io-csv-end]
      :dedent: 4

NumPy archives
--------------

In Python, load a ``.npz`` file with ``numpy.load`` and pass its time and value arrays to ``SampledTraj``. The trajectory type is inferred from the values.

.. code-block:: python

   import numpy as np
   from codac import SampledTraj

   data = np.load("traj_vec.npz")
   traj = SampledTraj(data["t"], data["x"])

In the above example, if ``x`` is a 2D array of shape ``(N,n)``, then ``traj`` is a ``SampledTraj_Vector``.

Another example:

.. tabs::

  .. group-tab:: Python

    .. code-block:: python

      import numpy as np
      from codac import *

      t = np.array([0.0, 0.5, 1.0, 1.5])
      x = np.array([
          [0.0, 1.0],
          [0.5, 1.5],
          [1.0, 2.0],
          [1.5, 2.5],
      ])

      traj = SampledTraj(t, x)
      print(traj) # outputs: SampledTraj. [0, 1.5]↦[[0, 1.5][1, 2.5]], 4 pts


Binary serialization
--------------------

``serialize`` and ``deserialize`` save and restore ``SampledTraj`` objects in binary form. The ``.cdc`` extension is conventional. An ``AnalyticTraj`` can be sampled before saving, but the symbolic expression itself is not serialized.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-serialization-beg]
      :end-before: [traj-ops-serialization-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-serialization-beg]
      :end-before: [traj-ops-serialization-end]
      :dedent: 4

See :ref:`sec-tools-serialization` for details about the binary format.

Visualization
-------------

Use ``Figure2D.plot_trajectory`` for scalar time series and ``Figure2D.plot_trajectories`` for vector time series. Spatial trajectories can be displayed with ``DefaultFigure.draw_trajectory``. See :doc:`../visualization/functions`.
