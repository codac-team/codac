.. _sec-linear-inversion:

Reliable inversions of matrices
===============================

  Main author: `Damien Massé <https://lab-sticc.univ-brest.fr/~dmasse>`_

.. doxygenfunction:: codac2::inverse_enclosure(const Eigen::MatrixBase<OtherDerived>&)
  :project: codac

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [linear-1-beg]
      :end-before: [linear-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [linear-1-beg]
      :end-before: [linear-1-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [linear-1-beg]
      :end-before: [linear-1-end]
      :dedent: 0


.. doxygenfunction:: codac2::inverse_correction
  :project: codac

.. doxygenfunction:: codac2::infinite_sum_enclosure
  :project: codac