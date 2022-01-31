//============================================================================
//                               P Y I B E X
// File        : exploredAreaTools.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Fev  9, 2015
//============================================================================

#ifndef __EXPLORED_AREA_TOOLS__
#define __EXPLORED_AREA_TOOLS__
#include "ibex_IntervalVector.h"

#include <vector>
#include <array>
using ibex::IntervalVector;
using ibex::Interval;
using std::vector;
typedef std::array<int, 2> IntBound;
typedef vector<Interval> Tube;

// typedef struct Tube {
//     Interval* values;
//     int tab_size;
//     Tube(int size) : tab_size(size){
//       values = new Interval[size];
//     }
//     Tube(Tube& x): tab_size(x.size()){
//       values = new Interval[tab_size];
//       for(int i = 0; i < tab_size; i++){
//         values[i] = x[i];
//       }
//     }
//     Tube(std::vector<Interval> vals): tab_size(vals.size()){
//       values = new Interval[tab_size];
//       for(int i = 0; i < tab_size; i++){
//         values[i] = vals[i];
//       }
//     }
//     Tube() : tab_size(0), values(NULL) {}
//     void resize(int size){
//         tab_size = size;
//         if (values == NULL)
//             delete[] values;
//         values = new Interval[tab_size];
//     }
//     int size();
//     Interval& operator[](int idx);
//     ~Tube(){
//         delete[] values;
//     }
// } Tube;
// inline int Tube::size() { return tab_size; }
// inline Interval& Tube::operator[](int idx){
//     return values[idx];
// }

typedef struct Record {
    IntervalVector B;
    int count;
    vector<IntBound> limits;

    Record(IntervalVector& B, vector<IntBound>& limits):
        B(B), limits(limits), count(2){}
} Record;


#endif // __EXPLORED_AREA_TOOLS__
