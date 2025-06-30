.. _sec-tuto-cprob-lesson-b:

Lesson B: Data association
==========================

This lesson will extend the previous exercise by dealing with a **data association** problem together with localization: the landmarks perceived by the robot are now **indistinguishable**: they all look alike. The goal of this exercise is to develop our own contractor in order to solve the problem.


Introduction: indistinguishable landmarks
-----------------------------------------

As before, our goal is to deal with the localization of a mobile robot for which a map of the environment is available.

It is assumed that:

* the map is static and made of point landmarks;
* **the landmarks are indistinguishable** (which was not the case before);
* the position of each landmark is known;
* the position of the robot is not known.

This problem often occurs when using sonars for localization, where it is difficult to distinguish a landmark from another. The challenge in this type of scenario is the difficulty of **data associations**, *i.e.* establishing feature correspondences between perceptions of landmarks and the map of these landmarks, known beforehand. This is especially the case of challenging underwater missions, for which one can reasonably assume that the detected landmarks cannot be distinguished from the other in sonar images. For instance, two different rocks may have the same aspect and dimension in the acquired images, as illustrated by the following figure.

.. figure:: img/indistinguishable.png
  :width: 500px

  Different underwater rocks perceived with one side-scan sonar. These observations are view-point dependent and the sonar images are noisy, which makes it difficult to distinguish one rock from another only from image processing. These sonar images have been collected by the *Daurade* robot (DGA-TN Brest, Shom, France) equipped with a Klein 5000 lateral sonar.

Due to these difficulties, we consider that we cannot compute reliable data associations that stand on image processing of the observations. We will rather consider the data association problem **together with state estimation**.


.. _sec-tuto-04-formalism:

Data association: formalism
---------------------------

Up to now, we assumed that one perceived landmark was identified without ambiguity.
However, when several landmarks :math:`\mathbf{m}_{1}`, :math:`\dots`, :math:`\mathbf{m}_{\ell}` exist, the observation data may not be *associated*: we do not know to which landmark the measurement :math:`\mathbf{y}` refers. We now consider several measurements denoted by :math:`\mathbf{y}^i`. Hence the observation constraint has the form:

.. math:: 

  \left\{ \begin{array}{l}
  \mathbf{g}\big(\mathbf{x},\mathbf{y}^{i},\mathbf{m}^{i}\big)=\mathbf{0},\\
  \big(\mathbf{m}^{i}\in[\mathbf{m}_{1}]\big)\vee\dots\vee\big(\mathbf{m}^{i}\in[\mathbf{m}_{\ell}]\big),
  \end{array}\right.

with :math:`\mathbf{m}^{i}` the **unidentified landmark** of the observation :math:`\mathbf{y}^i`. The :math:`\vee` notation expresses the logical disjunction *or*.
Equivalently, the system can be expressed as:

.. math:: 

  \left\{ \begin{array}{l}
  \mathbf{g}\big(\mathbf{x},\mathbf{y}^{i},\mathbf{m}^{i}\big)=\mathbf{0},\\
  \mathbf{m}^{i}\in\mathbb{M}=\left\{ [\mathbf{m}_{1}],\dots,[\mathbf{m}_{\ell}]\right\},
  \end{array}\right.

where :math:`\mathbb{M}` is the bounded map of the environment: the set of all landmarks represented by their bounded positions.
In other words, we do not know the right parameter vector :math:`\mathbf{m}_{1}`, :math:`\dots`, :math:`\mathbf{m}_{\ell}` associated with the observation :math:`\mathbf{y}^i`. This is a problem of data association.
The following figure illustrates :math:`\mathbb{M}` with the difficulty to differentiate landmarks in underwater extents.

.. figure:: img/rus_rochers_map.jpg
  :width: 500px
  
  The yellow robot, equipped with a side-scan sonar, perceives at port side some rocks :math:`\mathbf{m}^{i}` lying on the seabed. The rocks, that can be used as landmarks, are assumed to belong to small georeferenced boxes :math:`\mathbb{M}` enclosing uncertainties on their positions. The robot is currently not able to make any difference between the rocks, as it is typically the case in underwater extents when acoustic sensors are used to detect features.

In this exercise, the data association problem is solved together with state estimation, without image processing. The constraint :math:`\mathbf{g}\big(\mathbf{x},\mathbf{y}^{i},\mathbf{m}^{i}\big)=\mathbf{0}` has been solved in :ref:`sec-tuto-cprob-lesson-a`, it remains to deal with the constraint :math:`\mathbf{m}^{i}\in\mathbb{M}`. We will call it the *constellation constraint*.


The constellation constraint and its contractor
-----------------------------------------------

When solving a problem with a constraint propagation approach, we may not have the contractors at hand for dealing with all the involved constraints. In this lesson for instance, we assume that we do not have a contractor for dealing with this constellation constraint. The goal of the following exercise is to see how to build our own contractor. We will see that it is easy to extend our framework with new contractors, without loss of generality.

**We now focus on the constellation constraint without relation with the other equations.** The constraint is expressed by :math:`\mathcal{L}_{\mathbb{M}}\left(\mathbf{a}\right)~:~\mathbf{a}\in\mathbb{M}`.

Let us consider a constellation of :math:`\ell` landmarks of :math:`\mathbb{IR}^{2}`, :math:`\mathbb{M}=\{[\mathbf{m}_{1}],\dots,[\mathbf{m}_{\ell}]\}`, and a box :math:`[\mathbf{a}]\in\mathbb{IR}^2`. Our goal is to compute the smallest box containing :math:`\mathbb{M}\cap[\mathbf{a}]`. In other words, we want to contract the box :math:`[\mathbf{a}]` so that we only keep the best envelope of the landmarks that are already included in :math:`[\mathbf{a}]`. An illustration is provided by the following figures:

.. figure:: img/rus_rochers_bc.jpg
  :width: 500px

.. figure:: img/rus_rochers_ac.jpg
  :width: 500px

  Illustration of the constellation contractor, before and after the contraction step. The set :math:`\mathbb{M}` of landmarks is depicted by white boxes. Colored boxes depict several cases of :math:`[\mathbf{a}]`, before and after contraction. In this example, the red perception leads to a reliable identification since the box contains only one item of :math:`\mathbb{M}`. We recall that the sonar image in background is not involved in this contraction: it is only used for understanding the application of this contractor. Here, the operator is reliable as it does not remove any significant rock. 

The definition of the contractor is given by:

.. math::

  \mathcal{C}_\mathbb{M}\left(\left[\mathbf{a}\right]\right)=\bigsqcup_{j=1}^{\ell}\big(\left[\mathbf{a}\right]\cap[\mathbf{m}_{j}]\big),

where :math:`\bigsqcup`, called *squared union*, returns the smallest box enclosing the union of its arguments.


.. admonition:: Exercise
  
  *(you may insert your code in the previous Lesson A)*

  **B.1.** We can now build our own contractor class, ``MyCtc``. To do it, we have to derive the base class ``Ctc_IntervalVector`` (in Python/Matlab) or ``Ctc<MyCtc,IntervalVector>`` (in C++) that is common to all contractors on boxes. You can start from the following new class:

  .. tabs::

    .. code-tab:: py

      class MyCtc(Ctc_IntervalVector):
      
        def __init__(self, M_: list):
          Ctc_IntervalVector.__init__(self, 2) # the contractor acts on 2d boxes
          self.M = M_                          # attribute needed later on for the contraction
      
        def contract(self, a):

          # Insert contraction formula here (question B.2)

          return a # in Python, the updated value must be returned

    .. code-tab:: cpp

      class MyCtc : public Ctc<MyCtc,IntervalVector>
      {
        public:

          MyCtc(const std::vector<IntervalVector>& M)
            : Ctc<MyCtc,IntervalVector>(2), // the contractor acts on 2d boxes
            _M(M) // attribute needed later on for the contraction
          {

          }

          void contract(IntervalVector& a) const
          {
            // Insert contraction formula here (question B.2)
          }

        protected:

          const std::vector<IntervalVector> _M;
      };

  | Note that:

  * ``M`` represents the set :math:`\mathbb{M}`, made of 2d ``IntervalVector`` objects;
  * ``a`` (in ``.contract()``) is the box :math:`[\mathbf{a}]` (2d ``IntervalVector``) to be contracted.

  **B.2.** Propose a simple implementation of the method ``.contract()`` for contracting :math:`[\mathbf{a}]` as:

  .. math::

    \mathcal{C}_\mathbb{M}\left(\left[\mathbf{a}\right]\right)=\bigsqcup_{j=1}^{\ell}\big(\left[\mathbf{a}\right]\cap[\mathbf{m}_{j}]\big)

  The user manual may help you to :ref:`compute operations on sets <sec-manual-intervals-operations>` such as unions or intersections, appearing in the formula. Note that you cannot change the definition of the ``.contract()`` method. 

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_B.py
          :language: py
          :start-after: [B-q2-beg]
          :end-before: [B-q2-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_B.cpp
          :language: c++
          :start-after: [B-q2-beg]
          :end-before: [B-q2-end]
          :dedent: 2

  **B.3.** Test your contractor with:
  
  * a set :math:`\mathbb{M}=\big\{(1.5,2.5)^\intercal,(3,1)^\intercal,(2,2)^\intercal,(2.5,3)^\intercal,(3.5,2)^\intercal,(4,1)^\intercal,(1.5,0.5)^\intercal\big\}`, all boxes inflated by :math:`[-0.05,0.05]`;
  
  * three boxes to be contracted: :math:`[\mathbf{a}^1]=([1.25,3],[1.6,2.75])^\intercal`, :math:`[\mathbf{a}^2]=([2,3.5],[0.6,1.2])^\intercal`, and :math:`[\mathbf{a}^3]=([1.1,3.25],[0.2,1.4])^\intercal`.

  You can try your contractor with the following code:

  .. tabs::

    .. code-tab:: py

      # M = [ ...
      for Mi in M:
        Mi.inflate(0.05)

      # a1 = IntervalVector([ ...
      # a2 = IntervalVector([ ...
      # a3 = IntervalVector([ ...

      ctc_constell = MyCtc(M)

      a1 = ctc_constell.contract(a1)
      a2 = ctc_constell.contract(a2)
      a3 = ctc_constell.contract(a3)

    .. code-tab:: cpp

      std::vector M = {
        IntervalVector({{...},{...}});
      };

      for(auto& Mi : M)
        Mi.inflate(0.05);

      IntervalVector a1({{...
      IntervalVector a2({{...
      IntervalVector a3({{...

      MyCtc ctc_constell(M);

      ctc_constell.contract(a1);
      ctc_constell.contract(a2);
      ctc_constell.contract(a3);

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_B.py
          :language: py
          :start-after: [B-q3-beg]
          :end-before: [B-q3-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_B.cpp
          :language: c++
          :start-after: [B-q3-beg]
          :end-before: [B-q3-end]
          :dedent: 2

.. figure:: img/CtcConstell_constell.png
  :width: 600px

  Expected result for Question **B.3** (you may verify your results only by printing boxes values). Non-filled boxes are initial domains before contraction. Filled boxes are contracted domains. The green one, :math:`[\mathbf{a}^3]`, is *identified* since it contains only one item of the constellation :math:`\mathbb{M}`: there is no ambiguity about the constellation point represented by the box :math:`[\mathbf{a}^3]`.

  Numerical results are given:

  .. code::

    [a1]: ([1.45, 2.05] ; [1.95, 2.55])    (blue box)
    [a2]: ([2.95, 3.05] ; [0.95, 1.05])    (green box)
    [a3]: ([1.45, 3.05] ; [0.45, 1.05])    (red box)


Application for localization
----------------------------

| This contractor is really simple, but it will help us to solve more complex problems.
| We now have all the material to solve the full problem of state estimation with data association.

We will localize the robot in the map :math:`\mathbb{M}` created in Question **B.3**.

.. admonition:: Exercise

  **B.4.** In your code, define a robot with the following pose :math:`\mathbf{x}=\left(2,1,\pi/6\right)^\intercal` (the same as in Lesson A).

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_B.py
          :language: py
          :start-after: [B-q4-beg]
          :end-before: [B-q4-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_B.cpp
          :language: c++
          :start-after: [B-q4-beg]
          :end-before: [B-q4-end]
          :dedent: 2

  **B.5.** Display the robot and the landmarks in a graphical view with:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_B.py
        :language: py
        :start-after: [B-q5-beg]
        :end-before: [B-q5-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_B.cpp
        :language: c++
        :start-after: [B-q5-beg]
        :end-before: [B-q5-end]
        :dedent: 2

  You should obtain this result.

  .. figure:: img/loc_robot_landmarks.png
    :width: 300px

  **B.6.** We will generate a set of range-and-bearing measurements made by the robot. For this, we will use an observation function to compute the range and bearing between a state ``x`` and a landmark ``mi``:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_B.py
        :language: py
        :start-after: [B-q6-beg]
        :end-before: [B-q6-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_B.cpp
        :language: c++
        :start-after: [B-q6-beg]
        :end-before: [B-q6-end]
        :dedent: 0

  The returned value is a vector of [range]×[bearing] interval values.

  Compute and display the measurements in the view with ``.draw_pie()``, as we did in the previous lesson.
  The *identity* (*i.e.* the position) of each landmark will be given in the last components of the observation vectors:  

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_B.py
        :language: py
        :start-after: [B-q7-beg]
        :end-before: [B-q7-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_B.cpp
        :language: c++
        :start-after: [B-q7-beg]
        :end-before: [B-q7-end]
        :dedent: 2

  You should obtain a figure similar to this one:

  .. figure:: img/loc_robot_landmarks_obs.png
    :width: 300px


State estimation without data association
-----------------------------------------

We will go step by step and not consider the data association problem for the moment.

We will first reuse the fixpoint method of the previous Lesson, and apply it on this set of observations.

.. admonition:: Exercise

  **D.7.** Use a second fixpoint method to perform the state estimation of the robot (the contraction of :math:`[\mathbf{x}]`) by considering iteratively all the observations.

  We will assume that:

  * the identity (the position) of the related landmarks is known;
  * the heading :math:`x_3` of the robot is known.

  In the code, the *identity* of each landmark is known in the sense that a measurement ``v_obs[i]`` refers to a landmark ``M[i]``.

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_B.py
          :language: py
          :start-after: [B-q7b-beg]
          :end-before: [B-q7b-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_B.cpp
          :language: c++
          :start-after: [B-q7b-beg]
          :end-before: [B-q7b-end]
          :dedent: 2

  **B.8.** Display the resulted contracted box :math:`[\mathbf{x}]` with:

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_B.py
          :language: py
          :start-after: [B-q8-beg]
          :end-before: [B-q8-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_B.cpp
          :language: c++
          :start-after: [B-q8-beg]
          :end-before: [B-q8-end]
          :dedent: 2

  You should obtain this result in black (considering uncertainties proposed in Question **B.6**):

  .. figure:: img/loc_robot_landmarks_obs_box.png
    :width: 300px

    Range-and-bearing static localization, with several observations of identified landmarks.


State estimation with data association
--------------------------------------

We now assume that the identities of the landmarks are not known. This means that we will not use the third and fourth components of the observation vectors ``yi`` anymore in the resolution of the problem.

.. admonition:: Exercise

  **B.9.** Update the fixpoint functions for solving the state estimation together with the data association problem. The algorithm should now involve the ``mi`` objects as well as the new contractor constellation contractor that you developed in **Questions B1--3**. 

  You should obtain exactly the same result as in Question **B.8**.

  .. container:: toggle, toggle-hidden

    .. tabs::

      .. group-tab:: Python

        .. literalinclude:: src/lesson_B.py
          :language: py
          :start-after: [B-q9-beg]
          :end-before: [B-q9-end]
          :dedent: 0

      .. group-tab:: C++

        .. literalinclude:: src/lesson_B.cpp
          :language: c++
          :start-after: [B-q9-beg]
          :end-before: [B-q9-end]
          :dedent: 2


  **B.11.** *Same result* means that the data association worked: each measurement has been automatically associated to the correct landmark without ambiguity.

  We can now look at the set ``m`` containing the associations. If one :math:`[\mathbf{m}^i]` contains only one item of :math:`\mathbb{M}`, then it means that the landmark of the measurement :math:`\mathbf{y}^i` has been identified. 

  | We will draw a point in the middle of each :math:`[\mathbf{m}^i]` that contains only one item of  :math:`\mathbb{M}`.
  | In Question **B.3**, we defined :math:`\mathbb{M}` as a set of boxes with a width of :math:`2\times 0.05`. Then, if one box :math:`[\mathbf{m}^i]` has a maximal diameter that is less than :math:`0.1`, it means that it contains one single item of :math:`\mathbb{M}`.
  | This can be verified in the main contraction loop:

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src/lesson_B.py
        :language: py
        :start-after: [B-q10-beg]
        :end-before: [B-q10-end]
        :dedent: 0

    .. group-tab:: C++

      .. literalinclude:: src/lesson_B.cpp
        :language: c++
        :start-after: [B-q10-beg]
        :end-before: [B-q10-end]
        :dedent: 2

  Expected result:

  .. figure:: img/loc_robot_landmarks_obs_box_id.png
    :width: 300px

    All the boxes have been identified. 


Conclusion
----------

This association has been done only by merging all information together. We define contractors from the equations of the problem, and let the information propagate in the domains. This has led to a state estimation solved concurrently with a data association.

In the next lessons, we will go a step further by making the robot move. 
This will introduce differential constraints, and a new domain for handling uncertain trajectories: tubes.

.. .. rubric:: Underwater application

.. We could apply this solver on actual data, such as the underwater application presented in the introduction of this lesson. The identification of the rocks on the seafloor would be done by the fusion of all data, without image processing.
.. The same algorithm also provides a reliable way to gather different views of a same object, without using data processing.

.. This topic of data association with underwater robotics has been the subject of a paper proposed for this ICRA 2020 conference:

.. | **Set-membership state estimation by solving data association**
.. | Simon Rohou, Benoît Desrochers, Luc Jaulin, *ICRA 2020*
.. | `Download the paper <http://simon-rohou.fr/research/datasso/datasso_paper.pdf?PHPSESSID=88a679b3n54fh04kt3l5lnmvv6>`_

.. Here is a video providing an overview of the problem and how to solve it. This is mainly the object of this Lesson B and the next one, that will deal with the dynamics of the robot.

.. .. raw:: html

..   <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto; margin-bottom: 30px;">
..       <iframe src="https://www.youtube.com/embed/rkzouwuwo4I" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
..   </div>

Related content
---------------

.. |datasso-pdf| replace:: **Download the paper**
.. _datasso-pdf: https://www.simon-rohou.fr/research/datasso/datasso_paper.pdf

.. admonition:: Related publication
  
  | **Set-membership state estimation by solving data association**
  | S. Rohou, B. Desrochers, L. Jaulin
  | *ICRA: IEEE International Conference on Robotics and Automation*, 2020.
  | |datasso-pdf|_