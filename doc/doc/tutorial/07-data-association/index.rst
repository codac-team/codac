.. _sec-tuto-07:

Lesson G: Dynamic localization with data association
====================================================

In this lesson, we will extend the application of :ref:`sec-tuto-04`. The data association problem was easily treated in a static context. We will now make the robot move and see that a complex problem, hardly solvable with convention methods, can be easily dealt with constraint programming and tubes.


.. contents:: Content of this lesson


Video
-----

The application of localization with data association has been presented during the ICRA 2020 conference:

| **Set-membership state estimation by solving data association**
| Simon Rohou, Benoît Desrochers, Luc Jaulin, *ICRA 2020*
| `Download the paper <http://simon-rohou.fr/research/datasso/datasso_paper.pdf?PHPSESSID=88a679b3n54fh04kt3l5lnmvv6>`_

Here is a video providing an overview of the problem and how to solve it. We will build the related solver in this lesson.

.. raw:: html

  <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto; margin-bottom: 30px;">
      <iframe src="https://www.youtube.com/embed/rkzouwuwo4I" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
  </div>


Formalism
---------

The equations of the problem are given by:

.. math::

  \left\{ \begin{array}{llc}
  \dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),\mathbf{u}(t)\big) & &  \text{(evolution eq.)}\\
  \mathbf{g}\big(\mathbf{x}(t_{i}),\mathbf{y}^{i},\mathbf{m}^{i}\big)=\mathbf{0} & &  \text{(observation eq.)}\\
  \mathbf{m}^{i}\in\mathbb{M} & & \text{(data association)}\\
  \end{array}\right.