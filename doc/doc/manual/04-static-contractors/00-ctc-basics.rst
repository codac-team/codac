.. rubric:: How a contractor is implemented

Contractors are C++/Python objects. Some of them can be instantiated according to the problem to deal with. All of them will contract domains with the ``.contract()`` method.

.. rubric:: Pre-defined contractors

.. rst-class:: fit-page

  For ease of use, some contractor objects that do not require parameters are predefined in the namespace ``ctc``.
  They can be used directly without instantiation.

  ====================================  =======  ======================================================  =================================
  Contractor                            Type     Code                                                    Doc
  ====================================  =======  ======================================================  =================================
  :math:`\mathcal{C}_{\textrm{polar}}`  Static   | ``ctc::polar`` for C++                                :ref:`Link <sec-manual-ctcpolar>`
                                                 | ``ctc.polar`` for Python                                                              
  :math:`\mathcal{C}_{\textrm{dist}}`   Static   | ``ctc::dist``  for C++                                :ref:`Link <sec-manual-ctcdist>`
                                                 | ``ctc.dist`` for Python                                                              
  :math:`\mathcal{C}_{\frac{d}{dt}}`    Dynam.   | ``ctc::deriv`` for C++                                :ref:`Link <sec-manual-ctcderiv>`
                                                 | ``ctc.deriv`` for Python                                                              
  :math:`\mathcal{C}_{\textrm{eval}}`   Dynam.   | ``ctc::eval``  for C++                                :ref:`Link <sec-manual-ctceval>`
                                                 | ``ctc.eval`` for Python                                                              
  :math:`\mathcal{C}_{\textrm{delay}}`  Dynam.   | ``ctc::delay`` for C++                                --
                                                 | ``ctc.delay`` for Python                                                               
  ====================================  =======  ======================================================  =================================