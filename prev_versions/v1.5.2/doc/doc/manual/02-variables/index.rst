.. _sec-manual-variables:

################################
Basic structures for real values
################################

Codac provides basic structures to handle real values such as vectors :math:`\mathbf{x}`, matrices :math:`\mathbf{X}` and trajectories :math:`x(\cdot)`. They can for instance represent variables we are looking for in our Constraint Networks, as well as measurements or parameters. The may also represent the bounds of our domains.

Note that they are subject to uncertainties related to the approximation of real numbers by floating-point values. In the case of trajectories, uncertainties may also be related to time discretization.

These objects are **not involved during the process of constraint propagation**, that only relies on :ref:`domains presented in the next part <sec-manual-domains>`.

.. toctree::

  01-var-static
  02-var-dynamic