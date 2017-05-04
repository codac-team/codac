Algebraic contractors
---------------------

The following example is taken from the paper *Reliable non-linear state estimation involving time uncertainties*.

:numref:`arithmeticctcexampletubex` and :numref:`arithmeticctcexampletubey`
present two scalar tubes :math:`[x](\cdot)` and :math:`[y](\cdot)`. :numref:`arithmeticctcexampletubea`,
:numref:`arithmeticctcexampletubeb` and :numref:`arithmeticctcexampletubec` picture tubes computed
from :math:`[x](\cdot)` and :math:`[y](\cdot)` based on algebraic constraints. An observation on 
:math:`[y](\cdot)` leads to contractions on :math:`[a](\cdot)`, :math:`[b](\cdot)` and :math:`[c](\cdot)`.

.. _arithmeticctcexampletubex:
.. figure:: ../../img/examples/02_arithmetic_ctc/example_ctc_tube_x.png
  :width: 50%

  Arbitrary envelope of feasible :math:`x(\cdot)`.


.. _arithmeticctcexampletubey:
.. figure:: ../../img/examples/02_arithmetic_ctc/example_ctc_tube_y.png
  :width: 50%

  Arbitrary contracted envelope of feasible :math:`y(\cdot)`.


.. _arithmeticctcexampletubea:
.. figure:: ../../img/examples/02_arithmetic_ctc/example_ctc_tube_a.png
  :width: 50%

  Contracted envelope of feasible :math:`a(\cdot)=x(\cdot)+y(\cdot)`.


.. _arithmeticctcexampletubeb:
.. figure:: ../../img/examples/02_arithmetic_ctc/example_ctc_tube_p.png
  :width: 50%

  Contracted envelope of feasible :math:`p(\cdot)=\arctan(y(\cdot))`.


.. _arithmeticctcexampletubec:
.. figure:: ../../img/examples/02_arithmetic_ctc/example_ctc_tube_q.png
  :width: 50%

  Contracted envelope of feasible :math:`q(\cdot)=2\sin\left(\frac{a(\cdot)}{2}\right)+\sqrt{2p(\cdot)}`.


To run the example, launch the Vibes viewer and, from a terminal:

.. code-block:: bash

  ./build/examples/cpp/02_arithmetic_ctc/ex_arithmetic_ctc

The sources are available in:

.. code-block:: none

  ./examples/cpp/02_arithmetic_ctc/main.cpp