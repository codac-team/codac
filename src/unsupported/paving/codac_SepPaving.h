//============================================================================
//                               P Y I B E X
// File        : SepPaving.h
// Author(s)   : Benoit Desrochers, Luc Jaulin
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  15, 2015
//============================================================================

#ifndef __SEPPAVING_H__
#define __SEPPAVING_H__
#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <ibex_Interval.h>
#include <ibex_IntervalVector.h>
#include <ibex_LargestFirst.h>
#include <ibex_Sep.h>
#include <ibex_Ctc.h>



#include "codac_PavingVisitor.h"
#include "codac_PSetNode.h"

using std::vector;
using std::ostream;
using std::cout;
// using BINARY_OP = std::function<ThickBoolean (const ThickBoolean&,const ThickBoolean&)>;
// using UNARY_OP = std::function<ThickBoolean (const ThickBoolean&)>;

using ibex::LargestFirst;
using ibex::Sep;

namespace pyibex {

class SepPaving;

using SepVisitor = PavingVisitor<SepPaving, IntervalVector>;

class SepPaving : public Sep
{
using Node = PSetNode;
public:
		Node root;
		ibex::LargestFirst bisector;

		// Constructors
    SepPaving(int dim);
    SepPaving(const IntervalVector& v);
		SepPaving(const IntervalVector& p, const ibex::LargestFirst& bisector);
		SepPaving(const SepPaving& p);


    SepPaving(IntervalVector& p, ibex::Sep& pdc,double eps, bool display=false);

		SepPaving(const std::string& filename);
		//~SepPaving ();

		// Tools
		SepPaving& Reunite();
		SepPaving& Clear();
		void visit(SepVisitor & visitor);
		bool is_empty();

		friend ostream& operator<<(ostream&, const SepPaving&);

		// Paving algorithms
		SepPaving& Sivia(ibex::Sep& sep,double eps);

		void Sivia_ops(Sep &sep, double eps, bool intersection=false);

    // Paving tests
		// ThickBoolean contains(const IntervalVector& box);

    void ctcOutside(IntervalVector &box);
    void ctcInside(IntervalVector &box);

		void save(const std::string& filename);

		void separate(IntervalVector& xin, IntervalVector& xout);

		IntervalVector getBoundingBox();

		int dim();

    // std::pair<bool, std::vector<IntervalVector> > Intersect(const IntervalVector& Xm, const IntervalVector& Xp);

    // SepPaving& sqCap(SepPaving& B, Function& f);
    // SepPaving& Sivia(IntervalVector(*F)(const IntervalVector&),SepPaving& X,ThickBoolean(*op)(const ThickBoolean&, const ThickBoolean&),double eps);



private:
	/* nothing */
};



class CtcPaving : public ibex::Ctc
{
public:
	CtcPaving(SepPaving& paving): ibex::Ctc(paving.dim()), paving(paving) {}

	virtual void contract(IntervalVector& X){
		IntervalVector xin(X);
		paving.separate(xin, X);
	}

	SepPaving& paving;

};

//==============================================================================
//=========================  Inline implementation  ============================
//==============================================================================

inline bool SepPaving::is_empty(){
		// this->Reunite();
		// return (root.value() == OUT);
		return false;
}

inline IntervalVector SepPaving::getBoundingBox(){
	return root.m_box_in | root.m_box_out;
}

inline int SepPaving::dim(){
	return getBoundingBox().size();
}
// inline ThickBoolean SepPaving::check_empty(){
	// if ( is_empty() )
	// 	return OUT;
	// std::list<Node*> L;
  // L.push_back(&root);
  // while (!L.empty()){
  //   Node* n=L.front();    L.pop_front();
  //   if (n->isLeaf() && n->value() == IN) {
	// 		return IN;
	//   } else if ( !n->isLeaf() ){
  //     L.push_back(n->left());
  //     L.push_back(n->right());
  //   }
  // }
	// return UNK;
// }



} // end Namespace Ibex;
//----------------------------------------------------------
#endif // __SETPAVING_H__
