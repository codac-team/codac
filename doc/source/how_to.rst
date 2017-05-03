How to handle tubes with Tubex
==============================

Instantiation
-------------

To create a Tube object initialized with a constant image :math:`[y],\forall t`:

.. code-block:: c++

  float timestep = 0.1;     // slice width
  Interval domain(0,10);    // [t0,tf]

  Tube x(domain, timestep); // (by default) [y]=[-oo,oo]
  Tube y(domain, timestep, Interval::POS_REALS); // [y]=[0,oo]
  Tube z(domain, timestep, Interval(-42.0, 17.0)); // [y]=[-42.0, 17.0]
  Tube w(x, 3.0); // same structure as x, with [y]=[3]

To create a Tube object from a analytical expression:

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

To create a Tube object from a map of values (will strictly wrap the map with linear interpolation):

.. code-block:: c++

  map<double,double> map_values;
  map_values[0.0] = 2.0;  map_values[1.0] = 3.2;
  map_values[1.2] = 3.4;  map_values[8.5] = -36.0;

  Tube x1(domain, timestep, Interval::EMPTY_SET);
  Tube x2(x1); // x2 is empty, as weel as x1

  // Strict wrapping:
  x1.feed(map_values);
  // Thickness of 2 centered on the values:
  x2.feed(map_values, Interval(-1.0,1.0));

  // Inflation of x1 (then x1==x2):
  x1.inflate(1.0);


Parameters
----------

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
  int input2index(double t) const;
  double index2input(int index) const;

Classical operations on sets are applicable on similar tubes:

.. code-block:: c++

  Tube x4 = (x1 | x2) & x3;

**Note:** by *similar* we mean tubes of same timestep and domain.


Evaluation and set-inversion
----------------------------

To evaluate a specific part of a Tube object:

.. code-block:: c++

  // Union of bounded values (tube image):
  Interval y1 = x1.image();
  Interval y2 = x1[Interval(2.0,8.5)]; // over a subdomain
  Interval y3 = x1[5.08];              // at a given input t
  Interval y4 = x1[(int)5];            // of a given slice (6th)
  // Note: y4 == y3 \in y2 \subset y1

Set-inversion:




.. code-block:: c++

  const std::pair<ibex::Interval,ibex::Interval> eval(const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;
  ibex::Interval invert(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS) const;
  void invert(const ibex::Interval& intv_y, std::vector<ibex::Interval> &v_intv_t, const ibex::Interval& intv_t ibex::Interval::ALL_REALS) const;



Updates
-------

Setting a value for its :math:`k`-th slice (second argument has to be *int*):

.. code-block:: c++

  // Setting values (for a slice)
  x1.set(Interval(3,8), 4); // k==5


Setting a slice's value from an input :math:`t` (second argument has to be *double*):

.. code-block:: c++

  // Setting values (for a given date)
  x1.set(Interval(-3,3), 4.2);

Setting values over a given subdomain :math:`[t]\subseteq[t_0,t_f]` (second argument has to be *ibex::Interval*):

.. code-block:: c++

  // Setting values (within a given interval of times)
  x1.set(Interval(1,3), Interval(6.2,6.7)); // [y],[t]



.. code-block:: c++

  ibex::Interval operator[](const ibex::Interval& intv_t) const;
  const ibex::Interval& image() const;
  void set(const ibex::Interval& intv_y, const ibex::Interval& intv_t = ibex::Interval::ALL_REALS);





Tube arithmetic
---------------

.. code-block:: c++

  Tube operator+(const ibex::Interval& x1, const Tube& x2);
  Tube operator-(const ibex::Interval& x1, const Tube& x2);
  Tube operator*(const Tube& x1, const ibex::Interval& x2);
  Tube operator/(const Tube& x1, const ibex::Interval& x2);
  Tube abs(const Tube& x);

**Note:** by *similar* we mean tubes of same timestep and domain.


Contractors
-----------

.. code-block:: c++

  bool ctcFwdBwd(const Tube& derivative_tube, const ibex::Interval& initial_value = ibex::Interval::ALL_REALS);
  bool ctcObs(const Tube& derivative_tube, ibex::Interval& t, ibex::Interval& y, bool fwd_bwd = true);
  bool ctcOut(const ibex::Interval& t, const ibex::Interval& y);
  bool ctcIntertemporal(ibex::Interval& t1, ibex::Interval& t2) const;
  bool ctcIntertemporal(ibex::Interval& y, ibex::Interval& t1, ibex::Interval& t2) const;
  bool ctcPeriodic(const ibex::Interval& period);
  static bool contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6, Tube& x7, Tube& x8, const ibex::Function& f);
  bool ctcAbs(Tube& y, Tube& x);


Integration
-----------

.. code-block:: c++

  Tube primitive(const ibex::Interval& initial_value = ibex::Interval(0.)) const;
  ibex::Interval integral(double t) const;
  ibex::Interval integral(const ibex::Interval& t) const;
  std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t) const;
  ibex::Interval integral(const ibex::Interval& t1, const ibex::Interval& t2) const;
  std::pair<ibex::Interval,ibex::Interval> partialIntegral(const ibex::Interval& t1, const ibex::Interval& t2


Serialization
-------------

In case of heavy computations, a Tube object can be serialized within a binary file and fastly re-created afterwards:

.. code-block:: c++

  Tube x1(domain, timestep);
  x1.serialize("x1.tube"); // will create a binary file "x1.tube"
  Tube x2("x1.tube"); // will create a new tube x2 from the binary file
  // then, x1==x2

Maps of values can be serialized jointly with the Tube object:

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

To print a Tube object in a terminal:

.. code-block:: c++
  
  cout << x1;
  // or, for a custom decimal precision
  x1.print(5); // new precision: 5

A simple 2D rendering of a tube is available using the `Vibes viewer <http://enstabretagnerobotics.github.io/VIBES/>`_:

.. code-block:: c++

  displayTube(map_graphics, &x1, "Tube [x1](·)", 300, 200);

The above line will display the tube within a Vibes window located at (300,200)px.
The `map_graphics` variable is a map of pointers that has to be defined once.
A complete example is provided hereinafter.

.. code-block:: c++
  
  vibes::beginDrawing(); vibes::axisAuto();
  map<Tube*,VibesFigure_Tube*> map_graphics;

  // code...

  displayTube(map_graphics, &x1, "Tube [x1](·)", 300, 200);

  // code...

  for(auto it = map_graphics.begin(); it != map_graphics.end(); ++it)
    delete it->second; // deleting pointers to graphical tools
  vibes::endDrawing();

See more in :ref:`graphicaltools`.