.. _sec-functions-analytic-operators:

Analytic operators
==================

  Main authors: `Simon Rohou <https://www.simon-rohou.fr/research/>`_, `Damien Massé <https://lab-sticc.univ-brest.fr/~dmasse/>`_
  
.. raw:: html

  <style>
    table
    {
      font-size: 0.9em;
    }

    td:has(.bg-ok), td:has(.bg-bok), td:has(.bg-nok) {
      width: 2.5em;
    }

    .bg-title {
      background-color: var(--color-sidebar-background);
      color: white;
      font-weight: bold;
    }

    td:has(.bg-title) {
      background-color: var(--color-sidebar-background);
    }

    .vert {
      vertical-align: bottom;
      writing-mode: vertical-lr;
      transform: scale(-1, -1);
    }
  </style>

Codac provides a list of operators for calculating intervals, defining expressions and constructing contractors.


Operators currently available
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following table lists the mathematical operators currently available. The availability is indicated for four operations:
  1. Natural forward evaluation
  2. Centered-form forward evaluation
  3. Differentiation
  4. Backward (reverse) evaluation

When operators are available for operations 2--3, then an ``AnalyticFunction`` can be built upon them and the ``.eval(EvalMode.CENTERED, ...)`` and ``.diff(...)`` methods can be successfully called. When operators are available for operation 4, then analytic contractors such as ``CtcInverse`` will work.

.. role:: bg-ok
.. role:: bg-bok
.. role:: bg-nok
.. role:: bg-title
.. role:: vertical

.. role:: raw-html(raw)
  :format: html

.. |okk| replace:: :bg-ok:`✓`
.. |bok| replace:: :bg-bok:`✓`
.. |nok| replace:: :bg-nok:`✗`
.. |N|   replace:: :vertical:`Natur.`
.. |C|   replace:: :vertical:`Centr.`
.. |D|   replace:: :vertical:`Diff.`
.. |CHI| replace:: :math:`\begin{split}\chi(x_1,x_2,x_3) =\\ \begin{cases}x_2 & \text{if } x_1 \leqslant 0, \\x_3 & \text{if } x_1>0.\end{cases}\end{split}`
.. |CHI_TYPES| replace:: ``x1``: scalar, ``x2``, ``x3``: any (same types)
.. |EXT| replace:: :math:`\begin{split}f(x) \textrm{ if } x \in D_f,\\ g(x) \textrm{ otherwise}\end{split}`

.. |AbsOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_abs.h"><code class="docutils literal notranslate"><span class="pre">AbsOp</span></code></a>`

.. |AcosOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_acos.h"><code class="docutils literal notranslate"><span class="pre">AcosOp</span></code></a>`

.. |AsinOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_asin.h"><code class="docutils literal notranslate"><span class="pre">AsinOp</span></code></a>`

.. |AtanOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_atan.h"><code class="docutils literal notranslate"><span class="pre">AtanOp</span></code></a>`

.. |CeilOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_ceil.h"><code class="docutils literal notranslate"><span class="pre">CeilOp</span></code></a>`

.. |ChiOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_chi.h"><code class="docutils literal notranslate"><span class="pre">ChiOp</span></code></a>`

.. |CosOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_cos.h"><code class="docutils literal notranslate"><span class="pre">CosOp</span></code></a>`

.. |CoshOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_cosh.h"><code class="docutils literal notranslate"><span class="pre">CoshOp</span></code></a>`

.. |CrossProdOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_cross_prod.h"><code class="docutils literal notranslate"><span class="pre">CrossProdOp</span></code></a>`

.. |DetOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_det.h"><code class="docutils literal notranslate"><span class="pre">DetOp</span></code></a>`

.. |ExpOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_exp.h"><code class="docutils literal notranslate"><span class="pre">ExpOp</span></code></a>`

.. |ExtendOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_extend.h"><code class="docutils literal notranslate"><span class="pre">ExtendOp</span></code></a>`

.. |FloorOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_floor.h"><code class="docutils literal notranslate"><span class="pre">FloorOp</span></code></a>`

.. |LogOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_log.h"><code class="docutils literal notranslate"><span class="pre">LogOp</span></code></a>`

.. |SignOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_sign.h"><code class="docutils literal notranslate"><span class="pre">SignOp</span></code></a>`

.. |SinOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_sin.h"><code class="docutils literal notranslate"><span class="pre">SinOp</span></code></a>`

.. |SinhOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_sinh.h"><code class="docutils literal notranslate"><span class="pre">SinhOp</span></code></a>`

.. |SqrOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_sqr.h"><code class="docutils literal notranslate"><span class="pre">SqrOp</span></code></a>`

.. |SqrtOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_sqrt.h"><code class="docutils literal notranslate"><span class="pre">SqrtOp</span></code></a>`

.. |TanOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_tan.h"><code class="docutils literal notranslate"><span class="pre">TanOp</span></code></a>`

.. |TanhOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_tanh.h"><code class="docutils literal notranslate"><span class="pre">TanhOp</span></code></a>`

.. |FlattenOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_flatten.h"><code class="docutils literal notranslate"><span class="pre">FlattenOp</span></code></a>`

.. |TransposeOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_transpose.h"><code class="docutils literal notranslate"><span class="pre">TransposeOp</span></code></a>`

.. |AddOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_arith_add.h"><code class="docutils literal notranslate"><span class="pre">AddOp</span></code></a>`

.. |SubOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_arith_sub.h"><code class="docutils literal notranslate"><span class="pre">SubOp</span></code></a>`

.. |MulOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_arith_mul.h"><code class="docutils literal notranslate"><span class="pre">MulOp</span></code></a>`

.. |DivOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_arith_div.h"><code class="docutils literal notranslate"><span class="pre">DivOp</span></code></a>`

.. |MaxOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_max.h"><code class="docutils literal notranslate"><span class="pre">MaxOp</span></code></a>`

.. |MinOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_min.h"><code class="docutils literal notranslate"><span class="pre">MinOp</span></code></a>`

.. |ModOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_mod.h"><code class="docutils literal notranslate"><span class="pre">ModOp</span></code></a>`

.. |PowOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_pow.h"><code class="docutils literal notranslate"><span class="pre">PowOp</span></code></a>`

.. |Atan2Op| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_atan2.h"><code class="docutils literal notranslate"><span class="pre">Atan2Op</span></code></a>`

.. |ComponentOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_component.h"><code class="docutils literal notranslate"><span class="pre">ComponentOp</span></code></a>`

.. |SubvectorOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_subvector.h"><code class="docutils literal notranslate"><span class="pre">SubvectorOp</span></code></a>`

.. |VectorOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_vec.h"><code class="docutils literal notranslate"><span class="pre">VectorOp</span></code></a>`

.. |MatrixOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/operators/codac2_mat.h"><code class="docutils literal notranslate"><span class="pre">MatrixOp</span></code></a>`

.. |OctaSymOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/actions/codac2_OctaSym_operator.h"><code class="docutils literal notranslate"><span class="pre">OctaSymOp</span></code></a>`

.. |TrajOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/trajectory/codac2_Traj_operator.h"><code class="docutils literal notranslate"><span class="pre">TrajectoryOp</span></code></a>`

.. |TubeOp| replace:: :raw-html:`<a href="https://github.com/codac-team/codac/blob/codac2/src/core/domains/tube/codac2_Tube_operator.h"><code class="docutils literal notranslate"><span class="pre">TubeOp</span></code></a>`

Only the :bg-ok:`✓` operators are supported at the moment.
If you notice any mathematical operators missing from the list below, feel free to contribute to the library. You can submit your suggestions or pull requests on the `GitHub repository of Codac <https://github.com/codac-team/codac>`_.

| The sources of the operators are available in the ``operators`` folder:
| https://github.com/codac-team/codac/blob/codac2/src/core/operators

.. table:: List of available operators in Codac
  :name: operators-table

  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+-------------------------+------------+
  |                                                     |                      |               |                                     | Fwd eval.               | Bwd. eval. |
  |                                                     |                      |               |                                     +--------+--------+-------+            |
  | Operator                                            | Syntax               | Struct        | Operands type                       | |N|    | |C|    | |D|   |            |
  +=====================================================+======================+===============+=====================================+========+========+=======+============+
  | :bg-title:`Unary operations`                                                                                                                                            |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`|x|`                                         | ``abs(x)``           | |AbsOp|       | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\arccos(x)`                                  | ``acos(x)``          | |AcosOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\arcsin(x)`                                  | ``asin(x)``          | |AsinOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\arctan(x)`                                  | ``atan(x)``          | |AtanOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\lceil x \rceil`                             | ``ceil(x)``          | |CeilOp|      | ``x``: scalar                       ||okk|   ||nok|   ||nok|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\cos(x)`                                     | ``cos(x)``           | |CosOp|       | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\cosh(x)`                                    | ``cosh(x)``          | |CoshOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\det(\mathbf{A})`                            | ``det(A)``           | |DetOp|       | ``A``: matrix                       ||bok|   ||bok|   ||bok|  ||bok|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\exp(x)`                                     | ``exp(x)``           | |ExpOp|       | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\lfloor x \rfloor`                           | ``floor(x)``         | |FloorOp|     | ``x``: scalar                       ||okk|   ||nok|   ||nok|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\log(x)`                                     | ``log(x)``           | |LogOp|       | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathrm{sgn}(x)`                             | ``sign(x)``          | |SignOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\sin(x)`                                     | ``sin(x)``           | |SinOp|       | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\sinh(x)`                                    | ``sinh(x)``          | |SinhOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`x^2`                                         | ``sqr(x)``           | |SqrOp|       | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\sqrt{x}`                                    | ``sqrt(x)``          | |SqrtOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\tan(x)`                                     | ``tan(x)``           | |TanOp|       | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\tanh(x)`                                    | ``tanh(x)``          | |TanhOp|      | ``x``: scalar                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathrm{flatten}(\mathbf{X})`                | ``flatten(X)``       | |FlattenOp|   | ``X``: matrix                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{X}^\intercal`                        | ``transpose(x)``     | |TransposeOp| | ``X``: matrix                       ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :bg-title:`Binary operations`                                                                                                                                           |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`x_1+x_2`                                     | ``x1+x2``            | |AddOp|       | ``x1``, ``x2``: scalar              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}_1+\mathbf{x}_2`                   | ``x1+x2``            |               | ``x1``, ``x2``: vector              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{X}_1+\mathbf{X}_2`                   | ``X1+X2``            |               | ``x1``, ``x2``: matrix              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`x_1-x_2`                                     | ``x1-x2``            | |SubOp|       | ``x1``, ``x2``: scalar              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}_1-\mathbf{x}_2`                   | ``x1-x2``            |               | ``x1``, ``x2``: vector              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{X}_1-\mathbf{X}_2`                   | ``X1-X2``            |               | ``x1``, ``x2``: matrix              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`x_1\cdot x_2`                                | ``x1*x2``            | |MulOp|       | ``x1``, ``x2``: scalar              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`x_1\cdot\mathbf{x}_2`                        | ``x1*x2``            |               | ``x1``: scalar, ``x2``: vector      ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}_1\cdot x_2`                       | ``x1*x2``            |               | ``x1``: vector, ``x2``: scalar      ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`x_1\cdot\mathbf{X}_2`                        | ``x1*X2``            |               | ``x1``: scalar, ``X2``: matrix      ||okk|   ||okk|   ||okk|  ||nok|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}_1\cdot\mathbf{x}_2`               | ``x1*x2``            |               | ``x1``: row, ``x2``: vector         ||okk|   ||nok|   ||nok|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{X}_1\cdot\mathbf{x}_2`               | ``X1*x2``            |               | ``X1``: matrix, ``x2``: vector      ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{X}_1\cdot\mathbf{X}_2`               | ``X1*X2``            |               | ``X1``: matrix, ``X2``: matrix      ||okk|   ||okk|   ||okk|  ||nok|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`x_1/x_2`                                     | ``x1/x2``            | |DivOp|       | ``x1``, ``x2``: scalar              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}_1/x_2`                            | ``x1/x2``            |               | ``X1``: vector, ``x2``: scalar      ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{X}_1/x_2`                            | ``X1/x2``            |               | ``X1``: matrix, ``x2``: scalar      ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}_1\times\mathbf{x}_2`              | ``cross_prod(x1,x2)``| |CrossProdOp| | ``x1``, ``x2``: vector              ||okk|   ||okk|   ||okk|  ||nok|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\max(x_1,x_2)`                               | ``max(x1,x2)``       | |MaxOp|       | ``x1``, ``x2``: scalar              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\min(x_1,x_2)`                               | ``min(x1,x2)``       | |MinOp|       | ``x1``, ``x2``: scalar              ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`x_1\bmod x_2`                                | --                   | |ModOp|       | --                                  ||nok|   ||nok|   ||nok|  ||bok|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`(x_1)^{x_2}`                                 | | ``pow(x1,x2)``     | |PowOp|       | ``x1``, ``x2``: scalar              ||okk|   ||okk|   ||okk|  ||okk|       |
  |                                                     | | ``x1^x2``          |               |                                     |        |        |       |            |
  |                                                     | | ``x1**x2`` (py)    |               |                                     |        |        |       |            |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathrm{arctan2}(y,x)`                       | ``atan2(y,x)``       | |Atan2Op|     | ``y``, ``x``: scalar                ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | |EXT|                                               | ``extend(fx,gx)``    | |ExtendOp|    | ``fx``, ``gx``: any (same types)    ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :bg-title:`Ternary operations`                                                                                                                                          |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | |CHI|                                               | ``chi(x1,x2,x3)``    | |ChiOp|       | |CHI_TYPES|                         ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :bg-title:`Vectorial / matricial operations`                                                                                                                            |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`x_i` (vector coeff)                          | ``x[i]``             | |ComponentOp| | ``x``: vector, ``i``: scalar        ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+               +-------------------------------------+--------+--------+-------+------------+
  | :math:`X_{ij}` (mat. coeff)                         | ``X(i,j)``           |               | ``X``: matrix, ``i``, ``j``: scalar ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}_{i:j}` (subvector)                | ``x.subvector(i,j)`` | |SubvectorOp| | | ``x``: vector expression          ||okk|   ||okk|   ||okk|  ||okk|       |
  |                                                     |                      |               | | ``i``, ``j``: scalar              |        |        |       |            |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`[x_1,x_2,\dots]^\intercal`                   | ``vec(x1,x2,...)``   | |VectorOp|    | ``x1``, ``...``: scalar             ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\left(\mathbf{x}_1,\mathbf{x}_2,\dots\right)`| ``mat(x1,x2,...)``   | |MatrixOp|    | ``x1``, ``...``: vector             ||okk|   ||okk|   ||okk|  ||nok|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\sigma(\mathbf{x})`                          | ``s(x)``             | |OctaSymOp|   | ``s``: action, ``x``: vector        ||okk|   ||okk|   ||okk|  ||okk|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :bg-title:`Temporal operations`                                                                                                                                         |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`\mathbf{x}(t)`                               | ``x(t)``             | |TrajOp|      | ``x``: trajectory, ``t``: scalar    ||okk|   ||nok|   ||nok|  ||nok|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+
  | :math:`[\mathbf{x}](t)`                             | ``x(t)``             | |TubeOp|      | ``x``: tube, ``t``: scalar          ||okk|   ||nok|   ||nok|  ||nok|       |
  +-----------------------------------------------------+----------------------+---------------+-------------------------------------+--------+--------+-------+------------+

| Note: the operator :math:`\det` is only available for :math:`1\times 1` and :math:`2\times 2` matrices.
| Note: the operator :math:`\bmod` is only available for real periods (double precision), interval periods are not yet supported.


Expressions involving a non-supported centered-form operation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If an operator, for which the centered form is not defined, is involved in an expression, then this expression cannot be evaluated using the centered form (calculation is disabled for the entire operation). A simple natural evaluation will then be computed.


Expressions involving a temporal operator
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Temporal operations for involving trajectories or tubes in analytic expressions require an intermediate operation:

.. tabs::
  
  .. code-tab:: py

    x = # some sampled trajectory...
    g = x.as_function() # intermediate operation

    t = ScalarVar()
    h = AnalyticFunction(
      [t], g(t)
    )

  .. code-tab:: c++

    x = // some sampled trajectory...
    g = x.as_function(); // intermediate operation

    ScalarVar t;
    AnalyticFunction h(
      {t}, g(t)
    );


Direct use of operators
^^^^^^^^^^^^^^^^^^^^^^^

Operators can be used directly without building an ``AnalyticFunction``. They are organized in structures named ``<OperatorCode>Op`` and each structure provides ``.fwd()`` and ``.bwd()`` methods.
For example, the operator ``cos`` is proposed with:

.. tabs::
  
  .. code-tab:: py

    class CosOp:

      @staticmethod
      def fwd(x1):
        # Default natural forward evaluation
        # ...

      @staticmethod
      def bwd(y, x1):
        # Backward evaluation
        # ...

  .. code-tab:: c++

    struct CosOp
    {
      // Default natural forward evaluation
      static Interval fwd(const Interval& x1);

      // Backward evaluation
      static void bwd(const Interval& y, Interval& x1);
    };

| For the names of available structures, please refer to Table :ref:`operators-table`.
| An example of use of ``CosOp`` is given below:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [9-beg]
      :end-before: [9-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [9-beg]
      :end-before: [9-end]
      :dedent: 4

  .. group-tab:: Matlab

    .. literalinclude:: src.m
      :language: matlab
      :start-after: [9-beg]
      :end-before: [9-end]
      :dedent: 0
