from pyibex import Function, SepProj, SepInverse, IntervalVector, oo


def SepDiff(Sa, Sb):
    """Compute B-A."""
    f1 = Function("p[2]", "a[2]", "(p[0] + a[0], p[1] + a[1])")
    f2 = Function("p[2]", "a[2]", "(a[0], a[1])")

    sep_invB = SepInverse(~Sb, f1)
    sep_invA = SepInverse(Sa, f2)

    sep2 = sep_invB & sep_invA

    y_init = IntervalVector(2, [-oo, oo])

    sep = ~SepProj(sep2, y_init, 0.001)
    return sep


def SepSum(Sa, Sb):
    """Compute A+B."""
    f1 = Function("p[2]", "a[2]", "(p[0] - a[0], p[1] - a[1])")
    f2 = Function("p[2]", "a[2]", "(a[0], a[1])")

    sep_invB = SepInverse(Sb, f1)
    sep_invA = SepInverse(Sa, f2)

    sep2 = sep_invB & sep_invA

    y_init = IntervalVector(2, [-oo, oo])

    sep = SepProj(sep2, y_init, 0.001)
    return sep

def SepSum2(Sa, Sb):
    """Compute A+B."""
    f1 = Function("p[2]", "a[2]", "(p[0] + a[0], p[1] + a[1])")
    f2 = Function("p[2]", "a[2]", "(a[0], a[1])")

    sep_invB = SepInverse(Sb, f1)
    sep_invA = SepInverse(Sa, f2)

    sep2 = sep_invB & sep_invA

    y_init = IntervalVector(2, [-oo, oo])

    sep = SepProj(sep2, y_init, 0.001)
    return sep


def Inflate(Sb, y_init):
    """Compute A+B."""
    f1 = Function("p[2]", "a[2]", "(p[0] - a[0], p[1] - a[1])")
    f2 = Function("p[2]", "a[2]", "(a[0], a[1])")

    sep_invB = SepInverse(Sb, f1)
    # sep_invA = SepInverse(Sa, f2)

    # sep2 = sep_invB

    # y_init = box

    sep = SepProj(sep_invB, y_init, 0.001)
    return sep

def Erode(Sb, y_init):
    """Compute A+B."""
    f1 = Function("p[2]", "a[2]", "(p[0] + a[0], p[1] + a[1])")
    f2 = Function("p[2]", "a[2]", "(a[0], a[1])")

    sep_invB = SepInverse(~Sb, f1)
    # sep_invA = SepInverse(Sa, f2)

    # sep2 = sep_invB

    # y_init = box

    sep = ~SepProj(sep_invB, y_init, 0.001)
    return sep
