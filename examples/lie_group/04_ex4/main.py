from codac import *
from lie_group_ex4_separator import lie_group_ex4_separator


def example_4_continuous():
    print("Running example 4\n")
    # Generate reference
    domain = Interval(0, 15)
    timestep = 0.01
    x0 = IntervalVector(4)
    x0.init(Interval(0, 0))
    f = Function("x", "y", "z", "x4", "(cos(z); sin(z); sin(0.4*x4); 1)")
    ctc_lohner = CtcLohner(f)
    a = TubeVector(domain, timestep, f.nb_arg())
    a.set(x0, 0.)
    ctc_lohner.contract(a)
    print("Reference generated ", a)

    epsilon = 0.01

    X0 = IntervalVector(3)
    X0[0] = Interval(-0.1, 0.1)
    X0[1] = Interval(-0.1, 0.1)
    X0[2] = Interval(-0.4, 0.4)

    fullSep = lie_group_ex4_separator(a, X0)

    proj = IntervalVector([[-6, 6], [-15, 0]])
    sep_proj = SepProj(fullSep, proj, epsilon)

    mapping = IntervalVector([[-4, 4], [-4, 4]])

    beginDrawing()
    fig = VIBesFigMap("Example 4 continuous")
    fig.set_properties(50, 50, 800, 800)
    background_box = IntervalVector(mapping)
    fig.axis_limits(background_box)
    SIVIA(mapping, sep_proj, epsilon, True, True, "Example 4 continuous", False,
          VIBesFigPaving.LIE_SET_COLOR_MAP)

    fig.draw_box(X0, "#00FF00A3[#00FF00A3]")
    fig.add_tube(a, 'reference', 0, 1)
    fig.set_tube_color(a, "k[k]")
    fig.show()
    fig.axis_limits(background_box)


def example_4_discrete():
    print("Running example 4\n")
    # Generate reference
    domain = Interval(0, 15)
    timestep = 0.01
    x0 = IntervalVector(4)
    x0.init(Interval(0, 0))
    f = Function("x", "y", "z", "x4", "(cos(z); sin(z); sin(0.4*x4); 1)")
    ctc_lohner = CtcLohner(f)
    a = TubeVector(domain, timestep, f.nb_arg())
    a.set(x0, 0.)
    ctc_lohner.contract(a)
    print("Reference generated ", a)

    epsilon = 0.01

    X0 = IntervalVector(3)
    X0[0] = Interval(-0.1, 0.1)
    X0[1] = Interval(-0.1, 0.1)
    X0[2] = Interval(-0.4, 0.4)
    mapping = IntervalVector([[-4, 4], [-4, 4]])

    fullSep = lie_group_ex4_separator(a, X0)

    projections = [IntervalVector([[-6, 6], [0, 0]]),
                   IntervalVector([[-6, 6], [-1, -1]]),
                   IntervalVector([[-6, 6], [-2, -2]]),
                   IntervalVector([[-6, 6], [-3, -3]]),
                   IntervalVector([[-6, 6], [-4, -4]]),
                   IntervalVector([[-6, 6], [-14, -14]]),
                   IntervalVector([[-6, 6], [-15, -15]])
                   ]

    sep_proj = SepProj(fullSep, projections[0], epsilon)

    for i in range(1, len(projections)):
        sep_proj |= SepProj(fullSep, projections[i], epsilon)

    beginDrawing()

    fig = VIBesFigMap("Example 4 discrete")
    fig.set_properties(50, 50, 800, 800)
    background_box = IntervalVector(mapping)
    fig.axis_limits(background_box)
    SIVIA(mapping, sep_proj, epsilon, True, True, "Example 4 discrete", False,
          VIBesFigPaving.LIE_SET_COLOR_MAP)

    fig.draw_box(X0, "#00FF00A3[#00FF00A3]")
    fig.add_tube(a, 'reference', 0, 1)
    fig.set_tube_color(a, "k[k]")
    fig.show()
    fig.axis_limits(background_box)


if __name__ == "__main__":
    #example_4_continuous()
    example_4_discrete()
