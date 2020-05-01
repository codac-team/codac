.. _sec-manual-ctcdistance:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*********************************
CtcDist: :math:`\sqrt{x^2+y^2}=d`
*********************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \sqrt{(a_1-b_1)^2+(a_2-b_2)^2}=d \longrightarrow \mathcal{C}_{\textrm{dist}}\big([\mathbf{a}],[\mathbf{b}],[d]\big)

  .. tabs::

    .. code-tab:: c++

      ctc::dist.contract(a, b, d);

    .. code-tab:: py

      # todo


Example
-------
