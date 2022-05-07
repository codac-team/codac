.. _sec-installation:

############################
Installing the Codac library
############################

Codac is available in both C++17 and Python3. Note that you can also :ref:`use Codac online in Python <sec-py-project-online>`, without having to install the library on your machine.

.. role:: gbg

.. raw:: html

   <style>
      .gbg {background-color: #65B747; color: white;} 
   </style>

.. |linux-py| replace:: :gbg:`✓` (link)
.. _linux-py: 01-installation-python.html

.. |win-py| replace:: :gbg:`✓` (link)
.. _win-py: 01-installation-python.html

.. |macos-py| replace:: :gbg:`✓` (link)
.. _macos-py: 01-installation-python.html

.. |online-py| replace:: :gbg:`✓` (Repl.it)
.. _online-py: 02-py-project-online.html

.. |linux-cpp| replace:: :gbg:`✓` (link)
.. _linux-cpp: 01-installation-full-linux.html

.. |win-cpp| replace:: :gbg:`✓` (link)
.. _win-cpp: 01-installation-full-windows.html

.. |macos-cpp| replace:: :gbg:`✓` (link)
.. _macos-cpp: 01-installation-full-macos.html

The :gbg:`✓` configurations are officially supported at the moment:

+---------------+----------------+-----------------+-----------------+----------------+----------------+
|Language       |Linux (amd64)   |Windows (x64)    |Windows (x86)    |macOS (x86_64)  |Online          |
+===============+================+=================+=================+================+================+
|C++17          ||linux-cpp|_    ||win-cpp|_       ||win-cpp|_       ||macos-cpp|_    |                |
+---------------+----------------+-----------------+-----------------+----------------+----------------+
|Python 3.6     ||linux-py|_     ||win-py|_        ||win-py|_        ||macos-py|_     ||online-py|_    |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.7     ||linux-py|_     ||win-py|_        ||win-py|_        ||macos-py|_     |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.8     ||linux-py|_     ||win-py|_        ||win-py|_        ||macos-py|_     |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.9     ||linux-py|_     ||win-py|_        ||win-py|_        ||macos-py|_     |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.10    ||linux-py|_     ||win-py|_        ||win-py|_        ||macos-py|_     |                |
+---------------+----------------+-----------------+-----------------+----------------+----------------+

| **Click on the links in the table to access the related installation procedures.**
| If a configuration in this table does not work, please `contact us <https://github.com/codac-team/codac/issues>`_.

Note that if you want to contribute to Codac, you have to make the full C++ installation.



.. _sec-installation-graphics:

Graphical tools
^^^^^^^^^^^^^^^

For using Codac with graphical outputs, you will need the VIBes viewer.

`VIBes <http://enstabretagnerobotics.github.io/VIBES/>`_ is a visualization system that aims at providing people working with intervals a way to display results (boxes, pavings), without worrying about GUI programming.
Some methods have been implemented in Codac in order to extend VIBes' features to tubes purposes.

You can `download the VIBes viewer directly from the official page <http://enstabretagnerobotics.github.io/VIBES/>`_ (click on the *Last Release* link).

.. admonition:: (optional) Get the very last version of VIBes from the sources

  You can also install the last version from the sources available on `the GitHub development repository <https://github.com/ENSTABretagneRobotics/VIBES>`_.
  For instance, on Linux systems, a fast installation can be made using the following command lines:

  .. code-block:: bash
    
    sudo apt-get install qt5-default libqt5svg5-dev qtbase5-dev cmake git
    git clone https://github.com/ENSTABretagneRobotics/VIBES
    cd VIBES/viewer ; mkdir build ; cd build ; cmake .. ; sudo make install

.. \todo: test sudo make install and executable access

You can click on the icon to launch it, or use a terminal. For instance, on Linux systems:

.. code-block:: bash
  
  VIBes-viewer

This will open the VIBes window, ready to display things:

.. figure:: /manual/07-graphics/img/vibes_window.png

Keep it open!