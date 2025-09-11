.. _sec-intervals:

Intervals
=========

Codac provides data structures for handling basic interval sets. These structures represent the building blocks of most domain structures provided in Codac. The elementary interval structures are:

- :ref:`The Interval class <sec-intervals-class>`: represents a real bounded interval :math:`[x^{-},x^{+}]`.
- ``IntervalVector`` (or ``IntervalRow``): represents a vector (or row) where each component is an interval.
- ``IntervalMatrix``: represents a matrix where each element is an interval.


.. toctree::
   :hidden:

   Interval_class.rst
   .. Vector_class.rst
   IntervalVector_class.rst

..    What is an interval? <http://codac.io>
   The Interval class <http://codac.io>
   Boolean intervals <http://codac.io>


Integration with the Eigen library
----------------------------------

The ``IntervalVector`` and ``IntervalMatrix`` structures of Codac are built upon `the Eigen library <https://eigen.tuxfamily.org>`_, using the ``Interval`` type as a template parameter. Eigen is a well-known C++ library for linear algebra, providing optimized operations for vectors and matrices. 

Leveraging Eigen offers several advantages:

- **Performance**: Eigen provides highly optimized matrix and vector operations, making interval computations more efficient.
- **Flexibility**: Using templates allows for seamless integration of interval arithmetic into Eigen's existing functionalities.
- **Scalability**: Eigen's design allows for handling large-scale numerical computations effectively.

By building on Eigen, Codac ensures that interval arithmetic benefits from modern, high-performance linear algebra techniques.

However, some Eigen algorithms do not work with the ``Interval`` type as a template parameter. For instance, the decomposition of an ``IntervalMatrix`` will require a specific algorithm designed for intervals, instead of reals.
Some implementations are provided in Codac for interval algebra, see the dedicated pages of this manual.
