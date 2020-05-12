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



Related publication
-------------------

.. |tubint-pdf| replace:: **Download the paper**
.. _tubint-pdf: http://simon-rohou.fr/research/tubint/tubint_paper.pdf

.. admonition:: Related publication
  
  | **Guaranteed computation of robot trajectories**
  | S. Rohou, L. Jaulin, L. Mihaylova, F. Le Bars, S. M. Veres
  | *Robotics and Autonomous Systems*, 93:76--84, 2017.
  | |tubint-pdf|_