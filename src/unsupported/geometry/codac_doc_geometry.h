//============================================================================
//                                P Y I B E X
// File        : pyibex_doc_geometry.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 30, 2017
// Comments    :
//============================================================================

const char* DOC_CTCPOLAR_TYPE=
R"_doc(Polar contractor

Contraction associated with the change between Cartesian to Polar coordinates.
    x = \rho cos(\theta) and y = \rho sin(\theta)

)_doc";

const char* DOC_CTCPOLAR_CONTRACT1=
R"_doc(contract the input box

Args:
    box (IntervalVector): input box ([x],[y],[\rho],[\theta])
)_doc";

const char* DOC_CTCPOLAR_CONTRACT2=
R"_doc(contract the input intervals

Args:
    x (Interval): [x]
    x (Interval): [y]
    \rho (Interval): [\rho]
    \theta (Interval): [\theta]
)_doc";

const char* DOC_CTCPOLARXY_TYPE=
R"_doc(

)_doc";

const char* DOC_SEPPOLARXY_TYPE=
R"_doc(

)_doc";

const char* DOC_CTCSEGMENT_TYPE=
R"_doc(

)_doc";

const char* DOC_SEPPOLYGON_TYPE=
R"_doc(

)_doc";

const char* DOC_SEPDISKEXISTS_TYPE=
R"_doc(

)_doc";

const char* DOC_CATAN2_TYPE=
R"_doc(

)_doc";

const char* DOC_CMOD_TYPE=
R"_doc(

)_doc";

const char* DOC_CMOD_BWD_TYPE=
R"_doc(

)_doc";


const char* DOC_CTCHULL_TYPE=
R"_doc(Compute the outer contraction from a separator using a paver.
  It returns the union of all inner and underterminated boxes.
Args:
    sep (Sep) : the separator to use
    eps (double) : accuracy of the approximation used by the paver
)_doc";
