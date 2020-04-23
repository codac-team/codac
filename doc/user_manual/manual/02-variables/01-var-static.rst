.. _sec-manual-varstatic:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

************************************
Static variables (vectors, matrices)
************************************

Basic structures such as vectors :math:`\mathbf{x}` or matrices :math:`\mathbf{X}` are proposed in order to handle constants, measurements or parameters.

They do not represent temporal evolutions contrary to trajectories, presented :ref:`in the next section <sec-manual-vardyn>`.
Note also that they are not involved during the process of constraint propagation, that only relies on domains presented in the next part.

.. contents::

.. note::

  .. Figure:: ../../img/ibex_logo.jpg
    :align: right
  .. Figure:: ../../img/pyibex_logo.jpg
    :align: right
  
  These static variables come from the `IBEX library <http://www.ibex-lib.org>`_. They are briefly presented here for the sake of consistency. For more information, please refer to the `IBEX documentation <http://www.ibex-lib.org/doc/interval.html#intervals-vectors-and-matrices>`_ for C++ use or to the `pyIbex manual <http://benensta.github.io/pyIbex/sphinx/quickstart.html>`_ for Python usage.
