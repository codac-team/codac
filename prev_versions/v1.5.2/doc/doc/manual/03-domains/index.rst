.. _sec-manual-domains:

################################
Domains: sets of feasible values
################################

Constraint Programming consists in reducing sets of feasible solutions to are not compliant with constraints defining a problem. The constraints are dealt with by operators called **contractors**, presented in :ref:`the next part of this manual <sec-manual-contractors>`. The sets are called **domains**, introduced now.

For now, we consider two kinds of domains corresponding to:

* **static variables** that do not evolve with time: reals, vectors, matrices;
* **dynamic variables** depicting a temporal evolution: trajectories.

The related intervals, boxes and tubes are presented in the following pages:

.. toctree::

  01-dom-intervals
  02-dom-tubes
  
See also: :ref:`codac-unsupported <sec-codac-unsupported>`.