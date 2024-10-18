.. _sec-manual-contractors-dyn:

############################################
Catalog of contractors for dynamical systems
############################################

We now focus on time-related constraints. The following contractors provide a way to apply them on various domains including tubes, intervals and boxes.

.. toctree::

  01-ctc-deriv
  02-ctc-eval
  03-ctc-lohner
  04-ctc-picard
  05-ctc-delay
  06-ctc-linobs
  07-ctc-chain
  
See also: :ref:`codac-unsupported <sec-codac-unsupported>`.

------------------------------------------------------

.. include:: ../04-static-contractors/00-ctc-basics.rst


.. rubric:: Methods common to dynamic contractor objects

.. rst-class:: fit-page

  In the following table, ``ctc`` is an instantiation of a ``Ctc`` class.

  =================================  ======================================================================
  Code                               Meaning
  =================================  ======================================================================
  ``ctc.preserve_slicing()``         Specifies whether the contractor can impact the tube's slicing or not
  ``ctc.restrict_tdomain(tdomain)``  Limits the temporal domain of the contractions
  =================================  ======================================================================