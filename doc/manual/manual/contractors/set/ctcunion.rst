.. _sec-ctc-set-ctcunion:
.. _sec-sep-set-sepunion:

Union: CtcUnion and SepUnion
============================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_


``CtcUnion`` and ``SepUnion`` implement the logical union/disjunction counterpart of
``CtcInter`` and ``SepInter``:

.. list-table::
  :header-rows: 1
  :widths: 20 35 45

  * - Class
    - Combines
    - Meaning
  * - ``CtcUnion``
    - contractors
    - disjunction of constraints, obtained by joining the contracted branches
  * - ``SepUnion``
    - separators
    - separator associated with the union of the represented sets

Both classes can be constructed explicitly or with the ``|`` operator.


CtcUnion: disjunction of contractors
------------------------------------

For box contractors, ``CtcUnion`` applies every stored contractor independently to the
same input box and returns a box enclosing the union of the resulting branches:

.. math::

  \mathcal{C}_{\mathrm{union}}([\mathbf{x}])
  =
  \operatorname{hull}\!\left(
    \bigcup_{i=1}^{n}\mathcal{C}_i([\mathbf{x}])
  \right),

where :math:`\operatorname{hull}` denotes the smallest box containing the union.

This evaluation differs from ``CtcInter``: the branches are **not** applied
sequentially. Each contractor receives the original input domain.

.. note::

  The exact union of several boxes is generally not a box. ``CtcUnion`` therefore
  returns a box enclosure of this union. This enclosure may contain points that do not
  belong to any individual branch.

The usual construction uses ``|`` or the ``CtcUnion`` constructor.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [union-ctc-1-beg]
      :end-before: [union-ctc-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [union-ctc-1-beg]
      :end-before: [union-ctc-1-end]
      :dedent: 2

Applying it to a box gives the hull of the contracted branches:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [union-ctc-2-beg]
      :end-before: [union-ctc-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [union-ctc-2-beg]
      :end-before: [union-ctc-2-end]
      :dedent: 2

For the two boxes used here, the exact union is not itself a box. Its interval hull is

.. math::

  [-12,10]\times[-2,4].


SepUnion: union of represented sets
-----------------------------------

``SepUnion`` is the separator counterpart. If separators
:math:`\mathcal{S}_1,\ldots,\mathcal{S}_n` are associated with sets
:math:`\mathbb{S}_1,\ldots,\mathbb{S}_n`, then ``SepUnion`` is associated with

.. math::

  \mathbb{S}_{\mathrm{union}}
  =
  \mathbb{S}_1 \cup \cdots \cup \mathbb{S}_n.

Construction follows the same syntax as for ``CtcUnion``:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [union-sep-1-beg]
      :end-before: [union-sep-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [union-sep-1-beg]
      :end-before: [union-sep-1-end]
      :dedent: 2


Building a union incrementally
------------------------------

Both classes support ``|=`` and ``nb()``.

For ``CtcUnion``, a union may be created from the dimension only. Before a branch is
added, it represents the empty disjunction: contracting a box with it returns the empty
domain. Adding contractors with ``|=`` then builds the disjunction incrementally.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [union-ctc-3-beg]
      :end-before: [union-ctc-3-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [union-ctc-3-beg]
      :end-before: [union-ctc-3-end]
      :dedent: 2

``SepUnion`` currently has no constructor taking only a dimension, so incremental
construction starts from an existing separator:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [union-sep-2-beg]
      :end-before: [union-sep-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [union-sep-2-beg]
      :end-before: [union-sep-2-end]
      :dedent: 2


.. admonition:: Technical documentation

  See the `C++ API documentation of CtcUnion <../../api/html/classcodac2_1_1_ctc_union.html>`_
  and the `C++ API documentation of SepUnion <../../api/html/classcodac2_1_1_sep_union.html>`_.
