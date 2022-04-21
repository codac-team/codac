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
  pip3 install codac


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
|Python 3.6     |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |--              ||online-py|_    |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.7     |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.8     |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.9     |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+                +
|Python 3.10    |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |--              |                |
+---------------+----------------+-----------------+-----------------+----------------+----------------+

If your configuration is missing in this table, please `contact us <https://github.com/codac-team/codac/issues>`_. We may be able to make it available for Codac.


Update your Codac Python package
--------------------------------

For updating your already installed version of Codac, the procedure is:

.. code-block:: bash

  pip3 install codac --upgrade


Test your Codac Python package
------------------------------

In order to verify that your installation is working properly in Python, you can run:

.. code-block:: bash

  python -m unittest discover codac.tests