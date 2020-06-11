.. _sec-manual-implementation:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently performed on the library.
  A new stable version of Tubex will be released in the coming weeks.

********************
Tubes implementation
********************

.. contents::


.. note::

  It should be emphasized that, mainly for efficiency reasons, we do not use inheritance hierarchy between the types of ``Tube`` and ``TubeVector``. So, a ``Tube`` is not a particular ``TubeVector``, although one might legitimately expect this (this would also be a perfectly valid design). This is in contrast with Matlab, where, basically, everything is an array of matrices. This choice is compliant with the IBEX design.