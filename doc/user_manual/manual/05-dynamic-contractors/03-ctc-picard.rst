.. _sec-manual-ctcpicard:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

***************************************************************
CtcPicard: :math:`\dot{\mathbf{x}}(t)=f\big(\mathbf{x}(t)\big)`
***************************************************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}\dot{\mathbf{x}}(t)=f\big(\mathbf{x}(t)\big)\end{array}\right. \longrightarrow \mathcal{C}_{\textrm{picard}}\big([x](\cdot),[v](\cdot)\big)

  .. tabs::

    .. code-tab:: c++

      CtcPicard ctc_picard(Function f("<var1>", "<var2...>", "<exp>"));
      ctc_picard.contract(x);

    .. code-tab:: py

      # todo


.. admonition:: Technical documentation

  See the `API documentation of this class <../../../api/html/classtubex_1_1_ctc_picard.html>`_.