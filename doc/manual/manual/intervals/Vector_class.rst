.. _sec-vector-class:

Vectors, rows and Matrices
==========================

This page describes how to construct, use, and manipulate ``Vector``,  ``Row`` and ``Matrix`` objects.
Vectors and rows are treated as one-dimensional matrices (either rows or columns), and most functionality is shared with general matrices.

Codac uses the `Eigen library <https://eigen.tuxfamily.org>`_ for its matricial structures and calculations, with `custom methods added inside the Eigen classes <https://libeigen.gitlab.io/eigen/docs-nightly/TopicCustomizing_Plugins.html>`_. In the same way, the interval counterpart objects (``IntervalVector``,  ``IntervalRow`` and ``IntervalMatrix``) adopt the structure of Eigen (same class name or operations) with an interval arithmetic and reliable outputs.

The following sections illustrate construction, element access, initialization, resizing, substructure manipulation, and various properties of vectors and matrices.


Construction
------------

Vectors and matrices can be constructed using initializer lists, sizes, or raw arrays. Vectors are typically used when either the number of rows or columns is fixed to 1.

.. tabs::

  .. code-tab:: cpp

    // Vector from initializer list
    Vector v = {1.0, 2.0, 3.0};

    // Vector from std::vector<double>
    Vector v2(std::vector<double>{4.0, 5.0, 6.0});

    // Vector of fixed size, zero-initialized (if scalar type is not interval)
    Vector v3(3);

    // Vector from array
    double data[] = {7.0, 8.0, 9.0};
    Vector v4(3, data);

  .. code-tab:: cpp

    // Matrix with given number of rows and columns
    Vector M(2, 3); // 2 rows, 3 columns

    // Matrix from array
    double values[] = {1, 2, 3, 4, 5, 6};
    Vector M2(2, 3, values); // fills elements row-by-row

    // Matrix with uninitialized (or zero) values
    Vector M3(2, 3);

Element Access
--------------

Vector and matrix elements can be accessed via parentheses `()` or brackets `[]` (for vectors). You can both read and write elements.

.. tabs::

  .. code-tab:: cpp

    // Vector access
    double x = v[0];
    v(1) = 42.0;

  .. code-tab:: cpp

    // Matrix access
    double y = M(1, 2);  // row 1, column 2
    M(0, 0) = 3.14;

Initialization
--------------

You can set all entries of a matrix or vector to a given value using `init()`:

.. tabs::

  .. code-tab:: cpp

    v.init(0.0); // Set all elements of vector to 0

  .. code-tab:: cpp

    M.init(1.0); // Set all matrix elements to 1

Equality Comparison
-------------------

Check whether two objects are equal in shape and contents using `==`.

.. tabs::

  .. code-tab:: cpp

    if (v == v2)
      std::cout << "Vectors are equal.\n";

    if (M == M2)
      std::cout << "Matrices match.\n";

Factory Constructors
--------------------

Use these static constructors to create matrices or vectors with common patterns.

.. tabs::

  .. code-tab:: cpp

    Vector v1 = Vector::zero(4);            // Vector with 4 zeros
    Vector v2 = Vector::ones(4);            // Vector with all 1s
    Vector v3 = Vector::constant(4, 3.14);  // Vector filled with 3.14
    Vector v4 = Vector::random(4);          // Random values

  .. code-tab:: cpp

    Vector M1 = Vector::zero(2, 3);             // 2x3 matrix with zeros
    Vector M2 = Vector::ones(2, 3);             // matrix with ones
    Vector M3 = Vector::constant(2, 3, 2.5);     // constant values
    Vector M4 = Vector::eye(3, 3);              // 3x3 identity matrix
    Vector M5 = Vector::random(2, 3);           // matrix with random elements

Resize (with Data Preservation)
-------------------------------

Unlike Eigen’s default `resize()`, these methods preserve overlapping values during resizing.

.. tabs::

  .. code-tab:: cpp

    v.resize_save_values(6);  // Resize vector from 3 to 6 elements

  .. code-tab:: cpp

    M.resize_save_values(4, 5); // Resize 2x3 matrix to 4x5

Substructures and Assignment
----------------------------

You can extract subvectors and insert values from one vector to another.

.. tabs::

  .. code-tab:: cpp

    Vector sub = v.subvector(1, 3);   // Extract elements v[1] to v[3]
    v.put(2, sub);                    // Insert sub at index 2

Properties and Structural Checks
--------------------------------

.. tabs::

  .. code-tab:: cpp

    bool square = M.is_squared();   // True if M is square (rows == cols)
    bool contains_nan = M.is_nan(); // Any NaN values?

    double norm2 = v.squared_norm();      // ||v||^2
    double min_val = v.min_coeff();       // Minimum entry
    double max_val = v.max_coeff();       // Maximum entry

    Index idx_min = v.min_coeff_index();  // Index of min value
    Index idx_max = v.max_coeff_index();  // Index of max value

  .. code-tab:: cpp

    auto D = v.diag_matrix();  // Convert vector into diagonal matrix

Element-wise Operations
-----------------------

Apply rounding operations to each element.

.. tabs::

  .. code-tab:: cpp

    auto vf = floor(v);    // floor of each value
    auto vc = ceil(v);     // ceil of each value
    auto vr = round(v);    // round to nearest

  .. code-tab:: cpp

    auto mf = floor(M);    // floor of each element
    auto mc = ceil(M);
    auto mr = round(M);

.. note::

   These rounding operations are disabled for matrices with interval elements.

Examples
--------

Here’s a short example illustrating usage of several features together:

.. tabs::

  .. code-tab:: cpp

    Vector a = Vector::random(5);
    Vector b = Vector::ones(5);

    a.init(3.14);
    double max = a.max_coeff();

    Vector diag = a.diag_matrix();

    Vector slice = a.subvector(1, 3);
    a.put(0, slice);

    if (a == b)
      std::cout << "Same values!" << std::endl;

See Also
--------

- :ref:`Interval class documentation <interval_class>`
- :ref:`Matrix operations reference <matrix_ops>`