.. _sec-tuto-cprob-lesson-c:

Lesson C: Dynamic localization
==============================

We now propose to extend the previous lesson to the dynamic case, *i.e.* with a mobile robot evolving in the middle of a field of indistinguishable landmarks.

Formalism
---------

The state is now with a fourth dimension for depicting the velocity of the robot. We are now addressing the following state estimation problem, with :math:`\mathbf{x}\in\mathbb{R}^4`: 

.. math:: 

  \left\{ \begin{array}{lll}
  \dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),\mathbf{u}(t)\big) &  & \textrm{(evolution equation)}\\
  \mathbf{y}_i=\mathbf{g}\big(\mathbf{x}(t_i),\mathbf{m}_i\big) &  & \textrm{(observation equation)}\\
  \mathbf{m}_i\in\mathbb{M} &  & \textrm{(association constraint)}
  \end{array}\right.

The asynchronous measurements :math:`\mathbf{y}_i\in\mathbb{R}^2` are obtained for each time instants :math:`t_i\in[t_0,t_f]`.
In this lesson, because the times :math:`t_i` are supposed to be exactly known, these constraints will be dealt with as *restrictions* on tubes. Note that in case of time uncertainties, :math:`t_i\in[t_i]`, we should have used a specific contractor for reliability purposes.

The evolution function :math:`\mathbf{f}` depicts the evolution of the state with respect to some input vector :math:`\mathbf{u}`:

.. math::

  \dot{\mathbf{x}}=\mathbf{f}(\mathbf{x},\mathbf{u})=\left( \begin{array}{c}
    x_4\cos(x_3) \\
    x_4\sin(x_3) \\
    u_1 \\
    u_2
  \end{array}\right),

where :math:`(x_1,x_2)^\intercal` is the 2d position of the robot, :math:`x_3` its heading and :math:`x_4` its velocity.

The input :math:`\mathbf{u}(\cdot)` is unknown, but we assume that we have continuous measurements for the headings :math:`x_3(\cdot)` and the speeds :math:`x_4(\cdot)`, with some bounded uncertainties defined by intervals :math:`[e_3]=[-0.02,0.02]`, :math:`[e_4]=[-0.02,0.02]`.

Lastly, we do not have any knowledge about the initial position of the robot.


Decomposition
-------------

.. admonition:: Exercise

  **C.1.** In the first lesson, we wrote a decomposition of the static range and bearing problem.
  We extend this decomposition to deal with the dynamical case. This will make appear new intermediate variables.
  We propose the following decomposition:

  - external trajectory variables: :math:`\mathbf{x}(\cdot)\in[t_0,t_f]\to\mathbb{R}^4`

  - intermediate trajectory variables: :math:`\mathbf{v}(\cdot)\in[t_0,t_f]\to\mathbb{R}^4`

  - external static variables (for each observation): :math:`\mathbf{y}_i\in\mathbb{R}^2`, :math:`\mathbf{m}_i\in\mathbb{R}^2`

  - intermediate static variables (for each observation): :math:`a_i\in\mathbb{R}`, :math:`\mathbf{d}_i\in\mathbb{R}^2`

  - constraints:

  .. math::

    \left\{ \begin{array}{rl}
    (i) & \mathcal{L}_\textrm{polar}\big(d_1,d_2,y_{i,1},a_i\big):~~~\mathbf{d}_i=y_{i,1}\cdot\left(\hspace{-0.1cm}\begin{array}{c}
    ~\cos(a_i)~\\
    ~\sin(a_i)~
    \end{array}\hspace{-0.1cm}\right)\\
    (ii) & d_{i,1} = m_{i,1}-x_1(t_i) \\
    (iii) & d_{i,2} = m_{i,2}-x_2(t_i) \\
    (iv) & a_i=x_3(t_i)+y_{i,2} \\
    \textcolor{blue}{(v)} & \textcolor{blue}{\mathbf{v}(\cdot)=\mathbf{f}\left(\mathbf{x}(\cdot)\right)} \\
    \textcolor{blue}{(vi)} & \textcolor{blue}{\dot{\mathbf{x}}(\cdot)=\mathbf{v}(\cdot)}
    \end{array}\right.

  | The blue constraints are related to the dynamical case.
  | The input :math:`\mathbf{u}(\cdot)` is not involved in this decomposition, as the velocity and speed are measured.


Initialization
--------------

Before going into a set-membership state estimation, we set up a simulation environment to test different random situations.

.. admonition:: Exercise

  *(reuse your code from the previous lesson)*

  **C.2.** Using the following code, generate a set of randomly positionned landmarks:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_C.py
        :language: py
        :start-after: [C-q2-beg]
        :end-before: [C-q2-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_C.cpp
        :language: c++
        :start-after: [C-q2-beg]
        :end-before: [C-q2-end]
        :dedent: 2

  **C.3.** Now, we simulate a mobile robot moving in the middle of the field of landmarks. The trajectory is defined by a set of random waypoints. The class ``RobotSimulator`` is used to simplify the simulation:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_C.py
        :language: py
        :start-after: [C-q3-beg]
        :end-before: [C-q3-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_C.cpp
        :language: c++
        :start-after: [C-q3-beg]
        :end-before: [C-q3-end]
        :dedent: 2

  **C.4.** We update the observation function ``g`` of the previous lessons for the dynamical case. To ease the code, the returned observation vector will contain the observation time :math:`t_i`.

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_C.py
        :language: py
        :start-after: [C-q4-beg]
        :end-before: [C-q4-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_C.cpp
        :language: c++
        :start-after: [C-q4-beg]
        :end-before: [C-q4-end]
        :dedent: 0

  **C.5.** Finally, we run the simulation for generating and drawing the observation vectors. ``obs`` contains all the observation vectors.

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_C.py
        :language: py
        :start-after: [C-q5-beg]
        :end-before: [C-q5-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_C.cpp
        :language: c++
        :start-after: [C-q5-beg]
        :end-before: [C-q5-end]
        :dedent: 2


State estimation with constraint programming
--------------------------------------------

.. admonition:: Exercise

  **C.6.** **Domains.** We first create the tube :math:`[\mathbf{x}](\cdot)` that will contain the set of feasible state trajectories. In Codac, a tube is built as a list of slices, the temporal sampling of which is defined as a *temporal domain* (``TDomain``) that must be created beforehand. Several tubes can stand on a same ``TDomain``. Then, the tube ``x`` is created according to the problem: we do not know its first two components for the positions, but the headings and velocities are known with some bounded errors. We define a function ``h`` for this purpose of initialization.

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_C.py
        :language: py
        :start-after: [C-q6-beg]
        :end-before: [C-q6-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_C.cpp
        :language: c++
        :start-after: [C-q6-beg]
        :end-before: [C-q6-end]
        :dedent: 2

  In the above code, the intermediate variable :math:`\mathbf{v}(\cdot)` has also its own domain ``v``. It is initialized to :math:`[-\infty,\infty]` for each dimension.


  **C.7.** **Contractors.** We re-use the contractors of the previous lesson: :math:`\mathcal{C}_+`, :math:`\mathcal{C}_-`, :math:`\mathcal{C}_{\mathrm{polar}}`, :math:`\mathcal{C}_{\mathbb{M}}`. It remains to create the contractors related to the tubes (blue constraints :math:`(v)` and :math:`(vi)`). One of them is already available in the catalog of contractors of the library: :math:`\mathcal{C}_{\mathrm{deriv}}\left([\mathbf{x}](\cdot),[\mathbf{v}](\cdot)\right)` for the constraint :math:`\dot{\mathbf{x}}(\cdot)=\mathbf{v}(\cdot)`. The other :math:`\mathcal{C}_\mathbf{f}` contractor can be defined as previously, according to the evolution function.

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_C.py
          :language: py
          :start-after: [C-q7-beg]
          :end-before: [C-q7-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_C.cpp
          :language: c++
          :start-after: [C-q7-beg]
          :end-before: [C-q7-end]
          :dedent: 2

  **C.8.** **Fixpoint resolution.** Finally, the propagation loops need to be updated to incorporate the dynamic constraints.
  Note that the contractors :math:`\mathcal{C}_\mathbf{f}` and :math:`\mathcal{C}_{\mathrm{deriv}}` apply to the whole tubes :math:`[\mathbf{x}](\cdot)` and :math:`[\mathbf{v}](\cdot)`. Furthermore, the class ``CtcInverse`` can contract tubes using the ``.contract_tube(..)`` method, exactly as we would do for boxes. Finally, a *restriction* on a tube (*i.e.* setting a value to a slice) can be done using the ``.set(y,t)`` method, for setting the interval vector value ``y`` at time ``t``.

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_C.py
          :language: py
          :start-after: [C-q8-beg]
          :end-before: [C-q8-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_C.cpp
          :language: c++
          :start-after: [C-q8-beg]
          :end-before: [C-q8-end]
          :dedent: 2




You should obtain a result similar to:

.. figure:: img/datasso_solved.png
  
  Localization by solving data association: the state trajectory :math:`\mathbf{x}(\cdot)` (in white) has been estimated (in blue) together with the identification of the perceived landmarks.


.. rubric:: Why is this problem of localization and data association difficult?

#. We do not know the **initial condition** of the system. Contrary to other approaches, this solver made of contractors does not require some initial vector :math:`\mathbf{x}_0` to start the estimation. Information is taken into account from anytime in :math:`[t_0,t_f]`.

#. The constraints are heterogeneous: some of them are said **continuous** (they act on continuous domains of values, for instance intervals). Other are **discrete** (for instance, the identity of landmarks, estimated among a discrete set of :math:`n` possible values). And finally, some constraints come from **differential equations** (for instance for depicting the robot evolution). In this solver, we show that any kind of constraint can be combined, without going into a complex resolution algorithm.

#. We do **not have to linearize**, and thus there is no approximation made here. This means that the equations are directly set in the solver, without transformation. Furthermore, the results are **reliable**: we can guarantee that the actual trajectory is inside the tube :math:`[\mathbf{x}](\cdot)`.