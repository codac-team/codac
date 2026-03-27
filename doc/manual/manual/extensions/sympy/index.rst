.. _sec-extensions-sympy:

SymPy (for symbolic mathematics)
================================

  Main authors: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

The ``AnalyticFunction`` class already provides interval automatic differentiation through its ``.diff()`` method.
This built-in differentiation computes enclosures of Jacobian values over interval inputs and is the natural tool for guaranteed interval analysis.

In some situations however, one may need an explicit *symbolic* expression obtained from an existing Codac analytic expression:

* a simplified expression;
* a symbolic derivative;
* a symbolic gradient, Hessian, or Jacobian;
* a truncated Taylor expansion;
* a polynomial rewritten in Horner form;
* or a symbolic equality test between two ``AnalyticFunction`` objects.

This is the purpose of the SymPy bridge presented on this page.
It converts a Codac analytic expression into a SymPy expression, applies symbolic transformations in SymPy, and converts the result back into a Codac ``AnalyticFunction``.

The result remains a first-class Codac object: it can be evaluated, composed, differentiated by intervals, or reused in contractors exactly like any other ``AnalyticFunction``.

Installing the ``codac-sympy`` extension
----------------------------------------

This feature is provided as a Codac extension.

In C++, using it requires:

* a Codac build configured with the same Python support as the Python binding, see in particular :ref:`sec-dev-info-binding`;
* and the Python package ``sympy`` available in the Python environment used by the extension.

In Python, the extension is included by default in Codac.
The remaining requirement is therefore simply to install the Python package ``sympy``:

.. code-block:: bash

   pip install sympy

The same package is also required by the C++ extension, since the symbolic backend relies on the Python SymPy runtime.
The Python interpreter is managed internally by the extension in C++, and is of course already available when Codac is used from Python.
No explicit interpreter initialization is needed in user code.

Finally, in order to include the features of the extension:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-1-beg]
      :end-before: [sympy-1-end]
      :dedent: 0

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-1-beg]
      :end-before: [sympy-1-end]
      :dedent: 0

  .. code-tab:: matlab

    % todo


Simplification
--------------

Simplification is often the best first symbolic operation to apply.
It can remove obvious redundancies, expose simpler formulas, and produce expressions that are easier to read, compare, or differentiate.

.. doxygenfunction:: codac2::sympy_simplify(const AnalyticFunction<ScalarType>&)
  :project: codac

A classical trigonometric identity is simplified as expected:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-2-beg]
      :end-before: [sympy-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-2-beg]
      :end-before: [sympy-2-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

Simplification is also useful on purely algebraic expressions:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-3-beg]
      :end-before: [sympy-3-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-3-beg]
      :end-before: [sympy-3-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

Partial derivatives and symbolic derivatives
--------------------------------------------

The primitive symbolic differentiation operator is ``sympy_partial_diff``.
It returns the symbolic partial derivative of a scalar-valued function with respect to one input variable.

.. doxygenfunction:: codac2::sympy_partial_diff(const AnalyticFunction<ScalarType>&, const ScalarVar&)
  :project: codac

For scalar arguments:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-4-beg]
      :end-before: [sympy-4-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-4-beg]
      :end-before: [sympy-4-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

When a vector argument is involved, a direct component can be used as the differentiation variable.
Note that this notation is usually clearer than referring to a flattened input index, but this type of use is also possible.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-5-beg]
      :end-before: [sympy-5-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-5-beg]
      :end-before: [sympy-5-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

The function ``sympy_diff`` provides symbolic derivatives for scalar functions.
For a univariate scalar function, ``sympy_diff(f)`` returns the first derivative.
Higher-order derivatives are also available.

.. doxygenfunction:: codac2::sympy_diff(const AnalyticFunction<ScalarType>&,const ScalarVar&,Index)
  :project: codac

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-6-beg]
      :end-before: [sympy-6-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-6-beg]
      :end-before: [sympy-6-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

Gradient, Hessian and Jacobian
------------------------------

The symbolic gradient of a scalar function is obtained with ``sympy_gradient``.

.. doxygenfunction:: codac2::sympy_gradient(const AnalyticFunction<ScalarType>&)
  :project: codac

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-7-beg]
      :end-before: [sympy-7-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-7-beg]
      :end-before: [sympy-7-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

The symbolic Hessian matrix of a scalar function is obtained with ``sympy_hessian``.

.. doxygenfunction:: codac2::sympy_hessian(const AnalyticFunction<ScalarType>&)
  :project: codac

For the scalar function

.. math::

   f(x,y) = x y + \sin(x) + y^2,

its Hessian is

.. math::

   H_f(x,y) =
   \begin{pmatrix}
     -\sin(x) & 1 \\
     1 & 2
   \end{pmatrix}.

In Codac:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-8-beg]
      :end-before: [sympy-8-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-8-beg]
      :end-before: [sympy-8-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

For a vector-valued function :math:`\mathbf{f} : \mathbb{R}^n \to \mathbb{R}^m`, symbolic differentiation returns a Jacobian matrix as another ``AnalyticFunction``:

.. doxygenfunction:: codac2::sympy_diff(const AnalyticFunction<VectorType>&)
  :project: codac

.. math::

   J_{\mathbf{f}}(\mathbf{x}) =
   \begin{pmatrix}
   \dfrac{\partial f_1}{\partial x_1} & \cdots & \dfrac{\partial f_1}{\partial x_n} \\
   \vdots & \ddots & \vdots \\
   \dfrac{\partial f_m}{\partial x_1} & \cdots & \dfrac{\partial f_m}{\partial x_n}
   \end{pmatrix}.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-9-beg]
      :end-before: [sympy-9-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-9-beg]
      :end-before: [sympy-9-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

Symbolic equality
-----------------

Symbolic equality is especially useful to validate a transformation, a differentiation result, or a rewriting.

.. doxygenfunction:: codac2::sympy_equal(const AnalyticFunction<ScalarType>&, const AnalyticFunction<ScalarType>&)
  :project: codac

For example, the following two scalar functions are symbolically equal although they are expressed with different arguments:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-10-beg]
      :end-before: [sympy-10-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-10-beg]
      :end-before: [sympy-10-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

Series expansions
-----------------

The bridge can also compute truncated Taylor series for scalar functions.

.. doxygenfunction:: codac2::sympy_series(const AnalyticFunction<ScalarType>&, const ScalarVar&, double, Index)
  :project: codac

For a univariate function:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-11-beg]
      :end-before: [sympy-11-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-11-beg]
      :end-before: [sympy-11-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

A multivariate function can also be expanded with respect to one scalar variable while the other arguments are treated as constants:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-12-beg]
      :end-before: [sympy-12-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-12-beg]
      :end-before: [sympy-12-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

Horner form
-----------

For polynomial expressions, Horner form is a classical rewriting in interval methods.
It is usually much more favorable for interval evaluations because it reduces repeated occurrences of the variables and therefore helps reduce pessimism.
In practice, this may significantly improve interval polynomial evaluations.

.. doxygenfunction:: codac2::sympy_horner(const AnalyticFunction<ScalarType>&)
  :project: codac

For the polynomial

.. math::

   p(x) = 2x^5 + x^3 - 3x^2,

SymPy can rewrite the expression in Horner form as

.. math::

   p(x) = x^2\bigl(-3+x(1+2x^2)\bigr).

In Codac:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [sympy-13-beg]
      :end-before: [sympy-13-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [sympy-13-beg]
      :end-before: [sympy-13-end]
      :dedent: 4

  .. code-tab:: matlab

    % todo

Relationship with interval automatic differentiation
----------------------------------------------------

Symbolic differentiation with SymPy and interval automatic differentiation address two related but different needs.

Interval automatic differentiation:

* works directly on interval inputs;
* returns an ``IntervalMatrix`` enclosure of Jacobian values;
* is guaranteed and specifically designed for interval computations.

Symbolic differentiation with SymPy:

* works on the symbolic structure of the expression;
* returns another ``AnalyticFunction``;
* is useful when one needs an explicit derivative, Hessian, Jacobian, or rewritten expression that can later be evaluated, composed, or manipulated as a Codac function.

Symbolic differentiation does not replace interval differentiation.
It complements it.

Extending the bridge
--------------------

The SymPy bridge already covers a practical subset of Codac analytic expressions and symbolic transformations.
It is however intended to grow with user needs.

If a missing operator, rewriting, simplification, or symbolic transformation would be useful for your applications, please contact the Codac developers.
This extension can be enriched progressively.
