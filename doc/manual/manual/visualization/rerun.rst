.. _sec-graphics-rerun:

The Rerun visualizer
====================

  Main author: `Quentin Brateau <https://teusner.github.io>`_

One of the graphical outputs supported in Codac is `the Rerun visualizer <https://rerun.io/>`_: an interactive, multimodal 2D and 3D visualization engine designed for robotics and spatial computing.

Installation
------------

To install the standalone Rerun viewer application, you can install the Rerun CLI using ``pip`` or ``cargo``:

.. code-block:: bash

  # via pip
  pip install rerun-sdk

  # or via cargo (Rust)
  cargo install rerun-cli

Use
---

Rerun can be used with both ``Figure2D`` and ``Figure3D`` via the ``GraphicOutput::RERUN`` flag (in C++) or ``GraphicOutput.RERUN`` (in Python).

.. tabs::

  .. code-tab:: py

    from codac import *

    # 2D figure with Rerun
    fig2d = Figure2D("My 2D Figure", GraphicOutput.RERUN)
    fig2d.draw_box(IntervalVector([[-2, 2], [-2, 2]]), StyleProperties(Color.blue(0.5), "boxes"))

    # 3D figure with Rerun and OBJ
    fig3d = Figure3D("My 3D Figure", GraphicOutput.OBJ | GraphicOutput.RERUN)
    fig3d.draw_sphere([0, 0, 0], Matrix.eye(3, 3), StyleProperties(Color.yellow(0.6), "sphere"))

  .. code-tab:: c++

    #include <codac>

    using namespace codac2;

    // 2D figure with Rerun
    Figure2D fig2d("My 2D Figure", GraphicOutput::RERUN);
    fig2d.draw_box({{-2, 2}, {-2, 2}}, {Color::blue(0.5), "boxes"});

    // 3D figure with Rerun and OBJ
    Figure3D fig3d("My 3D Figure", GraphicOutput::OBJ | GraphicOutput::RERUN);
    fig3d.draw_sphere({0, 0, 0}, Matrix::eye(3, 3), {Color::yellow(0.6), "sphere"});

Layers and Organization
-----------------------

All shapes and pavings drawn on a Rerun figure can be assigned to a named **layer** via ``StyleProperties``:

.. tabs::

  .. code-tab:: py

    fig.draw_box(box, StyleProperties(Color.green(0.5), "my_layer"))

  .. code-tab:: c++

    fig.draw_box(box, {Color::green(0.5), "my_layer"});

In the Rerun viewer, entities are organized under their layer hierarchy (``<figure_name>/<layer>/...``). Each layer can be independently toggled, recolored, or adjusted in opacity.

For 3D pavings, boxes are batched per layer into efficient 3D box primitives, and 3D meshes (triangles, polygons, surfaces, parallelepipeds) include computed vertex normals and albedo lighting for realistic shading and depth perception.

Streaming and Saving
--------------------

When a figure is created with Rerun output enabled, all drawing operations stream directly into a ``.rrd`` (Rerun Recording Data) file (by default ``<figure_name>.rrd``).

You can also explicitly save the recording to a specific file:

.. tabs::

  .. code-tab:: py

    fig.save("my_visualization.rrd")

  .. code-tab:: c++

    fig.save("my_visualization.rrd");

Visualizing the Results
-----------------------

To visualize the generated ``.rrd`` file in the Rerun viewer:

.. code-block:: bash

  rerun my_visualization.rrd

Live Spawning
-------------

You can also automatically spawn the Rerun viewer application in real time during the execution of your program by setting the environment variable ``CODAC_RERUN_SPAWN=1``:

.. code-block:: bash

  CODAC_RERUN_SPAWN=1 ./my_program
