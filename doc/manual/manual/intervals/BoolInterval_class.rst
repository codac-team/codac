.. _sec-intervals-boolinterval-class:

The BoolInterval enumeration
============================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

A :class:`~codac2.BoolInterval` (Boolean interval) is a compact representation of an
uncertain truth value. It is primarily used as a **reliable return type** for predicates
evaluated under uncertainties (interval inputs, rounding effects, geometric tolerances, etc.).

Definition
----------

Let :math:`\mathbb{B}=\{\mathsf{false},\mathsf{true}\}`. A boolean interval represents a subset
of :math:`\mathbb{B}`: :math:`[b]\subseteq\mathbb{B}`.

In Codac, ``BoolInterval`` is an enumeration of four canonical values:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [boolinterval-class-1-beg]
      :end-before: [boolinterval-class-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [boolinterval-class-1-beg]
      :end-before: [boolinterval-class-1-end]
      :dedent: 4


Two useful set values are:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [boolinterval-class-2-beg]
      :end-before: [boolinterval-class-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [boolinterval-class-2-beg]
      :end-before: [boolinterval-class-2-end]
      :dedent: 4


Operations on ``BoolInterval`` are documented below:

.. doxygenfunction:: codac2::operator&(BoolInterval x, BoolInterval y)
  :project: codac

.. doxygenfunction:: codac2::operator|(BoolInterval x, BoolInterval y)
  :project: codac

.. doxygenfunction:: codac2::operator&&(BoolInterval x, BoolInterval y)
  :project: codac

.. doxygenfunction:: codac2::operator||(BoolInterval x, BoolInterval y)
  :project: codac

.. doxygenfunction:: codac2::operator~(BoolInterval x)
  :project: codac


.. admonition:: Technical documentation

  See the `C++ API documentation of the BoolInterval class <../../api/html/codac2___bool_interval_8h.html>`_.