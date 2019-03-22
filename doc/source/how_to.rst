How to handle tubes with Tubex
==============================

Start a program
---------------

To write a program with Tubex, use the following canvas:

.. code-block:: c++

  #include "tubex.h"

  using namespace std;
  using namespace tubex;

  int main()
  {
    // write your own code here

  }


Instantiation
-------------

To create a :code:`Tube` object initialized with a constant image :math:`[y],\forall t`:

.. code-block:: c++

  float timestep = 0.1;     // slice width
  Interval domain(0,10);    // [t0,tf]

  Tube x(domain, timestep); // (by default) [y]=[-oo,oo]
  Tube y(domain, timestep, Interval::POS_REALS); // [y]=[0,oo]
  Tube z(domain, timestep, Interval(-42.0, 17.0)); // [y]=[-42.0, 17.0]
  Tube w(x, 3.0); // same structure as x, with [y]=[3]

To create a :code:`Tube` object from an analytical expression:

.. code-block:: c++
  
  // [x1](t)=[f](t)
  Tube x1(domain, timestep,
          Function("t", "(t-5)^2 + [-0.5,0.5]")); // [f](t)
  // [x2](t)=f(t)+[-0.5,0.5]
  Tube x2(domain, timestep,
          Function("t", "(t-5)^2"), // f(t)
          Interval(-0.5,0.5)); 
  // [x3](t)=[f1(t),f2(t)]
  Tube x3(domain, timestep,
          Function("t", "(t-5)^2 - 0.5"),  // f1(t)
          Function("t", "(t-5)^2 + 0.5")); // f2(t)
  // note: x1==x2==x3

To create a :code:`Tube` object from a map of values (will strictly wrap the map with linear interpolation):

.. code-block:: c++

  map<double,double> map_values;
  map_values[0.0] = 2.0;  map_values[1.0] = 3.2;
  map_values[1.2] = 3.4;  map_values[8.5] = -36.0;

  Tube x1(domain, timestep, Interval::EMPTY_SET);
  Tube x2(x1); // x2 is empty, as well as x1

  // Strict wrapping:
  x1.feed(map_values);
  // For a thickness of 2, centered on the values:
  x2.feed(map_values, Interval(-1.0,1.0));

  // Inflation of x1 (then x1==x2):
  x1.inflate(1.0);


Basics
------

Basic features:

.. code-block:: c++

  double vol = x1.volume(); // sum of each slice volume
  double distance = x1.dist(x2); // difference between tubes volumes
  double timestep = x1.dt(); // sampling time
  bool emptiness = x1.isEmpty();
  bool discontinuity = x1.isDiscontinuous();
  Interval intv_t = x1.domain(); // [t0,tf]
  bool test = x1.isInteriorSubset(x2); // (x1 \subset x2)?
  Tube subtube = x1.subtube(Interval(2.0,5.0));

Working with tube's slices:

.. code-block:: c++

  int nb_slices = x1.size(); // number of slices

  double t1 = 2.3;
  int i = x1.input2index(t1); // index of the slice containing input t1
  double ti = x1.index2input(i); // input referencing the slice 'i'
  // Note: ti not necessarily equal to t1

Classical operations on sets are applicable on similar tubes:

.. code-block:: c++

  Tube x4 = (x1 | x2) & x3;

**Note:** by *similar* we mean tubes of same timestep and domain.


Evaluation and set-inversion
----------------------------

To evaluate a specific part of a :code:`Tube` object:

.. code-block:: c++

  // Union of bounded values (tube image):
  Interval y1 = x1.image();
  Interval y2 = x1[Interval(2.0,8.5)]; // over a subdomain
  Interval y3 = x1[5.08];              // at a given input t
  Interval y4 = x1[(int)5];            // of a given slice (6th)
  // Note: y4 == y3 \in y2 \subset y1

To enclose the bounds of :math:`f^-` or :math:`f^+`:

.. code-block:: c++

  pair<Interval,Interval> p_bounds;

  p_bounds = x1.eval();
  // p_bounds.first  -> union of f^-(t), for all t
  // p_bounds.second -> union of f^+(t), for all t

  // Same operation, restricted over a subdomain [8,10]
  p_bounds = x1.eval(Interval(8,10));

A tube set-inversion may result in a set of intervals, each one of them being a subset of the tube domain (see definition in :ref:`theory`). The :code:`invert` method returns the union of these subsets, or the set of solutions within a :code:`vector` of :code:`Interval` objects.

.. code-block:: c++

  // Approximation of the Kernel of x1:
  Interval kernel = x1.invert(0);

  // Set-inversion: [x1]^-1([2,3])
  Interval intv_ta = x1.invert(Interval(2,3));

  // Same set-inversion restricted over a subdomain [8,10]
  Interval intv_tb = x1.invert(Interval(2,3), Interval(8,10));

  // Set-inversion returning sets of solutions:
  vector<Interval> v_intv_t;
  x1.invert(Interval(-1,1), v_intv_t);


Updates
-------

Setting a value for its :math:`k`-th slice (second argument has to be :code:`int`):

.. code-block:: c++

  // Setting values (for a slice)
  x1.set(Interval(3,8), 4); // k==5


Setting a slice's value from an input :math:`t` (second argument has to be :code:`double`):

.. code-block:: c++

  // Setting values (for a given date)
  x1.set(Interval(-3,3), 4.2);

Setting values over a given subdomain :math:`[t]\subseteq[t_0,t_f]` (second argument has to be an :code:`Interval`):

.. code-block:: c++

  // Setting values (within a given interval of times)
  x1.set(Interval(1,3), Interval(6.2,6.7)); // [y],[t]

**Note:** be careful when updating a tube without the use of dedicated contractors. Tube discretization has to be
kept in mind whenever an update is performed for some input :math:`t`. For guaranteed operations, please
see the :ref:`contractors` section.


Tube arithmetic
---------------

The following operations have to be performed on similar tubes.
By *similar* we mean tubes of same timestep and domain.

Classical mathematical functions are applicable on tubes:

.. code-block:: c++

  Tube x2 = abs(x1);
  Tube x3 = cos(x1) + sqrt(x2 + pow(x1, Interval(2,3)));


.. _contractors:

Contractors
-----------

Differential constraint
^^^^^^^^^^^^^^^^^^^^^^^

The contractor :math:`\mathcal{C}_{\frac{d}{dt}}` relies on the differential constraint :math:`\dot{x}=v`, with :math:`x(\cdot)\in[x](\cdot)`, :math:`v(\cdot)\in[v](\cdot)`.
It is applicable by the method :code:`ctcFwdBwd()`.
This has been the subject of the paper `Guaranteed computation of robot trajectories <http://simon-rohou.fr/research/tubint/tubint_paper.pdf>`_.

*Note:* a contraction cannot be expected for the tube :math:`[v](\cdot)`.

.. code-block:: c++

  Tube x(domain, timestep), v(domain, timestep);
  bool contraction = x.ctcFwdBwd(v);
  // contraction is 'true' in case of any contraction on x

Evaluation constraint
^^^^^^^^^^^^^^^^^^^^^

The contractor :math:`\mathcal{C}_{\textrm{eval}}` relies on the evaluation constraint :math:`y=x(t)`, with :math:`t\in[t]`, :math:`y\in[y]`, :math:`x(\cdot)\in[x](\cdot)`.
It is applicable by the method :code:`ctcEval()`.
This has been the subject of the paper `Reliable non-linear state estimation involving time uncertainties <http://simon-rohou.fr/research/tubeval/tubeval_paper.pdf>`_.

*Note:* the derivative tube :math:`[v](\cdot)` is required.

.. code-block:: c++
  
  Interval intv_t, intv_y;
  Tube x(domain, timestep), v(domain, timestep);
  bool contraction = x.ctcEval(v, intv_t, intv_y);
  // contraction is 'true' in case of
  // any contraction on x, intv_t or intv_y


Non-observation constraint
^^^^^^^^^^^^^^^^^^^^^^^^^^

The contractor :math:`\mathcal{C}_{\textrm{out}}` relies on the observation constraint :math:`y\not=x(t)`, :math:`\forall t\in[t],y\in[y]`, :math:`x(\cdot)\in[x](\cdot)`.
It is applicable by the method :code:`ctcOut()`.

*Note:* a contraction cannot be expected for :math:`[t]` or :math:`[y]`.

.. code-block:: c++
  
  Interval intv_t, intv_y;
  Tube x(domain, timestep);
  bool contraction = x.ctcOut(intv_t, intv_y);
  // contraction is 'true' in case of any contraction on x


Inter-temporal constraint
^^^^^^^^^^^^^^^^^^^^^^^^^

The contractor :math:`\mathcal{C}_{t_1,t_2}` relies on the evaluation constraint :math:`x(t_1)=x(t_2)`, with :math:`t_1\in[t_1]`, :math:`t_2\in[t_2]`, :math:`x(\cdot)\in[x](\cdot)`.
It is applicable by the method :code:`ctcIntertemporal()`.
This will be the subject of the paper *Loop-based localization method for mobile robots*.

*Note:* in this implementation, a contraction cannot be expected for the tube :math:`[x](\cdot)`.

.. code-block:: c++

  Interval t1, t2;
  bool contraction = x.ctcIntertemporal(t1, t2);
  // contraction is 'true' in case of any contraction on t1 or t2

An extension is available for the constraint :math:`x(t_1)=x(t_2)=y`, with :math:`t_1\in[t_1]`, :math:`t_2\in[t_2]`, :math:`y\in[y]`, :math:`x(\cdot)\in[x](\cdot)`.

.. code-block:: c++

  Interval t1, t2, y;
  bool contraction = x.ctcIntertemporal(y, t1, t2);
  // contraction is 'true' in case of any contraction on t1, t2 or y


Periodic constraint
^^^^^^^^^^^^^^^^^^^

The contractor :math:`\mathcal{C}_{T}` relies on the observation constraint :math:`x(t)=x(t+T)`, with :math:`T\in[T]`, :math:`x(\cdot)\in[x](\cdot)`.

.. code-block:: c++

  Interval T;
  bool contraction = x.ctcPeriodic(T);
  // contraction is 'true' in case of any contraction on x or T


Algebraic constraints
^^^^^^^^^^^^^^^^^^^^^

Custom contractors can be implemented based on any algebraic constraint thanks to the `IBEX library <http://www.ibex-lib.org/>`_.
A :code:`Function` object (`see more <http://www.ibex-lib.org/doc/function.html>`_) has to be defined so that it vanishes when the constraint is achieved.

Example for the minimal contractor :math:`\mathcal{C}_{+}` presented in :ref:`theory`.

.. code-block:: c++

  bool contraction = contract(a, x, y,
          Function("a", "x", "y", "a - (x + y)"));
  // contraction is 'true' in case of any contraction on a, x or y

Another example with a *distance* constraint :math:`d=\sqrt{x^2+y^2}` applied on :math:`[x](\cdot)`, :math:`[y](\cdot)` and :math:`[d](\cdot)`.

.. code-block:: c++

  bool contraction = contract(x, y, d,
          Function("x", "y", "d", "d - sqrt(x^2+y^2)"));
  // contraction is 'true' in case of any contraction on x, y or d


Integration
-----------

Computation of the primitive :math:`\int_{0}[x](\tau)d\tau`:

.. code-block:: c++
  
  Tube primitive = x.primitive();

Computation of :math:`[s]=\int_{0}^{[t]}[x](\tau)d\tau`:

.. code-block:: c++
  
  Interval t;
  Interval s = x.integral(t);

Computation of :math:`[s]=\int_{[t_1]}^{[t_2]}[x](\tau)d\tau`:

.. code-block:: c++
  
  Interval t1, t2;
  Interval s = x.integral(t1, t2);

A decomposition of the integral of :math:`[x](\cdot)=[x^-(\cdot),x^+(\cdot)]` with :math:`[s^-]=\int_{[t_1]}^{[t_2]}x^-(\tau)d\tau` and :math:`[s^+]=\int_{[t_1]}^{[t_2]}x^+(\tau)d\tau` is computable by:

.. code-block:: c++

  Interval t1, t2;
  pair<Interval,Interval> s;
  s = x.partialIntegral(t1, t2);
  // s.first is [s^-]
  // s.second is [s^+]

*Note:* :math:`[s]=[s^-]\cup[s^+]`.


Serialization
-------------

In case of heavy computations, a :code:`Tube` object can be serialized within a binary file and fastly re-created afterwards:

.. code-block:: c++

  Tube x1(domain, timestep);
  x1.serialize("x1.tube"); // will create a binary file "x1.tube"
  Tube x2("x1.tube"); // will create a new tube x2 from the binary file
  // then, x1==x2

Maps of values can be serialized jointly with the :code:`Tube` object:

.. code-block:: c++

  Tube x1(domain, timestep);
  map<double,double> map_values_a, map_values_b, map_values_c;

  // Serialization of one map:
  x1.serialize("x1_one_map.tube", map_values_a);

  // Serialization of a set of maps:
  vector<map<double,double> > vector_maps;
  vector_maps.push_back(map_values_a);
  vector_maps.push_back(map_values_b);
  x1.serialize("x1_all_maps.tube", vector_maps);

  // Deserialization:
  vector_maps.clear();
  Tube x2("x1_one_map.tube", map_values_c);
  Tube x3("x1_all_maps.tube", vector_maps);

  // then, x1==x2==x3 and map_values_c==map_values_a


Graphics
--------

To print a :code:`Tube` object in a terminal:

.. code-block:: c++
  
  cout << x1;
  // or, for a custom decimal precision
  x1.print(5); // new precision: 5

A simple 2D rendering of a tube is available using the `Vibes viewer <http://enstabretagnerobotics.github.io/VIBES/>`_:

.. code-block:: c++

  VibesFigure_Tube::show(&x1, "Tube [x1](·)", 300, 200);

The above line will display the tube within a Vibes window located at (300,200)px.
The :code:`endDrawing()` static method has to be called to properly end drawings.
A brief example is provided hereinafter.

.. code-block:: c++

  // code...

  VibesFigure_Tube::show(&x1, "Tube [x1](·)", 300, 200);

  // code...

  VibesFigure_Tube::endDrawing();

See more in :ref:`graphicaltools`.