#define binary_op(f) \
  \
  Tube f(const Tube& tube_x1, const Tube& tube_x2) \
  { \
    DomainException::check(tube_x1, tube_x2); \
    StructureException::check(tube_x1, tube_x2); \
    DimensionException::check(tube_x1, tube_x2); \
    Tube new_tube_x(tube_x1); \
    TubeSlice *slice_x1, *first_slice_x1 = new_tube_x.getFirstSlice(); \
    TubeSlice *slice_x2, *first_slice_x2 = tube_x2.getFirstSlice(); \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      IntervalVector box1 = slice_x1->codomain(); \
      IntervalVector box2 = slice_x2->codomain(); \
      for(int i = 0 ; i < box1.size() ; i++) \
        box1[i] = ibex::f(box1[i], box2[i]); \
      slice_x1->setEnvelope(box1); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      if(slice_x1 == first_slice_x1) \
      { \
        IntervalVector box1 = slice_x1->inputGate(); \
        IntervalVector box2 = slice_x2->inputGate(); \
        for(int i = 0 ; i < box1.size() ; i++) \
          box1[i] = ibex::f(box1[i], box2[i]); \
        slice_x1->setInputGate(box1); \
      } \
      IntervalVector box1 = slice_x1->outputGate(); \
      IntervalVector box2 = slice_x2->outputGate(); \
      for(int i = 0 ; i < box1.size() ; i++) \
        box1[i] = ibex::f(box1[i], box2[i]); \
      slice_x1->setOutputGate(box1); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    return new_tube_x; \
  } \
  \
  Tube f(const IntervalVector& x1, const Tube& tube_x2) \
  { \
    DimensionException::check(tube_x2, x1); \
    Tube new_tube_x(tube_x2); \
    TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      slice_x->setEnvelope(ibex::f(x1, slice_x->codomain())); \
      slice_x = slice_x->nextSlice(); \
    } \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      if(slice_x == first_slice_x) \
        slice_x->setInputGate(ibex::f(x1, slice_x->inputGate())); \
      slice_x->setOutputGate(ibex::f(x1, slice_x->outputGate())); \
      slice_x = slice_x->nextSlice(); \
    } \
    return new_tube_x; \
  } \
  \
  Tube f(const Tube& tube_x1, const IntervalVector& x2) \
  { \
    DimensionException::check(tube_x1, x2); \
    Tube new_tube_x(tube_x1); \
    TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      slice_x->setEnvelope(ibex::f(slice_x->codomain(), x2)); \
      slice_x = slice_x->nextSlice(); \
    } \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      if(slice_x == first_slice_x) \
        slice_x->setInputGate(ibex::f(slice_x->inputGate(), x2)); \
      slice_x->setOutputGate(ibex::f(slice_x->outputGate(), x2)); \
      slice_x = slice_x->nextSlice(); \
    } \
    return new_tube_x; \
  } \
  \
  Tube f(const Trajectory& x1, const Tube& tube_x2) \
  { \
    DomainException::check(tube_x2, x1); \
    DimensionException::check(tube_x2, x1); \
    Tube new_tube_x(tube_x2); \
    TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      slice_x->setEnvelope(ibex::f(x1[slice_x->domain()], slice_x->codomain())); \
      slice_x = slice_x->nextSlice(); \
    } \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      if(slice_x == first_slice_x) \
        slice_x->setInputGate(ibex::f(x1[Interval(slice_x->domain().lb())], slice_x->inputGate())); \
      slice_x->setOutputGate(ibex::f(x1[Interval(slice_x->domain().ub())], slice_x->outputGate())); \
      slice_x = slice_x->nextSlice(); \
    } \
    return new_tube_x; \
  } \
  \
  Tube f(const Tube& tube_x1, const Trajectory& x2) \
  { \
    DomainException::check(tube_x1, x2); \
    DimensionException::check(tube_x1, x2); \
    Tube new_tube_x(tube_x1); \
    TubeSlice *slice_x, *first_slice_x = new_tube_x.getFirstSlice(); \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      slice_x->setEnvelope(ibex::f(slice_x->codomain(), x2[slice_x->domain()])); \
      slice_x = slice_x->nextSlice(); \
    } \
    slice_x = first_slice_x; \
    while(slice_x != NULL) \
    { \
      if(slice_x == first_slice_x) \
        slice_x->setInputGate(ibex::f(slice_x->inputGate(), x2[Interval(slice_x->domain().lb())])); \
      slice_x->setOutputGate(ibex::f(slice_x->outputGate(), x2[Interval(slice_x->domain().ub())])); \
      slice_x = slice_x->nextSlice(); \
    } \
    return new_tube_x; \
  } \
  \
  TubeSlice f(const TubeSlice& slice_x1, const TubeSlice& slice_x2) \
  { \
    DomainException::check(slice_x1, slice_x2); \
    DimensionException::check(slice_x1, slice_x2); \
    TubeSlice new_slice_x(slice_x1); \
    new_slice_x.setEnvelope(ibex::f(new_slice_x.codomain(), slice_x2.codomain())); \
    new_slice_x.setInputGate(ibex::f(new_slice_x.inputGate(), slice_x2.inputGate())); \
    new_slice_x.setOutputGate(ibex::f(new_slice_x.outputGate(), slice_x2.outputGate())); \
    return new_slice_x; \
  } \
  \
  TubeSlice f(const Interval& x1, const TubeSlice& slice_x2) \
  { \
    DimensionException::check(slice_x2, x1); \
    TubeSlice new_slice_x(slice_x2); \
    new_slice_x.setEnvelope(ibex::f(x1, new_slice_x.codomain())); \
    new_slice_x.setInputGate(ibex::f(x1, new_slice_x.inputGate())); \
    new_slice_x.setOutputGate(ibex::f(x1, new_slice_x.outputGate())); \
    return new_slice_x; \
  } \
  \
  TubeSlice f(const TubeSlice& slice_x1, const IntervalVector& x2) \
  { \
    DimensionException::check(slice_x1, x2); \
    TubeSlice new_slice_x(slice_x1); \
    new_slice_x.setEnvelope(ibex::f(new_slice_x.codomain(), x2)); \
    new_slice_x.setInputGate(ibex::f(new_slice_x.inputGate(), x2)); \
    new_slice_x.setOutputGate(ibex::f(new_slice_x.outputGate(), x2)); \
    return new_slice_x; \
  } \
  \
  TubeSlice f(const Trajectory& x1, const TubeSlice& slice_x2) \
  { \
    DomainException::check(slice_x2, x1); \
    DimensionException::check(slice_x1, slice_x2); \
    TubeSlice new_slice_x(slice_x2); \
    new_slice_x.setEnvelope(ibex::f(x1[new_slice_x.domain()], new_slice_x.codomain())); \
    new_slice_x.setInputGate(ibex::f(x1[Interval(new_slice_x.domain().lb())], new_slice_x.inputGate())); \
    new_slice_x.setOutputGate(ibex::f(x1[Interval(new_slice_x.domain().ub())], new_slice_x.outputGate())); \
    return new_slice_x; \
  } \
  \
  TubeSlice f(const TubeSlice& slice_x1, const Trajectory& x2) \
  { \
    DomainException::check(slice_x1, x2); \
    DimensionException::check(slice_x1, x2); \
    TubeSlice new_slice_x(slice_x1); \
    new_slice_x.setEnvelope(ibex::f(new_slice_x.codomain(), x2[new_slice_x.domain()])); \
    new_slice_x.setInputGate(ibex::f(new_slice_x.inputGate(), x2[Interval(new_slice_x.domain().lb())])); \
    new_slice_x.setOutputGate(ibex::f(new_slice_x.outputGate(), x2[Interval(new_slice_x.domain().ub())])); \
    return new_slice_x; \
  }
