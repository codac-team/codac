.. _sec-installation-py:

###############################
Installing Tubex for Python use
###############################

.. note::

  For the moment, the Python version is only available for Linux systems, :ref:`or online <sec-py-project-online>`.

Install the Tubex Python package
--------------------------------

In case you want to use Tubex only with Python, then the installation procedure is simply:

.. code-block:: bash
  
  pip3 install --upgrade pip # you may have to upgrade pip (19.0.0 required at least)
  pip3 install "pyibex>=1.8.1" tubex-lib

where `pyIbex <http://benensta.github.io/pyIbex/>`_ is the Python wrapper of the `IBEX library <http://www.ibex-lib.org/doc/install.html>`_ on which Tubex is based. Note that Tubex requires pyIbex 1.8.1 at least.

.. role:: gbg

.. raw:: html

   <style>
      .gbg {background-color: #65B747; color: white;} 
   </style>

.. |online-py| replace:: :gbg:`✓` (Repl.it)
.. _online-py: 02-py-project-online.html

The :gbg:`✓` configurations are officially supported at the moment:

+---------------+----------------+----------------+----------------+----------------+
|Language       |Linux           |Windows         |macOS           |Online          |
+===============+================+================+================+================+
|Python 3.4     |:gbg:`✓`        |soon available ?|--              ||online-py|_    |
+---------------+----------------+----------------+----------------+                +
|Python 3.5     |:gbg:`✓`        |soon available ?|--              |                |
+---------------+----------------+----------------+----------------+                +
|Python 3.6     |:gbg:`✓`        |soon available ?|--              |                |
+---------------+----------------+----------------+----------------+                +
|Python 3.7     |:gbg:`✓`        |soon available ?|--              |                |
+---------------+----------------+----------------+----------------+                +
|Python 3.8     |:gbg:`✓`        |soon available ?|--              |                |
+---------------+----------------+----------------+----------------+----------------+

.. note::

  | (5th June 2020) We are currently working on making Tubex available on Windows in Python.
  | Any help is welcome.
  | Meanwhile, you can :ref:`use Tubex online in Python <sec-py-project-online>`.

If your configuration is missing in this table, please `contact us <https://github.com/SimonRohou/tubex-lib/issues>`_. We may be able to make it available for Tubex.


Update your Tubex Python package
--------------------------------

For updating your already installed version of Tubex, the procedure is:

.. code-block:: bash

  pip3 install tubex-lib --upgrade