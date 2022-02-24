# Get Started

First install pyibex with conda:

```
conda install -c benensta pyibex
```
See [Installation section](installation.html) for more details:


**PyIbex** functionalities and objects are under the *pyibex* namespace.

```python
    from pyibex import Interval
```


# Basic Types

## Intervals

The type Interval represents a closed set of `$\mathbb{R}$`. The set of all Intervals
are denoted `$\mathbb{IR}$`


```python

    # Load pyibex lib
    from pyibex import Interval

    # Create new Intervals
    a = Interval.EMPTY_SET # create an empty interval
    a = Interval.ALL_REALS # create [-oo, oo]
    a = Interval()         # create [-oo, oo]
    a  = Interval(-2, 3)   # create [-2, 3]
```

## IntervalVector (box)

An IntervalVector, or a *Box*, is a Cartesian product of n intervals. The set of all boxes
of `$\mathbb{R}^n$` is denoted `$\mathbb{IR}^n$`.
With *pyibex* it can be created :
 - with the dimension and an initial interval [x_lb, x_ub].
 - with a point passed as a list [x1, x2, ..., xn]
 - with a list of list of bounds [[x1_lb, x1_ub], [x2_lb, x2_ub], ..., [xn_lb, xn_ub]]
 - with a list of interval ([x1], [x2], ..., [xn])

```python
    # Create IntervalVector
    IntervalVector( 2, [1,3]) # ==> box [1, 3]x[1,3]
    IntervalVector( 2, Interval(1,3)) # ==> box [1, 3]x[1,3]
    IntervalVector([1,2,3]) # ==> box [1,1]x[2,2]x[3,3]
    IntervalVector([[-1,3], [3,10], [-3, -1]]) # ==> box  [-1,3]x[
    IntervalVector( [a, Interval(-1,0), Interval(0) ] )
```

## Inclusion Functions

Ibex-lib provides a very powerful framework to build *inclusion function*. See http://www.ibex-lib.org/doc/tutorial.html#functions

With *pyibex*, functions are only defined with strings.  First arguments define variables
while the last one represents the expression of the function.
Vector argument can be defined with an array like syntax.

For instance, the function `$f:(x,y) \longrightarrow x\cdot\sin(x+y)-2$`
can be defined with:

```python
  from pyibex import Function
  # define f
  f1 = Function("x", "y", "x*sin(x+y)-2")
  # or with an array like syntax
  f2 = Function("x[2]", "x[0]*sin(x[0]+x[1])-2")
  # Evaluation with x=[1,3], y = [1,4]
  f.eval(IntervalVector([[1,3], [1,4]]))
```

Vector valued function can be written by using parenthesis syntax "( ..., ..., ...)".
For instance :

```math
  f:(x,y,z)\longrightarrow \left\{\begin{array}{c} x^2-y \\  y-2z \\ z^3-\sqrt{y-x}\end{array}\right.
```

is defined in python by:

```python

  f = Function("x","y","z", "( x^2-y, y-2*z, z^3-sqrt(y-x))")

```
Using the **minibex** syntxe (see http://www.ibex-lib.org/doc/tutorial.html#using-the-minibex-syntax)
function can be defined from a text file.

for instance:

```text
  /* myfunction.txt */
  function f(x,y,z)
    return (x^2-y, y-2*z, z^3-sqrt(y-x));
  end
```

```python
  f = Function("myfunction.txt")
```


## Contractors


```python

    # Define a Function from an equation
    f = Function("x", "y", "x^2 + y^2 - 3")

    # FwdBwd Contractor
    ctc1 = CtcFwdBwd(f, CmpOp.LEQ, FwdMode.AFFINE_MODE)

    # CtcIn/CtcOut contractors :math:`$f \in [-2.5, 3.5]$`
    ctcIn = CtcIn(f, Interval(3).inflate(0.5))
    ctcOut = CtcNotIn(f, Interval(3).inflate(0.5))

    # Operations on Contractors
    # composition of two contractor
    ctc = ctcIn & ctcOut
    # union of a python list of contractors
    ctc1 = CtcUnion([ctcIn, ctcOut, ctc1])
```
