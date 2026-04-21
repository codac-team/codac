.. _sec-ctc-set-ctcinter:

The CtcInter contractor
=======================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_


.. doxygenclass:: codac2::CtcInter
  :project: codac

Basic usage
-----------

The most common way to create an intersection contractor is to combine two existing
contractors with ``&``.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [ctcinter-1-beg]
      :end-before: [ctcinter-1-end]
      :dedent: 2

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [ctcinter-1-beg]
      :end-before: [ctcinter-1-end]
      :dedent: 2

Once built, the contractor can be applied as any other box contractor.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [ctcinter-2-beg]
      :end-before: [ctcinter-2-end]
      :dedent: 2

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [ctcinter-2-beg]
      :end-before: [ctcinter-2-end]
      :dedent: 2

In this example, ``c1`` restricts the box to :math:`[-10,10]\times[-2,2]` and ``c2``
restricts it to :math:`[-12,2]\times[0,4]`. Their conjunction therefore contracts the
initial box to:

.. math::

  [-10,10]\times[-2,2] \ \cap \ [-12,2]\times[0,4]
  \,=\, [-10,2]\times[0,2].

Building a conjunction incrementally
------------------------------------

A ``CtcInter`` may also be created from a prescribed domain size and then populated later.
In that case, the object is initially neutral: as long as no sub-contractor is added, it
has no effect on the contracted box.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [ctcinter-3-beg]
      :end-before: [ctcinter-3-end]
      :dedent: 2

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [ctcinter-3-beg]
      :end-before: [ctcinter-3-end]
      :dedent: 2

The method ``nb()`` returns the current number of sub-contractors stored in the
intersection.

.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../api/html/classcodac2_1_1_ctc_inter.html>`_.
