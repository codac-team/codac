//============================================================================
//                               P Y I B E X
// File        : ThickPaving.h
// Author(s)   : Benoit Desrochers, Luc Jaulin
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  15, 2015
//============================================================================

#ifndef __PAVING_H__
#define __PAVING_H__
#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <ibex_Interval.h>
#include <ibex_IntervalVector.h>
#include <ibex_LargestFirst.h>

#include <ThickTest.h>
#include <ThickBoolean.h>
#include <PavingVisitor.h>

#include <PNode.h>

using std::cout;
using std::ostream;
using std::vector;

namespace ibex
{
	class ThickPaving;
	using ThickVisitor = ibex::PavingVisitor<ThickPaving, ThickBoolean>;
	using BINARY_OP = std::function<ThickBoolean(const ThickBoolean &, const ThickBoolean &)>;
	using UNARY_OP = std::function<ThickBoolean(const ThickBoolean &)>;

	using FuncTest = std::function<ThickBoolean(const IntervalVector &X)>;

	bool isThickNotInclude(const IntervalVector &am, const IntervalVector &ap, const IntervalVector &bm, const IntervalVector &bp);
	bool isThickNotInclude(const IntervalVector &am, const IntervalVector &ap, const IntervalVector &b);
	bool isThickIntersect(const IntervalVector &am, const IntervalVector &ap, const IntervalVector &b);
	bool isThickIntersect(const IntervalVector &am, const IntervalVector &ap, const IntervalVector &bm, const IntervalVector &bp);

	class ThickPaving
	{
		using Node = PNode<ThickBoolean>;

	public:
		Node root;
		ibex::LargestFirst bisector;

		// Constructors
		ThickPaving(int dim);
		ThickPaving(const IntervalVector &v, ThickBoolean b);
		ThickPaving(const IntervalVector &p, ThickBoolean b, const LargestFirst &bisector);
		ThickPaving(const ThickPaving &p);

		ThickPaving(IntervalVector &p, ThickTest &pdc, double eps, BINARY_OP op = opInter, bool display = false);
		ThickPaving(IntervalVector &p, FuncTest &test, double eps, BINARY_OP op = opInter, bool display = false);

		ThickPaving(const std::string &filename);
		//~ThickPaving ();

		// Tools
		ThickPaving &Reunite();
		ThickPaving &Clear(ThickBoolean);
		void visit(ThickVisitor &visitor);
		bool is_empty();

		friend ostream &operator<<(ostream &, const ThickPaving &);
		ThickPaving &change(UNARY_OP &op);
		// Paving algorithms
		ThickPaving &Sivia(ThickTest &pdc, double eps, BINARY_OP op = opInter);
		ThickPaving &Sivia(FuncTest &test, double eps, BINARY_OP op = opInter);
		// ThickPaving Sivia(ThickTest& pdc,BINARY_OP op,double eps);
		ThickPaving &USivia(ThickTest &pdc, double eps);

		ThickPaving &Sivia_visu(ThickTest &pdc, double eps, BINARY_OP op);
		ThickPaving &Sivia_visu(FuncTest &test, double eps, BINARY_OP op);

		ThickPaving &FastSivia(ThickTest &pdc, double eps, BINARY_OP op);
		ThickPaving &FastSivia(FuncTest &test, double eps, BINARY_OP op);

		ThickBoolean erode(FuncTest &test, double eps, BINARY_OP op);
		ThickBoolean erode(ThickTest &pdc, double eps, BINARY_OP op);

		void Contract_distance_gt_ThickPaving(double z, IntervalVector &X);
		void Contract_distance_gt_ThickPaving(Node &n, double z, IntervalVector &X);
		// Paving tests
		ThickBoolean contains(const IntervalVector &box);
		// ThickBoolean Inside2(const IntervalVector& box, std::vector<Node*>& lst);
		ThickBoolean Intersect(const IntervalVector &Xm, const IntervalVector &Xp);
		ThickBoolean Intersect_fct(const IntervalVector &Xm, const IntervalVector &Xp);

		ThickBoolean check_empty();

		void ctcOutside(IntervalVector &box);
		void ctcInside(IntervalVector &box);
		void ctcTransform(ThickPaving &B, IntervalVector &T);

		void save(const std::string &filename);

		// std::pair<bool, std::vector<IntervalVector> > Intersect(const IntervalVector& Xm, const IntervalVector& Xp);

		// ThickPaving& sqCap(ThickPaving& B, Function& f);
		// ThickPaving& Sivia(IntervalVector(*F)(const IntervalVector&),ThickPaving& X,ThickBoolean(*op)(const ThickBoolean&, const ThickBoolean&),double eps);

	private:
		ThickBoolean Inside2(const IntervalVector &X, std::vector<Node *> &lst);
		ThickBoolean fastIntersection2(const IntervalVector &Xm, const IntervalVector &Xp, std::vector<Node *> &lst);
		std::pair<bool, std::vector<IntervalVector>> fastIntersection(const IntervalVector &Xm, const IntervalVector &Xp);
		ThickBoolean Xm_inter_Xp_inside_P(IntervalVector X, std::vector<Node *> &lst);
		/* nothing */
	};

	//=========================================================================================
	//=========================  Inline implementation  =======================================
	//=========================================================================================

	inline bool ThickPaving::is_empty()
	{
		this->Reunite();
		return (root.value() == OUT);
		// for( int i = 0; i < B.size(); i++){
		//     if (val[i] == EMPTY)
		//         return true;
		// }
		// return false;
		// return (B.size() == 1  && val[0] == EMPTY);
	}

	inline ThickBoolean ThickPaving::check_empty()
	{
		if (is_empty())
			return OUT;
		std::list<Node *> L;
		L.push_back(&root);
		while (!L.empty())
		{
			Node *n = L.front();
			L.pop_front();
			if (n->isLeaf() && n->value() == IN)
			{
				return IN;
			}
			else if (!n->isLeaf())
			{
				L.push_back(n->left());
				L.push_back(n->right());
			}
		}
		return UNK;
	}

	void op_binaire(ThickPaving &Z, ThickPaving &X, ThickPaving &Y, BINARY_OP op, int k = 0, int i = 0, int j = 0);
	void op_unaire(ThickPaving &Z, ThickPaving &X, UNARY_OP op, int k = 0, int i = 0);
	void Contract_distance_gt_ThickPaving(ThickPaving &Z, double z, IntervalVector &X, int k = 0);
	void Set_Contractor_Inter(ThickPaving &X, ThickPaving &Y, ThickPaving &Z);
	void Set_Contractor_Subset(ThickPaving &X, ThickPaving &Y);
	void Set_Contractor_Disjoint(ThickPaving &X, ThickPaving &Y);
	void Set_Contractor_Overlap(ThickPaving &X, ThickPaving &Y);
	ThickBoolean Overlap(ThickPaving &X, ThickPaving &Y);
	ThickPaving Sqcap(ThickPaving &X, ThickPaving &Y);
	ThickPaving Complement(ThickPaving &X);

} // namespace ibex
//----------------------------------------------------------
#endif // __PAVING_H__
