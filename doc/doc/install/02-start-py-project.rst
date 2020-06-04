.. _sec-start-py-project:

######################
Start a Python project
######################

.. tip::
   | You are using C++?
   | :ref:`sec-start-cpp-project`

| Tubex is ready to be used on your computer.
| You can now import the ``tubex_lib`` package and start using it:

.. code-block:: py
  
  from pyibex import *
  from tubex_lib import *

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

In order to visualize the tube, you need to launch the VIBes viewer independently. On Linux, you can for instance execute:

.. code-block:: bash

  VIBes-viewer

If everything is well installed on your computer, you should see the following window appear:

.. Figure:: img/helloworld.png