.. rubric:: How a contractor is implemented

Contractors are C++/Python objects. Some of them can be instantiated according to the problem to deal with. All of them will contract domains with the ``.contract()`` method.

.. rubric:: Pre-defined contractors

For ease of use, some contractor objects that do not require parameters are predefined in the namespace ``ctc``.
They can be used directly without instantiation.

====================================  =======  ==============  ====================================
Contractor                            Type     Code            Documentation
====================================  =======  ==============  ====================================
:math:`\mathcal{C}_{\textrm{polar}}`  Static   ``ctc::polar``  :ref:`Link <sec-manual-ctcpolar>`
:math:`\mathcal{C}_{\textrm{dist}}`   Static   ``ctc::dist``   :ref:`Link <sec-manual-ctcdistance>`
:math:`\mathcal{C}_{\frac{d}{dt}}`    Dynam.   ``ctc::deriv``  :ref:`Link <sec-manual-ctcderiv>`
:math:`\mathcal{C}_{\textrm{eval}}`   Dynam.   ``ctc::eval``   :ref:`Link <sec-manual-ctceval>`
:math:`\mathcal{C}_{\textrm{delay}}`  Dynam.   ``ctc::delay``  --
====================================  =======  ==============  ====================================