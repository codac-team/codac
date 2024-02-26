//============================================================================
//                               P Y I B E X
// File        : ThickBoolean.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Nov  9, 2015
//============================================================================

#ifndef __THICK_BOOLEAN_H__
#define __THICK_BOOLEAN_H__

#include <iostream>
#include <tuple>
#include <functional>
namespace ibex{

enum class ThickBoolean {
    OUT_=0, 			// [0]
    IN_,					// [1]
    MAYBE_,				// [?]
    MAYBE_OUT_,		// [0, ?]
    MAYBE_IN_,		// [?, 1]
    UNK_,					// [0, 1]
    EMPTY_				// [empty]
};



// to avoid using ThickBoolean::XXXX
#define OUT ThickBoolean::OUT_
#define IN ThickBoolean::IN_
#define MAYBE ThickBoolean::MAYBE_
#define MAYBE_IN ThickBoolean::MAYBE_IN_
#define MAYBE_OUT ThickBoolean::MAYBE_OUT_
#define EMPTY ThickBoolean::EMPTY_
#define UNK ThickBoolean::UNK_


/**
 * \brief logical <and> between two thickbooleans
 * [0] && [1] = [0]
 */
ThickBoolean opAnd(const ThickBoolean& x, const ThickBoolean& y);
ThickBoolean operator&&(const ThickBoolean& x, const ThickBoolean& y);

/**
 * \brief logical <or> between two thickbooleans
 * [0] || [1] = [1], [?] || [?,1] = [?, 1]
 */
ThickBoolean opOr(const ThickBoolean& x, const ThickBoolean& y);
ThickBoolean operator||(const ThickBoolean& x, const ThickBoolean& y);

/**
 * \brief  intersection between two thickbooleans
 * [0] & [1] = empty, [?] && [?,1] = [?, 1]
 */
ThickBoolean opInter(const ThickBoolean& x, const ThickBoolean& y);
ThickBoolean operator&(const ThickBoolean& x, const ThickBoolean& y);

/**
 * \brief union between two thickbooleans
 * [0] || [1] = [0,1], [?] || [?,1] = [?, 1]
 */
ThickBoolean opUnion(const ThickBoolean& x, const ThickBoolean& y);
ThickBoolean operator|(const ThickBoolean& x, const ThickBoolean& y);

/**
 * \brief return true if b is a singleton
 * ie. b \in { [1], [0], [?] }
 */
bool is_singleton(const ThickBoolean& b);

/**
 * \brief return true if <b> \in { [1], [?,1], [?] }
 */
bool can_be_inside(const ThickBoolean& b);

/**
 * \brief return true if <b> \in { [0], [0,?], [?] }
 */
bool can_be_outside(const ThickBoolean& b);



/**
 * \brief print the thick boolean
 */
std::ostream& operator<<(std::ostream& os, const ThickBoolean& obj);


//==============================================================================
//=======================  Inline implementation  ==============================
//==============================================================================

inline ThickBoolean opAnd(const ThickBoolean& x, const ThickBoolean& y){
  if (x == EMPTY || y == EMPTY ) return EMPTY;
  if (x == OUT || y == OUT ) return OUT;
  if (x == UNK || y == UNK) return UNK;
  if (x == MAYBE_OUT || y == MAYBE_OUT) return MAYBE_OUT;
  if (x == MAYBE || y == MAYBE) return MAYBE;
  if (x == MAYBE_IN || y == MAYBE_IN) return MAYBE_IN;
  if (x == y) return x;
  return IN;
}

inline ThickBoolean operator&&(const ThickBoolean& x, const ThickBoolean& y) {
	 return opAnd(x,y);
 }

//------------------------------------------------------------------------------


const static ThickBoolean table_or[6][6] = {
	{ OUT,   			IN, 	 MAYBE, MAYBE_OUT,        MAYBE_IN,    UNK },
	{ IN, 				IN, 	    IN, IN,      IN,       IN},
	{ MAYBE,   		IN, 	 MAYBE,    MAYBE,  MAYBE_IN, MAYBE_IN},
	{ MAYBE_OUT,	IN, 	 MAYBE,  MAYBE_OUT,  MAYBE_IN,      UNK},
	{ MAYBE_IN, 	IN, MAYBE_IN,   MAYBE_IN,        MAYBE_IN, MAYBE_IN},
	{ UNK,   			IN, MAYBE_IN,      UNK,  MAYBE_IN, UNK}
};

inline ThickBoolean opOr(const ThickBoolean& x, const ThickBoolean& y){
  // if (x == EMPTY || y == EMPTY ) return EMPTY;
  // if (x == IN || y == IN ) return IN;
  // if (x == MAYBE_IN || y == MAYBE_IN) return MAYBE_IN;
  // if (x == MAYBE || y == MAYBE) return MAYBE;
  // if (x == MAYBE_OUT || y == MAYBE_OUT) return MAYBE_OUT;
  // if (x == UNK || y == UNK) return UNK;
  // if (x == y) return x;
  // return OUT;

	if (x == EMPTY || y == EMPTY ) return EMPTY;
  return table_or[(int) x][(int) y];
}


inline ThickBoolean operator||(const ThickBoolean& x, const ThickBoolean& y){
	return opOr(x,y);

}

//------------------------------------------------------------------------------
// const static ThickBoolean table_Inter[6][6] = {
//   { OUT,   EMPTY, OUT,        OUT,        EMPTY,    OUT },
//   { EMPTY, IN,    IN,         EMPTY,      IN,       IN},
//   { OUT,   IN,    MAYBE,      MAYBE_OUT,  MAYBE_IN, MAYBE},
//   { OUT,   EMPTY, MAYBE_OUT,  MAYBE_OUT,  UNK,      MAYBE_OUT},
//   { EMPTY, IN,    MAYBE_IN,   UNK,        MAYBE_IN, MAYBE_IN},
//   { OUT,   IN,    MAYBE,      MAYBE_OUT,  MAYBE_IN, UNK}
// };

// Version qui marche avec conflit !!
const static ThickBoolean table_Inter[6][6] = {
  { OUT,   EMPTY,        OUT,        OUT,    EMPTY,  OUT },
  { EMPTY,    IN,         IN,      EMPTY,       IN,  IN },
  { OUT,      IN,      MAYBE,  MAYBE_OUT,  MAYBE_IN, UNK },
  { OUT,   EMPTY,  MAYBE_OUT,  MAYBE_OUT,       UNK, UNK },
  { EMPTY,    IN,   MAYBE_IN,       UNK,   MAYBE_IN, UNK },
  { OUT,      IN,        UNK,       UNK,        UNK, UNK }
};

// const static ThickBoolean table_Inter[6][6] = {
//   { OUT,   EMPTY,        OUT,        OUT,    UNK,  OUT },
//   { EMPTY,    IN,         IN,      UNK,       IN,  IN },
//   { OUT,      IN,      MAYBE,  MAYBE_OUT,  MAYBE_IN, UNK },
//   { OUT,   UNK,  MAYBE_OUT,  MAYBE_OUT,       UNK, UNK },
//   { UNK,    IN,   MAYBE_IN,       UNK,   MAYBE_IN, UNK },
//   { OUT,      IN,        UNK,       UNK,        UNK, UNK }
// };




inline ThickBoolean opInter(const ThickBoolean& x, const ThickBoolean& y){
  if (x == EMPTY || y == EMPTY ) return EMPTY;
  return table_Inter[(int) x][(int) y];
}

inline ThickBoolean operator&(const ThickBoolean& x, const ThickBoolean& y){
	return opInter(x,y);
}

//------------------------------------------------------------------------------

const static ThickBoolean table_Union[6][6] = {
  { OUT,        UNK,      MAYBE_OUT, MAYBE_OUT, UNK,      UNK },
  { UNK,        IN,       MAYBE_IN,  UNK,       MAYBE_IN, UNK },
  { MAYBE_OUT,  MAYBE_IN, MAYBE,     MAYBE_OUT, MAYBE_IN, UNK },
  { MAYBE_OUT,  UNK,      MAYBE_OUT, MAYBE_OUT, UNK,      UNK },
  { UNK,        MAYBE_IN, MAYBE_IN,  UNK,       MAYBE_IN, UNK },
  { UNK,        UNK,      UNK,       UNK,       UNK,      UNK }
};


inline ThickBoolean opUnion(const ThickBoolean& x, const ThickBoolean& y){
  if (x == EMPTY) return y;
  if (y == EMPTY) return x;
  return table_Union[(int) x][(int) y];
}

inline ThickBoolean operator|(const ThickBoolean& x, const ThickBoolean& y){
	return opUnion(x,y);
}

//------------------------------------------------------------------------------

const static ThickBoolean table_ThickInter[6][6] = {
  { OUT,   EMPTY , EMPTY,       OUT,  OUT,      OUT},
  { EMPTY,    IN , EMPTY,        IN,       IN, IN },
  { EMPTY, EMPTY , MAYBE,     MAYBE, MAYBE, MAYBE },
  { EMPTY,    IN , MAYBE, MAYBE_OUT, MAYBE,      MAYBE_OUT },
  { EMPTY,    IN , MAYBE,       MAYBE,       MAYBE_IN, MAYBE_IN },
  { EMPTY,    IN , MAYBE,       MAYBE_OUT,       MAYBE_IN,      UNK }
};


inline ThickBoolean opThickInter(const ThickBoolean& x, const ThickBoolean& y){
  if (x == EMPTY) return y;
  if (y == EMPTY) return x;
  return table_ThickInter[(int) x][(int) y];
}

//------------------------------------------------------------------------------
inline bool is_singleton(const ThickBoolean& b){
	return ( (b == IN) || (b == OUT) || (b == MAYBE) || (b == EMPTY) );
}

//------------------------------------------------------------------------------

inline bool can_be_inside(const ThickBoolean& b){
	return ( (b == IN) || (b == MAYBE_IN) || (b == MAYBE));
}

inline bool can_be_outside(const ThickBoolean& b){
	return ( (b == OUT) || (b == MAYBE_OUT) || (b == MAYBE) );
}

//------------------------------------------------------------------------------
inline ThickBoolean opNot(const ThickBoolean& x){
  if (x == IN) return OUT;
  if (x == OUT) return IN;
  if (x == MAYBE_IN) return MAYBE_OUT;
  if (x == MAYBE_OUT) return MAYBE_IN;
  return x;
}

inline ThickBoolean opUpper(const ThickBoolean& x){
  if (x == MAYBE_IN || x == MAYBE) return IN;
  else if (x == MAYBE_OUT) return UNK;
  return x;
}

inline ThickBoolean opLower(const ThickBoolean& x){
  if (x == MAYBE_OUT || x == MAYBE) return OUT;
  else if (x == MAYBE_IN) return UNK;
  return x;
}

inline ThickBoolean opInner(const ThickBoolean& x){
  if (x == MAYBE_OUT || x == OUT) return MAYBE;
  // else if (x == MAYBE_IN) return UNK;
  return x;
}

inline ThickBoolean opRestrict(const ThickBoolean& x, const ThickBoolean& y){
  return opAnd(x, opNot(y));
}

inline ThickBoolean opInterLB(const ThickBoolean& x, const ThickBoolean& y){
  // ThickBoolean xx = ( x == MAYBE_IN || x == MAYBE ) ? IN : x;
  ThickBoolean yy = ( y == MAYBE_IN || y == MAYBE ) ? IN : y;
  ThickBoolean tmp = opInter(x,yy);
  return (tmp != MAYBE_OUT) ? tmp : UNK;
}

inline ThickBoolean opInterUB(const ThickBoolean& x, const ThickBoolean& y){
  // ThickBoolean xx = ( x == MAYBE_OUT || x == MAYBE ) ? OUT : x;
  ThickBoolean yy = ( y == MAYBE_OUT || y == MAYBE ) ? OUT : y;
  ThickBoolean tmp = opInter(x,yy);
  return (tmp != MAYBE_IN) ? tmp : UNK;
}

inline ThickBoolean opXor(const ThickBoolean& x, const ThickBoolean& y){
  // ThickBoolean xx = ( x == MAYBE_OUT || x == MAYBE ) ? OUT : x;

  return opUnion( opRestrict(x, y), opRestrict(y, x));
}


inline ThickBoolean opUNK(const ThickBoolean &x){
  return (x == UNK) ? x : UNK;
}


//
inline ThickBoolean opSupSub(const ThickBoolean &in, const ThickBoolean &out){
// Boolean operation for combining 2 tests such that:
//    - in => IN if inside A subset, OUT if outside A subset UNK otherwise
//    - out => IN if inside A supset, OUT if outside A supset UNK otherwise
  // std::cerr << in <<  " " << out << "\n";
  if (in == OUT){
    if (out == OUT) return OUT;
    if (out == IN) return MAYBE;
    if (out == UNK) return MAYBE_OUT;
  } else if (in == IN){
    if (out == OUT) return EMPTY;
    if (out == IN) return IN;
    if (out == UNK) return IN;
  } else if (in == UNK){
    if (out == OUT) return OUT;
    if (out == IN) return MAYBE_IN;
    if (out == UNK) return UNK;
  }

  return in | out;
}

//------------------------------------------------------------------------------

inline std::tuple<Interval, Interval> thickBooleanSum(const std::vector<ThickBoolean>& l_boolean){
  Interval sum_lb = Interval(0);
  Interval sum_ub = Interval(0);
  for (int i=0; i<l_boolean.size(); i++) {
    switch(l_boolean[i]){
      case IN:
        sum_lb += 1.;
        sum_ub += 1.;
        break;
      case MAYBE:
        sum_ub += 1.;
        break;
      case MAYBE_OUT:
        sum_ub += Interval(0, 1);
        break;
      case MAYBE_IN:
        sum_lb += Interval(0, 1);
        sum_ub += 1.;
        break;
      case UNK:
        sum_lb += Interval(0, 1);
        sum_ub += Interval(0, 1);
    }
  }
  return std::make_tuple(sum_lb, sum_ub);
}

//------------------------------------------------------------------------------

inline std::ostream& operator<<(std::ostream& os, const ThickBoolean& obj){
  switch(obj){
    case IN: os << "IN "; break;
    case OUT: os << "OUT "; break;
    case MAYBE: os << "MAYBE "; break;
    case MAYBE_IN: os << "MAYBE_IN "; break;
    case MAYBE_OUT: os << "MAYBE_OUT "; break;
    case EMPTY: os << "EMPTY "; break;
    case UNK: os << "UNK "; break;
  }
  return os;
}




}	// end namespace ibex


#endif //__THICK_BOOLEAN_H__
