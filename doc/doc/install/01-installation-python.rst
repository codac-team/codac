:orphan:

.. _sec-installation-py:

###############################
Installing Codac for Python use
###############################

.. note::

  For the moment, the Python version is only available for Linux, Windows, :ref:`or online <sec-py-project-online>`.

Install the Codac Python package
--------------------------------

In case you want to use Codac only with Python, then the installation procedure is simply:

.. code-block:: bash
  
  pip3 install --upgrade pip # you may have to upgrade pip (19.0.0 required at least)
  pip3 install "pyibex>=1.9.2" codac

where `pyIbex <http://benensta.github.io/pyIbex/>`_ is the Python wrapper of the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_ on which Codac is based. Note that Codac requires pyIbex 1.9.2 at least.

.. role:: gbg

.. raw:: html

   <style>
      .gbg {background-color: #65B747; color: white;} 
   </style>

.. |online-py| replace:: :gbg:`✓` (Repl.it)
.. _online-py: 02-py-project-online.html

The :gbg:`✓` configurations are officially supported at the moment:

+---------------+----------------+-----------------+-----------------+----------------+----------------+
|Language       |Linux 64 bits   |Windows (x86)    |Windows (x64)    |macOS           |Online          |
+===============+================+=================+=================+================+================+
|Python 3.4     |                |:gbg:`✓`         |:gbg:`✓`         |--              ||online-py|_    |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.5     |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.6     |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.7     |:gbg:`✓`        |                 |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.8     |:gbg:`✓`        |                 |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.9     |:gbg:`✓`        |                 |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+----------------+

If your configuration is missing in this table, please `contact us <https://github.com/codac-team/codac/issues>`_. We may be able to make it available for Codac.


Update your Codac Python package
--------------------------------

For updating your already installed version of Codac, the procedure is:

.. code-block:: bash

  pip3 install codac --upgrade