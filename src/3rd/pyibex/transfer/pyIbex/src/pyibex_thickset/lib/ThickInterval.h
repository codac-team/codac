//============================================================================
//                               P Y I B E X
// File        : ThickInterval.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Aug  31, 2016
//============================================================================

#ifndef __THICKINTERVAL__
#define __THICKINTERVAL__

#include <ibex_Interval.h>
#include <ibex_IntervalVector.h>
#include <ThickBoolean.h>
#include <sstream>

using ibex::ThickBoolean;
using ibex::Interval;

#define CHECK_THICKINTERVAL(lb_, ub_) \
  if(lb_.is_empty() || ub_.is_empty() || \
     lb_.ub() > ub_.ub() || ub_.lb() < lb_.lb()){ \
    lb_.set_empty(); \
    ub_.set_empty(); \
  }

class ThickInterval
{
public:
  // ThickInterval(ibex::Interval& lb, ibex::Interval& ub);
  ThickInterval(const ibex::Interval& lb, const ibex::Interval& ub);
  ThickInterval(const ibex::Interval& bounds);
  ThickInterval(const double& lb,const double& ub);
  ThickInterval(){};


  /**
   * \brief return True of this is equal to b ( lb and ub )
   */
  bool operator==(const ThickInterval&b) const{
    return ( lb() == b.lb() && ub() == b.ub() );
  }
  /**
   * \brief return true if this intersects b
   * for all [[a]] \cap [[b]] != \emptyset
   */
  bool intersects(const ThickInterval& b) const;

  /**
   * \brief return true if this intersects b
   * for all [[a]] \cap [[b]] = \emptyset
   */
  bool is_disjoint(const ThickInterval& b) const;

  /**
   * \brief Test if this is not included in b
   * for all [[a]] not \subset [[b]]
   */
  bool is_not_subset(const ThickInterval& b) const;

  /**
   * \brief Test if this is subset in <b>
   * for all, [[a]] \subset [[b]] 
   */
  bool is_subset(const ThickInterval& b) const;

  /**
   * \brief Return the lower bound of the thickInterval
   */
  ibex::Interval lb() const { return this->lb_;}

  /**
   * \brief Return the upper bound of the thickInterval
   */
  ibex::Interval ub() const  { return this->ub_;}

	/**
	 * \brief	return true if the thickInterval is an ibex::Interval
	 *  ie. the lower bound and the upper bound are singleton
	 */
	bool is_interval();

	/**
	 * \brief return true if the ThickInterval
	 * ie. one of its bound is a degenerated interval
	 */
	bool is_degenerated();

  /**
   * \brief return true if the thick interval is empty
   */
  bool is_empty() const;

	/**
	 * \brief return the intersection of this and b
	 */
	ThickInterval inter(const ThickInterval& b) const;
  ThickInterval operator&(const ThickInterval& b) const;

	/**
	 * \brief return the super set of the thick interval
	 * ie. the union of the lower and upper bound
	 */
	ibex::Interval superset() const;

  /**
   * \brief return the subset of the thick interval
   * ie. the interval between of the lower and upper bound
   */
  ibex::Interval subset() const;

  /**
   * \brief print the thick interval
   */
  const std::string to_string() const;

  /**
   * \brief check if this is a subset ( with the thick interval meaning) of y
   */
  ThickBoolean is_inside(const Interval& y) const;
  /**
	 * \brief print the thick interval
	 */
	friend std::ostream& operator<<(std::ostream& o, const ThickInterval& a) {
      return o << "[" << a.lb() << " ; " << 	a.ub() << "] ";
   }


private:
  /**
   * \brief lower and upper bound of the thick interval
   */
  ibex::Interval lb_, ub_;



};

//==========================================================================================
//=======================  Inline implementation  ==========================================
//==========================================================================================

// constexpr int S::sz = 256;

inline ThickInterval::ThickInterval(const ibex::Interval& lb, const ibex::Interval& ub):
    lb_(lb), ub_(ub) { CHECK_THICKINTERVAL(this->lb_, this->ub_);
}

inline ThickInterval::ThickInterval(const ibex::Interval& bounds):
    lb_(bounds.lb()), ub_(bounds.ub()) { CHECK_THICKINTERVAL(this->lb_, this->ub_); }

inline ThickInterval::ThickInterval(const double& lb, const double& ub):
    lb_(lb), ub_(ub) { CHECK_THICKINTERVAL(this->lb_, this->ub_); }

inline bool ThickInterval::intersects(const ThickInterval &b) const{
    ibex::Interval tmp1 = b.lb() - this->ub();
    ibex::Interval tmp2 = this->lb() - b.ub();
    return ( tmp1.ub() <= 0 && tmp2.ub() <= 0);
}

inline bool ThickInterval::is_disjoint(const ThickInterval &b) const{
    ibex::Interval tmp1 = this->ub() - b.lb();
    ibex::Interval tmp2 = b.ub() - this->lb();
    return ( tmp1.ub() <= 0 && tmp2.ub() <= 0);
}

inline bool ThickInterval::is_not_subset(const ThickInterval& b) const{
    ibex::Interval tmp1 = this->lb() - b.lb();
    ibex::Interval tmp2 = b.ub() - this->ub();
    return ( tmp1.ub() <= 0 || tmp2.ub() <= 0);
}

inline bool ThickInterval::is_subset(const ThickInterval& b) const{
    ibex::Interval tmp1 = b.lb() - this->lb();
    ibex::Interval tmp2 = this->ub() - b.ub();
    return ( tmp1.ub() <= 0 && tmp2.ub() <= 0);
}

inline bool ThickInterval::is_interval() {
  return this->lb().is_degenerated() && this->ub().is_degenerated();
}
inline bool ThickInterval::is_degenerated() {
  return this->lb().is_degenerated() || this->ub().is_degenerated();
}

inline bool ThickInterval::is_empty() const{
	return lb().is_empty() && ub().is_empty();
}

inline ibex::Interval ThickInterval::superset() const{
	return ibex::Interval(this->lb().lb(), this->ub().ub() );
}

inline ibex::Interval ThickInterval::subset() const{
  if (lb().ub() < ub().lb())
    return ibex::Interval(this->lb().ub(), this->ub().lb() );
  return ibex::Interval::EMPTY_SET;
}


inline ThickInterval ThickInterval::inter(const ThickInterval& b) const{
	if  ( !(this->superset() & b.superset()).is_empty() ){
    Interval new_lb = ibex::max(this->lb(), b.lb());
    Interval new_ub = ibex::min(this->ub(), b.ub());

		return ThickInterval( new_lb &  Interval(NEG_INFINITY, new_ub.ub() ),
                          new_ub & Interval(new_lb.lb(), 1e10) );
	}
	return ThickInterval( ibex::Interval::EMPTY_SET,ibex::Interval::EMPTY_SET );
}

inline ThickInterval ThickInterval::operator&(const ThickInterval& b) const{
	return this->inter(b);
}

inline ThickBoolean ThickInterval::is_inside(const Interval& y) const{
  if ( this->superset().is_subset(y) ){
    return IN;
  } else if ( this->superset().is_disjoint(y) ){
    return OUT;
  } else {
    bool b1 = this->intersects(y);
    bool b2 = this->is_not_subset(y);
    if ( b1 && b2 ) return MAYBE;
    if (!b1 && b2) return MAYBE_OUT;
    if (!b2 && b1) return MAYBE_IN;
  }
  return UNK;
}

inline  const std::string ThickInterval::to_string() const{
    std::stringstream ss;
    ss << "[" <<  lb() << " ; " << ub() << "] " ;
    return ss.str();
}



#endif // __THICKINTERVAL__
