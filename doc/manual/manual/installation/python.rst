.. _sec-install-py:

Python installation
===================

  Main authors: `Fabrice Le Bars <https://www.ensta-bretagne.fr/lebars/>`_, `Simon Rohou <https://www.simon-rohou.fr/research/>`_

In case you want to use Codac only with Python, then the installation procedure is simply:

1. **Dependencies**: Ensure you have a supported version of Python (>=3.6).
2. **Install via pip**: Use the following command to install Codac:

   .. code-block:: bash
   
      pip install codac --pre
      # Option --pre has to be set because Codac v2 is only available in pre-release


You can also update and test your Codac Python package.

* **Update via pip**: Use the following command to update your already installed Codac package:
   .. code-block:: bash

      pip install codac --upgrade --pre
      # Option --pre has to be set because Codac v2 is only available in pre-release


* **Testing the installation**: In order to verify that your installation is working properly in Python, you can run:
   .. code-block:: bash

     python -m unittest discover codac.tests


Troubleshooting
---------------

Depending on your configuration, you may encounter difficulties when installing the Codac Python package. Here are a few tips to help you.

.. admonition:: Debian Bookworm and possibly other configurations

  | ``pip install ...`` or ``pip3 install ...`` commands may only work inside `virtual environments <https://packaging.python.org/en/latest/guides/installing-using-pip-and-virtual-environments/>`_ or with ``--break-system-packages`` parameter, *e.g.*:

  .. code-block:: bash

    pip3 install --break-system-packages codac

.. admonition:: macOS Big Sur and later (x86_64)

  | Python wheels provided for Catalina or earlier `may need to be installed with something similar to <https://stackoverflow.com/questions/64847996/force-usage-of-10-x-wheel-in-macos-big-sur>`_: 

  .. code-block:: bash

    sudo pip3 install --upgrade vibes # Add here any other non-binary wheels dependencies...
    sudo pip3 install --upgrade --nodeps --platform macosx_10_9_x86_64 --only-binary=:all: --target=/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.8/lib/python3.8/site-packages codac

  Depending on the way Python was installed, the path to specify after ``--target`` may differ, *e.g.* if Python was installed from https://www.python.org/ftp/python/3.10.4/python-3.10.4-macos11.pkg, it may be ``/Library/Frameworks/Python.framework/Versions/3.10/lib/python3.10/site-packages``. Otherwise, run ``python3 -m site`` to check the ``site-packages`` full path in ``sys.path`` list. Also, the value ``10_9`` may need to be changed to ``10_14`` (or possibly another value) for some Python versions.


Finally, note that unsupported configurations can still probably follow the instructions from :ref:`sec-dev-info-binding`, for building the Codac Python binding locally on your machine.