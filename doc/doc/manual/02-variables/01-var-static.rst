.. _sec-manual-varstatic:

*****************
Vectors, matrices
*****************

Basic structures such as vectors :math:`\mathbf{x}` or matrices :math:`\mathbf{X}` are proposed in order to handle constants, measurements or parameters.

They are not aimed at representing temporal evolutions contrary to trajectories, presented :ref:`in the next section <sec-manual-vardyn>`.
Note also that they are not involved during the process of constraint propagation, that only relies on domains presented in the next part.

.. contents::

.. note::

  .. Figure:: ../../img/logo_ibex.jpg
    :align: right
  .. Figure:: ../../img/logo_pyibex.jpg
    :align: right
  
  These static variables come from the `IBEX library <http://www.ibex-lib.org>`_. They are briefly presented here for the sake of consistency. For more information, please refer to the `IBEX documentation <http://www.ibex-lib.org/doc/interval.html#intervals-vectors-and-matrices>`_ for C++ use or to the `pyIbex manual <http://benensta.github.io/pyIbex/sphinx/quickstart.html>`_ for Python usage.


.. _sec-manual-varstatic-vectors:

Vectors
-------

Vectors of reals, denoted in bold in this manual, are represented by the class ``Vector``.
Their use is intuitive:

.. tabs::

  .. code-tab:: py

    x1 = [0,1,3]                   # creates the vector x1=(0;1;3)
    x2 = [1]*3                     # creates the vector x2=(1;1;1)
    x3 = x1                        # creates a copy of x1

  .. code-tab:: c++

    Vector x1({0,1,3});            // creates the vector x1=(0;1;3)
    Vector x2(3,1);                // creates the vector x2=(1;1;1)
    Vector x3(x1);                 // creates a copy of x1

Vector operations are available to handle ``Vector`` objects:

.. tabs::

  .. code-tab:: py

    # Use default types of Python, or Numpy

  .. code-tab:: c++

    int n = x1.size();             // vector dimension (number of components): 3
    x1.resize(5);                  // updates x1 to (0;1;3;0;0)
    x3.resize(2);                  // updates x3 to (0;1)
    Vector x4 = x1.subvector(2,3); // creates x4=(3;0)
    x1.put(x2,2);                  // updates x1 to (0;1;1;4;1)

    // Access to the ith component:
    x2[1] = 4;                     // updates x2 to (1;4;1)


Note that these operations are also available for other vector types: :ref:`matrices <sec-manual-varstatic-matrices>`, :ref:`boxes <sec-manual-intervals-boxes>`, :ref:`vectors of trajectories <sec-manual-vardyn-trajectoryvector>`, and :ref:`vectors of tubes <sec-manual-tubes-tubevector>`.

Basic arithmetic operations are applicable on vectors such as binary operators: :math:`+,-,\cdot,/`

.. tabs::

  .. code-tab:: py

    # Use default types of Python, or Numpy

  .. code-tab:: c++

    x1 = 4*x1;                     // scalar multiplication of a vector
    x1 *= 4;                       // same operation (multiplication and assignment)
    Vector x5 = x3*x4;             // dot product


.. _sec-manual-varstatic-matrices:

Matrices
--------

``Matrix`` objects are exactly designed the same way as vectors. Here are listed different ways to build a matrix:

.. tabs::

  .. code-tab:: py

    # Use default types of Python, or Numpy

  .. code-tab:: c++

    Matrix m1({{1,2,3},{4,5,6}});  // creates a 2×3 matrix
    cout << m1 << endl;
    // Outputs:
    // ((1 ; 2 ; 3)
    //  (4 ; 5 ; 6))

    cout << m1.transpose() << endl;
    // Outputs:
    // ((1 ; 4)
    //  (2 ; 5)
    //  (3 ; 6))

    Matrix m2 = ones(2,2);         // a 2×2 matrix of ones
    Matrix m3 = eye(3);            // a 3×3 identity matrix

Other arithmetic operations apply on matrices in the same manner as for vectors.