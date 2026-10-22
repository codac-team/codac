.. _sec-domains-tubes-reference:

Tube API overview
=================

[retirer cette page et reverser éventuellement les fonctions vers d'autres pages]

.. list-table:: Temporal domain and slices
  :header-rows: 1
  :widths: 38 62

  * - Operation
    - Description
  * - ``create_tdomain([a,b],dt,with_gates)``
    - Create a shared time partition with optional gates
  * - ``td->sample(t)`` / ``td->truncate([a,b])``
    - Refine or restrict the temporal partition
  * - ``td->nb_tslices()`` / ``td->nb_tubes()``
    - Count temporal elements / attached tubes
  * - ``td->tslice(t)`` / ``td->tslices_vector()``
    - Access temporal elements
  * - ``s.t0_tf()`` / ``s.codomain()``
    - Read a slice's temporal support and interval enclosure
  * - ``s.is_gate()`` / ``s.input_gate()`` / ``s.output_gate()``
    - Inspect gate information

.. list-table:: ``SlicedTube`` operations
  :header-rows: 1
  :widths: 41 59

  * - Operation
    - Description
  * - ``SlicedTube(td,domain)`` / ``SlicedTube(td,f)``
    - Initialize from an interval codomain or an analytic function
  * - ``SlicedTube(td,traj)``
    - Enclose a trajectory on the partition
  * - ``x.tdomain()`` / ``x.nb_slices()`` / ``x.size()``
    - Shared partition / number of elements / output dimension
  * - ``x.set(...)`` / ``x.inflate(r)``
    - Assign values / enlarge enclosures
  * - ``x(t)`` / ``x([a,b])``
    - Evaluate over a time or an interval
  * - ``x.invert(y)``
    - Find compatible time enclosures
  * - ``x.integral(...)`` / ``x.primitive(...)``
    - Interval integration / primitive tube
  * - ``x[i]`` / ``x.subvector(i,j)``
    - Extract components of vector tubes
  * - ``x.mid()``
    - Construct a numerical midpoint trajectory
  * - ``tube_cart_prod(...)``
    - Assemble tubes sharing one ``TDomain``
  * - ``f.tube_eval(...)``
    - Evaluate analytic functions on tube arguments
  * - ``x.as_function()``
    - Use a tube in an analytic expression for forward evaluation

Operations between tubes require a shared temporal-domain object, even when independently created domains contain identical partitions. The available overloads depend on the codomain type; some template operations are not exposed in Python. Direct binary serialization of ``SlicedTube`` is not currently available.

See :doc:`tdomain`, :doc:`slice`, :doc:`slicedtube` and :doc:`tube_operations` for details and examples.
