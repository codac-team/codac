from codac import *


class lie_group_ex4_separator(Sep):
    def __init__(self, reference, constraint):
        super().__init__(reference.size())
        self.a = reference
        self.constraintVector = constraint
        self.phi = Function("x[3]","z[3]","w[3]",
                            "(x[0] + cos(x[2]-z[2])*(w[0]-z[0]) - sin(x[2]-z[2])*(w[1]-z[1]); \
                                 x[1] + sin(x[2]-z[2])*(w[0]-z[0]) + cos(x[2]-z[2])*(w[1]-z[1]); \
                                 x[2] + w[2] - z[2])")
        self.sepPhi = SepFwdBwd(self.phi, self.constraintVector)

    def separate(self, x_in, x_out):

        x4 = x_in[3]

        Xinit = IntervalVector(x_in)
        z = IntervalVector(4)
        z.init(Interval.ALL_REALS)
        w = IntervalVector(4)
        w.init(Interval.ALL_REALS)

        if(x4.is_empty()):
            x_out[0].set_empty()
            x_out[1].set_empty()
            x_out[2].set_empty()
            x_out[3].set_empty()
            return x_in, x_out

        z = z & self.a(-x4)
        if (z[0].is_empty() or z[1].is_empty() or z[2].is_empty() or z[3].is_empty()):
            x_out[0].set_empty()
            x_out[1].set_empty()
            x_out[2].set_empty()
            x_out[3].set_empty()
            return x_in, x_out

        w = w & self.a(0)
        if (w[0].is_empty() or w[1].is_empty() or w[2].is_empty() or w[3].is_empty()):
            x_out[0].set_empty()
            x_out[1].set_empty()
            x_out[2].set_empty()
            x_out[3].set_empty()
            return x_in, x_out

        fullBoxIn = IntervalVector(9)
        fullBoxIn[0] = x_in[0].copy()
        fullBoxIn[1] = x_in[1].copy()
        fullBoxIn[2] = x_in[2].copy()
        fullBoxIn[3] = z[0].copy()
        fullBoxIn[4] = z[1].copy()
        fullBoxIn[5] = z[2].copy()
        fullBoxIn[6] = w[0].copy()
        fullBoxIn[7] = w[1].copy()
        fullBoxIn[8] = w[2].copy()

        fullBoxOut = IntervalVector(fullBoxIn)
        z_out = IntervalVector(z)
        w_out = IntervalVector(w)
        x4out = Interval(x4)

        self.sepPhi.separate(fullBoxIn, fullBoxOut)

        x_in[0] = fullBoxIn[0].copy()
        x_in[1] = fullBoxIn[1].copy()
        x_in[2] = fullBoxIn[2].copy()
        z[0] = fullBoxIn[3].copy()
        z[1] = fullBoxIn[4].copy()
        z[2] = fullBoxIn[5].copy()
        w[0] = fullBoxIn[6].copy()
        w[1] = fullBoxIn[7].copy()
        w[2] = fullBoxIn[8].copy()

        x_out[0] = fullBoxOut[0].copy()
        x_out[1] = fullBoxOut[1].copy()
        x_out[2] = fullBoxOut[2].copy()
        z_out[0] = fullBoxOut[3].copy()
        z_out[1] = fullBoxOut[4].copy()
        z_out[2] = fullBoxOut[5].copy()
        w_out[0] = fullBoxOut[6].copy()
        w_out[1] = fullBoxOut[7].copy()
        w_out[2] = fullBoxOut[8].copy()


        if (self.a.invert(z, -x4)!= None):
            x4 = x4 & (-self.a.invert(z, -x4))
        else:
            x4.set_empty()
        if (self.a.invert(z_out, -x4out) != None):
            x4out = x4out & (-self.a.invert(z_out, -x4out))
        else:
            x4out.set_empty()

        assert((x4 | x4out) == Xinit[3])

        if(x4.is_empty()):
            x_in[0].set_empty()
            x_in[1].set_empty()
            x_in[2].set_empty()
            x_in[3].set_empty()
            x_out[3] = Interval(x4out)
            return x_in, x_out

        if (x4out.is_empty()):
            x_out[0].set_empty()
            x_out[1].set_empty()
            x_out[2].set_empty()
            x_out[3].set_empty()
            x_in[3] = Interval(x4)
            return x_in, x_out


        x_in[3] = x4.copy()
        x_out[3] = x4out.copy()

        return x_in, x_out