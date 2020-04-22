.. _sec-manual-variables-label:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

######################
Variables: real values
######################

Tubex provides simple objects to handle real values such as vectors :math:`\mathbf{x}`, matrices :math:`\mathbf{X}` and trajectories :math:`x(\cdot)`. They can for instance represent variables we are looking for in our constraint networks, as well as measurements or parameters.

Note that they are subject to uncertainties related to the approximation of real numbers by floating-point values. In the case of trajectories, uncertainties may also be related to time discretization.

These objects are not involved during the process of constraint propagation, that only relies on domains presented in the next part.

.. toctree::

  01-var-static
  02-var-dynamic