.. _sec-manual-functions:

###################
Inclusion functions
###################

.. contents::

This section focuses on mathematical functions (that are not related to programming functions) related to intervals, tubes, etc.

We use an inclusion function :math:`[\mathbf{f}]:\mathbb{IR}^n\to\mathbb{IR}^m` to enclose the image of a box :math:`[\mathbf{x}]` by :math:`\mathbf{f}` in a box such that :math:`\forall[\mathbf{x}]\in\mathbb{IR}^n, \mathbf{f}([\mathbf{x}])\subset[\mathbf{f}]([\mathbf{x}])`. Hence, a reliable enclosure of
the image set can be evaluated reasonably quickly. Furthermore, inclusion functions
can stand on analytical expressions or even algorithms based on datasets. The library provides a way to compute inclusion functions automatically from mathematical expressions. Evaluations of these functions, considering boxes or tubes as inputs, is then achievable.


An extended interval calculator
-------------------------------

Codac allows you to declare symbolically a function and to perform interval computations with it. For example, the following code:

.. tabs::

  .. code-tab:: py

    g = Function("x[2]", "b[2]", "sqrt(sqr(x[0]-b[0])+sqr(x[1]-b[1]))")
    f = TFunction("x", "y", "sin(x)+exp(t*y)")

  .. code-tab:: c++

    Function g("x[2]", "b[2]", "sqrt(sqr(x[0]-b[0])+sqr(x[1]-b[1]))");
    TFunction f("x", "y", "sin(x)+exp(t*y)");

defines the mathematical objects:

:math:`\left(\begin{array}{c}\mathbf{x}\\\mathbf{b}\end{array}\right) \mapsto \sqrt{(x_1-b_1)^2+(x_2-b_2)^2}`

and

:math:`\left(\begin{array}{c}x(t)\\y(t)\end{array}\right) \mapsto \sin\big(x(t)\big)+\exp\big(t\cdot y(t)\big)`.


In Codac we distinct two kinds of functions:

* *static* functions, such as :math:`\mathbf{x}\mapsto\sin(x_1)+\cos(\exp(x_2))`, implemented by ``Function`` objects;
* *temporal* functions that are time related. These *t*-functions are made for (inter)-temporal evaluations such as integral computations, delays, time uncertainties, and so on. The system variable :math:`t` is a predefined variable of ``TFunction`` objects.


Syntax
------

Function objects are only defined with strings. First arguments define variables while the last one represents the expression of the function. Vector arguments can be defined with an array like syntax, see for instance function ``g`` in the above code example.

Vector valued function can be written by using parenthesis syntax ``(.., .., ..)`` for the return value. For instance,

.. math::

  h:(x,y,z)\mapsto \left\{\begin{array}{l} x^2-y \\  y-2z \\ z^3-\sqrt{y-x}\end{array}\right.

is defined by:

.. tabs::

  .. code-tab:: py

    h = Function("x","y","z", "(x^2-y, y-2*z, z^3-sqrt(y-x))")
    #                         ^ returned value

  .. code-tab:: c++

    Function h("x","y","z", "(x^2-y, y-2*z, z^3-sqrt(y-x))");
    //                      ^ returned value


Functions can then be evaluated on intervals or boxes by using the ``eval(..)`` method, or ``eval_vector(..)`` in the case of vector returned values. ``TFunction`` are used on ``TubeVector`` objects, even if the time :math:`t` does not appear in the mathematical expression.

.. tabs::

  .. code-tab:: py

    # Simple scalar function:
    k = Function("x", "x*cos(x)")
    # Scalar function with vector arguments:
    g = Function("x[2]", "b[2]", "sqrt(sqr(x[0]-b[0])+sqr(x[1]-b[1]))")
    # Temporal function:
    f = TFunction("x", "sin(x)+exp(t*x)")
    # Vector valued function:
    h = Function("x","y","z", "(x^2-y, y-2*z, z^3-sqrt(y-x))")

    k.eval(Interval(-1,1))
    # ^ returns the interval [-1,1]

    h.eval_vector(IntervalVector([[-oo,1],[1,2],[-1,1]]))
    # ^ returns the box [-2,oo]×[-1,4]×[-oo,1]

    x1 = IntervalVector([[1,2],[-1,1]])
    m1 = IntervalVector(2, [4,5])
    g.eval(cart_prod(x1,m1))
    # ^ returns the interval [3.605..,7.211..]

    y = TubeVector(Interval(0,1), 0.1, TFunction("(sin(t))"))
    f.eval(y)
    # ^ returns the tube [0, 1]↦([0.999.., 3.065..]), 10 slices

  .. code-tab:: c++

    // Simple scalar function:
    Function k("x", "x*cos(x)");
    // Scalar function with vector arguments:
    Function g("x[2]", "b[2]", "sqrt(sqr(x[0]-b[0])+sqr(x[1]-b[1]))");
    // Temporal function:
    TFunction f("x", "sin(x)+exp(t*x)");
    // Vector valued function:
    Function h("x","y","z", "(x^2-y, y-2*z, z^3-sqrt(y-x))");

    k.eval(Interval(-1,1));
    // ^ returns the interval [-1,1]

    h.eval_vector(IntervalVector({{-oo,1},{1,2},{-1,1}}));
    // ^ returns the box [-2,oo]×[-1,4]×[-oo,1]

    IntervalVector x1({{1,2},{-1,1}});
    IntervalVector m1(2, Interval(4,5));
    g.eval(cart_prod(x1,m1));
    // ^ returns the interval [3.605..,7.211..]

    TubeVector y(Interval(0,1), 0.1, TFunction("(sin(t))"));
    f.eval(y);
    // ^ returns the tube [0, 1]↦([0.999.., 3.065..]), 10 slices


.. note::

  .. Figure:: ../04-static-contractors/img/CtcFunction_small.png
    :align: right
  
  For using contractors defined from functions, the ``CtcFunction`` class is available.

  :ref:`See more <sec-manual-ctcfunction>`.

Function formatting
-------------------

Let's consider the function that gives the distance to a beacon

:math:`f : \left(\begin{array}{c}\mathbf{x}\\\mathbf{b}\end{array}\right) \mapsto \sqrt{(x_1-b_1)^2+(x_2-b_2)^2}`

If :math:`\mathbf{b} = (b_1, b_2)^\intercal` is known *a priori*, for instance let :math:`\mathbf{b} = (1, 2)^\intercal`, its values could be hard-coded in the ``Function`` object:

.. tabs::

  .. code-tab:: py

    f = Function("x[2]", "sqrt(sqr(x[0]-1)+sqr(x[1]-2))")

  .. code-tab:: c++

    Function f("x[2]", "sqrt(sqr(x[0]-1)+sqr(x[1]-2))");

But sometimes it is useful to define this function at run-time, for example when the coordinates need to be calculated first. Then, the ``Function`` can be defined using string formatting. String formatting is natively supported in `Python3 <https://docs.python.org/3/library/string.html>`_ and is introduced in `C++20 <https://en.cppreference.com/w/cpp/utility/format/format>`_. For users of an older version of C++, a string formatting library can be used like `fmt <https://fmt.dev/latest/index.html>`_.

.. tabs::

  .. code-tab:: py

    f = Function("x[2]", "sqrt(sqr(x[0]-{0})+sqr(x[1]-{1}))".format(b1, b2))

  .. code-tab:: c++
    
    std::string function = fmt::format("sqrt(sqr(x[0]-{0})+sqr(x[1]-{1}))", b1, b2);
    Function f("x[2]", function.c_str());