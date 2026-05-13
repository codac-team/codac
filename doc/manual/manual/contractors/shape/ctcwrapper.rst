.. _sec-ctc-shape-ctcwrapper:

CtcWrapper
===============================

  Main author: `Maël Godard <https://godardma.github.io/>`_

The CtcWrapper is a contractor to contract a box with respect to a set represented by a Codac object.
Currently supported objects are:

- :ref:`IntervalVector <sec-intervals-intervalvector-class>`
- :ref:`Parallelepiped <subsec-zonotope-parallelepiped>`
- PavingOut

Methods
-------

Below is an example of the use of the CtcWrapper with a Parallelepiped.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [ctcparallelepiped-1-beg]
      :end-before: [ctcparallelepiped-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [ctcparallelepiped-1-beg]
      :end-before: [ctcparallelepiped-1-end]
      :dedent: 4

.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../api/html/classcodac2_1_1_ctc_wrapper.html>`_.