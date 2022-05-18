.. _sec-manual-separators:

############################
Set-inversion and separators
############################

A separator :math:`\mathcal{S}` is an operator that performs two complementary contractions. Associated with a set :math:`\mathbb{S}` and given a box :math:`[\mathbf{x}]`, the separator produces two sub-boxes :math:`[\mathbf{x}_ {in}]` and :math:`[\mathbf{x}_{out}]` that verify:

.. math::

  ([\mathbf{x}] \cap [\mathbf{x}_{in}]) \subset \mathbb{S} \\
  ([\mathbf{x}] \cap [\mathbf{x}_{out}]) \cap \mathbb{S} = \varnothing

A separator can also be viewed as a pair of contractors. For efficiency reasons, the ``.separate(…)`` method takes only two input-output arguments, :math:`[\mathbf{x}_{in}]` and :math:`[\mathbf{x}_{out}]`, each containing initially a copy of the box :math:`[\mathbf{x}]`.

Separators can be combined and involved in an algorithm for set-inversion such as SIVIA. An example of set-inversion of a non-linear function using separators is provided below:

.. tabs::

  .. code-tab:: py

    from codac import *
    from vibes import vibes

    # Define some non-linear function
    f = Function('x', 'y', 'x*cos(x-y)+y')

    # Build the separator associated to the constraint f(x,y) < 0
    sep = SepFunction(f, Interval(-oo,0))

    # Setup the initial box
    box = IntervalVector(2, [-10, 10])

    # Graphics
    vibes.beginDrawing()
    vibes.newFigure("Set inversion")
    vibes.setFigureProperties({"x":100, "y":100, "width":500, "height":500})
    SIVIA(box, sep, 0.21, fig_name="Set inversion")
    vibes.endDrawing()

  .. code-tab:: c++

    #include <codac.h>

    using namespace codac;

    int main()
    {
      // Define some non-linear function
      Function f("x", "y", "x*cos(x-y)+y");

      // Build the separator associated to the constraint f(x,y) < 0
      SepFunction sep(f, Interval(-oo,0));

      // Setup the initial box
      IntervalVector box(2, {-10, 10});

      // Graphics
      vibes::beginDrawing();
      vibes::newFigure("Set inversion");
      vibes::setFigureProperties(vibesParams("x",100, "y",100, "width",500, "height",500));
      SIVIA(box, sep, 0.21, "Set inversion");
      vibes::endDrawing();

      return EXIT_SUCCESS;
    }

The set-inversion leads to the following paving:

.. figure:: img/paving_set_inversion.png

  Set-inversion of the function :math:`f(x,y)=x\cdot\cos(x-y)+y`, considering the constraint :math:`f(x,y)\leqslant 0` (which can be formalized as :math:`f(x,y)\in[-\infty,0]`).

Green boxes only enclose vectors that are solutions of the inequality, blue ones do not contain solutions, and the yellow fronteer contains the boundary of the solution set.

The ``SIVIA(..)`` function allows several parameters for specifying result outputs (returned values, displayed colors). For instance:

.. tabs::

  .. code-tab:: py

    SIVIA(box,              # initial domain to be bisected
      sep,                  # related separator (or contractor if provided)
      0.21,                 # precision parameter (stopping condition)
      display_result=True,  # displaying boxes in a VIBes figure
      fig_name="SIVIA",     # name of the VIBes figure in case of display
      return_result=True,   # returning a map of lists of boxes
      # The following color map allows to define custom colors for subpavings:
      color_map={SetValue.IN:"k[r]", SetValue.OUT:"k[b]", SetValue.UNKNOWN:"k[y]"})

  .. code-tab:: c++

    SIVIA(box,              // initial domain to be bisected
      sep,                  // related separator (or contractor if provided)
      0.21,                 // precision parameter (stopping condition)
      true,                 // boolean for displaying boxes in a VIBes figure
      "SIVIA",              // name of the VIBes figure in case of display
      true,                 // boolean for returning a map of lists of boxes
      // The following color map allows to define custom colors for subpavings:
      {{SetValue::IN,"k[r]"},{SetValue::OUT,"k[b]"},{SetValue::UNKNOWN,"k[y]"}});

Separators
----------

The following separators apply on intervals and boxes:

.. toctree::

  01-sep-box

See also: :ref:`codac-unsupported <sec-codac-unsupported>`.