.. _sec-intervals-intervalvector-class:

The IntervalVector class
========================

Some specific commands in Python are provided below:

.. tabs::
  
  .. code-tab:: py

    x = IntervalVector([[1,2],[2,3],[3,4]])
    y = IntervalVector(3)

    i = 0
    for xi in x:
      y[i] = xi
      i = i+1

    # x == y

    a,b,c = x
    # a == x[0]
    # b == x[1]
    # c == x[2]

    v = IntervalVector([*x, [3,6]])
    # v == [[1,2],[2,3],[3,4],[3,6]]