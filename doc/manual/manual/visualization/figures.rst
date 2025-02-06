.. _sec-graphics-figures:

The Figure classes
==================

This page describes the classes used in Codac for 2D visualization.

.. _subsec-graphics-figures-graphical-outputs:

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

.. _subsec-graphics-figures-figure2d:

Figure2D
--------

The basic class for 2D visualization is Figure2D. It is used to create a figure that can be displayed in VIBes or saved in an xml file for IPE.
The constructor takes two arguments: the name of the figure and the graphical output. A boolean can be added to specify if the figure is to be used
DefaultView (see :ref:`subsec-graphics-figures-figure2d-defaultview`).

.. tabs::

  .. code-tab:: py

    fig = Figure2D("My figure", GraphicOutput.VIBES | GraphicOutput.IPE)

  .. code-tab:: c++

    Figure2D figure ("My Figure",GraphicOutput::VIBES|GraphicOutput::IPE);

.. _subsec-graphics-figures-figure2d-defaultview:

DefaultView
-----------

A DefaultView using only VIBes as graphical output is available. This figure is the one used by the function `draw_while_paving` by default. 
Any Figure2D object can be used as DefaultView with the set method:

.. tabs::

  .. code-tab:: py

    figure = Figure2D("My figure", GraphicOutput.VIBES | GraphicOutput.IPE)
    DefaultView.set(figure)

  .. code-tab:: c++

    std::shared_ptr<codac2::Figure2D> figure = std::make_shared<Figure2D>("My Figure",GraphicOutput::VIBES|GraphicOutput::IPE);
    DefaultView::set(figure);

Note that in C++ the figure must be a shared pointer in order to be passed to the `set` method.

Equivalently, a Figure2D can be used as DefaultView by setting the flag `set_as_default` to true in the constructor:

.. tabs::

  .. code-tab:: py

    fig = Figure2D("My figure", GraphicOutput.VIBES | GraphicOutput.IPE, True)

  .. code-tab:: c++

    Figure2D figure ("My Figure",GraphicOutput::VIBES|GraphicOutput::IPE,true);

.. _subsec-graphics-figures-figure2d-figure-properties:

Figure properties
-----------------

Once created, the properties of a Figure2D object can be modified using the following methods:

.. tabs::

  .. code-tab:: py

    fig.set_window_properties([50,50],[500,500]) # set the window position and size
    fig.set_axes(axis(0,[-10,10]), axis(1,[-10,10])) # set the range of values on each axis : 0 for x-axis, 1 for y-axis

  .. code-tab:: c++

    fig.set_window_properties({50,50},{500,500}); // set the window position and size
    fig.set_axes(axis(0,{-10,10}), axis(1,{-10,10})); // set the range of values on each axis : 0 for x-axis, 1 for y-axis