.. _sec-ctc-analytic-ctcinverse:

The CtcInverse contractor
=========================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

Consider a function :math:`\mathbf{f}:\mathbb{R}^n\to \mathbb{R}^p`. The ``CtcInverse`` contractor allows to deal with constraints under the form :math:`\mathbf{f}(\mathbf{x})\in[\mathbf{y}]` by contracting boxes :math:`[\mathbf{x}]\in\mathbb{IR}^n`. 

:math:`\mathbf{f}:\mathbb{R}^n\to \mathbb{R}^p` is the most classical case, but Codac provides generic solutions to define a wide variety of expressions such as functions defined as :math:`\mathbf{f}:X\to Y`, where:

- :math:`X` denotes the domain of :math:`\mathbf{f}` with the following cases:

  * :math:`X=\mathbb{R}`: one scalar input ;
  * :math:`X=\mathbb{R}^n`: one :math:`n`-d vector input ;
  * :math:`X=\mathbb{R}^{n\times m}`: one :math:`n\times m` matrix input ;
  * :math:`X=\mathbb{R}\times\mathbb{R}^{n}\times\mathbb{R}`: several types of mixed input, with no restrictions on order or number.

- :math:`Y` denotes the codomain of :math:`\mathbf{f}`, that can be either :math:`\mathbb{R}`, :math:`\mathbb{R}^p` or :math:`\mathbb{R}^{p\times q}`.

.. role:: bg-ok
.. role:: bg-bok
.. role:: bg-noa
.. role:: bg-title
.. role:: vertical
.. role:: medskip

.. role:: raw-html(raw)
  :format: html

.. |okk| replace:: :bg-ok:`✓`
.. |bok| replace:: :bg-bok:`✓`
.. |noa| replace:: :bg-noa:`✗ (C++ only)`
.. |ms| replace:: :medskip:`-`


The following table lists the combinations implemented in Codac.
The :bg-ok:`✓` cases correspond to efficient contractions involving centered form expressions, while :bg-bok:`✓` refer to classical forward/backward propagations.
Note that since Codac's design is based on template programming, the most generic use-cases of the ``CtcInverse`` are not available in Python or Matlab. 

.. tabs::

  .. group-tab:: Python

    .. table:: Possible usages of ``CtcInverse``
      :name: ctcinverse-table-py

      +-----------------------------------------------------+-------------------------------------------------------------------------------------------+
      |                                                     | Output :math:`Y`                                                                          |
      |                                                     +-------------------------+---------------------------+-------------------------------------+
      |                                                     | :math:`\mathbb{R}` |ms| | :math:`\mathbb{R}^n` |ms| | :math:`\mathbb{R}^{r\times c}` |ms| |
      +--------------------+--------------------------------+-------------------------+---------------------------+-------------------------------------+
      | Input(s) :math:`X` | :math:`\mathbb{R}`             | |noa|                   | |noa|                     | |noa|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | :math:`\mathbb{R}^n`           | |okk|                   | |okk|                     | |noa|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | :math:`\mathbb{R}^{r\times c}` | |noa|                   | |noa|                     | |noa|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | any mixed types                | |noa|                   | |noa|                     | |noa|                               |
      +--------------------+--------------------------------+-------------------------+---------------------------+-------------------------------------+

  .. group-tab:: C++

    .. table:: Possible usages of ``CtcInverse``
      :name: ctcinverse-table-cpp

      +-----------------------------------------------------+-------------------------------------------------------------------------------------------+
      |                                                     | Output :math:`Y`                                                                          |
      |                                                     +-------------------------+---------------------------+-------------------------------------+
      |                                                     | :math:`\mathbb{R}` |ms| | :math:`\mathbb{R}^n` |ms| | :math:`\mathbb{R}^{r\times c}` |ms| |
      +--------------------+--------------------------------+-------------------------+---------------------------+-------------------------------------+
      | Input(s) :math:`X` | :math:`\mathbb{R}`             | |bok|                   | |bok|                     | |bok|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | :math:`\mathbb{R}^n`           | |okk|                   | |okk|                     | |bok|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | :math:`\mathbb{R}^{r\times c}` | |bok|                   | |bok|                     | |bok|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | any mixed types                | |bok|                   | |bok|                     | |bok|                               |
      +--------------------+--------------------------------+-------------------------+---------------------------+-------------------------------------+

  .. group-tab:: Matlab

    .. table:: Possible usages of ``CtcInverse``
      :name: ctcinverse-table-matlab

      +-----------------------------------------------------+-------------------------------------------------------------------------------------------+
      |                                                     | Output :math:`Y`                                                                          |
      |                                                     +-------------------------+---------------------------+-------------------------------------+
      |                                                     | :math:`\mathbb{R}` |ms| | :math:`\mathbb{R}^n` |ms| | :math:`\mathbb{R}^{r\times c}` |ms| |
      +--------------------+--------------------------------+-------------------------+---------------------------+-------------------------------------+
      | Input(s) :math:`X` | :math:`\mathbb{R}`             | |noa|                   | |noa|                     | |noa|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | :math:`\mathbb{R}^n`           | |okk|                   | |okk|                     | |noa|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | :math:`\mathbb{R}^{r\times c}` | |noa|                   | |noa|                     | |noa|                               |
      |                    +--------------------------------+-------------------------+---------------------------+-------------------------------------+
      |                    | any mixed types                | |noa|                   | |noa|                     | |noa|                               |
      +--------------------+--------------------------------+-------------------------+---------------------------+-------------------------------------+