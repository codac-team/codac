#define unary_op(f) \
  \
  TubeVector f(const TubeVector& x1) \
  { \
    TubeVector x2(x1); \
    TubeSlice *slice_x1, *first_slice_x1 = x1.getFirstSlice(); \
    TubeSlice *slice_x2, *first_slice_x2 = x2.getFirstSlice(); \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      IntervalVector box = slice_x1->codomain(); \
      for(int i = 0 ; i < box.size() ; i++) \
        box[i] = ibex::f(box[i]); \
      slice_x2->setEnvelope(box); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    slice_x1 = first_slice_x1; \
    slice_x2 = first_slice_x2; \
    while(slice_x1 != NULL) \
    { \
      if(slice_x1 == first_slice_x1) \
      { \
        IntervalVector box = slice_x1->inputGate(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i]); \
        slice_x2->setInputGate(box); \
      } \
      IntervalVector box = slice_x1->outputGate(); \
      for(int i = 0 ; i < box.size() ; i++) \
        box[i] = ibex::f(box[i]); \
      slice_x2->setOutputGate(box); \
      slice_x1 = slice_x1->nextSlice(); \
      slice_x2 = slice_x2->nextSlice(); \
    } \
    return x2; \
  } \
  \
  TubeSlice f(const TubeSlice& x1) \
  { \
    TubeSlice x2(x1); \
    IntervalVector box = x1.codomain(); \
    for(int i = 0 ; i < box.size() ; i++) \
      box[i] = ibex::f(box[i]); \
    x2.setEnvelope(box); \
    box = x1.inputGate(); \
    for(int i = 0 ; i < box.size() ; i++) \
      box[i] = ibex::f(box[i]); \
    x2.setInputGate(box); \
    box = x1.outputGate(); \
    for(int i = 0 ; i < box.size() ; i++) \
      box[i] = ibex::f(box[i]); \
    x2.setOutputGate(box); \
    return x2; \
  }
