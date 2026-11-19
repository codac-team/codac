.. _sec-intervals-rounding:

Floating-point rounding and library compatibility (C++)
=======================================================

This page concerns the C++ interface of Codac.

Codac relies on `GAOL <https://github.com/goualard-f/GAOL>`_ for interval arithmetic.
To guarantee interval enclosures, the current implementation uses upward rounding (toward :math:`+\infty`).
The floating-point rounding mode also affects computations performed outside Codac in the same thread.

Some libraries assume the usual round-to-nearest mode.
For example, using `OpenCV <https://opencv.org/>`_ alongside Codac may cause unexpected results or runtime errors, including segmentation faults in functions such as ``cv::resize``.
This is a rounding-mode compatibility issue rather than necessarily a memory-management bug.
Its occurrence depends on the platform, build configuration, and operations used.
See also `IBEX issue #402 <https://github.com/ibex-team/ibex-lib/issues/402>`_.

Workaround
----------

Before calling a library that expects round-to-nearest, select this mode using ``gaol::round_nearest()`` or ``std::fesetround(FE_TONEAREST)`` from ``<cfenv>``.
Before resuming Codac interval calculations, restore upward rounding using ``gaol::round_upward()``.

The previous rounding mode can also be saved using ``std::fegetround()`` and restored using ``std::fesetround()``.
If this approach is used, ensure that upward rounding is active before performing further Codac interval calculations.

.. warning::

   Do not perform Codac interval arithmetic while round-to-nearest is active, since the current implementation relies on upward rounding for its guarantees.
   Rounding modes are generally thread-local, so a change in one thread does not automatically configure other threads.

Why Codac does not restore the rounding mode automatically
----------------------------------------------------------

The current implementation uses GAOL in a configuration that assumes upward rounding to avoid repeated changes of the processor's rounding mode.
GAOL also provides a ``preserve-rounding`` build option, but Codac explicitly selects upward rounding in several places.
Enabling this option alone is therefore not a validated compatibility fix for Codac.
