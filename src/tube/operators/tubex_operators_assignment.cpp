#define assignment_op(f) \
  \
  Tube& Tube::f(const Tube& tube_x) \
  { \
    StructureException::check(*this, tube_x); \
    DimensionException::check(*this, tube_x); \
    IntervalVector y(tube_x.dim()); \
    TubeSlice *slice, *first_slice = getFirstSlice(); \
    TubeSlice *slice_x, *first_slice_x = tube_x.getFirstSlice(); \
    slice = first_slice; \
    slice_x = first_slice_x; \
    while(slice != NULL) \
    { \
      y = slice->codomain(); \
      y.f(slice_x->codomain()); \
      slice->setEnvelope(y); \
      slice = slice->nextSlice(); \
      slice_x = slice_x->nextSlice(); \
    } \
    slice = first_slice; \
    slice_x = first_slice_x; \
    while(slice != NULL) \
    { \
      if(slice == first_slice) \
      { \
        y = slice->inputGate(); \
        y.f(slice_x->inputGate()); \
        slice->setInputGate(y); \
      } \
      y = slice->outputGate(); \
      y.f(slice_x->outputGate()); \
      slice->setOutputGate(y); \
      slice = slice->nextSlice(); \
      slice_x = slice_x->nextSlice(); \
    } \
    return *this; \
  } \
  \
  Tube& Tube::f(const Trajectory& traj_x) \
  { \
    DomainException::check(*this, traj_x); \
    DimensionException::check(*this, traj_x); \
    IntervalVector y(traj_x.dim()); \
    TubeSlice *slice, *first_slice = getFirstSlice(); \
    slice = first_slice; \
    while(slice != NULL) \
    { \
      y = slice->codomain(); \
      y.f(traj_x[slice->domain()]); \
      slice->setEnvelope(y); \
      slice = slice->nextSlice(); \
    } \
    slice = first_slice; \
    while(slice != NULL) \
    { \
      if(slice == first_slice) \
      { \
        y = slice->inputGate(); \
        y.f(traj_x[Interval(slice->domain().lb())]); \
        slice->setInputGate(y); \
      } \
      y = slice->outputGate(); \
      y.f(traj_x[Interval(slice->domain().ub())]); \
      slice->setOutputGate(y); \
      slice = slice->nextSlice(); \
    } \
    return *this; \
  } \
  \
  TubeSlice& TubeSlice::f(const TubeSlice& slice_x) \
  { \
    DomainException::check(*this, slice_x); \
    DimensionException::check(*this, slice_x); \
    IntervalVector y = codomain(); y.f(slice_x.codomain()); setEnvelope(y); \
    y = inputGate(); y.f(slice_x.inputGate()); setInputGate(y); \
    y = outputGate(); y.f(slice_x.outputGate()); setOutputGate(y); \
    return *this; \
  } \
  \
  TubeSlice& TubeSlice::f(const Trajectory& traj_x) \
  { \
    DomainException::check(*this, traj_x); \
    DimensionException::check(*this, traj_x); \
    IntervalVector y = codomain(); y.f(traj_x[domain()]); setEnvelope(y); \
    y = inputGate(); y.f(traj_x[Interval(domain().lb())]); setInputGate(y); \
    y = outputGate(); y.f(traj_x[Interval(domain().ub())]); setOutputGate(y); \
    return *this; \
  }
