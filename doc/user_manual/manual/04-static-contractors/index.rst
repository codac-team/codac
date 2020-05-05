.. _sec-manual-contractors:

#############################################
Catalog of contractors for static constraints
#############################################

The **domains** previously presented will be reduced by means of **contractors**: operators designed to eliminate unfeasible solutions according to **constraints**.
We distinct two categories of contractors: those that are time-related (and that involve tubes) and the static ones presented in this chapter.

Therefore, the following contractors apply on intervals and boxes:

.. toctree::

  01-ctc-function
  02-ctc-dist
  03-ctc-polar
  04-ctc-constell
  05-ctc-ibex

------------------------------------------------------

.. rubric:: How a contractor is implemented

Contractors are C++/Python objects. Some of them can be instantiated according to the problem to deal with. All of them will contract domains with the ``.contract()`` method.