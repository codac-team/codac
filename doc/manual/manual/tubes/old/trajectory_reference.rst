.. _sec-trajectories-reference:

Trajectory API overview
=======================

[retirer cette page et reverser éventuellement les fonctions vers d'autres pages]

.. list-table:: Common operations
  :header-rows: 1
  :widths: 31 34 35

  * - Operation
    - ``SampledTraj``
    - ``AnalyticTraj``
  * - ``tdomain()`` / ``codomain()`` / ``size()``
    - Time domain, range and output dimension
    - Time domain, interval range and output dimension
  * - ``x(t)`` / ``x([a,b])``
    - Interpolated value / range hull
    - Analytic value / interval evaluation
  * - ``sampled(dt)``
    - Adds samples; keeps original timestamps by default
    - Samples the analytic expression
  * - ``sampled_as(reference)``
    - Interpolates at reference timestamps
    - Evaluates at reference timestamps
  * - ``truncate_tdomain([a,b])``
    - Inserts boundary samples
    - Restricts the evaluation domain
  * - ``primitive(dt)``
    - Numerical integration
    - Numerical integration
  * - ``as_function()``
    - Embeds the interpolant in an analytic expression
    - Embeds the analytic law in an expression

.. list-table:: Additional ``SampledTraj`` operations
  :header-rows: 1
  :widths: 36 64

  * - Operation
    - Description
  * - ``set(value,time)`` / iteration
    - Insert or replace a sample / traverse stored samples
  * - ``nb_samples()``
    - Number of distinct timestamps; ``size()`` gives the codomain dimension
  * - ``x[i]`` / ``subvector(i,j)``
    - Extract components of a vector trajectory
  * - ``shift_tdomain(dt)`` / ``stretch_tdomain([a,b])``
    - Translate or rescale timestamps
  * - ``derivative()`` / ``primitive()``
    - Numerical derivative / trapezoidal integral on existing samples
  * - ``traj_cart_prod(...)``
    - Combine trajectories with identical timestamps
  * - ``continuous_traj(x)``
    - Correct apparent jumps in a periodic scalar signal

The C++ API also offers ``same_sampling()`` and ``as_scalar_trajs()``.[placer cette information dans une section dédiée (j'imagine dans une des pages précédentes)] Some template overloads differ from the Python bindings; for example, ``sampled_as()`` accepts a reference with another codomain type in C++, whereas the supplied Python bindings require matching types [trop technique pour être d'intérêt ici, à retirer]. For ``traj_eval()``, Python supports homogeneous scalar or vector inputs, while C++ also supports mixed arguments. [à placer dans la section d'évaluation appropriée]

See :doc:`sampledtraj`, :doc:`analytictraj` and :doc:`trajectory_operations` for examples and the associated semantics.
