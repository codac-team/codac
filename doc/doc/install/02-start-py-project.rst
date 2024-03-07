.. _sec-start-py-project:

######################
Start a Python project
######################

.. tip::
   | You are using C++?
   | :ref:`sec-start-cpp-project`

| Assuming :ref:`Codac Python package is installed <sec-installation-py>`, you can import the ``codac`` package and start using it:

.. code-block:: py
  
  from codac import *

  x = Tube(Interval(0,10), 0.01, TFunction("cos(t)+abs(t-5)*[-0.1,0.1]"))

  beginDrawing()
  fig = VIBesFigTube("My first tube")
  fig.add_tube(x, "x")
  fig.show()
  endDrawing()

| This script will create a simple tube and display it.
| You can run it (if saved in a file :file:`myscript.py`) with:

.. code-block:: bash

  python3 myscript.py

In order to visualize the tube, you need to launch before the :ref:`VIBes viewer <sec-installation-graphics>` independently.

If everything is well installed on your computer, you should see the following window appear:

.. Figure:: img/helloworld.png