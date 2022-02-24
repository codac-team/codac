//============================================================================
//                               P Y I B E X
// File        : ThickBox_impl.hpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  9, 2016
//============================================================================

  /**
   * \brief return True of this is equal to b ( lb and ub )
   */
  bool operator==(const ThickBox& b) const{
    bool res = true;
    for( int i = 0; i < size(); i++) {
      res &= (itvs[i] == b[i]);
    }
    return res;
  }

  /**
   * \brief print value of the ThickBox
   */
  const std::string to_string() const;

  /**
   * \brief Return the size (dimension) if this
   */
  size_t size() const {return itvs.size();}

  /**
   * \brief Return the idx component of this
   */
  const ThickInterval & operator[](size_t idx) const {return itvs[idx];}

  /**
   * \brief Return the idx component of this
   */
  ThickInterval & operator[](size_t idx) {return itvs[idx];}

  /**
   * \brief Return the superset (union of lb and ub) of this
   */
  IntervalVector superset() const;

  /**
   *  \brief Return the subset of this
   *  \note some components of rhe returned IntervalVector can be empty.
   *        In this case, the returned boolean is set to true
   */
  std::pair<IntervalVector, bool> subset_ext() const;

  /**
   * \brief Return the subset of this but without empty components
   */
  std::pair<IntervalVector, std::vector<bool> > inner_box() const;

  /**
   * \brief Return the lower box of this
   */
  IntervalVector lb() const;

  /**
   * \brief Return the upper box of this
   */
  IntervalVector ub() const;

  /**
   * \brief Return true if this intersects box
   * for all, [[a]] \cap b != \emptyset
   */
  bool intersects(const IntervalVector &box) const;

  /**
   * \brief Return true if this intersects box
   * for all, [[a]] \cap b = \emptyset
   */
  bool is_disjoint(const IntervalVector &box) const;

  /**
   * \brief Return true if this is not included in box
   * * for all, [[a]] subset  [b]
   */
  bool is_subset(const IntervalVector &box) const;
  
  /**
   * \brief Return true if this is not included in box
   * * for all, [[a]] subset  [[b]]
   */
  bool is_subset(const ThickBox &box) const;

  /**
   * \brief Return true if this is not included in box
   * for all, [[a]] not subset [b]
   */
  bool is_not_subset(const IntervalVector &box) const;

  /**
   * \brief Return the intersection of this and box
   */
  ThickBox ThickBox::inter(const ThickBox& box) const{
    
  }

  /**
   * \brief Return the intersection of this and box.
   *        Overloading of operator &
   */
  ThickBox operator&(const ThickBox& b);

  /**
   * \brief test is this is inside b
   */
  ThickBoolean is_inside(const IntervalVector& y);


  friend std::ostream& operator<<(std::ostream& o, const ThickBox& a) {
      return o << a.to_string();
   }

   /**
    * \brief return true if the thick box is empty
    */
   bool is_empty() const;
private:


  std::vector<ThickInterval> itvs;
};

