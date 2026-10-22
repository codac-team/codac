.. _sec-domains-tubes-examples:

Programming examples
====================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

The following examples illustrate basic operations on sliced tubes.

Example 1: building and evaluating a scalar tube
------------------------------------------------

The following example builds a scalar tube over a sampled temporal domain and assigns different values on successive temporal intervals.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-example-1-beg]
      :end-before: [tube-example-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-example-1-beg]
      :end-before: [tube-example-1-end]
      :dedent: 4



Example 2: refining a temporal domain from a tube update
--------------------------------------------------------

Local assignments refine the shared temporal partition when necessary.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-example-2-beg]
      :end-before: [tube-example-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-example-2-beg]
      :end-before: [tube-example-2-end]
      :dedent: 4

The same partition is updated for all tubes sharing the temporal domain.

Example 3: tube from an analytic function
-----------------------------------------

For :math:`f(t)=10\cos(t)+t`, a ``SlicedTube`` encloses the function over each time slice rather than evaluating it only at isolated timestamps.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-example-3-beg]
      :end-before: [tube-example-3-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-example-3-beg]
      :end-before: [tube-example-3-end]
      :dedent: 4


Example 4: derivative-aware slice reasoning
-------------------------------------------

The following pattern uses a derivative tube and ``CtcDeriv`` to sharpen a scalar slice. The slice can then build a polygonal enclosure used for refined evaluation or inversion.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-example-4-beg]
      :end-before: [tube-example-4-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-example-4-beg]
      :end-before: [tube-example-4-end]
      :dedent: 4

The derivative constraint can narrow the tube before evaluating individual slices.

Example 5: integration and primitive tubes
------------------------------------------

The integration API supports point and interval queries and can also build a primitive tube.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-example-5-beg]
      :end-before: [tube-example-5-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-example-5-beg]
      :end-before: [tube-example-5-end]
      :dedent: 4



Example 6: Cartesian product of tubes
-------------------------------------

When several sliced tubes share the same temporal domain, you can build a vector-valued tube by stacking their codomains slice by slice.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [tube-example-6-beg]
      :end-before: [tube-example-6-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tube-example-6-beg]
      :end-before: [tube-example-6-end]
      :dedent: 4

All inputs must share the same temporal-domain object.

For the analogous operation on ``SampledTraj`` objects, see :ref:`sec-trajectories-operations` (``traj_cart_prod``).
