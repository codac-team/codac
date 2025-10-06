# [Codac: constraint-programming for robotics](http://codac.io/v2) [![Build Status](https://github.com/codac-team/codac/workflows/.github/workflows/tests.yml/badge.svg)](https://github.com/codac-team/codac/actions)

See the official website: http://codac.io/v2

Codac (Catalog Of Domains And Contractors) is a C++/Python/Matlab library providing tools for interval computations and constraint programming over real numbers, trajectories and sets.
It has numerous applications in parameter estimation, guaranteed integration, robot localization, and provides reliable outputs.

The toolbox allows to approximate feasible solutions of non-linear and/or differential systems. Since the solution of these complex systems cannot generally be calculated exactly, Codac uses numerical analysis to compute bounds on the sets of feasible solutions. The assets are **guarantee** (ensuring that no solutions are lost thanks to rigorous interval arithmetic) and thus **exhaustiveness** (capturing all possible values when multiple solutions exist).

Codac can therefore be used to establish numerical proofs or approximate solutions for complex systems involving variables of various types, such as real numbers, vectors, trajectories, uncertain sets, graphs, *etc.* Most of the library's developers are motivated by challenges in mobile robotics, where Codac offers new perspectives.

Recent advances in interval methods have been made by the community, and the Codac library brings together some of the corresponding state-of-the-art implementations, with the goal of making them easy to combine.

## Short example: solving an equation

One of Codac's applications is to solve systems of equations.
The following example [[1]](https://cyber.bibl.u-szeged.hu/index.php/actcybern/article/view/4438) computes a reliable outer approximation of the solution set of the equation system:

$$
  \left( \begin{array}{c}
    -x_3^2+2 x_3 \sin(x_3 x_1)+\cos(x_3 x_2)\\
    2 x_3 \cos(x_3 x_1)-\sin(x_3 x_2)
  \end{array}\right)=\mathbf{0}.
$$

The solution set is approximated from an initial box $[\mathbf{x}_0]=[0,2]\times[2,4]\times[0,10]$. The bisection involved in the paving algorithm is configured to provide boxes with a precision $\epsilon=4\times 10^{-3}$.

```python
from codac import *

x = VectorVar(3)
f = AnalyticFunction([x], [
  -(x[2]^2)+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
  2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
])

ctc = CtcInverse(f, [0,0])
DefaultFigure.pave([[0,2],[2,4],[0,10]], ctc, 0.004)
```

The result is a set of non-overlapping boxes containing the set of feasible solutions. The following figure shows a projection of the computed set.

<img src="./doc/manual/manual/example_malti.png" width="400px" />

*Outer approximation of the solution set, computed with ``CtcInverse``. Blue parts are guaranteed to be solution-free. Computation time: 0.609s. 3624 boxes.*


## Short example: solving an inequality

The previous example showed a way of solving systems of the form $\mathbf{f}(\mathbf{x})=\mathbf{0}$. The library also provides tools for solving generic systems expressed as $\mathbf{f}(\mathbf{x})\in[\mathbf{y}]$ where $[\mathbf{y}]$ is an interval or a box.

The following code allows to compute the set of vectors $\mathbf{x}\in\mathbb{R}^2$ satisfying the inequality:

$$
  x_1\cos(x_1-x_2)+x_2 \leqslant 0
$$

```python
x = VectorVar(2)
f = AnalyticFunction([x], x[0]*cos(x[0]-x[1])+x[1])
sep = SepInverse(f, [-oo,0])
DefaultFigure.pave([[-10,10],[-10,10]], sep, 0.004)
```

<img src="./doc/manual/manual/example_ineq.png" width="400px" />

*Approximation of an enclosure of the solution set computed with ``SepInverse``. The blue parts are guaranteed to have no solution, while any vector in the green boxes is a solution to the inequality. Computation time: 0.0809s.*


## Contributors

This list is in alphabetical order by surname.

- Auguste Bourgois
- [Cyril Bouvier](http://www.lirmm.fr/~bouvier/index.en.html)
- [Quentin Brateau](https://teusner.github.io)
- Gilles Chabert
- [Julien Damers](http://damers.org)
- Benoît Desrochers
- [Peter Franek](http://www.cs.cas.cz/~franek)
- [Maël Godard](https://godardma.github.io)
- [Nuwan Herath M.](https://members.loria.fr/NHerathMudiyanselage)
- [Luc Jaulin](https://www.ensta-bretagne.fr/jaulin)
- [Fabrice Le Bars](https://www.ensta-bretagne.fr/lebars)
- [Morgan Louédec](https://morgan-louedec.fr)
- [Damien Massé](https://lab-sticc.univ-brest.fr/~dmasse)
- [Bertrand Neveu](http://imagine.enpc.fr/~neveub)
- Verlein Radwan
- [Andreas Rauh](https://www.interval-methods.de)
- [Simon Rohou](http://simon-rohou.fr/research)
- [Joris Tillet](https://perso.ensta-paris.fr/~tillet/)
- [Gilles Trombettoni](https://www.lirmm.fr/~trombetton)
- [Christophe Viel](https://www.ensta-bretagne.fr/viel/)
- [Raphael Voges](https://raphael-voges.de)

We appreciate all contributions, whether they are code, documentation, bug reports, or suggestions. If you believe you should be listed here and are not, please contact us to update the list.


## How to cite Codac

The main reference to the Codac library is [the following paper](https://www.simon-rohou.fr/research/codac/codac_paper.pdf):
<pre>
@article{codac_lib,
  title={The {C}odac Library},
  url={https://cyber.bibl.u-szeged.hu/index.php/actcybern/article/view/4388},
  DOI={10.14232/actacyb.302772},
  journal={Acta Cybernetica},
  volume={26},
  number={4},
  series = {Special Issue of {SWIM} 2022},
  author={Rohou, Simon and Desrochers, Benoit and {Le Bars}, Fabrice},
  year={2024},
  month={Mar.},
  pages={871-887}
}
</pre>