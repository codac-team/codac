.. _sec-trajectories-sampledtraj:

Sampled trajectories
====================

In Python, the ``SampledTraj`` name is a convenience wrapper that mimics the
C++ template deduction style. Depending on the provided samples, it builds and
returns one of the following objects:

- ``SampledTraj_Scalar``
- ``SampledTraj_Vector``
- ``SampledTraj_Matrix``

This keeps the user-facing syntax short and consistent with ``AnalyticTraj``.


Loading a sampled trajectory from a ``.npz`` file
-------------------------------------------------

A sampled trajectory cannot be constructed directly from a file path.
The recommended workflow is:

1. load the ``.npz`` file with ``numpy.load``;
2. extract the arrays containing the sampling times and sampled values;
3. call ``SampledTraj(t, x)``.

The wrapper then deduces the appropriate trajectory type from ``x``.

.. tabs::

  .. group-tab:: Python

    .. code-block:: python

      import numpy as np
      from codac import *

      data = np.load("traj_vec.npz")
      traj = SampledTraj(data["t"], data["x"])

      print(type(traj))

In the above example, if ``x`` is a 2D array of shape ``(N,n)``, then
``traj`` is a ``SampledTraj_Vector``.

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