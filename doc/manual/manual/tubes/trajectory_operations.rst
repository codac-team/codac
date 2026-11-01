.. _sec-trajectories-operations:

Operations on trajectories
==========================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

Arithmetic and calculus
-----------------------

Operations such as ``x1+x2`` or ``sin(x1)`` compute values at the stored timestamps. The resulting ``SampledTraj`` interpolates linearly between these values. Arithmetic between two sampled trajectories requires identical timestamps.

For a scalar or vector trajectory :math:`x`, ``primitive(dt)`` approximates the integral

.. math::

   p(t)=\int_{t_0}^{t}x(\tau)\,d\tau,\qquad p(t_0)=0,

using the trapezoidal rule, with integration step ``dt``. For a ``SampledTraj``, ``primitive()`` instead integrates over its existing sample intervals:

.. math::

   p(t_{i+1})=p(t_i)+\frac{t_{i+1}-t_i}{2}(x_i+x_{i+1}).

``derivative()`` approximates the derivative of a sampled trajectory by finite differences. These numerical operations do not provide guaranteed bounds.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-calculus-beg]
      :end-before: [traj-ops-calculus-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-calculus-beg]
      :end-before: [traj-ops-calculus-end]
      :dedent: 4



Cartesian products
------------------

``traj_cart_prod(x1,x2,...)`` combines scalar or vector sampled trajectories into a vector trajectory. The timestamps must be identical. Use ``sampled_as()`` to align trajectories when necessary.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-cartprod-beg]
      :end-before: [traj-ops-cartprod-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-cartprod-beg]
      :end-before: [traj-ops-cartprod-end]
      :dedent: 4

Common time domain
------------------

``truncate_to_common_tdomain(x1,x2,...)`` truncates all trajectories in place to the intersection of their time domains. An empty intersection raises an error.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-common-tdomain-beg]
      :end-before: [traj-ops-common-tdomain-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-common-tdomain-beg]
      :end-before: [traj-ops-common-tdomain-end]
      :dedent: 4

Unwrapping periodic signals
---------------------------

``continuous_traj()`` removes apparent jumps in a sampled scalar signal, for instance a heading crossing its angular discontinuity.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-continuous-beg]
      :end-before: [traj-ops-continuous-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-continuous-beg]
      :end-before: [traj-ops-continuous-end]
      :dedent: 4

The function detects jumps between consecutive samples and corrects them using a period estimated from the observed values. This is particularly useful for trajectories representing angles. Since the period is estimated rather than specified, the correction may be approximate.

Applying analytic functions
---------------------------

``f.traj_eval(x1,x2,...)`` evaluates an ``AnalyticFunction`` on sampled trajectories at the timestamps of the first input, interpolating the other inputs as needed. In Python, inputs are all scalar trajectories or all vector trajectories, while C++ also accepts mixed inputs. For interval enclosures on tubes, see :ref:`sec-domains-tubes-slicedtube-function-eval`.

For example, given a heading :math:`\theta(t)` and a speed :math:`v(t)`, an analytic function computes the Cartesian velocity components:

.. math::

   \mathbf{u}(t)=\begin{pmatrix}v(t)\cos\theta(t)\\v(t)\sin\theta(t)\end{pmatrix}.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-eval-beg]
      :end-before: [traj-ops-eval-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-eval-beg]
      :end-before: [traj-ops-eval-end]
      :dedent: 4

For a vector-valued input trajectory, use a ``VectorVar``:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-fun-vector-beg]
      :end-before: [traj-ops-fun-vector-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-fun-vector-beg]
      :end-before: [traj-ops-fun-vector-end]
      :dedent: 4

Using a trajectory in an analytic expression
--------------------------------------------

``as_function()`` makes a trajectory available inside an analytic expression of time. This is useful when an expression depends on an existing time series.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-asfunction-beg]
      :end-before: [traj-ops-asfunction-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-asfunction-beg]
      :end-before: [traj-ops-asfunction-end]
      :dedent: 4

The trajectory operator currently supports forward evaluation but not general backward contraction or derivative propagation through the expression.

Converting trajectories and tubes
---------------------------------

``SlicedTube(td,traj)`` encloses a trajectory on a temporal partition. To enclose an unknown signal, additional measurement and interpolation uncertainty may be needed. Conversely, ``tube.mid()`` gives a numerical midpoint trajectory without retaining the tube's uncertainty.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [traj-ops-tube-beg]
      :end-before: [traj-ops-tube-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [traj-ops-tube-beg]
      :end-before: [traj-ops-tube-end]
      :dedent: 4
