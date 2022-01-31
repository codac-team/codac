//============================================================================
//                               P Y I B E X
// File        : sweepTest.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Oct  1, 2016
//============================================================================
#define _USE_MATH_DEFINES
#include <cmath>

#include "sweepTest.h"
namespace ibex {

SweepTest::SweepTest(const Tube& x, const Tube& y, const Tube& ux, const Tube&uy, double _L):
    ThickTest(2),
    x(x), y(y),ux(ux), uy(uy), L(x.size(), _L) {
}

SweepTest::SweepTest(const Tube& x, const Tube& y, const Tube& ux, const Tube&uy, const Tube& L):
    ThickTest(2),
    x(x), y(y),ux(ux), uy(uy), L(L) {
}


ThickBoolean SweepTest::test(const IntervalVector& _X){
    // backtrack contains limits and box for the parent box.
    // If the list is empty, we look between t0 and tMax
    // else we find the smallest box which contains X
    // which should be the parent box.
    IntervalVector X(_X);
    vector<IntBound>limits;
    limits.push_back( {{ 0, static_cast<int>(x.size())-1}});
    return fast_test(X,limits);
    if (this->backtrack.empty()){
        limits.push_back( {{ 0, static_cast<int>(x.size())-1}});
    } else {
        double dim_min = std::numeric_limits<double>::max();
        std::list<Record>::iterator it_dst;
        for (std::list<Record>::iterator it = backtrack.begin(); it != backtrack.end(); it++){
            if(X.is_subset((*it).B)){
                if ((*it).B.max_diam() < dim_min){
                    dim_min = (*it).B.max_diam();
                    it_dst = it;
                }
            }
        }
        limits = (*it_dst).limits;
        (*it_dst).count--;
        if( (*it_dst).count == 0){
            this->backtrack.erase(it_dst);
        }
    }
    ThickBoolean ret = fast_test(X,limits);
    if (ret == UNK){
        this->backtrack.push_back(Record(X, limits));
    }

    return ret;
}


ThickBoolean SweepTest::fast_test(IntervalVector& X,
                       vector<IntBound> &T,
                       bool debug){
    vector<IntBound> T_new;
    int nb_no = 0;
    bool is_in = false;
    // is_enable = true;
    for (int k = 0; k < T.size(); k++){
//        if(T[k].is_empty()){
//            nb_no++;
//            continue;
//        }
        int i0 = T[k][0]; //round(T[k].lb());
        int i1 = T[k][1]; //round(T[k].ub());

        Tube v1 = genV1(X,i0, i1); //, x, y, ux, uy, i0, i1);
        Tube v2 = genV2(X,i0, i1); //, x, y, ux, uy, i0, i1);

        vector<IntBound> T2;
        vector<IntBound> T1 = extract_zeros(v1, T2);
        debug = false;

        // V1 change de signe
        if(T1.size() > 0 && is_inside(v2, T1) == true)
            return IN;

        T1.insert(T1.end(), T2.begin(), T2.end());
        bool is_out = is_outside(v2, T1);
        for(int i = 0; i < T1.size(); i++){
            T1[i][0] += i0;
            T1[i][1] += i0;
        }
        T_new.insert(T_new.end(), T1.begin(), T1.end());

        if( is_out == true){
            nb_no++;
            continue;
        }
    }

    if(nb_no == T.size()){
        return OUT;
    }

    ThickBoolean res = UNK;
    if (  X.max_diam()  <= x[x.size()-1].diam()*5){


// #define __SECOND_METHOD__
#ifdef __SECOND_METHOD__
        res = test_exist_v2(X,T_new, is_in);
        if (is_singleton(res))
            return res;
#else
        res = test_exist_v(X,T_new, is_in);
        if (is_singleton(res))
            return res;

        // if(test_exist_v(X,T_new, is_in) == true){
        //     if(is_in == true){
        //         // is_enable = false;
        //         return MAYBE;
        //     }
        // }
#endif
  }
    T =T_new;
    for(int i = 0; i < T.size(); i++){
        // pour garder le changement de signe
        T[i][0] = std::max(0, T[i][0]-1);
        T[i][0] = std::min((int) x.size()-1, T[i][0]+1);
        T[i][1] = std::max(0, T[i][1]-1);
        T[i][1] = std::min((int) x.size()-1, T[i][1]+1);
//        T[i] = Interval(0, x.size()-1) & T[i].inflate(1);
    }

    // std::cerr << " resturn " << res << "\n";
    return res;
}



ThickBoolean SweepTest::test_exist_v(IntervalVector& X,
                              vector<IntBound>& T1,
                              bool & is_in__){
    bool ok = true;
    bool is_in = false;
    bool is_out = true;
    for(int i = 0; i < T1.size(); i++){
        int i0 = T1[i][0]; //round(T1[i].lb());
        int i1 = T1[i][1]; //round(T1[i].ub());
        Tube v2m(i1-i0+1), v2p(i1-i0+1);
        genV2(X, v2m, v2p, i0, i1); //, ax, ay, ux, uy, i0, i1);

        Tube v1_plus(i1-i0+1),v1_moins(i1-i0+1);
        genV1(X, v1_plus, v1_moins, i0, i1); //, ax, ay, ux, uy, i0, i1);

        bool b1 = keep_constant_sign(v1_plus, true);
        bool b2 = keep_constant_sign(v1_moins, false);
        bool b3 = is_always_negatif(v2m);

        if (!is_in){
          is_in  |= exist_t(v1_plus, v1_moins, v2p);
        }

        is_out &=  (b1 || b2 || b3);
        // if (b4)
        //     is_in |= true;
        // if( !( (b1 || b2) || b3) ){
        //     ok = false;
        // }

    }
    // std::cerr << is_in << " "  << is_out << "\n";
    if (is_in && is_out)
      return MAYBE;
    else if (!is_in && is_out)
      return MAYBE_OUT;
    else if (is_in && !is_out)
      return MAYBE_IN;
    return UNK;
}


// return candidate wrt the heading
std::tuple<double,double> SweepTest::getBounds(int i, int dim){
  // double xmin, xmax, ymin, ymax;
  switch(dim){
    case 0:
      return std::make_tuple(x[i].lb(), y[i].lb());
    case 1:
      return std::make_tuple(x[i].lb(), y[i].ub());
    case 2:
      return std::make_tuple(x[i].ub(), y[i].lb());
    case 3:
      return std::make_tuple(x[i].ub(), y[i].ub());
    default:
      throw std::runtime_error("Error dim must be <= 3");
  };
}
std::tuple<Tube, Tube> SweepTest::genFunction(IntervalVector& box, int idx_deb, int idx_end, int dim){

  if( idx_deb == -1) idx_deb = 0;
  if( idx_end == -1) idx_end = x.size()-1;

  Tube V1(idx_end-idx_deb+1);
  Tube V2(idx_end-idx_deb+1);
  for(int i = idx_deb; i <= idx_end ; i++){
    double xmin, xmax, ymin, ymax;
    std::tie(xmin, ymin) = getBounds(i, dim);
    V1[i-idx_deb] = ux[i]*(box[1]-ymin) - ( uy[i] * (box[0] - xmin));
    V2[i-idx_deb] =  this->L[i] -  ibex::abs( ux[i] * (box[0] - xmin) + uy[i]*(box[1] - ymin) );

  }
  return make_tuple(V1, V2);
}




std::tuple<bool, bool> SweepTest::check_function(IntervalVector& box, int idx_deb, int idx_end){

  std::cerr << idx_deb*0.05 << " " << idx_end*0.05 << " ";
  // sThickBoolean res = false;
  bool is_in = false, is_out = true;
  for (int i = 0; i < 4 ; i++){
    Tube V1, V2;
    std::tie(V1, V2) = genFunction(box, idx_deb, idx_end, i);
    std::vector<IntBound> T2;
    std::vector<IntBound> T = extract_zeros(V1, T2);

    is_in |= (T.size()> 0 && is_inside(V2, T) );
    T.insert(T.end(), T2.begin(), T2.end());
    is_out &= (T.size() ==  0 || is_outside(V2, T) );
    // res |= check_function2(box, idx_deb, idx_end, i);
    // std::cerr << res << " ";
  }

  std::cerr << ">>>>" << is_in << " " << is_out << "\n";
  return std::make_tuple(is_in, is_out);

}

ThickBoolean SweepTest::test_exist_v2(IntervalVector& X,vector<IntBound>& T1, bool & is_in___){

    ThickBoolean ok = UNK;
    // std::cerr << T1.size() <<  "\n";
    bool res_in = false;
    // bool is_out = true;
    bool is_out[4] = {true, true, true, true};
    for(int i = 0; i < T1.size(); i++){
        int i0 = T1[i][0]; //round(T1[i].lb());
        int i1 = T1[i][1]; //round(T1[i].ub());

        for (int i = 0; i < 4 ; i++){
          Tube V1, V2;
          std::tie(V1, V2) = genFunction(X, i0, i1, i);
          std::vector<IntBound> T2;
          std::vector<IntBound> T = extract_zeros(V1, T2);

          res_in |= (T.size()> 0 && is_inside(V2, T) );
          T.insert(T.end(), T2.begin(), T2.end());
          is_out[i] &= (T.size() ==  0 || is_outside(V2, T) );
          // res |= check_function2(box, idx_deb, idx_end, i);
          // std::cerr << res << " ";
        }

        if (res_in == false){
          Tube v2m(i1-i0+1), v2p(i1-i0+1);
          genV2(X, v2m, v2p, i0, i1); //, ax, ay, ux, uy, i0, i1);

          Tube v1_plus(i1-i0+1),v1_moins(i1-i0+1);
          genV1(X, v1_plus, v1_moins, i0, i1); //, ax, ay, ux, uy, i0, i1);

          res_in  |= exist_t(v1_plus, v1_moins, v2p);

        }


        // bool res_in, res_out;
        // std::tie(res_in, res_out) = check_function(X, i0, i1);
        // is_in |= res_in;
        // is_out &= res_out;
    }
    bool res_out = is_out[0] | is_out[1] | is_out[2] | is_out[3];
    // std::cerr << "Result : " << res_in << " " << res_out << "\n";

    if (res_in && res_out)
      return MAYBE;
    else if (!res_in && res_out)
      return MAYBE_OUT;
    else if (res_in && !res_out)
      return MAYBE_IN;
    return UNK;

}

ThickBoolean SweepTest::check_function2(IntervalVector& box, int idx_deb, int idx_end, int idx_bound){

  if( idx_deb == -1) idx_deb = 0;
  if( idx_end == -1) idx_end = x.size()-1;

  Tube V1(idx_end-idx_deb+1);
  Tube V2(idx_end-idx_deb+1);

  //    v1_plus.resize(idx_end-idx_deb+1);
  //    v1_moins.resize(idx_end-idx_deb+1);
  bool res= true;
  double xmin, ymin;
  for(int i = idx_deb; i <= idx_end; i++){
      std::tie(xmin, ymin) = getBounds(i, idx_bound);

      V1[i-idx_deb] = ux[i]*(box[1]-ymin) - ( uy[i] * (box[0] - xmin));
      V2[i-idx_deb] =  this->L[i] -  ibex::abs( ux[i] * (box[0] - xmin) + uy[i]*(box[1] - ymin) );
      res &=  (!V1[i-idx_deb].contains(0) || V2[i-idx_deb].ub() < 0);
  }

  // return OUT if for all t, f(x(t)) != 0 or g(x(t)) < 0
  if (res) return  OUT;
  vector<IntBound> T_v1p;
  std::cerr << idx_bound << " - ";
  std::vector<IntBound> zeros = extract_zeros(V1, T_v1p);
  // for(const auto & v : zeros){
    // std::cerr << "[ " << (idx_deb+v[0])*0.05 << " "  << (idx_deb+v[1])*0.05 << " ] ";
  // }
  // std::cerr << "\n";
  // return IN if exist [t1, t2], f(x([t1,t2])) = 0 and g([t1,t2]) > 0;
  return (zeros.size()> 0 && is_inside(V2, zeros) ) ? IN : UNK;

  // std::cerr << res << " " << xmin << " " << ymin << "\n";
}

// ThickBoolean SweepTest::test_exist_v2(IntervalVector& X,vector<IntBound>& T1, bool & is_in___){
//
//     ThickBoolean ok = UNK;
//     // std::cerr << T1.size() <<  "\n";
//     bool is_in = false;
//     bool is_out = true;
//     for(int i = 0; i < T1.size(); i++){
//         int i0 = T1[i][0]; //round(T1[i].lb());
//         int i1 = T1[i][1]; //round(T1[i].ub());
//         // Tube v2_1(i1-i0+1), v2_2(i1-i0+1);
//         // Tube v1_1(i1-i0+1), v1_2(i1-i0+1);
//
//         // genV1_1()
//
//         // std::array<double, 4> bounds = getBounds(i)
//         bool res_in, res_out;
//         std::tie(res_in, res_out) = check_function(X, i0, i1);
//         is_in |= res_in;
//         is_out &= res_out;
//         // if (res == IN ) return IN;
//         // ThickBoolean res = found ? IN : UNK;
//         // if (found) return IN;
//
//
//
//         // genV1(X, v1_plus, v1_moins, i0, i1); //, ax, ay, ux, uy, i0, i1);
//         // genV2(X, v2m, v2p, i0, i1); //, ax, ay, ux, uy, i0, i1);
//
//
//       // ok = opUnion(ok, res );
//         // vector<IntBound> T_v1p, T_v1m;
//         //
//         // extract_zeros(v1_plus, T_v1p);
//         // extract_zeros(v1_moins, T_v1m);
//         //
//         // bool b1 = (T_v1p.size() == 0); //keep_constant_sign(v1_plus, true);
//         // bool b2 = (T_v1m.size() == 0); //keep_constant_sign(v1_moins, false);
//         //
//         // T_v1p.insert(T_v1p.end(), T_v1m.begin(), T_v1m.end());
//         // bool v2_neg = true;
//         // for( const auto & t: T_v1p){
//         //   int i0 = t[0]; //round(T1[i].lb());
//         //   int i1 = t[1]; //round(T1[i].ub());
//         //   Tube v2m(i1-i0+1), v2p(i1-i0+1);
//         //   genV2(X, v2m, v2p, i0, i1);
//         //   v2_neg &= is_always_negatif(v2m);
//         // }
//         //
//         // if (b1 || b2 || b2) return OUT;
//     }
//     std::cerr << "Result : " << is_in << " " << is_out << "\n";
//     if (is_in && is_out)
//       return MAYBE;
//     else if (!is_in && is_out)
//       return MAYBE_OUT;
//     else if (is_in && !is_out)
//       return MAYBE_IN;
//
//     return UNK;
//
//
//
//
//
//     //     bool b3 = is_always_negatif(v2m);
//     //
//     //     bool b4 = exist_t(v1_plus, v1_moins, v2p);
//     //     if (b4)
//     //         is_in |= true;
//     //     if( !( (b1 || b2) || b3) ){
//     //         ok = false;
//     //     }
//     // }
//     // if(ok == true)
//     //     return true;
//     // return false;
// }
//
//

// Compute v1= det(u, x-a)
Tube SweepTest::genV1(IntervalVector& box, int idx_deb, int idx_end){


    if( idx_deb == -1) idx_deb = 0;
    if( idx_end == -1) idx_end = x.size()-1;

    Tube v1(idx_end- idx_deb+1);
    for(int i = idx_deb; i <= idx_end; i++){
        v1[i-idx_deb] = ux[i]*(box[1]-y[i]) - ( uy[i] * (box[0] - x[i]));
    }
    return v1;
}

// Compute v1= det(u, x-a)
void SweepTest::genV1(IntervalVector& box,Tube& v1_plus, Tube& v1_moins, int idx_deb, int idx_end){


    if( idx_deb == -1) idx_deb = 0;
    if( idx_end == -1) idx_end = x.size()-1;

   v1_plus.resize(idx_end-idx_deb+1);
   v1_moins.resize(idx_end-idx_deb+1);

    for(int i = idx_deb; i <= idx_end; i++){
        const Interval& Iax = x[i];
        const Interval& Iay = y[i];
        const Interval& Iux = ux[i];
        const Interval& Iuy = uy[i];



        Interval ax_xp = (box[0] - Iax.lb());
        Interval ax_xm = (box[0] - Iax.ub());
        Interval ax_yp = (box[1] - Iay.lb());
        Interval ax_ym = (box[1] - Iay.ub());


        Interval abx_aym = min( min( Iux.ub()*ax_yp, Iux.ub()*ax_ym),
                                min( Iux.lb()*ax_yp, Iux.lb()*ax_ym));
        Interval abx_ayp = max( max( Iux.ub()*ax_yp, Iux.ub()*ax_ym),
                                max( Iux.lb()*ax_yp, Iux.lb()*ax_ym));

        Interval aby_axm = min( min( Iuy.ub()*ax_xp, Iuy.ub()*ax_xm),
                                min( Iuy.lb()*ax_xp, Iuy.lb()*ax_xm));
        Interval aby_axp = max( max( Iuy.ub()*ax_xp, Iuy.ub()*ax_xm),
                                max( Iuy.lb()*ax_xp, Iuy.lb()*ax_xm));


        v1_moins[i-idx_deb] = abx_aym - aby_axp;
        v1_plus[i-idx_deb] = abx_ayp - aby_axm;
    }
}


void SweepTest::genV1_1(IntervalVector& box,Tube& v1_1, Tube& v1_2, int idx_deb, int idx_end){


    if( idx_deb == -1) idx_deb = 0;
    if( idx_end == -1) idx_end = x.size()-1;

    Tube v1(idx_end- idx_deb+1);



//    v1_plus.resize(idx_end-idx_deb+1);
//    v1_moins.resize(idx_end-idx_deb+1);
    for(int i = idx_deb; i <= idx_end; i++){
        // double theta = std::atan2(uy[i], ux[i]);
        const Interval& Iax = x[i];
        const Interval& Iay = y[i];
        // const Interval& Iux = ux[i];
        // const Interval& Iuy = uy[i];

        double xmin, xmax, ymin, ymax;

        double theta = std::atan2(uy[i].mid(), ux[i].mid()) + M_PI_2;
        if (theta <= M_PI_2 && theta >= 0){
          xmin = Iax.lb(); xmax = Iax.ub();
          ymin = Iay.ub(); ymax = Iay.lb();
        } else if (theta >= M_PI_2){
          xmin = Iax.lb(); xmax = Iax.lb();
          ymin = Iay.ub(); ymax = Iay.ub();
        } else if (theta >= -M_PI_2 && theta <= 0){
          xmin = Iax.ub(); xmax = Iax.ub();
          ymin = Iay.lb(); ymax = Iay.lb();
        } else {
          xmin = Iax.ub(); xmax = Iax.lb();
          ymin = Iay.lb(); ymax = Iay.ub();
        }

        v1_1[i-idx_deb] = ux[i]*(box[1]-ymin) - ( uy[i] * (box[0] - xmin));
        v1_2[i-idx_deb] = ux[i]*(box[1]-ymax) - ( uy[i] * (box[0] - xmax));
    }
}

Tube SweepTest::genV2(IntervalVector& box, int idx_deb, int idx_end){


    if( idx_deb == -1) idx_deb = 0;
    if( idx_end == -1) idx_end = x.size()-1;

    Tube v2(idx_end - idx_deb+1);
    for(int i = idx_deb; i <= idx_end; i++){
        v2[i-idx_deb] = this->L[i] -  ibex::abs( ux[i] * (box[0] - x[i]) + uy[i]*(box[1] - y[i]) );
    }
    return v2;
}


void SweepTest::genV2(IntervalVector& box,Tube& v2_plus, Tube& v2_moins, int idx_deb, int idx_end){


    if( idx_deb == -1) idx_deb = 0;
    if( idx_end == -1) idx_end = x.size()-1;
   v2_plus.resize(idx_end-idx_deb+1);
   v2_moins.resize(idx_end-idx_deb+1);

    for(int i = idx_deb; i <= idx_end; i++){

        const Interval& Iax = x[i];
        const Interval& Iay = y[i];
        const Interval& Iux = ux[i];
        const Interval& Iuy = uy[i];



        Interval ax_xp = (box[0] -  Iax.lb() );
        Interval ax_xm = (box[0] -  Iax.ub() );
        Interval ax_yp = (box[1] -  Iay.lb() );
        Interval ax_ym = (box[1] -  Iay.ub() );

        Interval uax_xm = min( min( Iux.ub()*ax_xp, Iux.ub()*ax_xm),
                               min( Iux.lb()*ax_xp, Iux.lb()*ax_xm));
        Interval uax_xp = max( max( Iux.ub()*ax_xp, Iux.ub()*ax_xm),
                               max( Iux.lb()*ax_xp, Iux.lb()*ax_xm));

        Interval uax_ym = min( min( Iuy.ub()*ax_yp, Iuy.ub()*ax_ym),
                               min( Iuy.lb()*ax_yp, Iuy.lb()*ax_ym));
        Interval uax_yp = max( max( Iuy.ub()*ax_yp, Iuy.ub()*ax_ym),
                               max( Iuy.lb()*ax_yp, Iuy.lb()*ax_ym));


        Interval sum_m = (uax_xm + uax_ym);
        Interval sum_p = (uax_xp + uax_yp);




        v2_moins[i-idx_deb] = this->L[i] - ibex::max( Interval(0), ibex::sign(sum_m*sum_p))*ibex::min(abs(sum_m), abs(sum_p));
        v2_plus[i-idx_deb] = this->L[i] - ibex::max(abs(sum_m), abs(sum_p));

    }
}


//BoolInterval slow_test(IntervalVector& X,
//                       Tube& ax, Tube& ay,
//                       Tube &ux, Tube& uy,
//                       vector<Interval>& T,
//                       bool& green,
//                       bool debug = false){


//    Tube v1 = genV1(X, ax, ay, ux, uy);
//    Tube v2 = genV2(X, ax, ay, ux, uy);

//    vector<Interval> T2;
//    vector<Interval> T1 = extract_zeros(v1, T2);
//        debug = false;

//    // V1 change de signe
//    if(T1.size() > 0 && is_inside(v1, v2, T1) == true)
//        return YES;

//    T1.insert(T1.end(), T2.begin(), T2.end());
//    if( is_outside(v1, v2, T1)){
//        return NO;
//    }
//    bool is_in = false;
//    if(test_exist_v(X,ax,ay,ux,uy,T1, is_in) == true){
//        if(is_in == true ){
//            green = true;
//            return NO;
//        }
////        return NO;
//    }
//    return MAYBE;
//}




vector<IntBound> extract_zeros( Tube& f, vector<IntBound>& T2, int idx_deb, int idx_end){

    int state, sign;

    if( idx_deb == -1) idx_deb = 0;
    if( idx_end == -1) idx_end = f.size()-1;
    int i0 = idx_deb, i1;
    vector<IntBound> T;
    T2.clear();
    state = IDLE;

    for(int i = idx_deb; i <= idx_end; i++){
        sign = get_sign(f[i]);
        switch(state){
        case IDLE:
            if(sign == POSITIF){
                state = SIGN_PLUS;
            } else if (sign == NEGATIF){
                state = SIGN_MOINS;
            }
            i1 = i;
            if(state != IDLE && i0 != i1){
                T2.push_back({{i0,i1}});
            }
            break;
        case SIGN_PLUS:
            if(sign == ZERO){
                i0 = i-1;
                state = ZERO_PLUS;
            } else if( sign == NEGATIF){
                T.push_back({{i-1, i}});
                state = SIGN_MOINS;
//                cout << "ERREUR +-: Pas de passage vers 0\n";
            }
            break;

        case ZERO_PLUS:
            if(sign == NEGATIF){
                T.push_back({{i0, i}});
                state = SIGN_MOINS;
            } else if (sign == POSITIF){
                state = SIGN_PLUS;
                T2.push_back({{i0, i}});
            }
            break;

        case SIGN_MOINS:
            if(sign == ZERO){
                i0 = i-1;
                state = ZERO_MOINS;
            } else if( sign == POSITIF){
                T.push_back({{i-1, i}});
                state = SIGN_PLUS;
//                cout << "ERREUR -+: Pas de passage vers 0\n";
            }
            break;

        case ZERO_MOINS:
            if(sign == POSITIF){
                T.push_back({{i0, i}});
                state = SIGN_PLUS;
            } else if (sign == NEGATIF){
                state = SIGN_MOINS;
                T2.push_back({{i0, i}});
            }
            break;
        }
    }
    if(state == ZERO_MOINS || state == ZERO_PLUS || state == IDLE){
        T2.push_back({{i0, idx_end}});
    }
    return T;

}








}; // namespace ibex
