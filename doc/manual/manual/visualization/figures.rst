.. _sec-graphics-2d-figures:

The 2D Figure classes
=====================

  Main author: `Maël Godard <https://godardma.github.io>`_

This page describes the classes used in Codac for 2D visualization.

.. _subsec-graphics-2d-figures-graphical-outputs:

Graphical outputs
-----------------

Two graphical outputs are currently supported in Codac: :ref:`VIBes <sec-graphics-vibes>` and :ref:`IPE <sec-graphics-ipe>`. VIBes is used for real-time 
visualization while IPE creates a file that can be edited by the IPE editor. These outputs are referenced by the enumeration GraphicOutput:

.. tabs::

  .. code-tab:: py

    GraphicOutput.VIBES # for VIBes
    GraphicOutput.IPE # for IPE
    GraphicOutput.VIBES | GraphicOutput.IPE # for both

  .. code-tab:: c++

    GraphicOutput::VIBES  // for VIBes
    GraphicOutput::IPE  // for IPE
    GraphicOutput::VIBES | GraphicOutput::IPE // for both

Note that for the VIBes output to work, the VIBes viewer must be launched before the program is run.

.. _subsec-graphics-2d-figures-figure2d:

Figure2D
--------

The basic class for 2D visualization is Figure2D. It is used to create a figure that can be displayed in VIBes or saved in an xml file for IPE.
The constructor takes two arguments: the name of the figure and the graphical output. A boolean can be added to specify if the figure is to be used
DefaultFigure (see :ref:`subsec-graphics-2d-figures-defaultfigure`).

.. tabs::

  .. code-tab:: py

    fig = Figure2D("My figure", GraphicOutput.VIBES | GraphicOutput.IPE)

  .. code-tab:: c++

    Figure2D fig ("My Figure",GraphicOutput::VIBES|GraphicOutput::IPE);

.. _subsec-graphics-2d-figures-defaultfigure:

DefaultFigure
-------------

A DefaultFigure using only VIBes as graphical output is available. This figure is the one used by the function `draw_while_paving` by default. 
Any Figure2D object can be used as DefaultFigure with the set method:

.. tabs::

  .. code-tab:: py

    fig = Figure2D("My figure", GraphicOutput.VIBES | GraphicOutput.IPE)
    fig.is_default() # is False
    DefaultFigure.set(fig)
    fig.is_default() # is True

  .. code-tab:: c++

    std::shared_ptr<codac2::Figure2D> fig = std::make_shared<Figure2D>("My Figure",GraphicOutput::VIBES|GraphicOutput::IPE);
    fig->is_default() // is false
    DefaultFigure::set(fig);
    fig->is_default() // is true

Note that in C++ the figure must be a shared pointer in order to be passed to the `set` method.

Equivalently, a Figure2D can be used as DefaultFigure by setting the flag `set_as_default` to true in the constructor:

.. tabs::

  .. code-tab:: py

    fig = Figure2D("My figure", GraphicOutput.VIBES | GraphicOutput.IPE, True)

  .. code-tab:: c++

    Figure2D fig ("My Figure",GraphicOutput::VIBES|GraphicOutput::IPE,true);


.. _subsec-graphics-2d-figures-figure-properties:

Figure properties
-----------------

Once created, the properties of a Figure2D object can be modified using the following methods:

.. tabs::

  .. code-tab:: py

    fig.set_window_properties([50,50],[500,500]) # set the window position and size
    fig.set_axes(axis(0,[-10,10]), axis(1,[-10,10])) # set the x-axis index to 0 and its range to [-10,10], same for y with index 1 

  .. code-tab:: c++

    fig.set_window_properties({50,50},{500,500}); // set the window position and size
    fig.set_axes(axis(0,{-10,10}), axis(1,{-10,10})); // set the x-axis index to 0 and its range to [-10,10], same for y with index 1 

The same methods can be applied on the DefaultFigure object.

.. tabs::

  .. code-tab:: py

    DefaultFigure.set_window_properties([50,50],[500,500]) # set the window position and size
    DefaultFigure.set_axes(axis(0,[-10,10]), axis(1,[-10,10])) # set the x-axis index to 0 and its range to [-10,10], same for y with index 1 

  .. code-tab:: c++

    DefaultFigure::set_window_properties({50,50},{500,500}); // set the window position and size
    DefaultFigure::set_axes(axis(0,{-10,10}), axis(1,{-10,10})); // set the x-axis index to 0 and its range to [-10,10], same for y with index 1 

Many properties have an associated getter :

- size() for the figure size (type is Index)
- axes() to get a vector of the figure axes (type is FigureAxis)
- i() and j() to get respectively the figure x-axis and y-axis index (type is Index)
- pos() and window_size() to get the window position and size (type is Vector)
- scaled_unit() to get the scaling ratio from window to figure dimensions (type is double)

For temporal objects (such as trajectories or tubes), it is possible to restrict the temporal frame to display by using the ``set_tdomain()`` method.

**For animation purposes** a function ``clear()`` is available to clear the figure content before drawing again. 

VIBes only
----------

Some methods are exclusive to the real-time display with VIBes :

- center_viewbox : takes two Vector as arguments, the center and radius of each axis
- auto_scale : takes no argument, the figure will be automatically scaled to fit the window