.. _sec-manual-ctclohner:

.. warning::
  
  The writing of this page is in progress. Several changes are currently performed on the library.

**************************************************************************
CtcLohner: :math:`\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t)\big)`
**************************************************************************

.. contents::


Definition
----------

The Lohner contractor uses Lohner's guaranteed integration algorithm to contract a tube according to a differential
equation. This algorithm performs two main steps

- an estimation of a global enclosure of the system's trajectories over an integration step (i.e. the domain of a slice),
  which corresponds to the codomain of that slice;
- using this estimation and the input gate (or output gate in the backward mode), the corresponding output gate (or input gate
  in the backward mode) is then estimated;
- over a single slice, these steps can be iterated a few times to obtain tighter enclosures for both the gate and the codomain of the slice.

This contractor is supposed to yield better results than the Picard contractor, as long as the tubes are "thin enough".

.. important::
    
  .. math::

    \left.\begin{array}{r}\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t)\big)\end{array}\right. \longrightarrow \mathcal{C}_{\textrm{Lohner}}\big([\mathbf{x}](\cdot)\big)

  .. tabs::

    .. code-tab:: py

      ctc_lohner = CtcLohner(Function("<var1>", "<var2...>", "<exp>"))
      ctc_lohner.contract(x)

    .. code-tab:: c++

      CtcLohner ctc_lohner(Function("<var1>", "<var2...>", "<exp>"));
      ctc_lohner.contract(x);

.. important::

    The contractor might throw a runtime error when it cannot find a global enclosure over a specific time step. This usually
    happens when the time step is too large, and may therefore be avoided by reducing the discretisation frequency of the tube.

    .. admonition:: Remark

        Note that this behaviour may be tackled using automatic step adjustment. However, such a feature is not yet implemented.


Related content
---------------

.. admonition:: Mathematical documentation

  See `Auguste Bourgois' thesis <https://www.ensta-bretagne.fr/jaulin/thesis_auguste.pdf>`_. In particular, check Chapter 4.

.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classcodac_1_1_ctc_lohner.html>`_.
