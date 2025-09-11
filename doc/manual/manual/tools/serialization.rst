.. _sec-tools-serialization:

Binary serialization
====================

The library provides utility functions to serialize and deserialize various types of data in binary format. 

These functions are especially useful for optimizing performance by caching expensive computations: serialize results once, then use deserialization to reload them quickly in future runs, avoiding recomputation.

They can also be used to transfer data between C++, Python, Matlab implementations.


Supported Types
---------------

The serialization system currently supports:

- Built-in types (*e.g.*, ``int``, ``double``, ``float``) that are *trivially copyable*. The raw memory of the variables is directly written to or read from the stream ;
- ``Interval`` objects ;
- Matrix structures such as ``[Interval]Matrix``, ``[Interval]Vector``, and ``[Interval]Row``, with either scalar or ``Interval`` elements ;
- Any ``SampledTraj<T>`` objects, as long as ``serialize`` functions are defined for type ``T``.

All functions assume that the given streams are open and valid for the requested operation.


Example of use
--------------

The serialization system consists of two core functions: ``serialize(...)`` and ``deserialize(...)``, which handle binary input/output of supported types. In C++, the functions can be used with any ``std::ostream`` or ``std::istream`` object (for example, binary files). In Python, however, their usage is currently limited to file objects.

Below are some example to illustrate how the interface works:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [tools-1-beg]
      :end-before: [tools-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [tools-1-beg]
      :end-before: [tools-1-end]
      :dedent: 4

  .. code-tab:: matlab

    x1 = IntervalVector([[0,1],[-oo,0],Interval().pi]);
    % x1 = [ [0, 1] ; [-inf, 0] ; [3.14159, 3.1416] ]

    % Serializing the interval vector
    serialize(py.open('data.cdc', 'wb'), x1);

    % Deserializing the interval vector (possibly in another project)
    x2 = IntervalVector();
    deserialize(py.open('data.cdc', 'rb'), x2);

    % x1 == x2

*In this example, the* ``.cdc`` *extension is optional and serves as a suggested convention for Codac data.*


Notes
-----

- All serialization assumes consistent architecture (*e.g.*, same endianness and ``sizeof(double)``) between writing and reading.
- For custom types, serialization functions can be extended by writing new ``serialize``/``deserialize`` overloads.
- When (de)serializing from/to files, check that streams are open and valid before use.


Available functions
-------------------

.. doxygenfunction:: codac2::serialize(std::ostream&, const T&)
  :project: codac

.. doxygenfunction:: codac2::deserialize(std::istream&, T&)
  :project: codac

.. doxygenfunction:: codac2::serialize(std::ostream&, const Interval&)
  :project: codac

.. doxygenfunction:: codac2::deserialize(std::istream&, Interval&)
  :project: codac

.. doxygenfunction:: codac2::serialize(std::ostream&, const Eigen::Matrix<T,R,C>&)
  :project: codac

.. doxygenfunction:: codac2::deserialize(std::istream&, Eigen::Matrix<T,R,C>&)
  :project: codac

.. doxygenfunction:: codac2::serialize(std::ostream&, const SampledTraj<T>&)
  :project: codac

.. doxygenfunction:: codac2::deserialize(std::istream&, SampledTraj<T>&)
  :project: codac


.. admonition:: Technical documentation

  See the `C++ API documentation of these functions <../../api/html/codac2__serialization_8h>`_.