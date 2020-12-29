.. _sec-manual-ctclohner:

.. warning::
  
  The writing of this page is in progress.

**************************************************************************
CtcLohner: :math:`\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),t\big)`
**************************************************************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),t\big)\end{array}\right. \longrightarrow \mathcal{C}_{\textrm{Lohner}}\big([\mathbf{x}](\cdot)\big)

  .. tabs::

    .. code-tab:: py

      ctc_lohner = CtcLohner(TFunction("<var1>", "<var2...>", "<exp>"))
      ctc_lohner.contract(x)

    .. code-tab:: c++

      CtcLohner ctc_lohner(TFunction("<var1>", "<var2...>", "<exp>"));
      ctc_lohner.contract(x);



Related content
---------------

.. todo: Link to Auguste's thesis

.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classtubex_1_1_ctc_lohner.html>`_.