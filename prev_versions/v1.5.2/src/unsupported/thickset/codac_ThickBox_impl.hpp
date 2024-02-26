//============================================================================
//                               P Y I B E X
// File        : ThickBox.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  9, 2016
//============================================================================



//==============================================================================
// INLINE implementation
//==============================================================================
inline const std::string ThickBox::to_string() const{
    std::stringstream ss;
    ss << "{ ";
    for( const  auto & b : itvs){
        ss << b << " , ";
    }
    ss << "  }";
    return ss.str();
}

inline IntervalVector ThickBox::superset() const{
  IntervalVector res(itvs.size());
  for (size_t i = 0; i < itvs.size(); i++){
    res[i] = itvs[i].superset();
  }
  return res;
}

inline std::pair<IntervalVector, bool> ThickBox::subset_ext() const{
  bool status = true; // used to track if subset is no empty
  IntervalVector res(itvs.size());
  for (size_t i = 0; i < itvs.size(); i++){
    res[i] = itvs[i].subset();
    status &= !res[i].is_empty();
  }
  return std::pair<IntervalVector, bool>(res, status);
}

// Return the inner part such as if empty returns
inline std::pair<IntervalVector, std::vector<bool> > ThickBox::inner_box() const {
  IntervalVector res(itvs.size());
  std::vector<bool> status(itvs.size(), false);
  for (size_t i = 0; i < itvs.size(); i++){
    res[i] = itvs[i].subset();
    status[i]=res[i].is_empty();
    if (status[i]) {
      res[i] = codac::Interval( std::max(itvs[i].lb().lb(), itvs[i].ub().lb() ), std::min(itvs[i].lb().ub(), itvs[i].ub().ub() ) );
    }
  }
  return std::pair<IntervalVector, std::vector<bool> >(res, status);
}


inline IntervalVector ThickBox::lb() const{
  IntervalVector res(itvs.size());
  for (int i = 0; i < itvs.size(); i++){
    res[i] = itvs[i].lb();
  }
  return res;
}

inline IntervalVector ThickBox::ub() const{
  IntervalVector res(itvs.size());
  for (int i = 0; i < itvs.size(); i++){
    res[i] = itvs[i].ub();
  }
  return res;
}

inline bool ThickBox::intersects(const IntervalVector& box) const {
  bool res = true;
  for (int i = 0; i < box.size(); i++){
    res &= itvs[i].intersects(ThickInterval(box[i]));
  }
  return res;
}

inline bool ThickBox::is_not_subset(const IntervalVector& box) const {
  bool res = false;
  for (int i = 0; i < box.size(); i++){
    res |= itvs[i].is_not_subset(ThickInterval(box[i]));
  }
  return res;
}

inline bool ThickBox::is_subset(const IntervalVector &box) const{
  bool res = true;
  for (int i = 0; i < box.size(); i++){
    res &= itvs[i].is_subset(ThickInterval(box[i]));
  }
  return res;
}

inline bool ThickBox::is_subset(const ThickBox &box) const{
  bool res = false;
  for (int i = 0; i < box.size(); i++){
    res &= itvs[i].is_subset(box[i]);
  }
  return res;
}

inline ThickBox ThickBox::inter(const ThickBox& b) const{
  assert(this->size() == b.size());
  ThickBox res(b.size(), ThickInterval());
  for (int i = 0; i < this->size(); i++){
    res[i] = itvs[i] & b[i];
    // std::cout << res[i].is_empty() << "\n";
    if (res[i].is_empty()) return ThickBox(IntervalVector::empty(itvs.size()));
  }
  return res;
}

inline ThickBox ThickBox::operator&(const ThickBox& b){
	return this->inter(b);
}

inline ThickBoolean ThickBox::is_inside(const IntervalVector& y){
  if (superset().is_subset(y)){
    return IN;
  } else if (superset().is_disjoint(y)){
    return OUT;
  } else {
    bool b1 = intersects(y);
    bool b2 = is_not_subset(y);
    if ( b1 && b2 ) return MAYBE;
    if (!b1 && b2) return MAYBE_OUT;
    if (!b2 && b1) return MAYBE_IN;
  }
  return UNK;
}

inline bool ThickBox::is_empty() const {
  for( const  auto & b : itvs){
    if ( b.is_empty() ) return true;
  }
  return false;
}
