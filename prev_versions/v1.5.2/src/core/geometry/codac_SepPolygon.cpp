//============================================================================
//                                P Y I B E X
// File        : Separator for the constraint : Point inside a polygon
// Authors      : Benoit Desrochers, Gilles Chabert
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#include "ibex_Ctc.h"
#include "ibex_Array.h"
#include "ibex_BoolInterval.h"
#include "codac_SepPolygon.h"
#include "codac_CtcSegment.h"

#include <cmath>

using namespace std;

using namespace ibex;

namespace  codac {


Array<Ctc> segment_ctc_list(vector<double>& ax, vector<double>& ay, vector<double>& bx, vector<double>& by) {
    Array<Ctc> l(ax.size());

    for(unsigned int i=0; i<ax.size(); i++) {
        l.set_ref(i, *new codac::CtcSegment(ax[i],ay[i],bx[i],by[i]));
    }
    return l;
}

Array<Ctc> segment_ctc_list(vector< vector< vector<double> > > points) {
    Array<Ctc> l(points.size());

    for(unsigned int i=0; i<points.size(); i++) {
        l.set_ref(i, *new codac::CtcSegment(points[i][0][0],points[i][0][1],points[i][1][0],points[i][1][1]));
    }
    return l;
}

Array<Ctc> segment_ctc_list(vector< vector <double> > &vertices) {
    Array<Ctc> l(vertices.size());
    size_t n_vertices = vertices.size();
    for(unsigned int i=0; i<n_vertices; i++) {
        l.set_ref(i, *new codac::CtcSegment(
            vertices[i % n_vertices][0],
            vertices[i % n_vertices][1],
            vertices[(i+1) % n_vertices][0],
            vertices[(i+1)%n_vertices][1])
        );
    }
    return l;
}


} // end anonymous namespace

namespace codac {

SepPolygon::SepPolygon(vector< vector<double> > &vertices) :
            SepBoundaryCtc(*new CtcUnion(segment_ctc_list(vertices)),
                           *new PdcInPolygon(vertices)) {

}

SepPolygon::SepPolygon(vector< vector< vector<double> > > &points) :
            SepBoundaryCtc(*new CtcUnion(segment_ctc_list(points)),
                           *new PdcInPolygon(points)) {

}

SepPolygon::SepPolygon(vector<double> &_ax, vector<double> &_ay, vector<double> &_bx, vector<double> &_by) :
            SepBoundaryCtc(*new CtcUnion(segment_ctc_list(_ax,_ay, _bx, _by)),
                           *new PdcInPolygon(_ax,_ay,_bx,_by)) {

}



SepPolygon::~SepPolygon() {
	for(int i=0; i<((CtcUnion&) ctc_boundary).list.size(); i++) {
		delete & (((CtcUnion&) ctc_boundary).list[i]);
	}
	delete &ctc_boundary;

	delete &is_inside;
}

};
