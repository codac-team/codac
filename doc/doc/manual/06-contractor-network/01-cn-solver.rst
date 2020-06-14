.. _sec-manual-solver:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently performed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*****************
Building a solver
*****************

.. contents::

**More content coming soon..**

Simple example:

.. tabs::

  .. code-tab:: py

    ctc_plus = CtcFunction(Function("a", "b", "c", "a+b-c"))
    a = Interval(0,1)
    b = Interval(-1,1)
    c = Interval(1.5,2)

    cn = ContractorNetwork()
    cn.add(ctc_plus, [a, b, c])
    cn.contract()

    # a == [0.5,1]
    # b == [0.5,1]
    # c == [1.5,2]

  .. code-tab:: c++

    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c"));
    Interval a(0,1), b(-1,1), c(1.5,2);

    ContractorNetwork cn;
    cn.add(ctc_plus, {a, b, c}); 
    cn.contract();

    // a == [0.5,1]
    // b == [0.5,1]
    // c == [1.5,2]