.. _sec-tools-threading:

Multi-threading
===============

In Codac, some functions can be runned in parallel for faster computation. To do so, the library relies on CPU multi-threading.

Supported functions
-------------------

An exhaustive list of the functions that can parallelized in Codac is :

- PEIBOS

  - :ref:`PEIBOS for analytic functions <sec-functions-peibos>`

  - :ref:`PEIBOS for ODE integration <sec-extensions-capd-peibos>`

How it works
------------

By default, Codac uses only one thread for its computations. 
A getter ``nb_threads`` and a setter ``set_nb_threads`` are available to get and change the number of threads.

In addition, the function ``max_threads`` provides the maximum number of threads usable on the machine.

To use a given number of threads in a function, the setter just needs to be called before running the desired function. An example is provided below

.. tabs::
  
  .. code-tab:: py

    set_nb_threads(max_threads())  # using as many threads as possible
    PEIBOS(...)


  .. code-tab:: c++

    set_nb_threads(max_threads());  // using as many threads as possible
    PEIBOS(...);

  .. code-tab:: matlab

    set_nb_threads(max_threads());  % using as many threads as possible
    PEIBOS(...);  
    