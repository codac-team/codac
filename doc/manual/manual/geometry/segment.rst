.. _sec-geom-segment:

Segment
=======

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

The following class provides a reliable representation of a segment.

.. doxygenclass:: codac2::Segment
  :project: codac
  :members:

.. doxygenfunction:: codac2::operator&(const Segment&,const Segment&)
  :project: codac

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [segment-1-beg]
      :end-before: [segment-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [segment-1-beg]
      :end-before: [segment-1-end]
      :dedent: 4

.. doxygenfunction:: codac2::proj_intersection(const Segment&,const Segment&)
  :project: codac

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [segment-2-beg]
      :end-before: [segment-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [segment-2-beg]
      :end-before: [segment-2-end]
      :dedent: 4

.. doxygenfunction:: codac2::colinear(const Segment&,const Segment&)
  :project: codac

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [segment-3-beg]
      :end-before: [segment-3-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [segment-3-beg]
      :end-before: [segment-3-end]
      :dedent: 4