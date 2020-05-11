.. _sec-manual-ctceval:

***************************
CtcEval: :math:`y_i=x(t_i)`
***************************

:math:`y_i=x(t_i)` is a constraint that links a value :math:`y_i` to the evaluation of the trajectory :math:`x(\cdot)` at time :math:`t_i`. A dedicated contractor :math:`\mathcal{C}_{\textrm{eval}}` implements this constraint. One of its applications is the correction of the positions of a robot based on observations.

.. contents::


Definition
----------

.. important::
    
  .. math::

    \left.\begin{array}{r}y_i=x(t_i)\\\dot{x}(\cdot)=v(\cdot)\end{array}\right\} \longrightarrow \mathcal{C}_{\textrm{eval}}\big([t_i],[y_i],[x](\cdot),[v](\cdot)\big)

  .. tabs::

    .. code-tab:: c++

      ctc::eval.contract(ti, yi, x, v);

    .. code-tab:: py

      # todo

  .. rubric:: Prerequisite

  The tubes :math:`[x](\cdot)` and :math:`[v](\cdot)` must share:

  * the same slicing (same sampling of time)
  * the same *t*-domain :math:`[t_0,t_f]`
  * the same dimension of :math:`[y_i]` in the vector case

Theoretical illustration
------------------------




Localization example
--------------------

.. rubric:: Contracting the tube

Let us come back to the Lissajous example :ref:`of the previous page <sec-manual-ctcderiv-localization>`.

Assume now that we have no knowledge on :math:`[\mathbf{x}](\cdot)`, except that the feasible trajectories go through the box :math:`[\mathbf{b}]=[-0.73,-0.69]\times[0.64,0.68]` at some time :math:`t\in[4.3,4.4]`.

The tube is contracted over :math:`[t_0,t_f]` with its uncertain derivative :math:`[\mathbf{v}](\cdot)` given by:

.. math::

  [\mathbf{v}](t) = \left(\begin{array}{c}-2\sin(t)\\2\cos(2t)\end{array}\right)+\left(\begin{array}{c} [-0.01,0.01]\\ [-0.01,0.01]\end{array}\right)

.. tabs::

  .. code-tab:: c++

    // No initial knowledge on [x](·)
    double dt = 0.01;
    Interval tdomain(0.,5.);
    TubeVector x(tdomain, dt, 2); // initialization with [-∞,∞]×[-∞,∞]

    // New values for the temporal evaluation of [x](·)
    Interval t(4.3,4.4);
    IntervalVector b({{-0.73,-0.69},{0.64,0.68}});

    // Uncertain derivative of [x](·)
    TubeVector v(tdomain, dt, Function("(-2*sin(t) ; 2*cos(2*t))"));
    v.inflate(0.01);

    // Contraction
    CtcEval ctc_eval;
    ctc_eval.contract(t, b, x, v);
    // Note that in this case, no contraction is performed on [t] and [b]
    // Note also that we could use directly ctc::eval.contract(t, b, x, v)

  .. code-tab:: py

    # todo

The obtained tube is blue painted on the figure, the contraction to keep the trajectories going through :math:`[\mathbf{b}]` (red box) over :math:`[t]=[4.3,4.4]` is propagated over the whole *t*-domain:

.. figure:: img/ceval_lissajous_tube.png


.. #include <tubex.h>
.. #include <tubex-rob.h>
.. 
.. using namespace std;
.. using namespace tubex;
.. 
.. int main()
.. {
..   double dt = 0.01;
..   Interval tdomain(0.,5.);
..   TrajectoryVector x_truth(tdomain, Function("(2*cos(t) ; sin(2*t))"));
.. 
..   TubeVector x(tdomain, dt, 2);
.. 
..   TubeVector v(tdomain, dt, Function("(-2*sin(t) ; 2*cos(2*t))"));
..   v.inflate(0.01);
.. 
..   vibes::beginDrawing();
.. 
..   VIBesFigMap fig_map("Map");
..   fig_map.set_properties(1450, 50, 600, 300);
..   fig_map.add_trajectory(&x_truth, "x*", 0, 1, "white");
..   fig_map.add_tube(&x, "x", 0, 1);
..   fig_map.smooth_tube_drawing(true);
.. 
..   Interval t(4.3,4.4);
..   IntervalVector b({{-0.73,-0.69},{0.64,0.68}});
.. 
..   fig_map.draw_box(b, "#076594");
..   ctc::eval.contract(t, b, x, v);
..   fig_map.draw_vehicle(t.mid(), &x_truth, 0.25);
..   fig_map.draw_box(b, "#D74400");
.. 
..   fig_map.axis_limits(-0.1,0.1,-1.3,1.3, true);
..   fig_map.show(0.);
.. 
..   vibes::endDrawing();
.. }


.. rubric:: Contracting the evaluation box

Assume now that we know the actual trajectory to be bounded within the tube:

.. math::

  [\mathbf{x}](t) = \left(\begin{array}{c}2\cos(t)\\\sin(2t)\end{array}\right)+\left(\begin{array}{c} [-0.05,0.05]\\ [-0.05,0.05]\end{array}\right)

.. tabs::

  .. code-tab:: c++

    TubeVector x(tdomain, dt, Function("(2*cos(t) ; sin(2*t))"));
    x.inflate(0.05);

  .. code-tab:: py

    # todo

The tube is blue painted on the figure:

.. figure:: img/ceval_lissajous_box.png

The yellow robot depicts an unknown position :math:`\mathbf{x}` in the box :math:`[-1,0]\times[0.4,1.2]` at an unknown :math:`t\in[t_0,t_f]`.
The :math:`\mathcal{C}_{\textrm{eval}}` can be used to evaluate the position time and reduce the uncertainty on the possible positions.

.. tabs::

  .. code-tab:: c++

    t = Interval::all_reals(); // new initialization
    b = {{-1.,0.},{0.4,1.2}}; // (blue box on the figure)

    ctc_eval.contract(t, b, x);

    // [t] estimated to [4.15, 4.54]
    // [b] contracted to ([-1, -0.29] ; [0.4, 0.95])  (red on the figure)

  .. code-tab:: py

    # todo


.. #include <tubex.h>
.. #include <tubex-rob.h>
.. 
.. using namespace std;
.. using namespace tubex;
.. 
.. int main()
.. {
..   double dt = 0.01;
..   Interval tdomain(0.,5.);
..   TrajectoryVector x_truth(tdomain, Function("(2*cos(t) ; sin(2*t))"));
..   TubeVector x(x_truth, dt);
..   x.inflate(0.05);
.. 
..   vibes::beginDrawing();
.. 
..   VIBesFigMap fig_map("Map");
..   fig_map.set_properties(1450, 50, 600, 300);
..   fig_map.add_trajectory(&x_truth, "x*", 0, 1, "white");
..   fig_map.add_tube(&x, "x", 0, 1);
..   fig_map.smooth_tube_drawing(true);
.. 
..   Interval t;
..   IntervalVector b = {{-1.,0.},{0.4,1.2}};
..   fig_map.draw_box(b, "#076594");
..   ctc::eval.contract(t, b, x);
..   fig_map.draw_vehicle(t.mid(), &x_truth, 0.25);
..   fig_map.draw_box(b, "#D74400");
..
..   cout << t << b << endl;
.. 
..   fig_map.axis_limits(-0.1,0.1,-1.3,1.3, true);
..   fig_map.show(0.);
..   
..   vibes::endDrawing();
.. }



Related content
---------------

.. |tubeval-pdf| replace:: **Download the paper**
.. _tubeval-pdf: http://simon-rohou.fr/research/tubeval/tubeval_paper.pdf

.. admonition:: Related publication
  
  | **Reliable non-linear state estimation involving time uncertainties**
  | S. Rohou, L. Jaulin, L. Mihaylova, F. Le Bars, S. M. Veres
  | *Automatica*, 93:379--388, July 2018.
  | |tubeval-pdf|_


.. admonition:: Technical documentation

  See the `API documentation of this class <../../../api/html/classtubex_1_1_ctc_eval.html>`_.