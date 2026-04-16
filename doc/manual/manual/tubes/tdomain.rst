.. _sec-domains-tubes-tdomain:

The TDomain class
=================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

A :class:`~codac.TDomain` is the temporal partition shared by one or more sliced tubes.
It stores an ordered list of :class:`~codac.TSlice` objects. Each stored element is either

* a non-degenerate temporal slice :math:`[t_i,t_{i+1}]`, or
* a degenerate temporal slice :math:`[t_i,t_i]`, called a *gate*.

Gates are optional. When they are explicitly represented, the partition may alternate between
``gate / slice / gate / slice / ...``. This is the structure created by
``create_tdomain(t0_tf, dt, True)``.

Although optional, gates are useful when one needs to represent tube values explicitly at specific time instants. This is particularly relevant for guaranteed integration.

Creating temporal domains
-------------------------

A temporal domain can be created in three common ways:

* from the default constructor helper ``create_tdomain()``;
* from a single time interval ``create_tdomain([t0,tf])``;
* from a sampled interval ``create_tdomain([t0,tf], dt, with_gates)``.

The following examples show the expected structures for the sampled cases above, both with and without gates.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [tdomain-class-1-beg]
      :end-before: [tdomain-class-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tdomain-class-1-beg]
      :end-before: [tdomain-class-1-end]
      :dedent: 4


Inspecting the partition
------------------------

A temporal domain exposes:

* its global time interval ``t0_tf()``,
* the number of stored temporal elements ``nb_tslices()``,
* the number of attached tubes ``nb_tubes()``,
* the temporal slice containing a given time ``tslice(t)``,
* and a vector copy of the stored temporal slices ``tslices_vector()``.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [tdomain-class-2-beg]
      :end-before: [tdomain-class-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tdomain-class-2-beg]
      :end-before: [tdomain-class-2-end]
      :dedent: 4


Note that ``tslice(t)`` returns the gate when ``t`` matches an explicit gate, and
otherwise returns the unique non-degenerate slice containing ``t``.

Sampling and refinement
-----------------------

The temporal partition can be refined dynamically with:

* ``sample(t, with_gate=False)`` for one time value,
* ``sample([ta,tb], dt, with_gates=False)`` for repeated sampling over an interval.

Sampling can occur:

* inside the current domain, which splits an existing slice;
* outside the current domain, which extends the partition;
* with gates, which inserts a degenerate temporal slice at the sampling time.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [tdomain-class-3-beg]
      :end-before: [tdomain-class-3-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tdomain-class-3-beg]
      :end-before: [tdomain-class-3-end]
      :dedent: 4

One important implementation detail is that refinement preserves the attached tubes by cloning the
corresponding slice objects onto the new temporal elements. The new slices are then reattached to
the newly inserted ``TSlice`` iterators.

Shared refinement across several tubes
--------------------------------------

One practical consequence of the shared-``TDomain`` design is that refining the time
partition from one tube automatically updates the slice structure seen by all the
other tubes attached to the same temporal domain.
The following code illustrates this effect:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [tdomain-class-4-beg]
      :end-before: [tdomain-class-4-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tdomain-class-4-beg]
      :end-before: [tdomain-class-4-end]
      :dedent: 4

.. note::

   This behavior is intentional: it keeps all related tubes on a consistent shared
   temporal support, which greatly simplifies multi-tube operations such as
   arithmetic combinations and contractions.

Gates
-----

Explicit gates are useful when you want to represent endpoint values separately from the interior
codomain of a slice. The current API provides:

* ``all_gates_defined()`` to detect a fully gate-augmented partition,
* ``delete_gates()`` to remove all explicit gates.

Therefore, a :math:`t`-domain built from ``create_tdomain([0,1], 0.5, True)`` contains five temporal elements and is a
typical example of a gate-augmented partition.
In this implementation, a gate is not a separate data type: it is simply a degenerate
:class:`~codac.TSlice`, i.e., an interval :math:`[t,t]`.

Shared temporal structure
-------------------------

A :class:`~codac.TDomain` is shared by all sliced tubes built from it.
As a consequence, structural operations such as ``truncate(...)`` affect all
attached tubes, and the number of attached tubes can be queried with
``nb_tubes()``.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [tdomain-class-5-beg]
      :end-before: [tdomain-class-5-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tdomain-class-5-beg]
      :end-before: [tdomain-class-5-end]
      :dedent: 4