#define binary_op_h(f) \
  \
  Tube f(const Tube& tube_x1, const Tube& tube_x2); \
  Tube f(const ibex::IntervalVector& x1, const Tube& tube_x2); \
  Tube f(const Tube& tube_x1, const ibex::IntervalVector& x2); \
  Tube f(const TrajectoryVector& x1, const Tube& tube_x2); \
  Tube f(const Tube& tube_x1, const TrajectoryVector& x2); \
  TubeSlice f(const TubeSlice& slice_x1, const TubeSlice& slice_x2); \
  TubeSlice f(const ibex::IntervalVector& x1, const TubeSlice& slice_x2); \
  TubeSlice f(const TubeSlice& slice_x1, const ibex::IntervalVector& x2); \
  TubeSlice f(const TrajectoryVector& x1, const TubeSlice& slice_x2); \
  TubeSlice f(const TubeSlice& slice_x1, const TrajectoryVector& x2);
  