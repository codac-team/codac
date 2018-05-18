#define binary_op_h(f) \
  \
  Tube f(const Tube& tube_x1, const Tube& tube_x2); \
  Tube f(const ibex::Interval& x1, const Tube& tube_x2); \
  Tube f(const Tube& tube_x1, const ibex::Interval& x2); \
  Tube f(const Trajectory& x1, const Tube& tube_x2); \
  Tube f(const Tube& tube_x1, const Trajectory& x2); \
  TubeSlice f(const TubeSlice& slice_x1, const TubeSlice& slice_x2); \
  TubeSlice f(const ibex::Interval& x1, const TubeSlice& slice_x2); \
  TubeSlice f(const TubeSlice& slice_x1, const ibex::Interval& x2); \
  TubeSlice f(const Trajectory& x1, const TubeSlice& slice_x2); \
  TubeSlice f(const TubeSlice& slice_x1, const Trajectory& x2);
  