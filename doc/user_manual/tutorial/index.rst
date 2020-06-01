.. _sec-tuto:

#############################################
Tutorial: constraint programming for robotics
#############################################

.. centered:: Simon Rohou, Luc Jaulin, Benoît Desrochers, Raphael Voges
.. centered:: **8th June -- 28th June**

.. sidebar:: ICRA 2020 tutorial

  .. Figure:: ../img/logo_icra.png
    :align: center

  This tutorial is proposed in the `ICRA 2020 Conference <https://www.icra2020.org/program/workshops-and-tutorials>`_ (International Conference on Robotics and Automation).


.. About the tutorial
.. ------------------

This tutorial is about Constraint Programming (CP), Interval Analysis (IA) and their applications to mobile robot localization.

| **Interval analysis** yields methods to compute intervals in place of real numbers, enclosing uncertainties in the mean time.
| **Constraint Programming** aims at solving a complex problem by defining it in terms of constraints coming from the equations or the measurements.
| Both concepts match perfectly for a large number of applications including Robotics, which is the subject of this tutorial proposed in the `ICRA 2020 <https://www.icra2020.org/>`_ conference.


.. contents:: Contents of this page


Constraint programming?
-----------------------

There are several ways to deal with **state estimation** in mobile robotics. The **constraint programming** approach consists in defining a problem as a set of rules and letting a solver perform the estimation. For mobile robotics, rules are constraints coming from state equations or uncertainties from the measurements.

Efforts have been done to propose operators and **solvers** to apply these constraints. The goal of this tutorial is to learn how to use them and understand the efficiency of the approach on realistic robotic applications. We will see that some problems that are difficult to solve with conventional methods (Kalman filters, particle approaches) can be easily dealt with by constraint programming. This is for instance the case of poor observation measurements, time uncertainties, delays, or when the initial conditions of the system are not known.

The tutorial will stand on **the Tubex library**, that provides tools for computations over sets of reals and trajectories. It has been designed to deal with dynamical systems defined by non-linear differential equations and involving constraints such as trajectory evaluations, time uncertainties or delays. These computations stand on **interval analysis**, a well suited tool that reliably **propagates uncertainties**. 


Requirements
------------

Prerequisite for attending the tutorial are:

* basic knowledge of **Python or C++** (the exercises are available in both languages) ;
* although the tutorial is about state estimation, you do not need skills in Kalman filter or particle approaches.


Contact and registration
------------------------

The tutorial involves three platforms:

* the `Tubex website <http://simon-rohou.fr/research/tubex-lib>`_ (here you are!)
* the `MOOC platform <https://mooc.ensta-bretagne.fr/course>`_ where you can send your work and get the tutorial diploma
* the `Slack communication platform (#tt1) <https://icra20.slack.com/app_redirect?channel=tt1>`_ of ICRA for interactive meetings

All the exercises are available on the Tubex website. The registration to the MOOC platform is not mandatory, except if you want to share with the organizers your progression and difficulties, and to obtain the diploma.


Diploma
-------

To get the diploma, you need to send valid exercises to the organizers. A participant who gets a minimum total of 12 points will receive a diploma corresponding to this tutorial.
This diploma can be used by students to obtain the corresponding ECTS from their PhD courses, or to comply with any other requests by their home university.

.. An example of the diploma you can obtain is:



Duration and meeting sessions
-----------------------------

The tutorial will be held **from 8th of June to 28th of June**.
Interactive meetings sessions are planned each Wednesday afternoon at 2pm:

.. hlist::
  :columns: 4

  * 10th of June
  * 17th of June
  * 24th of June


Content of the tutorial
-----------------------

A list of exercises is proposed with realistic robotic applications.


**Week 1: June  8 -- June 14**

* Getting started with concepts of CP and IA
* Static range-only localization
* Static range-bearing localization


**Week 2: June 15 -- June 21**

* Dynamic range-only localization
* Localization with data association


**Week 3: June 22 -- June 28**

* Range-only SLAM


Organizers
----------

.. hlist::
  :columns: 2

  * `Simon Rohou <http://simon-rohou.fr/research/>`_
  * `Luc Jaulin <https://www.ensta-bretagne.fr/jaulin/>`_
  * `Benoît Desrochers <http://www.ensta-bretagne.fr/desrochers>`_
  * Raphael Voges

For any question, do not hesitate to use the `MOOC platform of this tutorial <https://mooc.ensta-bretagne.fr/course>`_, so that other participants can reply or see posted answers. We will also answer you on `the Slack communication platform (#tt1) <https://icra20.slack.com/app_redirect?channel=tt1>`_ for very short questions.