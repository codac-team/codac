# Get Started

First install pyibex with pip:

```
$ python -m pip install pyibex
```
Don't forget to upgrade your pip installation in order to be able to download the wheel files.
```bash
$ python -m pip install --upgrade pip
```
See [Installation section](installation.html) for more details:

You can check your installation by running:
```bash
$ python -m unittest discover pyibex.tests
```

# Basics

**PyIbex** functionalities and objects are under the *pyibex* namespace.

```python
    from pyibex import Interval
```

## Intervals

The type Interval represents a closed set of `$\mathbb{R}$`. The set of all Intervals
is denoted `$\mathbb{IR}$`

```python

    # Load pyibex lib
    from pyibex import Interval

    # Create new Intervals
    a = Interval.EMPTY_SET # an empty interval
    a = Interval.ALL_REALS # interval [-oo, oo]
    a = Interval(-oo, oo)  # interval [-oo, oo]
    a = Interval()         # interval [-oo, oo]
    a = Interval(-2, 3)    # interval [-2, 3]
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
    IntervalVector( 2, [1,3]) # ==> box [1,3]x[1,3]
    IntervalVector( 2, Interval(1,3)) # ==> box [1,3]x[1,3]
    IntervalVector([1,2,3]) # ==> box [1,1]x[2,2]x[3,3]
    IntervalVector([[-1,3], [3,10], [-3, -1]]) # ==> box [-1,3]x[3,10]x[-3,-1]
    IntervalVector( [a, Interval(-1,0), Interval(0) ] )
```

## Inclusion Functions

Ibex-lib provides a framework to build *inclusion function*. See http://www.ibex-lib.org/doc/tutorial.html#functions

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
  x = IntervalVector([[1,3], [1,4]])
  f.eval(x)
```
### Vector valued functions

Vector valued function can be written by using parenthesis syntax "( ..., ..., ...)".
For instance :

```math
  f:(x,y,z)\longrightarrow \left\{\begin{array}{c} x^2-y \\  y-2z \\ z^3-\sqrt{y-x}\end{array}\right.
```

is defined in python by:

```python

  f = Function("x","y","z", "( x^2-y, y-2*z, z^3-sqrt(y-x))")

```
### Using the minibex syntax.

Using the **minibex** syntax (see http://www.ibex-lib.org/doc/tutorial.html#using-the-minibex-syntax)
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
A *Contractor* `$\mathcal{C}$` is an operator `$\mathbb{IR}^{n}\mapsto\mathbb{IR}^{n}$` such that
```math
\begin{equation}
  \begin{array}{lll}
    \mathcal{C}([\mathbf{x}])\subset[\mathbf{x}] &  & \text{(contractance)}\\
    {}[\mathbf{x}]\subset\left[\mathbf{y}\right]\text{ }\Rightarrow\text{}\mathcal{C}([\mathbf{x}])\subset\mathcal{C}([\mathbf{y}]). &  & \text{(monotonicity)}
\end{array}
\end{equation}
```

A set `$\mathbb{X}$` is *consistent* with the contractor `$\mathcal{C}$` (we
will write `$\mathbb{X}\sim\mathcal{C}$`) if for all `$\left[\mathbf{x}\right]$`, we
have:
```math
\begin{equation}
\mathcal{C}([\mathbf{x}])\cap\mathbb{X}=[\mathbf{x}]\cap\mathbb{X}.
\end{equation}
```

In pyibex a contractor is called using the *contract* method, appled on a box or directly by calling the *__call__*
method.

### Forward / Backward Contractor

Given a function `$\mathbf{f}$` and a box `$[\mathbf{y}]$`,
the *CtcFwdBwd* object can be used to build a fwd/bwd contractor associated to
the set `$\mathbb{X} = \left\{ \mathbf{x} \mid \mathbf{f}(\mathbf{x}) \in [\mathbf{y}] \right\}$`:
For instance:
```python
    # Define a Function from an equation
    f = Function("x_1", "x_2", "x_1^2 + x_2^2 - 3")

    # FwdBwd Contractor f(x) > 0
    ctc = CtcFwdBwd(f, [0, oo])

    # Define an initial box X0 = [-5, 5]x[-5, 5]
    X0 = IntervalVector(2, [-5, 5])

```

### Operations on Contractors

```python
    # CtcIn/CtcOut contractors :math:`$f \in [-2.5, 3.5]$`
    ctcIn = CtcIn(f, Interval(3).inflate(0.5))
    ctcOut = CtcNotIn(f, Interval(3).inflate(0.5))

    # Operations on Contractors
    # composition of two contractor
    ctc = ctcIn & ctcOut
    # union of a python list of contractors
    ctc1 = CtcUnion([ctcIn, ctcOut, ctc1])
```
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

## Separators
### Defintion

A separator `$\mathcal{S}$` is an operator that performs two complementary contractions. Associated with a set `$\mathbb{S}$` and given a box `$[\mathbf{x}]$`, the separator produces two sub-boxes `$[\mathbf{x}_ {in}]$` and `$[\mathbf{x}_{out}]$` that verify:
```math
        ([\mathbf{x}] \cap [\mathbf{x}_{in}]) \subset \mathbb{S} \\
        ([\mathbf{x}] \cap [\mathbf{x}_{out}]) \cap \mathbb{S} = \emptyset
```
A separator can also be viewed a as pair of contractors. For efficiency reasons, the separate(…) function takes only two input-output arguments, `$\mathbf{x}_{in}$` and `$\mathbf{x}_{out}$`, each containing initially a copy of the box `$[\mathbf{x}]$`
### Example
```python
    from pyibex import *
    from vibes import *

    f = Function('x', 'y', 'x*cos(x-y)+y')
    # build the separator associated to the constraint f(x,y) < 0
    sep = SepFwdBwd(f, CmpOp.LEQ)
    # setup the initialbox [-10,10]x[-10,10]
    box = IntervalVector(2, [-10, 10])
    vibes.beginDrawing()
    pySIVIA(box, sep, 0.3)
    vibes.endDrawing()

```