.. _sec-linear-vecmat:

Vectors, rows, matrices
=======================

By default, ``Vector``, ``Row`` and ``Matrix`` objects are initialized with :math:`0` values.

*More documentation to come...*


.. _sec-linear-mat-preconditioning:

Matrix preconditioning
----------------------

.. doxygenfunction:: codac2::gauss_jordan(const Matrix&)
  :project: codac


.. _sec-linear-vecmat-VectorCompare:

Using ``Vector`` as keys in ``std::map`` (C++ use)
--------------------------------------------------

In C++, the ``Vector`` type can be used as a key in associative containers such
as ``std::map``. Since ``std::map`` requires a strict weak ordering between keys,
a custom comparison functor must be provided. Codac provides the
``VectorCompare`` struct for this purpose.

Below is a minimal example showing how to create a map whose keys are ``Vector``
objects and whose values are integers.

.. tabs::

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [vector-map-compare-1-beg]
      :end-before:  [vector-map-compare-1-end]
      :dedent: 4

  .. group-tab:: Python

    ``std::map`` is a C++ feature. In Python, dictionaries already support
    arbitrary immutable keys, but ``Vector`` objects are mutable and
    therefore cannot be used directly as dictionary keys.