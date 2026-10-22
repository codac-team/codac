.. _sec-trajectories-sampledtraj:

Sampled trajectories
====================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

``SampledTraj<T>`` stores time/value pairs :math:`(t_i,x_i)` with distinct, ordered timestamps. Between samples, the trajectory is linearly interpolated:

.. math::

   x(t)=x_i+\frac{t-t_i}{t_{i+1}-t_i}(x_{i+1}-x_i),\qquad t\in[t_i,t_{i+1}].

The codomain ``T`` can be a real, a vector or a matrix. In both C++ and Python, the value type of ``SampledTraj`` can be inferred from the constructor arguments.

Construction
------------

Create a sampled trajectory with ``set(value,time)`` or from a map of samples. Assigning a value at an existing timestamp replaces the previous one.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-sampled-create-beg]
      :end-before: [traj-sampled-create-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-sampled-create-beg]
      :end-before: [traj-sampled-create-end]
      :dedent: 4

Evaluation and properties
-------------------------

``x(t)`` interpolates at a scalar time. ``x([a,b])`` returns the interval hull of the interpolant over :math:`[a,b]`. ``tdomain()`` and ``codomain()`` give the temporal domain and the hull of the values. ``nb_samples()`` counts timestamps, whereas ``size()`` gives the codomain dimension.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-sampled-eval-beg]
      :end-before: [traj-sampled-eval-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-sampled-eval-beg]
      :end-before: [traj-sampled-eval-end]
      :dedent: 4

Accessing samples and components
--------------------------------

Iteration returns the stored pairs :math:`(t_i,x_i)` in time order. For a vector trajectory, ``x[i]`` extracts a scalar component and ``subvector(i,j)`` extracts consecutive components. In C++, ``as_scalar_trajs()`` returns all scalar components.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-sampled-iterate-beg]
      :end-before: [traj-sampled-iterate-end]
      :dedent: 4

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-sampled-components-beg]
      :end-before: [traj-sampled-components-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-sampled-iterate-beg]
      :end-before: [traj-sampled-iterate-end]
      :dedent: 4

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-sampled-components-beg]
      :end-before: [traj-sampled-components-end]
      :dedent: 4

Sampling
--------

``sampled(dt)`` adds samples on a regular temporal grid, retaining original samples by default. Set ``keep_original_values=False`` to keep only the requested grid. ``sampled_as(reference)`` evaluates the trajectory at the timestamps of another ``reference`` trajectory. In C++, ``same_sampling()`` checks whether two trajectories share the same timestamps.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-sampled-grid-beg]
      :end-before: [traj-sampled-grid-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-sampled-grid-beg]
      :end-before: [traj-sampled-grid-end]
      :dedent: 4

Changing the time domain
------------------------

``truncate_tdomain([a,b])`` restricts the domain and inserts interpolated values at its endpoints. ``shift_tdomain(dt)`` translates timestamps. ``stretch_tdomain([a,b])`` rescales them to a new temporal domain.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-sampled-time-beg]
      :end-before: [traj-sampled-time-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-sampled-time-beg]
      :end-before: [traj-sampled-time-end]
      :dedent: 4

See also: :doc:`trajectory_operations` and :doc:`trajectory_io`.
