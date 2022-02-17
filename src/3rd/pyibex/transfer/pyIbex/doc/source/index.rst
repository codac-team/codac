.. pyIbex documentation master file, created by
   sphinx-quickstart on Mon Apr 11 10:42:13 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to pyIbex's documentation!
==================================

Introduction
-------------

PyIbex is a python module providing an easy-to-use interface of `Ibex-lib`_
with high level functionalities such as Interval Arithmetics, Contractors programming
and Sets manipulation.

The core part of algorithm is generally written in C++ and binded with python using `pybind11`_

.. _`Ibex-lib`: http://www.ibex-lib.org
.. _`pybind11`: https://github.com/wjakob/pybind11

Modules descriptions
--------------------

*PyIbex* is composed on several sub-modules:

- `core <api.html>`_ ( mainly direct binding of Ibex-lib)
- `geometry <https://github.com/benensta/ibex-geometry/>`_
- `tubes <https://github.com/SimonRohou/tube-lib/>`_
- thickset (under developpement)




Contents:
----------

.. toctree::
   :maxdepth: 2

   quickstart
   api
   installation


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
