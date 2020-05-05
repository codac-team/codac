.. _sec-manual-ctcderiv:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*********************************
CtcDeriv: :math:`\dot{x}(t)=v(t)`
*********************************

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}\dot{x}(\cdot)=v(\cdot)\end{array}\right. \longrightarrow \mathcal{C}_{\frac{d}{dt}}\big([x](\cdot),[v](\cdot)\big)

  .. tabs::

    .. code-tab:: c++

      ctc::deriv.contract(x, v);

    .. code-tab:: py

      # todo



Related publication
-------------------

.. |tubint-pdf| replace:: **Download the paper**
.. _tubint-pdf: http://simon-rohou.fr/research/tubint/tubint_paper.pdf

.. admonition:: Related publication
  
  | **Guaranteed computation of robot trajectories**
  | S. Rohou, L. Jaulin, L. Mihaylova, F. Le Bars, S. M. Veres
  | *Robotics and Autonomous Systems*, 93:76--84, 2017.
  | |tubint-pdf|_