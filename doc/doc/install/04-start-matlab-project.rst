.. _sec-start-matlab-project:

######################
Start a MATLAB project
######################

.. tip::
   | You are using Python?
   | :ref:`sec-start-py-project`

.. warning::

  | MATLAB support is preliminary, limited tests have been made for now. See `this example <https://github.com/codac-team/codac/blob/master/examples/tuto/01_getting_started/a01_getting_started.m>`_.

| Codac is ready to be used if you have at least MATLAB R2019b, `a supported version of Python <https://fr.mathworks.com/help/matlab/matlab_external/install-supported-python-implementation.html>`_ and :ref:`installed Codac Python package <sec-installation-py>`.

.. admonition:: MATLAB specificities

   | Integers in function calls from the Codac library should be cast to `int32`, e.g. `4` would be `int32(4)` (however do not change those that are in strings), otherwise remember that all numbers are `double` by default in MATLAB. 
   | The operator `[index]` for a Codac object should be replaced with `.getitem(int32(index))` when getting its value while it should be replaced with `.setitem(int32(index))` when setting its value (please note that indices of Codac objects start at `0` contrary to typical MATLAB objects such as MATLAB arrays or cell arrays). 
   | Python lists of objects should be converted to MATLAB cell arrays. 
   | Also, when a Codac function needs a Python list parameter, the corresponding MATLAB cell array should be given as `py.list(...)`. 

.. code-block:: matlab
  
  import py.codac.*

  x = Tube(Interval(0,10), 0.01, TFunction("cos(t)+abs(t-5)*[-0.1,0.1]"));

  beginDrawing();
  fig = VIBesFigTube("My first tube");
  fig.add_tube(x, "x");
  fig.show();
  endDrawing();

| This script will create a simple tube and display it.

In order to visualize the tube, you need to launch before the :ref:`VIBes viewer <sec-installation-graphics>` independently.

If everything is well installed on your computer, you should see the following window appear:

.. Figure:: img/helloworld.png
