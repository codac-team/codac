.. _sec-dev-changelog:

Changelog
=========

Pull Request from godardma (17/04/26)
-------------------------------------


Commit 52b81c8 ([cmake] warning for Doxygen version)
----------------------------------------------------

set_axes change for Figure2D
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Since this PR, a bounding box can be passed to the set_axes method to specify the ranges of the x and y axes.
The old method with ``axis(id,Interval)`` still works

Python binding build requirement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doxygen 1.16.1 or newer is now required when ``WITH_PYTHON`` is enabled.
If this version is not available, the API documentation will not be generated,
which may cause issues when building the Python binding from source.
This has no impact on the C++ build nor on downloading/installing the Python packages.


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