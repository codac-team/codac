.. _sec-install-performances:

Performance considerations
==========================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

When using this library, the performance can vary significantly depending on how you configure your build using CMake (for more information, see the related page :ref:`sec-install-cpp`). The library supports three main configuration modes: ``Release``, ``Debug``, and ``FastRelease``. Each mode has its advantages and is suited to different use cases.

.. admonition:: Configuration mode of the bindings

   Note that the Python and Matlab bindings of Codac are provided in ``Release`` mode. 
   
Build configuration modes
~~~~~~~~~~~~~~~~~~~~~~~~~

- **Debug Mode**:  
   In ``Debug`` mode, CMake includes debugging symbols and disables optimizations. This makes the code slower compared to ``Release`` mode but allows for better debugging and tracing capabilities. This mode is particularly useful during development when detailed information about the program's behavior is needed to diagnose problems.
   In practice, both C++ functions ``assert(..)`` and ``assert_release(..)`` are kept in this mode, providing comprehensive information on debugging.

   To configure your build in ``Debug`` mode, use:

   .. code-block:: bash

      cmake -DCMAKE_BUILD_TYPE=Debug ..

- **Release Mode** (default):  
   In ``Release`` mode, CMake enables optimizations designed to improve the performance of the library. This configuration disables debugging symbols and allows the compiler to apply optimizations, such as inlining and loop unrolling, which result in faster execution. It also keeps error messages related to misuse of the library. **It is the default and recommended configuration for using Codac.**
   In practice, C++ functions ``assert(..)`` are removed in this mode but ``assert_release(..)`` functions are kept in order to communicate essential error messages to the user.

   To configure your build in ``Release`` mode, use the following command:

   .. code-block:: bash

      cmake -DCMAKE_BUILD_TYPE=Release ..

- **FastRelease Mode**:  
   The ``FastRelease`` mode is designed to provide slightly better performances than the standard ``Release`` mode. This mode is ideal for users who need maximum runtime performance and do not need the basic error messages. In practice, both C++ functions ``assert(..)`` and ``assert_release(..)`` are removed in this mode.

   To configure your build in ``FastRelease`` mode, use:

   .. code-block:: bash

      cmake -DCMAKE_BUILD_TYPE=Release -DFAST_RELEASE=ON ..

   In Python or Matlab, this mode is not available when you obtain `Codac via Pypi <https://pypi.org/project/codac>`_. To use Codac in Python/Matlab and in ``FastRelease`` mode, you need to :ref:`install Codac with its python binding locally on your machine <sec-dev-info-binding>`.