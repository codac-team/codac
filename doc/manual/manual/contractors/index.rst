.. _sec-ctc:

Contractors, separators
=======================

.. toctree::

  CtcInter <set/ctcinter>
  CtcInverse <analytic/ctcinverse>
  CtcDist <geometric/ctcdist>
  CtcPolar <geometric/ctcpolar>
  CtcWrapper <shape/ctcwrapper>
  CtcEval <dynamic/ctceval>
  CtcLohner <dynamic/ctclohner>
  CtcVisible <geometric/ctcvisible>
  SepImage <analytic/sepimage>

..   What are contractors? <http://codac.io>
..   The Ctc class <http://codac.io>
..   
..   basic/index.rst
..   linear/index.rst
..   set/index.rst
..   analytic/index.rst
..   geometric/index.rst
..   shape/index.rst
..   temporal/index.rst



Overview of contractors and separators
--------------------------------------

.. list-table::
   :header-rows: 1
   :widths: 50 50

   * - **Contractors**
     - **Separators**

   * - ``CtcIdentity``
     - —

   * - ``CtcEmpty``
     - —

   * - ``CtcLazy``
     - —

   * - ``CtcFixpoint``
     - —

   * - ``CtcGaussElim``
     - —

   * - ``CtcGaussSeidel``
     - —

   * - ``CtcLinearPrecond``
     - —

   * - ``CtcUnion``
     - ``SepUnion``

   * - :ref:`CtcInter <sec-ctc-set-ctcinter>`
     - ``SepInter``

   * - ``CtcQInter``
     - ``SepQInter``

   * - ``CtcCartProd``
     - ``SepCartProd``

   * - ``CtcProj``
     - ``SepProj``

   * - ``CtcNot``
     - ``SepNot``

   * - ``CtcAction``
     - ``SepAction``

   * - —
     - :ref:`SepImage <sec-ctc-analytic-sepimage>`

   * - :ref:`CtcInverse <sec-ctc-analytic-ctcinverse>`
     - ``SepInverse``

   * - ``CtcInverseNotIn``
     - —

   * - —
     - ``SepTransform``

   * - :ref:`CtcDist <sec-ctc-geom-ctcdist>`
     - —

   * - :ref:`CtcPolar <sec-ctc-geom-ctcpolar>`
     - ``SepPolarCart`` / ``SepCartPolar``

   * - ``CtcSegment``
     - —

   * - ``CtcPolygon``
     - ``SepPolygon``

   * - ``CtcPointCloud``
     - —

   * - ``CtcEllipse``
     - ``SepEllipse``

   * - ``CtcCross`` / ``CtcNoCross``
     - ``SepCross``

   * - ``CtcCtcBoundary``
     - ``SepCtcBoundary``

   * - :ref:`CtcWrapper <sec-ctc-shape-ctcwrapper>`
     - ``SepWrapper``

   * - ``CtcImage``
     - ``SepImage``

   * - ``CtcDiscreteSet``
     - —

   * - ``CtcDeriv``
     - —

   * - :ref:`CtcEval <sec-ctc-dynamic-ctceval>`
     - —

   * - ``CtcDelay``
     - —

   * - ``CtcLinobs``
     - —

   * - ``CtcLohner``
     - —

   * - ``CtcPicard``
     - —

   * - ``CtcChain``
     - —

   * - ``CtcDiffInclusion``
     - —

   * - —
     - ``SepCtcPair``

   * - ``CtcInnerOuter``
     - —
