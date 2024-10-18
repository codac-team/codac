:orphan:

.. _sec-installation-py:

###############################
Installing Codac for Python use
###############################

.. note::

  :ref:`An online version (i.e. without installation) is available <sec-py-project-online>`.

Install the Codac Python package
--------------------------------

In case you want to use Codac only with Python, then the installation procedure is simply:

.. code-block:: bash
  
  # You may have to upgrade pip (19.0.0 required at least)
  pip3 install --upgrade pip 
  pip3 install codac


.. role:: gbg

.. raw:: html

   <style>
      .gbg {background-color: #65B747; color: white;} 
   </style>

.. |online-py| replace:: :gbg:`✓` (Repl.it)
.. _online-py: 02-py-project-online.html

The :gbg:`✓` configurations are officially supported at the moment:

+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+----------------+
|Language       |Linux (amd64)   |Linux (arm64)   |Windows (x64)    |Windows (x86)    |macOS (arm64)   |macOS (x86_64)  |Online          |
+===============+================+================+=================+=================+================+================+================+
|Python 3.6     |:gbg:`✓`        |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |:gbg:`✓`        |:gbg:`✓`        ||online-py|_    |
+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+                +
|Python 3.7     |:gbg:`✓`        |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |:gbg:`✓`        |:gbg:`✓`        |                |
+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+                +
|Python 3.8     |:gbg:`✓`        |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |:gbg:`✓`        |:gbg:`✓`        |                |
+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+                +
|Python 3.9     |:gbg:`✓`        |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |:gbg:`✓`        |:gbg:`✓`        |                |
+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+                +
|Python 3.10    |:gbg:`✓`        |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |:gbg:`✓`        |:gbg:`✓`        |                |
+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+                +
|Python 3.11    |:gbg:`✓`        |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |:gbg:`✓`        |:gbg:`✓`        |                |
+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+----------------+
|Python 3.12    |:gbg:`✓`        |:gbg:`✓`        |:gbg:`✓`         |:gbg:`✓`         |:gbg:`✓`        |:gbg:`✓`        |                |
+---------------+----------------+----------------+-----------------+-----------------+----------------+----------------+----------------+

If a configuration in this table does not work, please `contact us <https://github.com/codac-team/codac/issues>`_.

.. warning::

  | **Debian Bookworm and possibly other configurations:**
  | ``pip install ...`` or ``pip3 install ...`` commands may only work inside `virtual environments <https://packaging.python.org/en/latest/guides/installing-using-pip-and-virtual-environments/>`_ or with ``--break-system-packages`` parameter, e.g.:

  .. code-block:: bash

    pip3 install --break-system-packages codac

.. warning::

  | **macOS Big Sur and later (x86_64):**
  | `Python wheels provided for Catalina or earlier may need to be installed with something similar to <https://stackoverflow.com/questions/64847996/force-usage-of-10-x-wheel-in-macos-big-sur>`_: 

  .. code-block:: bash

    sudo pip3 install --upgrade vibes # Add here any other non-binary wheels dependencies...
    sudo pip3 install --upgrade --nodeps --platform macosx_10_9_x86_64 --only-binary=:all: --target=/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.8/lib/python3.8/site-packages codac

  Depending on the way Python was installed, the path to specify after ``--target`` may differ, *e.g.* if Python was installed from https://www.python.org/ftp/python/3.10.4/python-3.10.4-macos11.pkg, it may be ``/Library/Frameworks/Python.framework/Versions/3.10/lib/python3.10/site-packages``. Otherwise, run ``python3 -m site`` to check the ``site-packages`` full path in ``sys.path list``. Also, the value ``10_9`` may need to be changed to ``10_14`` (or possibly another value) for some Python versions.

.. note::

  Unsupported computers can still probably follow `Installing local Python binding <../dev/info_dev.html>`_ .


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