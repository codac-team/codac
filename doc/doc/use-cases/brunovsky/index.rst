.. _sec-usecases-brunovsky:

.. warning::
  
  This page is related to a paper that is not published yet. 
  More content will appear soon.
  
############################################
Pose estimation with range-only observations
############################################

In the paper *Brunovsky decomposition for dynamic interval localization*, a new set-membership method has been proposed for estimating the trajectories of dynamical systems, when the states are completely unknown and only non-linear observations are available. 

The first part of the proposed method is symbolic and follows the decomposition of Brunovsky, *i.e.*, it decomposes the set of differential equations describing the dynamical system into two blocks of constraints: one block gathers non-linear analytical equations that do not involve differential operators, while the other block is composed of linear chains of integrators. The second part of the method, that relies on the symbolic decomposition, is numerical and based on a contractor approach. It involves a specific optimal operator for narrowing the sets of feasible solutions. 

This approach is shown to be efficient on a difficult problem of dynamical localization of a mobile robot, without any prior knowledge about its states.

*More content coming soon.*