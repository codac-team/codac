.. _sec-tuto-static-rangeonly-loc:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

Static range-only localization
==============================


Suppose that we want to estimate the distance :math:`r` between a vector :math:`\mathbf{x}=(0,0)^\intercal` and several vectors :math:`\mathbf{b}^1`, :math:`\mathbf{b}^2`, :math:`\mathbf{b}^3`.

We define domains (intervals and boxes):

* :math:`[r]=[7,8]`
* :math:`[\mathbf{x}]=[0,0]^2`
* :math:`[\mathbf{b}^1]=[1.5,2.5]\times[4,11]`
* :math:`[\mathbf{b}^2]=[3,4]\times[4,6.5]`
* :math:`[\mathbf{b}^3]=[5,7]\times[5.5,8]`

.. tabs::

  .. code-tab:: c++

    Interval r(7.,8.);
    IntervalVector x(2,0.);
    IntervalVector b1{{1.5,2.5},{4,11}};
    IntervalVector b2{{3,4},{4,6.5}};
    IntervalVector b3{{5,7},{5.5,8}};

  .. code-tab:: py

    # todo

Several calls to :math:`\mathcal{C}_{\textrm{dist}}` will allow the contraction of both :math:`[\mathbf{b}^i]` and :math:`[r]` in order to reach a consistency state:

.. tabs::

  .. code-tab:: c++

    CtcDist ctc_dist;

    ctc_dist.contract(x, b1, r);
    ctc_dist.contract(x, b2, r);
    ctc_dist.contract(x, b3, r);

    // note that we could also use directly the ctc::dist already created

  .. code-tab:: py

    # todo
