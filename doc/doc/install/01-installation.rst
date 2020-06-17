.. _sec-installation:

############################
Installing the Tubex library
############################

Tubex is available in both C++ and Python3.

It is fully supported on Linux systems (both languages). The current version of Tubex will be soon available for Windows and macOS.
Any help is more than welcome.

Note that you can also :ref:`use Tubex online in Python <sec-py-project-online>`, without having to install the library on your machine.

.. role:: gbg

.. raw:: html

   <style>
      .gbg {background-color: #65B747; color: white;} 
   </style>

.. |linux-py| replace:: :gbg:`✓` (link)
.. _linux-py: 01-installation-python.html

.. |win-py35| replace:: :gbg:`✓`   (2020/06/17)
.. _win-py35: 01-installation-python.html

.. |linux-cpp| replace:: :gbg:`✓` (link)
.. _linux-cpp: 01-installation-full-linux.html

.. |online-py| replace:: :gbg:`✓` (Repl.it)
.. _online-py: 02-py-project-online.html

.. |win-cpp| replace:: in progress (#21)
.. _win-cpp: https://github.com/SimonRohou/tubex-lib/issues/21

The :gbg:`✓` configurations are officially supported at the moment:

+---------------+----------------+-----------------------+----------------+----------------+
|Language       |Linux           |Windows                |macOS           |Online          |
+===============+================+=======================+================+================+
|C++11          ||linux-cpp|_    ||win-cpp|_             |--              |soon available ?|
+---------------+----------------+-----------------------+----------------+----------------+
|Python 3.4     ||linux-py|_     |soon available ?       |--              ||online-py|_    |
+---------------+----------------+-----------------------+----------------+                +
|Python 3.5     ||linux-py|_     ||win-py35|_            |--              |                |
+---------------+----------------+-----------------------+----------------+                +
|Python 3.6     ||linux-py|_     |soon available ?       |--              |                |
+---------------+----------------+-----------------------+----------------+                +
|Python 3.7     ||linux-py|_     |soon available ?       |--              |                |
+---------------+----------------+-----------------------+----------------+                +
|Python 3.8     ||linux-py|_     |soon available ?       |--              |                |
+---------------+----------------+-----------------------+----------------+----------------+

**Click on the links in the table to access the related installation procedures.**

If your configuration is missing in this table, please `contact us <https://github.com/SimonRohou/tubex-lib/issues>`_. We may be able to make it available for Tubex.

Note that if you want to contribute to Tubex, you have to make the full C++ installation.



.. _sec-installation-graphics:

Graphical tools
^^^^^^^^^^^^^^^

For using Tubex with graphical outputs, you will need the VIBes viewer.

`VIBes <http://enstabretagnerobotics.github.io/VIBES/>`_ is a visualization system that aims at providing people working with intervals a way to display results (boxes, pavings), without worrying about GUI programming.
Some methods have been implemented in Tubex in order to extend VIBes' features to tubes purposes.

You can `download the VIBes viewer directly from the official page <http://enstabretagnerobotics.github.io/VIBES/>`_ (click on the *Last Release* link).

.. admonition:: (optional) Get the very last version of VIBes from the sources

  You can also install the last version from the sources available on `the GitHub development repository <https://github.com/ENSTABretagneRobotics/VIBES>`_.
  For instance, on Linux systems, a fast installation can be made using the following command lines:

  .. code-block:: bash
    
    sudo apt-get install qt5-default libqt5svg5-dev cmake git
    git clone https://github.com/ENSTABretagneRobotics/VIBES
    cd VIBES/viewer ; mkdir build ; cd build ; cmake .. ; sudo make install

.. \todo: test sudo make install and executable access

You can click on the icon to launch it, or use a terminal. For instance, on Linux systems:

.. code-block:: bash
  
  VIBes-viewer

This will open the VIBes window, ready to display things:

.. figure:: /manual/07-graphics/img/vibes_window.png

Keep it open!