.. _sec-tuto-01:

Lesson A: Getting started with intervals and contractors
========================================================

Now that Tubex has been installed on your computer or usable online, we will get used to intervals, constraints and networks of contractors.
This will allow use to perform the state estimation of a static robot between some landmarks by the end of :ref:`Lesson B <sec-tuto-02>`.

.. contents:: Content of this lesson


Start a new project
-------------------

Start a new project as explained in :ref:`sec-start-py-project` or :ref:`sec-start-cpp-project`.

.. admonition:: Exercise

  **A.0.** Check that it is displaying a tube in a graphical view and print the tube in the terminal using:

  .. tabs::

    .. code-tab:: py

      print(x)

    .. code-tab:: c++

      cout << x << endl;

  You should see the following output:

  .. code-block:: bash

    Tube [0, 10]↦([-1.448469806203122, 1.500000000000001]), 1000 slices

  | This was to be sure that everything is working well in your environment.
  | Now, we will start from the following code:

  .. tabs::

    .. code-tab:: py

      from pyibex import *
      from tubex_lib import *

      # .. next questions will be here

    .. code-tab:: c++

      #include <tubex.h>

      using namespace std;
      using namespace tubex;

      int main()
      {
        // .. next questions will be here
      }


Using intervals for handling uncertainties
------------------------------------------

The values involved in robotic problems will be represented by **sets**. This allows to hold in the very same structure both the value (a measurement, or a model parameter) together with the related uncertainty. Therefore, a measurement :math:`x` will be handled by a set, more precisely an **interval**, denoted between brackets: :math:`[x]`. :math:`[x]` is made of two real bounds, :math:`x^-` and :math:`x^+`, and we say that a value :math:`x\in\mathbb{R}` belongs to :math:`[x]=[x^-,x^+]` iff :math:`x^-\leqslant x\leqslant x^+`.

This can be extended to other types of values such as vectors, matrices or trajectories. Then,

* reals :math:`x` of :math:`\mathbb{R}` will be enclosed in intervals: :math:`[x]`
* vectors :math:`\mathbf{x}` of :math:`\mathbb{R}^n` will be enclosed in interval-vectors (also called boxes): :math:`[\mathbf{x}]`
* later on, trajectories :math:`x(t)` will belong to tubes: :math:`[x](t)`

The initial definition of the bounds of these sets will be done according to the **amount of uncertainties** we are considering. For measurements, we will rely on the datasheet of the sensor to define for instance that a measurement :math:`y` will be represented by the interval :math:`[y − 2\sigma, y + 2\sigma]`, where :math:`\sigma` is the standard deviation coming from sensors specifications. In this case, we assume that the interval :math:`[y]` is **guaranteed to contain** the actual but unknown value with a 95% confidence rate.

The main advantage of this representation is that we will be able to apply lot of **reliable** operations on these sets while preserving the actual but unknown values. This means that we will never lose a feasible solution in the initial sets throughout the operations we will perform. This is done by performing the computations on the bounds of the sets. For instance, the difference of two intervals is also an interval defined by: :math:`[x]-[y]=[x^--y^+,x^+-y^-]`. Mathematically, we can prove that :math:`\forall x\in[x]` and :math:`\forall y\in[y]`, we have :math:`(x-y)\in([x]-[y])`.

| *Example:* :math:`[3,4]-[2,6]=[-3,2]`.
| If we take :math:`x=3\in[3,4]` and :math:`y=5\in[2,6]`, we check that :math:`-2\in[-3,2]`.

These simple operations on intervals can be extended to elementary functions such as :math:`\cos`, :math:`\exp`, :math:`\tan`, *etc*.
It must be emphasized that there is no need to make linearizations when dealing with **non-linear functions**.
Sometimes, when functions are monotonic, the computation is simple: :math:`\exp([x])=[\exp(x^-),\exp(x^+)]`. Otherwise, several algorithms and libraries exist to allow any mathematical operations on intervals such as :math:`\sin([x])`,  :math:`\sqrt{([x])}`, *etc*. 

The asset of reliability coming with interval analysis will help us to estimate difficult solutions and **make proofs**.


Hello Interval Analysis!
------------------------

Tubex is using C++/Python objects to represent intervals and boxes [#f1]_:

* ``Interval(lb, ub)`` will be used to create an interval :math:`[x]=[\textrm{lb},\textrm{ub}]`. There exists predefined values for intervals. Here are some examples of ``Interval`` objects:

  .. tabs::
    
    .. code-tab:: py

      x = Interval()                              # [-∞,∞] (default value)
      x = Interval(0, 10)                         # [0,10]
      x = Interval(1, oo)                         # [1,∞]
      x = Interval(-oo,3)                         # [-∞,3]
      x = Interval.EMPTY_SET                      # ∅
      # ...

    .. code-tab:: c++

      Interval x;                                 // [-∞,∞] (default value)
      Interval x(0, 10);                          // [0,10]
      Interval x(1, oo);                          // [1,∞]
      Interval x(-oo, 3);                         // [-∞,3]
      Interval x = Interval::EMPTY_SET;           // ∅
      // ...


* | ``IntervalVector(n)`` is used for :math:`n`-d vectors of intervals, also called *boxes*.
  | For instance:

  .. tabs::
    
    .. code-tab:: py

      x = IntervalVector(2, [-1,3])               # creates [x]=[-1,3]×[-1,3]=[-1,3]^2
      y = IntervalVector([[3,4],[4,6]])           # creates [y]= [3,4]×[4,6]
      z = IntervalVector(3, [0,oo])               # creates [z]=[0,∞]^3
      w = IntervalVector(y)                       # creates a copy: [w]=[y]

      v = (0.42,0.42,0.42)                        # one vector (0.42;0.42;0.42)
      iv = IntervalVector(v)                      # creates one box that wraps v:
                                                  #   [0.42,0.42]×[0.42,0.42]×[0.42,0.42]

    .. code-tab:: c++

      IntervalVector x(2, Interval(-1,3));        // creates [x]=[-1,3]×[-1,3]=[-1,3]^2
      IntervalVector y{{3,4},{4,6}};              // creates [y]= [3,4]×[4,6]
      IntervalVector z(3, Interval(0,oo));        // creates [z]=[0,∞]^3
      IntervalVector w(y);                        // creates a copy: [w]=[y]

      Vector v(3, 0.42);                          // one vector (0.42;0.42;0.42)
      IntervalVector iv(v);                       // creates one box that wraps v:
                                                  //   [0.42,0.42]×[0.42,0.42]×[0.42,0.42]

  One can access vector components as we do classically:

  .. tabs::
    
    .. code-tab:: py

      x[1] = Interval(0,10)                       # updates to [x]=[-1,3]×[0,10]

    .. code-tab:: c++

      x[1] = Interval(0,10);                      // updates to [x]=[-1,3]×[0,10]


.. admonition:: Technical documentation

  For full details about ``Interval`` and ``IntervalVector`` objects, please read the :ref:`sec-manual-intervals` page of the user manual.


.. admonition:: Exercise

  **A.1.** Let us consider two intervals :math:`[x]=[8,10]` and :math:`[y]=[1,2]`. Without coding the operation, what would be the result of :math:`[x]/[y]` (:math:`[x]` divided by :math:`[y]`)? Remember that the result of this interval-division is also an interval enclosing all feasible divisions.

  **A.2.** In your new project, compute and print the following simple operations on intervals:
  
  * :math:`[-2,4]\cdot[1,3]`
  * :math:`[8,10]/[-1,0]`
  * :math:`[-2,4]\sqcup[6,7]` with operator ``|``
  * :math:`\max([2,7],[1,9])`
  * :math:`\max(\varnothing,[1,2])`
  * :math:`\cos([-\infty,\infty])`
  * :math:`[-1,4]^2` with function ``sqr()``
  * :math:`([1,2]\cdot[-1,3]) + \max([1,3]\cap[6,7],[1,2])`

  | Note that :math:`\sqcup` is the hull union (``|``), *i.e.*, :math:`[x]\sqcup[y] = [[x]\cup[y]]`.
  | *For instance:* :math:`[-1,2]\sqcup[4,6]=[-1,6]`
  

  **A.3.** Create a 2d box :math:`[\mathbf{y}]=[0,\pi]\times[-\pi/6,\pi/6]` and print the result of :math:`|[\mathbf{y}]|` with ``abs()``.

.. hint::

  .. rubric:: How to use :math:`\pi`?

  .. tabs::
    
    .. code-tab:: py

      # In Python, you can use the math module:
      import math
      x = math.pi

    .. code-tab:: c++

      // In C++, you can use <math.h>:
      #include <math.h>
      double x = M_PI;

  Note that in this code, the variable ``x`` is not the exact :math:`\pi`! Of course, the mathematical one cannot be represented in a computer. But with intervals, we can manage reliable representations of floating point numbers. :ref:`See more <sec-manual-intervals-pi>`.


Functions, :math:`f([x])`
-------------------------

Custom functions can be used on sets. For instance, to compute:

.. math::

  f(x)=x^2+2x-\exp(x),

a ``Function`` object can be created by ``Function("<var1>", "<var2>", ..., "<expr>")`` and then evaluated over the set :math:`[x]`:

.. tabs::
  
  .. code-tab:: py

    x = Interval(-2,2)
    f = Function("x", "x^2+2*x-exp(x)")
    y = f.eval(x)

  .. code-tab:: c++

    Interval x(-2,2);
    Function f("x", "x^2+2*x-exp(x)");
    Interval y = f.eval(x);

The first arguments of the function (only one in the above example) are its input variables. The last argument is the expression of the output. The result is the set of images of all defined inputs through the function: :math:`[f]([x])=[\{f(x)\mid x\in[x]\}]`.

We can also define vector input variables and access their components in the function definition:

.. tabs::
  
  .. code-tab:: py

    f = Function("x[2]", "cos(x[0])^2+sin(x[1])^2") # the input x is a 2d vector

  .. code-tab:: c++

    Function f("x[2]", "cos(x[0])^2+sin(x[1])^2"); // the input x is a 2d vector

.. admonition:: Exercise

  **A.4.** For our robotic applications, we often need to define the distance function :math:`g`:
  
  .. math::

    g(\mathbf{x},\mathbf{b})=\sqrt{\displaystyle(x_1-b_1)^2+(x_2-b_2)^2},

  where :math:`\mathbf{x}\in\mathbb{R}^2` would represent for instance the 2d position of a robot, and :math:`\mathbf{b}\in\mathbb{R}^2` the 2d location of some landmark. Create :math:`g` and compute the interval distance :math:`[d]` between the boxes :math:`[\mathbf{x}]=[0,0]\times[0,0]` and :math:`[\mathbf{b}]=[3,4]\times[2,3]`. Note that in the library, the ``.eval()`` of functions only takes one argument: we have to concatenate the boxes :math:`[\mathbf{x}]` and :math:`[\mathbf{b}]` into one 4d interval-vector :math:`[\mathbf{c}]` and then compute :math:`g([\mathbf{c}])`.

  Print the result that you obtain for :math:`[d]=g([\mathbf{x}],[\mathbf{b}])`.


Graphics
--------

The graphical tool `VIBes <http://enstabretagnerobotics.github.io/VIBES/>`_ has been created to Visualize Intervals and BoxES. It is compatible with simple objects such as ``Interval`` and ``IntervalVector``. Its features have been extended in the Tubex library with objects such as ``VIBesFigMap``.

.. admonition:: Exercise

  **A.5.** Create a view with:

  .. tabs::
    
    .. code-tab:: py

      beginDrawing()
      fig = VIBesFigMap("Map")
      fig.set_properties(50, 50, 400, 400) # position and size
      
      # ... draw objects here
      
      fig.show() # display all items of the figure
      endDrawing()

    .. code-tab:: c++

      vibes::beginDrawing();
      VIBesFigMap fig("Map");
      fig.set_properties(50, 50, 400, 400); // position and size
      
      // ... draw objects here
      
      fig.show(); // display all items of the figure
      vibes::endDrawing();

  | **A.6.** Before the ``.show()`` method, draw the boxes :math:`[\mathbf{x}]` and :math:`[\mathbf{b}]` with the ``fig.draw_box(..)`` method. The computed interval range :math:`[d]` can be displayed as a ring centered on :math:`\mathbf{x}=(0,0)`. The ring will contain the set of all positions that are :math:`d`-distant from :math:`\mathbf{x}=(0,0)`, with :math:`d\in[d]`.

  To display each bound of the ring, you can use ``fig.draw_circle(x, y, rad)`` where ``x``, ``y``, ``rad`` are *double* values.

  .. hint::

    To access *double* bounds of an interval object ``x``, you can use the ``x.lb()``/``x.ub()`` methods for lower and upper bounds.

  | **A.7.** Now, repeat the operation with :math:`[\mathbf{x}]=[-0.1,0.1]\times[-0.1,0.1]`. You can for instance use the ``.inflate(0.1)`` method on ``x``.
  | Is the result reliable, according to the sets :math:`[\mathbf{x}]` and :math:`[\mathbf{b}]`? You may display the box :math:`([\mathbf{x}]+[\mathbf{b}])` to understand how the reliable interval distance is computed.


.. from pyibex import *
.. from tubex_lib import *
.. 
.. x = IntervalVector([[0,0],[0,0]])
.. b = IntervalVector([[3,4],[2,3]])
.. print(b)
.. 
.. x.inflate(0.1)
.. 
.. f = Function("a[2]", "b[2]", "sqrt((a[0]-b[0])^2+(a[1]-b[1])^2)")
.. 
.. box = cart_prod(x,b)
.. r = f.eval(box)
.. 
.. beginDrawing()
.. fig = VIBesFigMap("Map")
.. fig.set_properties(50, 50, 400, 400) # position and size
.. fig.draw_box(x, "red")
.. fig.draw_box(b)
.. fig.draw_box(b+x, "blue")
.. fig.draw_ring(0,0,r)
.. fig.show() # display all items of the figure
.. endDrawing()


.. admonition:: Technical documentation

  For full details about graphical features, please read the :ref:`sec-manual-vibes` page of the user manual.

  .. rubric:: Want to use colors? Here is an example you can try:

  .. tabs::
    
    .. code-tab:: py

      fig.draw_box(x, "red[yellow]") # red: edge color of the box, yellow: fill color

    .. code-tab:: c++

      fig.draw_box(x, "red[yellow]"); // red: edge color of the box, yellow: fill color


Contractors, :math:`\mathcal{C}([x])`
-------------------------------------

This was an initial overview of what is Interval Analysis. Now, we will introduce concepts from Constraint Programming and see how the two approaches can be coupled for solving problems.

In robotics, **constraints** are coming from the equations of the robot. They can be for instance the evolution function :math:`\mathbf{f}` or the observation equation with :math:`\mathbf{g}`. In the case of :abbr:`SLAM (Simultaneous Localization And Mapping)`, we may also define a constraint to express the inter-temporal relation between different states :math:`\mathbf{x}_1`, :math:`\mathbf{x}_2` at times :math:`t_1`, :math:`t_2`, for instance when a landmark has been seen two times.

Now, we want to apply the constraints in order to solve our problem. In the Constraint Programming community, we apply constraints on **domains** that represent sets of feasible values. The previously mentioned sets (intervals, boxes, tubes) will be used as domains. 

We will use **contractors** to implement constraints on sets. They are mathematical operators used to *contract* (reduce) a set, for instance a box, without losing any feasible solution. This way, contractors can be applied safely any time we want on our domains.

In Tubex, the contractors are also defined by C++/Python objects and are prefixed with ``Ctc``. For this lesson, we will use the ``CtcFunction`` class to define a contractor according to a function :math:`f`. Note that the resulting contractor will aim at solving a constraint in the form :math:`f(\mathbf{x})=0`. This contractor has to be instantiated from a ``Function`` object defining the constraint. For instance, the simple constraint :math:`(x+y=a)` is expressed as :math:`f(x,y,a)=x+y-a=0`, and can be implemented as a contractor :math:`\mathcal{C}_+` with:

.. tabs::

  .. code-tab:: py

    ctc_add = CtcFunction(Function("x", "y", "a", "x+y-a"))

  .. code-tab:: c++

    CtcFunction ctc_add(Function("x", "y", "a", "x+y-a"));

.. admonition:: Exercise

  **A.8.** Define a contractor :math:`\mathcal{C}_\textrm{dist}` related to the distance constraint between two 2d positions :math:`\mathbf{x}` and :math:`\mathbf{b}\in\mathbb{R}^2`. We will use the distance function previously defined, but in the form :math:`f(\mathbf{x},\mathbf{b},d)=0`.

| The contractor is then simply added to a **Contractor Network** (CN) that will manage the constraints on the different variables for solving the problem.
| For instance, we can use the previously defined :math:`\mathcal{C}_+` as:

.. tabs::

  .. code-tab:: py

    x = Interval(0,1)
    y = Interval(-2,3)
    a = Interval(1,20)
    
    cn = ContractorNetwork()   # Creating a Contractor Network
    cn.add(ctc_add, [x, y, a]) # Adding the C+ contractor to the network, 
                               # applied on three domains listed between braces
    cn.contract()
    
    # The three domains are then contracted as:
    # x=[0, 1], y=[0, 3], a=[1, 4]

  .. code-tab:: c++

    Interval x(0,1), y(-2,3), a(1,20);
    
    ContractorNetwork cn;       // Creating a Contractor Network
    cn.add(ctc_add, {x, y, a}); // Adding the C+ contractor to the network, 
                                // applied on three domains listed between braces
    cn.contract();
    
    // The three domains are then contracted as:
    // x=[0, 1], y=[0, 3], a=[1, 4]

Note that one contractor can be added several times in the CN. This is useful to apply several constraints implemented by the same operator, on different sets of variables.


.. admonition:: Exercise

  | **A.9.** Define a Contractor Network to implement three distance constraints.
  | Check the results with :math:`\mathcal{C}_\textrm{dist}([\mathbf{x}],[\mathbf{b}^i],[d])`, :math:`i\in\{1,2,3\}` and 
  
  * :math:`[d]=[7,8]`
  * :math:`[\mathbf{x}]=[0,0]^2`
  * :math:`[\mathbf{b}^1]=[1.5,2.5]\times[4,11]`
  * :math:`[\mathbf{b}^2]=[3,4]\times[4,6.5]`
  * :math:`[\mathbf{b}^3]=[5,7]\times[5.5,8]`

  We recall that the same :math:`\mathcal{C}_\textrm{dist}` object can appear several times in the CN.

  Draw the :math:`[\mathbf{b}^i]` boxes (``.draw_box()``) and :math:`[d]` (``.draw_circle()``) before and after the contractions, in order to assess the contracting effects.
  You should obtain this figure:

  .. figure:: img/ctc_dist.png
    :width: 500px

  As you can see, the four domains have been contracted after the ``.contract()`` method: even the bounded range :math:`[d]` has been reduced thanks to the knowledge provided by the boxes. In Constraint Programming, we only define the constraints of the problem and let the resolution propagate the information as much as possible.


We now have all the material to compute a solver for state estimation in the next section.


.. rubric:: Footnotes

.. [#f1] C++ objects originates from the `IBEX library <http://www.ibex-lib.org>`_, the Python counterpart comes from `pyIbex <http://benensta.github.io/pyIbex>`_.