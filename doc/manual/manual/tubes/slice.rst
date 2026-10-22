.. _sec-tubes-slice:

The Slice and TSlice classes
============================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

At the implementation level, sliced tubes are built from two complementary classes:

* :class:`~codac.TSlice`, which stores one temporal support,
* :class:`~codac.Slice`, which stores one codomain over that support for one given tube.

TSlice: one temporal element shared by all tubes
------------------------------------------------

A :class:`~codac.TSlice` inherits from :class:`~codac.Interval` and represents one temporal interval in a :class:`~codac.TDomain`. It may be a regular slice :math:`[t_i,t_{i+1}]` or a gate :math:`[t_i,t_i]`. It also stores the map from ``SlicedTubeBase*`` to the corresponding :class:`~codac.SliceBase` objects. In other words, a ``TSlice`` is the shared temporal node to which all tube-specific slices are attached.

The method ``is_gate()`` simply checks whether the temporal interval is degenerate. The method ``slices()`` provides access to the attached codomain slices.

Slice: one codomain over one temporal support
---------------------------------------------

A :class:`~codac.Slice` is the codomain carried by one sliced tube over one temporal interval. The codomain type is typically :class:`~codac.Interval` or :class:`~codac.IntervalVector`, and the class uses protected inheritance from this codomain type so that updates can be controlled and, if necessary, propagated to adjacent gates.

Each slice knows:

* its parent tube,
* its temporal interval ``t0_tf()``,
* its previous and next slices,
* its codomain,
* whether its temporal support is a gate.

Input and output gates
----------------------

A central concept in the current implementation is the distinction between the *interior codomain* of a slice and its *endpoint values*. For a non-gate slice, endpoint values are obtained through

* ``input_gate()``
* ``output_gate()``

If an explicit neighboring gate exists, its codomain is used. Otherwise, the endpoint enclosure is the intersection of adjacent slice codomains. This is what makes endpoint evaluation meaningful even when gates are not explicitly represented.

Without explicit gates, evaluating a tube at a slice boundary returns the intersection of the neighboring slice values.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [slice-endpoint-example-beg]
      :end-before: [slice-endpoint-example-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [slice-endpoint-example-beg]
      :end-before: [slice-endpoint-example-end]
      :dedent: 4


Evaluation and inversion at slice level
---------------------------------------

A scalar or vector slice can be evaluated with:

* ``s(t)`` at one time,
* ``s([ta,tb])`` over a temporal interval.

At the lower bound, evaluation returns the input gate. At the upper bound, it returns the output gate. Inside the slice, it returns the interior codomain. Outside the temporal support, it returns the unbounded codomain value.

The inversion API mirrors this structure:

* ``invert(y, t)`` computes a local inverse image,
* ``invert(y, v, t)`` uses a derivative slice for a sharper inverse image.

The following example illustrates inversion over a selected temporal interval.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: python
      :start-after: [slice-invert-example-beg]
      :end-before: [slice-invert-example-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [slice-invert-example-beg]
      :end-before: [slice-invert-example-end]
      :dedent: 4


Derivative-aware polygonal model
--------------------------------

For scalar codomains, and componentwise for vector codomains, a slice can build a polygonal enclosure from:

* its temporal support,
* its input and output gates,
* a derivative slice.

This is exposed by:

* ``polygon_slice(v)`` for scalar slices,
* ``polygon_slice_i(v, i)`` for vector slices.

This polygonal enclosure is used in derivative-aware evaluation and inversion.

Enclosed bounds
---------------

The method ``enclosed_bounds(t)`` returns a pair of codomain objects whose components enclose the reachable lower and upper bounds over the queried temporal interval. This is useful when you want more structural information than the plain union returned by a simple evaluation.
