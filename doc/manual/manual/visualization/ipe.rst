.. _sec-graphics-ipe:

The IPE editor
==============

  Main author: `Maël Godard <https://godardma.github.io>`_

One of the graphical outputs supported in Codac is `the IPE editor <https://ipe.otfried.org/>`_: a drawing editor for creating figures in PDF or SVG format. It is a powerful tool for creating graphics for presentations, papers ...

Installation
------------

For the installation of IPE, please refer to the "Download the current Ipe version" section of the IPE website: `IPE website <https://ipe.otfried.org/>`_.

Use
---

Codac provides a class ``Figure2D_IPE`` to use IPE in a more convenient way. It is highly recommended to use it via the ``Figure2D`` class.

For more details refer to the :ref:`dedicated page <sec-graphics-2d-figures>`.

Using this graphical output will generate a ``.xml`` file that can be opened and edited with the IPE editor.

**Note : for IPE to be able to scale the figure correctly, it is important to call the function set_axes on the figure before drawing on it.**