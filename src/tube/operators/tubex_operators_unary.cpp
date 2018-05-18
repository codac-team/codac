#define unary_op(f) \
  \
  Tube f(const Tube& x1) \
  { \
    Tube x2(x1); \
    TubeSlice *slice_x1, *first_slice_x1 = x1.getFirstSlice(); \
    TubeSlice *slice_x2, *first_slice_x2 = x2.getFirstSlice(); \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      slice_x2->setEnvelope(ibex::f(slice_x1->codomain())); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      if(slice_x1 == first_slice_x1) \
        slice_x2->setInputGate(ibex::f(slice_x1->inputGate())); \
      slice_x2->setOutputGate(ibex::f(slice_x1->outputGate())); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    return x2; \
  } \
  \
  TubeSlice f(const TubeSlice& x1) \
  { \
    TubeSlice x2(x1); \
    x2.setEnvelope(ibex::f(x1.codomain())); \
    x2.setInputGate(ibex::f(x1.inputGate())); \
    x2.setOutputGate(ibex::f(x1.outputGate())); \
    return x2; \
  }
