Simple arithmetic
-----------------

The following example is taken from `this paper <http://simon-rohou.fr/research/tubint/tubint_paper.pdf>`_.

:numref:`arithmeticexampletubex` and :numref:`arithmeticexampletubey`
present two scalar tubes :math:`[x](\cdot)` and :math:`[y](\cdot)`. The tube
arithmetic makes it possible to compute the following tubes, as depicted
by :numref:`arithmeticexampletubea`, :numref:`arithmeticexampletubeb` and :numref:`arithmeticexampletubec`:

.. math::

  \begin{equation}
    \begin{array}{rcl}
    [a](\cdot) & = & [x](\cdot)+[y](\cdot)\\{}
    [b](\cdot) & = & \sin\left([x](\cdot)\right)\\{}
    [c](\cdot) & = & \int_{0}[x]\left(\tau\right)d\tau
    \end{array}
    \label{eq:example:msinx}
  \end{equation}


.. _arithmeticexampletubex:
.. figure:: ../../img/examples/01_arithmetic/example_tube_x.png
  :width: 50%
  :align: left

  Tube :math:`[x](t)=t^{2}+[e]` where :math:`[e]\in\mathbb{IR}` is an arbitrary
  interval corresponding to tube's constant thickness.


.. _arithmeticexampletubey:
.. figure:: ../../img/examples/01_arithmetic/example_tube_y.png
  :width: 50%

  Tube :math:`[y](t)=-cos(t)+h(t)\cdot[e]` where :math:`h:\mathbb{R}\rightarrow\mathbb{R}`
  is an arbitrary function depicting a thickness changeover.


.. _arithmeticexampletubea:
.. figure:: ../../img/examples/01_arithmetic/example_tube_a.png
  :width: 50%

  Tube :math:`[a](t)=[x](t)+[y](t)`


.. _arithmeticexampletubeb:
.. figure:: ../../img/examples/01_arithmetic/example_tube_b.png
  :width: 50%

  Tube :math:`[b](t)=\sin\left([x](t)\right)`


.. _arithmeticexampletubec:
.. figure:: ../../img/examples/01_arithmetic/example_tube_c.png
  :width: 50%

  Tube :math:`[c](t)=\int_{0}^{t}[x](\tau)d\tau`


To run the example, launch the Vibes viewer and, from a terminal:

.. code-block:: bash

  ./build/examples/cpp/01_arithmetic/ex_arithmetic 

The sources are available in:

.. code-block:: none

  ./examples/cpp/01_arithmetic/main.cpp