.. _sec-dev-changelog:

Changelog
=========

Upcoming version
****************

Nothing reported yet since :ref:`version 2.1.1 <sec-dev-changelog-2-1-1>`.

.. _sec-dev-changelog-2-1-1:

Version 2.1.1
*************

Commit 4c9af64 ([qinter] Invert q-relaxed intersection convention)
-------------------------------------------------------------------

Meaning of ``q`` in ``CtcQInter`` and ``SepQInter``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**This is a breaking change: existing code keeps compiling, but computes something else.**

The q-relaxed intersection of :math:`n` sets is now parametrized by the number of
constraints that are allowed to be **violated**, where ``q`` used to be the number
of constraints that had to be **satisfied**. With :math:`n` sets, a former ``q``
becomes :math:`n-q`.

.. tabs::

  .. code-tab:: py

    # Three separators, at most one of which may be wrong.

    # Old way: q was the number of sets an element had to belong to
    # sep = SepQInter(2, s1,s2,s3)

    # New way: q is the number of sets it may be outside of
    sep = SepQInter(1, s1,s2,s3)

  .. code-tab:: c++

    // Three separators, at most one of which may be wrong.

    // Old way: q was the number of sets an element had to belong to
    // SepQInter sep(2, s1,s2,s3);

    // New way: q is the number of sets it may be outside of
    SepQInter sep(1, s1,s2,s3);

The same applies to ``CtcQInter`` and to the ``qinter()`` function.

Commit ff4fe62 ([ctc] added CtcWrapper(Y&& y) constructor)
-----------------------------------------------------------

``CtcWrapper`` can now be built from a temporary domain, which avoids naming an
intermediate variable when the wrapped value is built on the spot.

Commit 21c9a3c ([graphics] corrected bug with random colors)
--------------------------------------------------------------

Bugfix in the generation of random colors.

Commit b134a18 ([ctc] minor improvment in CtcUnion)
------------------------------------------------------

Internal improvement of ``CtcUnion``, with no change to its interface.


Version 2.1.0
*************

Commit b688eee ([ctc/sep] revised constructors of Ctc/Sep Inter/Union/QInter)
-------------------------------------------------------------------------------

Building an intersection or a union from several operators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``CtcInter``, ``CtcUnion``, ``CtcQInter``, ``SepInter``, ``SepUnion`` and
``SepQInter`` now accept a variadic list of operands as well as an initializer
list, instead of requiring a collection built beforehand:

.. tabs::

  .. code-tab:: py

    sep = SepInter(s1, s2, s3)      # variadic
    sep = SepInter([s1, s2, s3])    # or from a list

  .. code-tab:: c++

    SepInter sep(s1, s2, s3);       // variadic
    SepInter sep({s1, s2, s3});     // or from an initializer list

The former constructors still work, so this addition is backward compatible.

Commit ec5e3d2 ([graphics] save function for Figure2D, closes #377)
----------------------------------------------------------------------

``Figure2D`` gained a ``save(filename)`` method, which writes the current state
of the figure to a file. Together with ``clear()``, it is what lets an animation
export one file per frame; see the ``00_graphics/graphic_animation`` example.

Commit 91a656c ([graphics] solving IPE figure not clearing, closes #383)
---------------------------------------------------------------------------

Bugfix: ``clear()`` had no effect on the IPE output of a figure.

Commit 3212d3a ([graphics] dot detection for IPE file save)
--------------------------------------------------------------

The name given to an IPE output is now handled correctly when it already carries
an extension.

Commits 055ee10, e62d2f3 ([matlab] graphical example and bindings)
--------------------------------------------------------------------

The drawing functions are now available from MATLAB, with a dedicated graphical
example (``00_graphics/graphic_examples.m``).


Version 2.0.5
*************

Commit dacd8e7 ([op] corrected bug in several definition domains)
--------------------------------------------------------------------

Bugfix in the definition domains used for the backward evaluation of several
operators: ``abs``, ``acos``, ``asin``, ``atan2``, ``log``, ``max``, ``sign``,
``sqrt``, ``tan`` and the division. Contractions involving these operators could
remove values that were in fact feasible, so results obtained with an earlier
version and relying on them are worth recomputing.


Version 2.0.4
*************

Commit 9754d0f ([sep] SepInter and SepUnion can be built from a list of separators)
-------------------------------------------------------------------------------------

First step of what commit b688eee generalized in version 2.1.0.


Version 2.0.3
*************

Pull Request #379 from godardma (27/04)
---------------------------------------

``Zonotope`` and ``Parallelepiped`` center is now ``c`` instead of ``z`` to avoid ambiguity.
The same commit added ``operator+`` on ``Zonotope``.

Commits 1860083, 8c7b17e ([ctc] CtcParallelepiped is now CtcWrapper<Parallelepiped>)
--------------------------------------------------------------------------------------

``CtcParallelepiped`` was introduced and then immediately replaced by the generic
``CtcWrapper<Parallelepiped>``; use the latter. The same commit added the handling
of empty ``Zonotope`` objects. See :ref:`sec-ctc-shape-ctcwrapper`.

Commit 82fd8a3 ([sep] added SepPolarCart + binding)
-----------------------------------------------------

New separator ``SepPolarCart``, the counterpart of ``SepCartPolar``, available in
C++ and in Python.

Commit 7b6ba3a ([intv] added < and > operators)
-------------------------------------------------

``Interval`` gained the comparison operators ``<`` and ``>``. They return a
``BoolInterval`` enclosing the truth value of :math:`t<s` (resp. :math:`t>s`) for
every :math:`t` of the first interval and every :math:`s` of the second:
``TRUE`` or ``FALSE`` when the two intervals are separated, ``UNKNOWN`` when they
overlap, and ``EMPTY`` when either of them is empty. See
:ref:`sec-intervals-class` and :ref:`sec-intervals-boolinterval-class`.

Commits 710a154, fa00923 (Bugfix for IntvFullPivLU::solve)
-------------------------------------------------------------

``IntvFullPivLU::solve`` gave a wrong result when called with rectangular
matrices (more rows than columns). See :ref:`sec-linear-lu`.

Commit f815210 ([graphics] possibility to choose origin for axes)
--------------------------------------------------------------------

``Figure3D::draw_axes`` takes a second argument, the origin of the axes, which
defaults to the previous behaviour:

.. tabs::

  .. code-tab:: py

    fig.draw_axes(1.0, [1,2,3])   # size, then origin

  .. code-tab:: c++

    fig.draw_axes(1.0, {1,2,3});  // size, then origin

Commit 312cd79 ([doc] doc for multi-thread)
----------------------------------------------

Documentation of the threading facilities: see :ref:`sec-tools-threading`.


Version 2.0.2
*************

Commit ca1f6f4
--------------

set_axes change for Figure2D
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Since this PR, a bounding box can be passed to the set_axes method to specify the ranges of the x and y axes.
The old method with ``axis(id,Interval)`` still works

Version 2.0.0
*************

Commit 52b81c8 ([cmake] warning for Doxygen version)
----------------------------------------------------

Python binding build requirement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doxygen 1.16.1 or newer is now required when ``WITH_PYTHON`` is enabled.
If this version is not available, the API documentation will not be generated,
which may cause issues when building the Python binding from source.
This has no impact on the C++ build nor on downloading/installing the Python packages.

Pre-Release
***********

Pull Request from godardma (15/10)
----------------------------------

Parallelepiped drawing API
~~~~~~~~~~~~~~~~~~~~~~~~~~

Due to the addition of the Parallelepiped object the signature of the following functions has changed :

- DefaultFigure::draw_parallelepiped
- Figure2D::draw_parallelepiped
- Figure3D::draw_parallelepiped

They used to take a Vector and a Matrix as arguments, now they take a Parallelepiped object.

.. tabs::

  .. code-tab:: py

    # Old way
    # fig_2d.draw_parallelepiped([1.5,2.8], Matrix([[0.5,0.4],[0,0.2]]))
    # fig_3d.draw_parallelepiped([1,-1.5,1.5], Matrix([[1,0,0],[0,0.5,0],[0,0.2,0.1]]))
    
    # New way
    fig_2d.draw_parallelepiped(Parallelepiped([1.5,2.8], Matrix([[0.5,0.4],[0,0.2]])))
    fig_3d.draw_parallelepiped(Parallelepiped([1,-1.5,1.5], Matrix([[1,0,0],[0,0.5,0],[0,0.2,0.1]])))

  .. code-tab:: c++

    // Old way
    /*
    fig_2d.draw_parallelepiped({1.5,2.8},Matrix({{0.5,0.4},{0,0.2}}));
    fig_3d.draw_parallelepiped({1,-1.5,1.5},
                {1,0,0},{0,0.5,0},{0,0.2,0.1}});
    */
    
    // New way
    fig_2d.draw_parallelepiped({{1.5,2.8},Matrix({{0.5,0.4},{0,0.2}})});
    fig_3d.draw_parallelepiped({{1,-1.5,1.5},
                {{1,0,0},{0,0.5,0},{0,0.2,0.1}}});

Zonotope drawing API
~~~~~~~~~~~~~~~~~~~~

Due to the addition of the Zonotope object the signature of the following functions has changed :

- DefaultFigure::draw_zonotope
- Figure2D::draw_zonotope
- Figure3D::draw_zonotope

They used to take a Vector and a Matrix as arguments, now they take a Zonotope object.

.. tabs::

  .. code-tab:: py

    # Old way
    # fig_2d.draw_zonotope([4,1.5],[[-0.2,0.1],[-0.06,0.04],[0.2,0.04],
    #                               [0.06,-0.04],[0.01,-0.03],[0.08,0.18],[0,0]])
    # fig_3d.draw_zonotope([1.5,1.5,1.5],
    #                            [[0.3,0.2,0.4],[-0.2,0.1,0.3],[-0.2,-0.1,0.0],
    #                             [0.3,0.0,-0.1],[-0.1,0.05,0.2],[0.0,0.2,0.1]])
    
    # New way
    fig_2d.draw_zonotope(Zonotope([4,1.5],Matrix([[-0.2,-0.06,0.2,0.06,0.01,0.08,0],
                                            [0.1,0.04,0.04,-0.04,-0.03,0.18,0]])))
    fig_3d.draw_zonotope(Zonotope([-1.5,-1.5,-1.5],
        Matrix([[0.3,-0.2,-0.2,0.3,-0.2,-0.1,0.0,0.0],
                [0.2,0.1,-0.1,0.0,0.0,0.05,0.2,0.0],
                [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.1]])))

  .. code-tab:: c++

    // Old way
    /* 
    fig_2d.draw_zonotope({4,1.5},
		{{-0.2,0.1},{-0.06,0.04},{0.2,0.04},
                {0.06,-0.04},{0.01,-0.03},{0.08,0.18},{0,0}}); 
    fig_3d.draw_zonotope({1.5,1.5,1.5},
	       {{0.3,0.2,0.4},{-0.2,0.1,0.3},
                {-0.2,-0.1,0.0},{0.3,0.0,-0.1},
                {-0.1,0.05,0.2},{0.0,0.2,0.1}});
    */
    
    // New way
    fig_2d.draw_zonotope({{4,1.5},
		{{-0.2,-0.06,0.2,0.06,0.01,0.08,0},
                {0.1,0.04,0.04,-0.04,-0.03,0.18,0}}});
    fig_3d.draw_zonotope({{1.5,1.5,1.5},
	      {{0.3,-0.2,-0.2,0.3,-0.1,0.0},
                {0.2,0.1,-0.1,0.0,0.05,0.2},
                {0.4,0.3,0.0,-0.1,0.2,0.1}}});