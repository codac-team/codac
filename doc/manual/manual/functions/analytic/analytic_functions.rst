.. _sec-functions-analytic-class:

Analytic inclusion functions
============================

  Main authors: `Simon Rohou <https://www.simon-rohou.fr/research/>`_, `Damien Massé <https://lab-sticc.univ-brest.fr/~dmasse/>`_

The ``AnalyticFunction`` class allows to build analytic inclusion functions and evaluate them using interval analysis.

Considering a :math:`n`-dimensional box :math:`[\mathbf{x}]` as input, a function :math:`\mathbf{f}:\mathbb{R}^n\to\mathbb{R}^m` will output a set which is not necessarily a box.
To obtain a reliable enclosure of the image set :math:`\mathbf{f}([\mathbf{x}])`, we use inclusion functions.

An inclusion function :math:`[\mathbf{f}]:\mathbb{IR}^n\to\mathbb{IR}^m` outputs an interval set containing all the feasible images :math:`\mathbf{f}(\mathbf{x})` for any $\mathbf{x}\in[\mathbf{x}]$. Hence it must satisfy: :math:`\forall [\mathbf{x}]\in\mathbb{IR}^n, \mathbf{f}([\mathbf{x}])\subset[\mathbf{f}]([\mathbf{x}])`.
The output set :math:`[\mathbf{f}]([\mathbf{x}])` will also enclose unfeasible values, due to the wrapping effect of intervals, or because the inclusion function may not be minimal. The choice is made to have fast calculation times, rather than minimal enclosures. In all cases, calculations are guaranteed to contain all the images.

The class ``AnalyticFunction`` provides a convenient way to perform calculations on intervals, boxes, and interval matrices. It involves symbolic variables and expressions, and their evaluations rely on interval operators from interval analysis. Real values (reals of :math:`\mathbb{R}`, vectors, matrices) can also be involved, and will be automatically cast into their interval counterparts.

This page provides an overview of the ``AnalyticFunction`` class, its key features, and practical examples of how to use it.

.. note::

  .. Figure:: CtcInverse_small.png
    :align: right
  
  For defining a contractor based on an ``AnalyticFunction``, the ``CtcInverse`` class is available.
  It allows to solve constraints under the form :math:`\mathbf{f}(\mathbf{x})\in[\mathbf{y}]`.

  See more.


Variables
---------

Before defining an ``AnalyticFunction``, you need to define the variables that will be used in the function. The following classes allow to represent arguments:

- ``ScalarVar()``: constructor for a scalar variable
- ``VectorVar(n)``: constructor for a :math:`n`-d vector variable
- ``MatrixVar(r,c)``: constructor for a :math:`r\times c` matrix variable


Expressions
-----------

An ``AnalyticFunction`` object is built from:

* a list of arguments (``ScalarVar``, ``VectorVar`` or ``MatrixVar`` objects), that can be empty;
* a scalar or vectorial or matricial expression involving the arguments, constants, and possibly other functions.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [1-beg]
      :end-before: [1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [1-beg]
      :end-before: [1-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [1-beg]
      :end-before: [1-end]
      :dedent: 0

For ``VectorVar`` and ``MatrixVar``, as well as for vector or matrix expressions, coefficients can be accessed using the operators ``[i]`` and ``(i,j)`` respectively. For vector elements, the ``.subvector(i,j)`` method allows to get a subvector of the variable or expression.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [2-beg]
      :end-before: [2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [2-beg]
      :end-before: [2-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [2-beg]
      :end-before: [2-end]
      :dedent: 0

Variables are temporary objects used only to construct expressions. They can be freed once the function has been instantiated: there is no memory error in using an ``AnalyticFunction`` without its ``*Var`` arguments in the same scope, as in this example:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [3-beg]
      :end-before: [3-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [3-beg]
      :end-before: [3-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [3-beg]
      :end-before: [3-end]
      :dedent: 0


Function composition
--------------------

``AnalyticFunction`` objects can be easily composed using the ``()`` operator. For vectorial functions, the ``[]`` operator is also available:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [4-beg]
      :end-before: [4-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [4-beg]
      :end-before: [4-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [4-beg]
      :end-before: [4-end]
      :dedent: 0

Below is an example of a function composition involving matrix calculation for the rotation of a vector:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [4b-beg]
      :end-before: [4b-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [4b-beg]
      :end-before: [4b-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [4b-beg]
      :end-before: [4b-end]
      :dedent: 0


Evaluations
-----------

``AnalyticFunction`` objects represent interval inclusion functions: their evaluations consist in computing interval output sets, either ``Interval`` or ``IntervalVector`` outputs depending on the definition.

Evaluations can be done by the ``.eval()`` method of ``AnalyticFunction``.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [5-beg]
      :end-before: [5-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [5-beg]
      :end-before: [5-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [5-beg]
      :end-before: [5-end]
      :dedent: 0


For multivariate functions, it is possible to provide several inputs to the ``.eval()`` method, as in the above example with ``f2`` or ``f3``. In Codac C++, the arguments may be of different types (any mixed scalar, vector, or matrix types). In Python and Matlab however, the binding does not allow any combination of inputs, but several arguments of same structural types can be provided: a list of scalar values, or a list of vector values (``Vector``, ``IntervalVector``), or a list of matrix values (``Matrix``, ``IntervalMatrix``).


Evaluation modes
^^^^^^^^^^^^^^^^

The class provides three ways of evaluating functions:

* **natural evaluations** :math:`[\mathbf{f}_n]([\mathbf{x}])`: each operator of the analytic function is evaluated by intervals in a *natural* way. However, natural evaluations may be pessimistic in case of multiple occurences in the expressions. For instance, the evaluation of :math:`[x]-[x]` will not be :math:`0`. This is a classic drawback of interval arithmetic.

* **centered form evaluations** :math:`[\mathbf{f}_c]([\mathbf{x}])`: computations are made using Eq. :eq:`eq:centered:form`, depicting a centered form expression. Results are significantly improved for small boxes, but will be worse than a natural evaluation for larger boxes.

.. math:: [\mathbf{f}_c]([\mathbf{x}])=\mathbf{f}(\overline{\mathbf{x}})+[\mathbf{J}_{\mathbf{f}}]\left([\mathbf{x}]\right)\cdot\left([\mathbf{x}]-\overline{\mathbf{x}}\right)
  :label: eq:centered:form

* **both natural and centered form evaluations** :math:`[\mathbf{f}_n]([\mathbf{x}])\cap[\mathbf{f}_c]([\mathbf{x}])`: the two previous evaluations are carried out simultaneously, and the result is the intersection of the two evaluations. This allows to benefit from the best of both worlds: natural evaluations for large boxes and centered evaluations for small boxes.

By default, the latter mode is used for evaluations. To select a specific mode, the ``.eval()`` method can be configured with the desired evaluation mode prior to the inputs:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [6-beg]
      :end-before: [6-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [6-beg]
      :end-before: [6-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [6-beg]
      :end-before: [6-end]
      :dedent: 0

Although this class is dedicated to inclusion functions, it can also be used for real evaluations (without interval outputs). The ``.real_eval()`` method can be used for this purpose.


Interval automatic differentiation
----------------------------------

The class provides a simple and efficient way to compute the set of derivatives of functions through its ``.diff()`` method. 
The method computes derivatives with respect to the function's interval inputs directly, avoiding approximation errors and providing guaranteed results. It works by breaking down complex functions into simpler operations, and then systematically applying the chain rule to compute derivatives of each operation. Symbolic differentiation is not involved in this process.
The output is an ``IntervalMatrix`` corresponding to an enclosure :math:`[\mathbf{J}_{\mathbf{f}}]([\mathbf{x}])` of the Jacobian :math:`\mathbf{J}_{\mathbf{f}}([\mathbf{x}])`:

.. math::

  [\mathbf{J}_\mathbf{f}]([\mathbf{x}]) = \begin{pmatrix}
  [\frac{\partial f_1}{\partial x_1}]([\mathbf{x}]) & [\frac{\partial f_1}{\partial x_2}]([\mathbf{x}]) & \cdots & [\frac{\partial f_1}{\partial x_n}]([\mathbf{x}]) \\
  [\frac{\partial f_2}{\partial x_1}]([\mathbf{x}]) & [\frac{\partial f_2}{\partial x_2}]([\mathbf{x}]) & \cdots & [\frac{\partial f_2}{\partial x_n}]([\mathbf{x}]) \\
  \vdots & \vdots & \ddots & \vdots \\
  [\frac{\partial f_m}{\partial x_1}]([\mathbf{x}]) & [\frac{\partial f_m}{\partial x_2}]([\mathbf{x}]) & \cdots & [\frac{\partial f_m}{\partial x_n}]([\mathbf{x}])
  \end{pmatrix}.

The ``.diff()`` method can be used in the same way as the ``.eval()`` method.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [7-beg]
      :end-before: [7-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [7-beg]
      :end-before: [7-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [7-beg]
      :end-before: [7-end]
      :dedent: 0


Other properties
----------------

Let us consider a function :math:`[\mathbf{f}]:\mathbb{IR}^n\to\mathbb{IR}^m`, then the dimensions :math:`n` and :math:`m` can be accessed by:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [8-beg]
      :end-before: [8-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [8-beg]
      :end-before: [8-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [8-beg]
      :end-before: [8-end]
      :dedent: 0

In the case of multivariate functions, ``.input_size()`` returns the sum of the dimensions of the arguments.


The ``AnalyticFunction`` class supports many mathematical operations, and the full set of operators that can be used is described in the next page.