.. _sec-graphics-vibes:

The VIBes viewer
================

  Main author: `Simon Rohou <http://simon-rohou.fr/research>`_

One of the graphical outputs supported in Codac is `the VIBes viewer <https://enstabretagnerobotics.github.io/VIBES/>`_: a visualization system that aims at providing people working with intervals a way to display results (boxes, pavings), without worrying about GUI programming.

Installation
------------

For the installation of VIBes, please refer to the README of the github page of this library: `VIBes github page <https://github.com/ENSTABretagneRobotics/VIBES/>`_.


Launch VIBes
------------

VIBes is an independent program that has to be launched together with the executable of your project.

The first thing to do is launch VIBes. This will make appear the VIBes window, ready to display things (keep it open!):

.. figure:: img/vibes_window.png
  

| This window corresponds to the graphical **server**.
| The Python or C++ program you are developing is a **client**, able to communicate with the server from the API detailed in the following sections.

The VIBes API
-------------

The VIBes API is available for both C++ and Python:

* `C++ API <https://enstabretagnerobotics.github.io/VIBES/doxygen/cxx/>`_
* `Python API <https://enstabretagnerobotics.github.io/VIBES/sphinx/html/index.html>`_

Codac provides a class ``Figure2D_VIBes`` to use VIBes in a more convenient way. It is highly recommended to use it via the ``Figure2D`` class.

For more details refer to the :ref:`dedicated page <sec-graphics-2d-figures>`.