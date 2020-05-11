.. _sec-manual-ctcderiv:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*********************************
CtcDeriv: :math:`\dot{x}(t)=v(t)`
*********************************

:math:`\dot{x}(t)=v(t)` is the simplest differential constraint that binds a trajectory :math:`x(\cdot)` to its derivative :math:`v(\cdot)`. The related contractor :math:`\mathcal{C}_{\frac{d}{dt}}` allows contractions on the tube :math:`[x](\cdot)` to preserve only trajectories consistent with the derivatives enclosed in the tube :math:`[v](\cdot)`.

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

  .. rubric:: Prerequisite

  The tubes :math:`[x](\cdot)` and :math:`[v](\cdot)` must share:

  * the same slicing (same sampling of time)
  * the same *t*-domain :math:`[t_0,t_f]`
  * the same dimension in the vector case


.. figure:: img/cderiv_v.png

.. figure:: img/cderiv_x.png


.. #include <tubex.h>
.. 
.. using namespace std;
.. using namespace tubex;
.. 
.. int main()
.. {
..   double dt = 0.01;
..   Interval tdomain(0., M_PI);
.. 
..   Tube v(tdomain, dt, Function("sin(t+3.14+(3.14/2))/5+[-0.05,0.05]+(3.14-t)*[-0.01,0.01]"));
..   Tube x(tdomain, dt, Function("[-0.05,0.05]+2+t*t*[-0.01,0.01]"));
.. 
..   vibes::beginDrawing();
.. 
..   VIBesFigTube fig_x("Tube x");
..   fig_x.set_properties(100, 100, 600, 300);
..   fig_x.add_tube(&x, "x");
..   fig_x.show();
.. 
..   VIBesFigTube fig_v("Tube v");
..   fig_v.set_properties(100, 500, 600, 300);
..   fig_v.add_tube(&v, "v");
..   fig_v.show();
.. 
..   ctc::deriv.contract(x, v);
..   fig_x.show();  
.. 
..   vibes::endDrawing();
.. 
..   return EXIT_SUCCESS;
.. }



Related content
---------------

.. |tubint-pdf| replace:: **Download the paper**
.. _tubint-pdf: http://simon-rohou.fr/research/tubint/tubint_paper.pdf

.. admonition:: Related publication
  
  | **Guaranteed computation of robot trajectories**
  | S. Rohou, L. Jaulin, L. Mihaylova, F. Le Bars, S. M. Veres
  | *Robotics and Autonomous Systems*, 93:76--84, 2017.
  | |tubint-pdf|_


.. admonition:: Technical documentation

  See the `API documentation of this class <../../../api/html/classtubex_1_1_ctc_deriv.html>`_.