.. _sec-tuto-faq:

###############################
FAQ: Frequently Asked Questions
###############################


.. contents:: 


------------------------------------------------------------------

General questions related to intervals and constraint propagations
==================================================================

When making the union :math:`\sqcup` of two intervals, will some solution outside of the original scopes be also included?
--------------------------------------------------------------------------------------------------------------------------

Suppose that you want to compute the interval hull of two intervals :math:`[-2,4]` and :math:`[6,7]`:

.. math::

  [-2,4]\sqcup[6,7]=[-2,7]

Other values that are neither in :math:`[-2,4]` nor :math:`[6,7]`, such as :math:`5`, will be included too.
This is what we call **pessimism**. Computations with intervals are simple, reliable and we can easily deal with non-linear problems with it, but sometimes the solution set contains unwanted solutions. This is the case with this *squared union* :math:`\sqcup` that wraps two subsets into one interval.

There are however methods to deal with pessimism, by bisecting the space into subsets that represent more optimally the actual solution.

For instance, in the following 2d example, we want to represent the black symbol. Using one box is not optimal for the representation. Instead, we can bisect it. The more we bisect, the better will be the approximation.

.. figure:: img/triskell_sivia_rohou.png

  Subpavings computed by a SIVIA algorithm in various accuracy levels. The boundary :math:`\partial\mathbb{X}` of the true solution set is plotted by a black line. Inner and outer sets are respectively drawn by green and both yellow and green boxes. The part proven to not contain solutions is represented in blue. This approach enables the estimation of sets of any shape such as this triskelion.


How this constraint propagation approach compares with Kalman/particle filters?
-------------------------------------------------------------------------------

The interval approach eliminates quickly inconsistent parts of the space of feasible solutions, whereas particle filters will be attracted by states that are consistent with the data.
As a consequence, particle methods and interval approaches have two different views of the state estimation problem.
They are complementary and could be combined.


.. rubric:: In which cases constraint propagation (CP) approaches are competitive?

The comparison with Kalman or Particle Filters is mainly related to the problem we are dealing with: non-linearities, amount of data to consider, baud rate of communications between robots, *etc.* When the system is **non-linear**, when the **initial state is not known**, or when one have to rely on **very few observations**, the CP/interval approach appears to be more competitive. It also provides an excellent way to **make proofs**.

For linear systems with known initial conditions, or when the linearization makes sense, the Kalman filter will surely be the best method for state estimation.
:ref:`sec-tuto-02` is a typical problem where the Kalman approach is not adapted, and where we see that the interval approach can be compared and combined with a particle approach.


.. rubric:: The best of each world

Particle filter (PF) and interval / Constraint Propagation (CP) approaches can be combined. A set-membership approach (CP coupled with intervals) provides sets as results. Therefore, one can spread particles inside these sets. This allows particle approaches to better converge. At the end, one can state: *we ensure that the solution is inside this set (defined by bounds with CP/intervals); in addition, inside this set, it is more likely around this vector (estimation from the particle filter)*.


What will be the beat methos to choose the uncertainties on [y](.)
------------------------------------------------------------------

In E14, I inflated the [y](.) by 0.2 and the got the following tube. At 0.2  my tube encloses the trajectory. What will be the beat methos to choose the uncertainties on [y](.) ?

Hi Manvir,
The method is simple: assume that you have a measurement y, from a sensor. y is made of the actual but unknown value y* plus a noise n: y=y*+n. If we know that n is bounded by [n]=[-0.2,0.2] (this can be found in some datasheets of the sensor), then we can state that the actual value y* (that we are looking for) is bounded by: y*ϵy-[n]. 
Therefore, if you build a tube [y](.) from your continuous measurement y(.), and enlarge it by [n] (i.e., inflate it by width([n])/2=0.2), then you are 100% sure that the actual value y* will be inside [y](.). It is the case on your figure, red never goes outside the blue set. And so all the computations involving [y](.) will be 100% guaranteed too: by propagation of constraints, you will ensure that [x](.) also contains the actual trajectory of the robot.


*Next questions are related to the use of the API.*

------------------------------------------------------------------

Ask for help
============

How to access the documentation of the functions?
-------------------------------------------------

* API
* help in Python
* website

You can read this page detailing some functions of VIBesFig (and so VIBesFigMap). We will add more content to this part of the manual within the next few days.
In addition, you can have a look at the Tubex manual (besides that tutorial). If you want an exhaustive documentation, there is also the C++ list of functionalities here (it may help for Python use).



------------------------------------------------------------------

Basic operations (arithmetic)
=============================

I am using an operator such as cos, exp, sin and I obtain the following error:
------------------------------------------------------------------------------

TypeError: cos(): incompatible function arguments. The following argument types are supported:
    1. (arg0: float) -> float
    2. (arg0: pyibex.pyibex.Interval) -> pyibex.pyibex.Interval
    3. (arg0: tubex_lib.tube.Tube) -> tubex_lib.tube.Tube
    4. (arg0: tubex_lib.tube.Trajectory) -> tubex_lib.tube.Trajectory

The computation of cos/sqrt/sqr/.. are only allowed on scalar values (so not available for IntervalVector objects).
Contrary to abs that should work on an IntervalVector.


I have the error:
-----------------

TypeError: sqr(): incompatible function arguments. The following argument types are supported:
1. (arg0: pyibex.pyibex.Interval) -> pyibex.pyibex.Interval
Invoked with: Trajectory...

Solution: import pyibex before tubex
from pyibex import *
from tubex_lib import *

you need to import pyibex first, otherwise the sqrt function from pyibex will be set in current namespace, and pyibex does not know tubes and trajectories.


I Obtain this error :
---------------------

error: no match for ‘operator-’ (operand types are ‘tubex::Trajectory’ and ‘ibex::Interval’)
TrajectoryVector actual_y=sqrt(sqr(actual_x[0]-b[0])+sqr(actual_x[1]-b[1]));

And furthermore, you probably defined b as an IntervalVector, which means that b[0] and b[1] are Interval objects. You cannot compute the difference between some real value (or a trajectory) and a set of values (an interval).

You have two options:
* define and use a vector, for instance: Vector actual_b({0.5,1});
* use the mid of the sets (that are double values): b[0].mid(), b[1].mid()


------------------------------------------------------------------

Domains (intervals, boxes, tubes)
=================================

How to concatenate two IntervalVectors?
---------------------------------------

Three solution (the last one is the best)

>>> a = IntervalVector([[1,2],[3,4]])
>>> b = IntervalVector([[5,6],[7,8]])
>>> cart_prod(a,b)
([1, 2] ; [3, 4] ; [5, 6] ; [7, 8])

a = IntervalVector([[1,2],[3,4]])
b = IntervalVector([[5,6],[7,8]])
# 1
c = IntervalVector(4)
c.put(0,a)
c.put(2,b)
print(c)
> ([1, 2] ; [3, 4] ; [5, 6] ; [7, 8])
# 2
d = IntervalVector([a[0], a[1], b[0], b[1]])
print(d)
> ([1, 2] ; [3, 4] ; [5, 6] ; [7, 8])


------------------------------------------------------------------

Contractors (independent constraints)
=====================================



I don't have accurate results with my own Cdist contractor, why?
----------------------------------------------------------------

I got the problem.
You should write
ctc_dist = CtcFunction(Function("x[2]","b[2]","d","sqrt((x[0]-b[0])^2+(x[1]-b[1])^2) - d"))
instead of
ctc_dist = CtcFunction(Function("x[2]","b[2]","d","sqrt((x[0]-b[0])*(x[0]-b[0])+(x[1]-b[1])*(x[1]-b[1])) - d"))

You result is correct (you do not loos any solution), but less accurate. It is what we call the dependency problem.
When you write [a]*[a], you are less accurate than [a]^2.
For instance, [-2,2]*[-2,2]=[-4,4]
whereas [-2,2]^2=[0,4]
Tell me if you do not understand.


I don't understand how the C_dist formula works in the code
-----------------------------------------------------------

How .contract() makes the constrained results (small boxes inside of the origin boxes b1, b2, b3) by the formula ``f(|x-b|-d =0)``? Is Least Square Estimation working behind it?

The result only comes from contractors. There are no probabilistic or regression methods behind it.
It is only a composition of operators (contractors) that deals with the bounds of the sets.

For instance, in Lesson A, we compute the difference of two intervals by:
[3,4]−[2,6]=[−3,2]

Suppose we have three intervals [a]=[3,4], [b]=[2,6] and [c]=[-∞,∞].
You can also compute the difference only with a contractor C-.

In Python with a CN :
a=Interval(3,4)
b=Interval(2,6)
c=Interval()
cn = ContractorNetwork()
ctc_minus = CtcFunction(Function("a","b","c","a-b-c")) # a-b=c
cn.add(ctc_minus, [a,b,c])
cn.contract()
print(c)

The result is [c]=[−3,2].
It seems more complex than directly computing Interval(3,4)-Interval(2,6), but this formalism allows to contract any variable. In some cases, information in [c] could also be propagated to [a] or [b].

The mathematical definition of the contractor C- is in fact:
[IMAGE] Formula of the contractor C-

The same approach applies for the more-complex contractor Cdist. The difference is that Cdist is made as a composition of several simple contractors such as this C-. The library does the composition for you.

At the end, the results only come from intersection of sets and computations on the bounds of these sets.


I have one question regarding CtcDist Contractor.
-------------------------------------------------

The example mentioned at http://simon-rohou.fr/research/tubex-lib/doc/manual/04-static-contractors/02-ctc-dist.html#definition says that we need to iterate two times. Please explain how did we get to know that we need to iterate two times?

.. Luc:

In general, you have to contract several times up to the fixed point.
Here, probably you can observe that the fixed point is reached after two iterations.

Thanks. How can we find that we have reached fixed point situation? Also, my understanding is that domain will change in each iteration.  is that correct? If yes, I dont see domain changing in the program.

.. Luc:

When you contract several times, you get a sequence of nested boxes [x](0),[x](1),[x](2),...
Since the boxes are nested, you will converge to one box [x](oo).
Since your computer computes with floating point numbers, you will reach the fixed point in a finite number of steps.
In practice, you may stop as soon as the contraction [x](k+1) for [x](k) is not significant anymore.
Anyway, even if you stop before reaching the fixed point, you will always enclose all solution.






------------------------------------------------------------------

Contractor Networks (solving problems)
======================================

I don't know how to initialize the domains of my CN
---------------------------------------------------

If you do not have prior values for the domains (pre-defined sets), then the best is to set them as infinite.
The point is to define infinite domains and let the CN solve the problem for us. We could help CN by defining some prior sets in m before the resolution, but in fact there is no need to do it.
When I say "infinite domains", I mean that m will be a list of 2d boxes that have infinite bounds: [-∞,∞]×[-∞,∞]. The CN will then contract them.
Feel free to ask me if this is not clear enough!


I have an empty set result
--------------------------

* your domains are ill-defined, for instance the lower bound is higher than the upper bound: [12,3]=\varnothing
* the problem has no solution according to the constraints at stake



I have the error
----------------

ValueError: unable to convert the py::object into a valid tubex::Domain

With the code:

    yi = ([-0.84, -0.83], [-0.76, -0.75])

You create a list of intervals, but not an IntervalVector object. The CN needs an IntervalVector to run.
So what you did:

    yi = IntervalVector([Interval(-0.84, -0.83), Interval(-0.76, -0.75)])

solves the issue. You could also write:

    yi = IntervalVector([[-0.84, -0.83], [-0.76, -0.75]])




I am confused about how to apply a static contractor (Cdist) to a tube
----------------------------------------------------------------------

The Cdist contractor (the same as the one to create in Lesson A) takes three domains as input:
http://simon-rohou.fr/research/tubex-lib/doc/manual/04-static-contractors/02-ctc-dist.html
On this page you can see that the 3 domains are: one 2d box [a], one 2d box [b] and the bounded value (interval) of the distance: [d].
It is related to the constraint expressing the Euclidean distance d between the points a and b.
On our robotic case, our points are bounded by [x] and [b], the positions of the robot and the landmark. The distance is enclosed by [y]. Then, the contractor can apply like this:
Cdist([x],[b],[y])
In a CN, we can write:
cn.add(ctc.dist,[x,b,y])
Now, this was static. In a dynamic context, the things are exactly the same except that we will use tubes:
Cdist([x](·),[b],[y](·))
In a CN, we can write the same code (objects x and y are tubes, now):
cn.add(ctc.dist,[x,b,y])
This is equivalent to an infinite set of contractors like this:
Cdist([x](t0),[b],[y](t0))
Cdist([x](t1),[b],[y](t1))
...
Cdist([x](tf),[b],[y](tf))
Since [b] is not a tube, its value is repeated for each contractor.
So to sum up, for applying the constraint, you only have to write one line:
cn.add(ctc.dist,[x,b,y])



I have the error
----------------

"Assertion `y.size() == z.size()` failed"

When you create the TubeVector v from the TrajectoryVector traj_v, v gets automatically the size of traj_v (which is 2).
However, when you write:

    x = TubeVector(tdomain, dt)

x becomes a TubeVector with a default size=1. Then, the domains in the CN are not consistent as we try to evaluate a 1d tube with a 2d box.
With:
x = TubeVector(tdomain, dt, 2)
the assertion disappears.


------------------------------------------------------------------

Python related questions
========================



How can I get the last Python version of Tubex?
-----------------------------------------------

pip3 install tubex-lib --upgrade



------------------------------------------------------------------

C++ related questions
=====================

I have the error
----------------

error: no matching function for call to ‘tubex::VIBesFigMap::add_trajectory(tubex::TrajectoryVector&, const char [3], int, int)’
     fig_map.add_trajectory(actual_x, "x*", 0, 1);

the function needs a pointer to the trajectory



In C++. Should not the 'contract' method have a return value and not be "void"?
-------------------------------------------------------------------------------

How do we return the value of the contraction performed? I tried to return the contracted interval but of course the signature is different from the inherited one, so I get an error.

In C++, it is different than in Python (due to the spirit of the language). The update is done by "reference" which means that the argument given to the contract() method will be updated. No need to return a value in this case. We know that is it a "return value by reference" because of the "&" in "void contract(ibex::IntervalVector& a)".
So at the end, you should update "a" inside the function in order to return the contracted box.

I have the error
----------------

error: no match for ‘operator|’ (operand types are ‘std::ostream {aka std::basic_ostream<char>}’ and ‘ibex::Interval’)
cout << " m|n: " << m|n << endl;

My code looks as follows:

    m = Interval(-2,4);
    n = Interval(6,7);
    cout << "m: " << m << ", n: " << n << endl;
    cout << " m|n: " << m|n << endl;


Solution:
When using cout (streams), you can simply write the operation between parentheses to indicate that you want to stream the Interval object resulting from the operation:

Interval m(-2,4);
Interval n(6,7);
cout << "m: " << m << ", n: " << n << endl;
cout << " m|n: " << (m|n) << endl;

This way, you give more information to the compiler about the object to print.