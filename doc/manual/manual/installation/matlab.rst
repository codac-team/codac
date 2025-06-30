.. _sec-install-matlab:

MATLAB installation
===================

  Main authors: `Fabrice Le Bars <https://www.ensta-bretagne.fr/lebars/>`_, `Simon Rohou <https://www.simon-rohou.fr/research/>`_

Since 2019, MATLAB allows you to import Python packages into its environment. Codac offers a Python package dedicated to MATLAB, with the necessary adaptations to run in this environment (for example: indices starting at ``1``, alternatives to unsupported ``&`` and ``|`` operations, *etc.*). This Python package dedicated to MATLAB is called ``codac4matlab``. See `the related PyPI page <https://pypi.org/project/codac4matlab>`_.

1. **Dependencies**: Ensure you have MATLAB R2019b or later.
2. **Python integration**: Follow the instructions `here <https://www.mathworks.com/help/matlab/matlab_external/install-supported-python-implementation.html>`_ to set up Python integration.
3. **Install the Python package**:

   .. code-block:: bash
   
      pip install codac4matlab --pre
      # Option --pre has to be set because Codac v2 is only available in pre-release

Once Codac is installed, you can use it in your MATLAB scripts using the following import command:

.. tabs::
  
  .. code-tab:: matlab

   import py.codac4matlab.*

   % Your code...

.. py.importlib.import_module('codac')