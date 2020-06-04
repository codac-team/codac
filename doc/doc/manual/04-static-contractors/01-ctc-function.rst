.. _sec-manual-ctcfunction:

**************************************************************
Generic CtcFunction: :math:`\mathbf{f}(\mathbf{x})=\mathbf{0}`
**************************************************************

Lot of constraints can be expressed under the form :math:`\mathbf{f}(\mathbf{x})=\mathbf{0}` with :math:`\mathbf{f}` an analytic function possibly non-linear. The goal is to estimate the set of feasible vectors :math:`\mathbf{x}` of a domain :math:`[\mathbf{x}]` that satisfy this constraint.
A dedicated contractor can be built from :math:`\mathbf{f}` in order to contract boxes.

Note that the other form :math:`\mathbf{f}(\mathbf{x})\in[\mathbf{y}]` can also be treated by this contractor.

.. contents::


Definition
----------

.. important::
    
  .. math::

    \mathbf{f}(\mathbf{x})=\mathbf{0} \longrightarrow \mathcal{C}_{\mathbf{f}}\big([\mathbf{x}]\big)
    \mathrm{~~~~or~~~~}
    \mathbf{f}(\mathbf{x})\in[\mathbf{y}] \longrightarrow \mathcal{C}_{\mathbf{f},[\mathbf{y}]}\big([\mathbf{x}]\big)

  .. tabs::

    .. code-tab:: c++

      // For the constraint f(x)=0
      CtcFunction ctc_f(Function("<var1>", "<var2>", ..., "<expr>"));
      ctc_f.contract(x);

      // For the constraint f(x)\in[y]
      Interval y(...); // or IntervalVector if f is a vector function
      CtcFunction ctc_f(Function("<var1>", "<var2>", ..., "<expr>"), y);
      ctc_f.contract(x);

    .. code-tab:: py

      # For the constraint f(x)=0
      ctc_f = CtcFunction(Function("<var1>", "<var2>", ..., "<expr>"))
      ctc_f.contract(x)

      # For the constraint f(x)\in[y]
      y = Interval(...) # or IntervalVector if f is a vector function
      ctc_f = CtcFunction(Function("<var1>", "<var2>", ..., "<expr>"), y)
      ctc_f.contract(x)


.. note::

  .. Figure:: ../../img/logo_ibex.jpg
    :align: right
  .. Figure:: ../../img/logo_pyibex.jpg
    :align: right
  
  This contractor originates from the `IBEX library <http://www.ibex-lib.org>`_. It is briefly presented here for the sake of consistency. For more information, please refer to the `IBEX documentation <http://www.ibex-lib.org/doc/contractor.html>`_ for C++ use or to the `pyIbex manual <http://benensta.github.io/pyIbex/sphinx/api.html#ctcfwdbwd>`_ for Python usage.


.. rubric:: Optimality

This contractor is not necessarily optimal, depending on the expression of :math:`\mathbf{f}`.


Example
-------

Let us consider the following non-linear function:

.. math::

  f(\mathbf{x}) = x_1\cos(x_1-x_2)\sin(x_1)+x_2

A contractor for the constraint :math:`f(\mathbf{x})=0` can be built by:

.. tabs::

  .. code-tab:: c++

    CtcFunction ctc_f(Function("x1", "x2", "x1*cos(x1-x2)*sin(x1)+x2"));

  .. code-tab:: py

    ctc_f = CtcFunction(Function("x1", "x2", "x1*cos(x1-x2)*sin(x1)+x2"))

| The first parameters are the variables names. The last one is the expression of :math:`f`.
| Note that it is also possible to write vector variables:

.. tabs::

  .. code-tab:: c++

    CtcFunction ctc_f(Function("x[2]", "x[0]*cos(x[0]-x[1])*sin(x[0])+x[1]"));

  .. code-tab:: py

    ctc_f = CtcFunction(Function("x[2]", "x[0]*cos(x[0]-x[1])*sin(x[0])+x[1]"))


Then, a box :math:`[\mathbf{x}]` can be contracted by:

.. tabs::

  .. code-tab:: c++

    IntervalVector x({{-2.,-1.},{1.,2.5}});
    ctc_f.contract(x);

  .. code-tab:: py

    x = IntervalVector([[-2,-1],[1,2.5]])
    ctc_f.contract(x)

The boxes are contracted in order to remove some vectors that are not consistent with :math:`f(\mathbf{x})=0`. In the following figure, the exact solution for :math:`f(\mathbf{x})=0` is black painted. The initial boxes are depicted in blue, their contraction is represented in red.

.. figure:: img/CtcFunction.png

  Illustration of several contracted boxes with the above ``ctc_f`` contractor.

.. #include <tubex.h>
.. #include <tubex-rob.h>
.. #include "ibex_CtcHC4.h"
.. #include "ibex_SystemFactory.h"
.. #include "ibex_Ctc3BCid.h"
.. 
.. using namespace std;
.. using namespace tubex;
.. using namespace ibex;
.. 
.. int main()
.. {
..   SIVIAPaving p({{-3.,3.},{-3.,3.}});
..   p.compute(ibex::Function("x1", "x2", "x1*cos(x1-x2)*sin(x1)+x2"), {{0.}}, 0.01);
..   CtcFunction ctc_f(Function("x1", "x2", "x1*cos(x1-x2)*sin(x1)+x2"));
.. 
..   vibes::beginDrawing();
.. 
..   VIBesFigPaving fig_pav("test", &p);
.. 
..   vector<IntervalVector> v_x;
..   v_x.push_back({{-2.,-1.},{1.,2.5}});
..   v_x.push_back({{-1.7,-1.4},{-2.5,-0.8}});
..   v_x.push_back({{1.,2.},{-2.,2.}});
..   v_x.push_back({{-0.5,0.5},{-2.,2.}});
..   v_x.push_back({{-2.5,-1.},{0.,0.5}});
.. 
..   for(auto& x : v_x)
..   {
..     fig_pav.draw_box(x, "#006EA9");
..     ctc_f.contract(x);
..     fig_pav.draw_box(x, "#D14F06[#E2E2E2]");
..   }
..   
..   map<SetValue,string> color_map;
..   color_map[SetValue::MAYBE] = "black[black]";
..   color_map[SetValue::OUT] = "#ffffff00[#ffffff00]";
..   color_map[SetValue::IN] = "#ffffff00[#ffffff00]";
.. 
..   fig_pav.set_color_map(color_map);
..   fig_pav.show();
.. 
..   vibes::endDrawing();
.. }


Going further
-------------

This ``CtcFunction`` class is a generic shortcut to deal with :math:`\mathbf{f}(\mathbf{x})=\mathbf{0}` or :math:`\mathbf{f}(\mathbf{x})\in[\mathbf{y}]`. However, several algorithms exist to optimally deal with different classes of problems. A list of static contractors is provided in the IBEX library: `see more <http://www.ibex-lib.org/doc/contractor.html>`_.
The user is invited to use an appropriate tool to deal with the constraint at stake.

The IBEX contractor behind ``CtcFunction`` is a ``ibex::CtcFwdBwd`` coupled with a ``ibex::Ctc3BCid``.


.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classtubex_1_1_ctc_function.html>`_.