#define unary_param_op_h(f, p) \
  \
  Tube f(const Tube& x1, p param); \
  TubeSlice f(const TubeSlice& x1, p param);
