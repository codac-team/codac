.. _sec-manual-ctcpolar:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*************************************************************
CtcPolar: :math:`\big(x=\rho\cos\theta;y=\rho\sin\theta\big)`
*************************************************************

.. contents::

.. note::

  .. Figure:: ../../img/pyibex_logo.jpg
    :align: right
  
  This contractor directly comes from the `pyIbex library <http://benensta.github.io/pyIbex/>`_ and is temporarily provided here for ease of integration.


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}x=\rho\cos\theta\\y=\rho\sin\theta\end{array}\right\} \longrightarrow \mathcal{C}_{\textrm{polar}}\big([x],[y],[\rho],[\theta]\big)

  .. tabs::

    .. code-tab:: c++

      #include <tubex-pyibex.h>
      ctc::polar.contract(x, y, rho, theta);

    .. code-tab:: py

      # todo


Related publication
-------------------

.. |polar-pdf| replace:: **Download the paper**
.. _polar-pdf: https://www.ensta-bretagne.fr/jaulin/paper_polar.pdf

.. admonition:: Related publication
  
  | **A Minimal contractor for the Polar equation: application to robot localization**
  | B. Desrochers, L. Jaulin
  | *Engineering Applications of Artificial Intelligence*, 55(Supplement C):83–92, Oct. 2016.
  | |polar-pdf|_
