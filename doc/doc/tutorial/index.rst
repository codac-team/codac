.. _sec-tuto-main-page:

#############################################
Tutorial: constraint programming for robotics
#############################################

.. |logo_contredo| image:: img/logos/logo_contredo.png
   :height: 50px
   :target: https://anr.fr/Project-ANR-16-CE33-0024
.. |logo_dga| image:: img/logos/logo_dga.png
   :height: 60px
   :target: https://www.defense.gouv.fr/dga
.. |logo_ensta| image:: img/logos/logo_ensta.png
   :height: 50px
   :target: https://www.ensta-bretagne.fr/fr
.. |logo_gdrmacs| image:: img/logos/logo_gdrmacs.png
   :height: 60px
   :target: https://gdr-macs.cnrs.fr/
.. |logo_gdrrob| image:: img/logos/logo_gdrrob.png
   :height: 60px
   :target: https://www.gdr-robotique.org/
.. |logo_intcomp| image:: img/logos/logo_intcomp.gif
   :height: 40px
   :target: https://interval.louisiana.edu/reliable-computing-journal/
.. |logo_labsticc| image:: img/logos/logo_labsticc.png
   :height: 50px
   :target: https://www.labsticc.fr/en/index/

.. |logo_youtube| image:: img/logos/logo_youtube.png
   :height: 25px
.. |logo_youtube_small| image:: img/logos/logo_youtube.png
   :height: 15px

.. centered:: Simon Rohou, Luc Jaulin, Benoît Desrochers, Raphael Voges
.. centered:: **8th June -- 5th July**

.. centered:: |logo_ensta| |logo_labsticc| |logo_dga| 
.. centered:: |logo_contredo| |logo_gdrmacs| |logo_gdrrob| |logo_intcomp| 



.. sidebar:: ICRA 2020 tutorial

  .. Figure:: ../img/logo_icra.png
    :align: center

  This tutorial is proposed in the `ICRA 2020 Conference <https://www.icra2020.org/program/workshops-and-tutorials>`_ (International Conference on Robotics and Automation).


.. About the tutorial
.. ------------------

This tutorial is about Constraint Programming (CP), Interval Analysis (IA) and their applications to mobile robotics.

| **Interval analysis** yields methods to compute intervals in place of real numbers, enclosing uncertainties in the mean time.
| **Constraint Programming** aims at solving a complex problem by defining it in terms of constraints coming from the equations or the measurements.
| Both concepts match perfectly for a large number of applications including Robotics, which is the subject of this tutorial proposed in the `ICRA 2020 <https://www.icra2020.org/>`_ conference.


.. contents:: Contents of this page


Constraint programming?
-----------------------

There are several ways to deal with **state estimation** in mobile robotics. The **constraint programming** approach consists in defining a problem as a set of rules and letting a solver perform the estimation. For mobile robotics, rules are constraints coming from state equations.

Efforts have been done to propose operators and **solvers** to apply these constraints. The goal of this tutorial is to learn how to use them and understand the efficiency of the approach on realistic robotic applications. We will see that some problems that are difficult to solve with conventional methods (Kalman filters, particle approaches) can be easily dealt with by constraint programming. This is for instance the case of poor observation measurements, time uncertainties, delays, or when the initial conditions of the system are not known.

The tutorial will stand on :ref:`the Tubex library <home-page>`, that provides tools for computations over sets of reals and trajectories. It has been designed to deal with dynamical systems defined by non-linear differential equations and involving constraints such as trajectory evaluations, time uncertainties or delays. These computations stand on **interval analysis**, a well suited tool that reliably **propagates uncertainties**. 


Requirements
------------

Prerequisite for attending the tutorial are:

* basic knowledge of **Python or C++** (the exercises are available in both languages);
* although the tutorial is about state estimation, you do not need skills in Kalman or particle filters.

| Tubex is fully supported on Linux systems (C++ and Python3).
| We are currently working on making Tubex available for Windows and MacOS (any help is welcome!). Meanwhile, we propose :ref:`a Tubex-online solution <sec-py-project-online>`: this will allow you to use Tubex in Python without having to install the library on your machine.


Contact and registration
------------------------

| **For registration, please feel in this questionnaire:**
| https://docs.google.com/forms/d/1nuZ6lxMJ8SmoUeDJzVIjiMUdD5PmX7h72v_N9NtvGq4

The tutorial involves three platforms:

* the `Tubex website <http://simon-rohou.fr/research/tubex-lib>`_ (here you are!)
* the `MOOC platform <https://mooc.ensta-bretagne.fr/course/view.php?id=7>`_ where you can send your work and get the tutorial diploma
* the `Slack communication platform (#tt1) <https://icra20.slack.com/app_redirect?channel=tt1>`_ of ICRA for fast interactive meetings

This tutorial is proposed to the participants of `the ICRA conference <https://www.icra2020.org>`_. All the exercises are available on this Tubex website. The registration to `the MOOC platform <https://mooc.ensta-bretagne.fr/course/view.php?id=7>`_ is not mandatory, except if you want to share with the organizers your progression and difficulties, and to obtain the diploma.

|logo_youtube| See `the video for registration on the MOOC platform <https://www.youtube.com/watch?v=E_aj7Mz_LWs>`_.


Diploma
-------

| To get the diploma, you need to send valid exercises to the organizers. A participant who gets a minimum total of 12 points will receive a diploma corresponding to this tutorial. This diploma can be used by students to obtain the corresponding credits for their PhD courses (equivalent to 40 hours of lessons), or to comply with any other requests from their home university.
| `Here is an example of the diploma you can obtain <http://simon-rohou.fr/research/tubex-lib/public/publipost_tubmooc_2020.pdf>`_.


Duration and meeting sessions
-----------------------------

The tutorial will be held **from 8th of June to 5th of July**.
Interactive meetings sessions are planned each Wednesday afternoon at 2PM (UTC):

.. hlist::
  :columns: 4

  * 10th of June
  * 17th of June
  * 24th of June
  * 1rst of July


Content of the tutorial
-----------------------

A list of exercises is proposed with realistic robotic applications:

------------------------------------------------------

**Week 0: June 1 -- June 7** (installation)

Before starting the tutorial, you can read some words about the concepts of Constraint Programming and Interval Analysis. This will give you a first glimpse of the philosophy of this tutorial.

.. toctree:: 
  :maxdepth: 1

  /tutorial/00-getting-started/index

To get ready, you need to install the Tubex library on your computer.
Please follow the related page of the manual to see how to do it:

.. toctree:: 
  :maxdepth: 1

  /install/01-installation
  [video] For C++, we advice to use the Qt Creator IDE, see how to use it for Tubex <https://www.youtube.com/watch?v=ElV6xVVKhSg>

Then, depending on your preference between C++ or Python, you can run some *Hello World!* program to be sure everything is working well so far:

.. toctree:: 
  :maxdepth: 1

  /install/02-start-py-project
  /install/03-start-cpp-project

.. note::
  
  .. Figure:: img/logos/logo_replit.png
    :align: right

  In case you encounter difficulties to install Tubex on your computer, an alternative solution is to **try Tubex online** with Repl.it. You will find more information on the following page:

  * :ref:`sec-py-project-online`
  * |logo_youtube| See `the video for using Tubex online <https://www.youtube.com/watch?v=XTpJk4lIwgw>`_.

------------------------------------------------------

**Week 1: June 8 -- June 14**

.. image:: img/tuto_01.png
   :align: right
   :width: 200px

* **Lesson A**: Getting started with intervals and contractors
* **Lesson B**: Static range-only localization

.. * :ref:`sec-tuto-01`
.. * :ref:`sec-tuto-02`

| During this first week, we will install the library and perform the **state estimation** of a static robot between some landmarks. For the moment, we will assume that the robot does not move and is only receiving **range-only data** from the landmarks.
| This will be an introduction to :ref:`intervals <sec-manual-intervals>`, :ref:`constraints <sec-manual-contractors>` and :ref:`networks of contractors <sec-manual-cn>`.

| **Exercise to finish:** (before Monday 15th of June)
| -- the static range-only localization, to post on `the MOOC platform <https://mooc.ensta-bretagne.fr/course/view.php?id=7>`_.

------------------------------------------------------

**Week 2: June 15 -- June 21**

.. image:: img/tuto_02.png
   :align: right
   :width: 200px

* **Lesson C**: static range-bearing localization
* **Lesson D**: dynamic range-only localization

We will go a step further: now the landmarks are perceived with both **range and bearing data**. The resolution will involve new constraints, and a **decomposition** will be achieved.
In the second part, we will make the robot move and see how we can handle **uncertainties on trajectories**. This will be done by solving the range-only problem of Lesson B, now in a dynamical context with **asynchronous measurements**. 

| **Exercise to finish:** (before Monday 22th of June)
| -- the dynamic range and bearing localization, to post on `the MOOC platform <https://mooc.ensta-bretagne.fr/course/view.php?id=7>`_.

------------------------------------------------------

**Week 3: June 22 -- June 28**

.. image:: img/tuto_03.png
   :align: right
   :width: 200px

* **Lesson E**: localization with data association

Now, the landmarks perceived by the robot are **indistinguishable**. We still assume that we know their position, but the robot is not able to make the **association** between the map and the observations. This corresponds to the publication `Set-membership state estimation by solving data association <https://ras.papercept.net/proceedings/ICRA20/1405.pdf>`_, that is currently presented during this ICRA conference (see |logo_youtube_small| `the video presenting the paper <https://www.youtube.com/watch?v=rkzouwuwo4I>`_ and the `Slack channel #tua07_6 <https://icra20.slack.com/app_redirect?channel=tua07_6>`_).
We will solve this problem with Tubex. The goal of this exercise is to develop **our own contractor**.

| **Exercise to finish:** (before Monday 29th of June)
| -- the data association with bearing only observations, to post on `the MOOC platform <https://mooc.ensta-bretagne.fr/course/view.php?id=7>`_.

------------------------------------------------------

**Week 4: June 29 -- July 5**

.. image:: img/tuto_04.png
   :align: right
   :width: 200px

* **Lesson F**: range-only SLAM
* **Lesson G**: loop detection in robot trajectories

| We will end this tutorial with a **range-only SLAM** problem and see how Tubex can be used for **online missions**.
| The last part of this section will focus on new tools to detect and **prove loops** in robot trajectories from the only knowledge of proprioceptive measurements such as velocities.

**Exercise to finish:** to be defined

The following video illustrates the result of Lesson F:

.. raw:: html

  <div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%; height: auto; margin-bottom: 30px;">
      <iframe src="https://www.youtube.com/embed/Dw6-OHuW0vo" frameborder="0" allowfullscreen style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"></iframe>
  </div>

------------------------------------------------------


Organizers and technical support
--------------------------------

.. hlist::
  :columns: 3

  * `Simon Rohou <http://simon-rohou.fr/research/>`_
  * `Luc Jaulin <https://www.ensta-bretagne.fr/jaulin/>`_
  * `Benoît Desrochers <http://www.ensta-bretagne.fr/desrochers>`_
  * Raphael Voges
  * Julien Damers
  * Fabrice Le Bars

For any question, do not hesitate to use the `MOOC platform of this tutorial <https://mooc.ensta-bretagne.fr/course/view.php?id=7>`_, so that other participants can reply or see posted answers. We will also answer you on `the Slack communication platform (#tt1) <https://icra20.slack.com/app_redirect?channel=tt1>`_ for very short questions.