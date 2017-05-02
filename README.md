# tubex-lib [![Build Status](https://travis-ci.org/SimonRohou/tube-lib.svg)](https://travis-ci.org/SimonRohou/tube-lib)

*Tubex* is a C++ library providing tools to guarantee computations over sets of trajectories. 

In the field of *constraint programming*, problems are defined thanks to a constraint network involving variables submitted to constraints and known to belong to given domains. When dealing with dynamical systems, differential constraints have to be considered.
This library is a proposal to solve such problem. A *tube* is a domain for a trajectory variable (*e.g.* a signal evolving with time) for which several numerical, algebraic, non-linear, differential constraints will apply.

Functions called *contractors* are available to reduce these sets of trajectories given several constraints.

Computations stands on interval analysis and are, therefore, performed in a guaranteed way.
Tubex stands on the [IBEX library](http://www.ibex-lib.org), itself relying on reliable numerical libraries such as [Filib](http://www2.math.uni-wuppertal.de/~xsc/software/filib.html).

Note: a Python version ([pyIbex](http://www.ensta-bretagne.fr/desrochers/pyibex) wrapping) is planned.


![Image of Tube](http://rawgit.com/SimonRohou/tubex-lib/master/doc/img/tube_slices.png)


OLD
===

A proposal for an optimized Tube class compatible with the [Ibex library](http://www.ibex-lib.org/).

This data structure is based on a binary tree thus improving the reading. Some functions are multithreaded.

Definition
-------

A tube is classically defined in the bounded-error
community as an envelope enclosing an uncertain
signal or trajectory. Hence, we define a tube as an interval of trajectories.
Its implementation is classically made of a list of intervals or boxes.

Example
-------

We choose to work on the following tube:

![Image of Tube](http://rawgit.com/SimonRohou/tube-lib/master/doc/img/tube1_01.png)

We instanciate a tube over a temporal domain defined by the interval [0,46]. Its timestep (precision) is set to 1.0.
```cpp
// Instanciation
Tube tube(Interval(0.,46.), 1.);
```

We can set values by working with slices id (second argument has to be `int`):
```cpp
// Setting values (for a slice)
tube.set(Interval(3,8), 0);
tube.set(Interval(2,7), 1);
```

..or by working with times (second argument has to be `double`):
```cpp
// Setting values (for a given date)
tube.set(Interval(-4,4), 3.2);
tube.set(Interval(-9,-5), 5.9);
```

..or by working with intervals of times (second argument has to be of type `ibex::Interval`):
```cpp
// Setting values (within a given interval of times)
tube.set(Interval(1,3), Interval(36.2,45.7));
```

In the same way, values can be accessed by index, by time or by interval:

![Image of Tube](http://rawgit.com/SimonRohou/tube-lib/master/doc/img/tube1_02.png)

```cpp
// Reading values
cout << tube.image() << endl;                 // will display [-11, 13] (global evaluation)
cout << tube[7] << endl;                      // will display [-11, -7] (8th slice)
cout << tube[3.5] << endl;                    // will display [-4, 4] (t=3.5)
cout << tube[17.0] << endl;                   // will display [2,6]
cout << tube[16.9999999] << endl;             // will display [0, 6]
cout << tube[17.0000001] << endl;             // will display [2, 7]
cout << tube[Interval(9.5, 17.5)] << endl;    // will display [-9, 7] (for dates between 9.5 and 17.5)
cout << tube[Interval(19.2, 35.7)] << endl;   // will display [1, 13]
```