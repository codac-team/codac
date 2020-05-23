.. _sec-start-py-project:

######################
Start a Python project
######################

.. tip::
   | You are using C++?
   | :ref:`sec-start-cpp-project`

| Tubex is ready to be used on your computer.
| You can now import the ``pytubex`` package and start using it:

.. code-block:: py

  from pytubex import *

  tdomain = Interval(0,10)
  x = Tube(tdomain);

  print("My first tube: ", x)