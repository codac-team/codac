/*#define unary_param_op(f, p) \
  \
  Tube f(const Tube& x1, p param) \
  { \
    Tube x2(x1); \
    TubeSlice *slice_x1, *first_slice_x1 = x1.getFirstSlice(); \
    TubeSlice *slice_x2, *first_slice_x2 = x2.getFirstSlice(); \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      slice_x2->setEnvelope(ibex::f(slice_x1->codomain(), param)); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      if(slice_x1 == first_slice_x1) \
        slice_x2->setInputGate(ibex::f(slice_x1->inputGate(), param)); \
      slice_x2->setOutputGate(ibex::f(slice_x1->outputGate(), param)); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    return x2; \
  } \
  \
  TubeSlice f(const TubeSlice& x1, p param) \
  { \
    TubeSlice x2(x1); \
    x2.setEnvelope(ibex::f(x1.codomain(), param)); \
    x2.setInputGate(ibex::f(x1.inputGate(), param)); \
    x2.setOutputGate(ibex::f(x1.outputGate(), param)); \
    return x2; \
  }
*/