Iterative :math:`\dot{x}=-\sin(x)`
----------------------------------

Let us consider the following initial value problem (IVP):

.. math::

  \begin{equation}
    \left\{\begin{array}[l]{rcl}
      \dot{x}&=&-\sin(x)\\
      x_0&=&1
    \end{array}\right.
  \end{equation}

Decomposition into primitive constraints:

.. math::

  \left\{\begin{array}[l]{rcl}
    a(\cdot)&=&\sin\big(x(\cdot)\big)\\
    b(\cdot)&=&\int_0 a(\tau)d\tau\\
    x(\cdot)&=&x_0-b(\cdot)
  \end{array}\right.

Then, an iterative resolution process apply.

To run the example, launch the Vibes viewer and, from a terminal:

.. code-block:: bash

  ./build/examples/cpp/03_integ_msinx/ex_integ_msinx

The sources are available in:

.. code-block:: none

  ./examples/cpp/03_integ_msinx/main.cpp