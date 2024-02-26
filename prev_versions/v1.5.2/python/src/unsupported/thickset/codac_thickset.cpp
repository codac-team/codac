//============================================================================
//                               P Y I B E X
// File        : pyibex_thickset.h
// Author(s)   : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  15, 2015
//============================================================================


#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
namespace py = pybind11;
using py::self;
using py::init;
using py::keep_alive;
using py::class_;
using namespace pybind11::literals;

#include "../../core/contractors/static/codac_py_Ctc.h"
#include "../../core/separators/codac_py_Sep.h"

#include "codac_GeoImages.cpp"

#include <codac_type_caster.h>

// #include <SepPaving.h>
#include <codac_Function.h>
#include <codac_ThickPaving.h>
#include <codac_PavingVisitor.h>

#include <codac_ThickTest.h>
#include <codac_ThickBoolean.h>


using namespace codac;

class pyThickTest : public ThickTest {
public:
  /* Inherit the constructors */
  using ThickTest::ThickTest;
  /* Trampoline (need one for each virtual function) */
  virtual ibex::ThickBoolean test(const ibex::IntervalVector& box){
    PYBIND11_OVERLOAD_PURE(
      ibex::ThickBoolean,   /* return type */
      ThickTest,           /* Parent class */
      test,               /* Name of function */
      box                /* Argument(s) */
    );
  }
  virtual void setParams(const ibex::IntervalVector& box){
    PYBIND11_OVERLOAD(
      void,   /* return type */
      ThickTest,           /* Parent class */
      setParams,           /* Name of function */
      box                  /* Argument(s) */
    );
  }
};

// using ibex::ThickVisitor = PavingVisitor<ThickPaving, ThickBoolean>;
class pyThickVisitor : public ThickVisitor {
public:
  /* Inherit the constructors */
  using ThickVisitor::ThickVisitor;
  /* Trampoline (need one for each virtual function) */
  virtual void visit_leaf(const IntervalVector& box, ThickBoolean status){
    PYBIND11_OVERLOAD_PURE(void, ThickVisitor, visit_leaf, box, status);
  }

  virtual void visit_node(const IntervalVector& box){
    PYBIND11_OVERLOAD(void, ThickVisitor, visit_node, box);
  }

  virtual void pre_visit(const ThickPaving& p){
    PYBIND11_OVERLOAD(void, ThickVisitor, pre_visit, p);
  }
  virtual void post_visit(const ThickPaving& p){
    PYBIND11_OVERLOAD(void, ThickVisitor, post_visit, p);
  }
};


// using SepVisitor = PavingVisitor<SepPaving, IntervalVector>;
// class pySepVisitor : public SepVisitor {
// public:
//   /* Inherit the constructors */
//   using SepVisitor::SepVisitor;
//   /* Trampoline (need one for each virtual function) */
//   virtual void visit_leaf(const IntervalVector& boxIn, IntervalVector boxOut){
//     PYBIND11_OVERLOAD_PURE(void, SepVisitor, visit_leaf, boxIn, boxOut);
//   }

//   virtual void visit_node(const IntervalVector& boxIn, const IntervalVector& boxOut){
//     PYBIND11_OVERLOAD(void, SepVisitor, visit_node, boxIn, boxOut);
//   }

//   virtual void pre_visit(const SepPaving& p){
//     PYBIND11_OVERLOAD(void, SepVisitor, pre_visit, p);
//   }
//   virtual void post_visit(const SepPaving& p){
//     PYBIND11_OVERLOAD(void, SepVisitor, post_visit, p);
//   }
// };


class ThickTranslateInPaving : public ThickTest{

public:
	ThickTranslateInPaving(ThickPaving& P, const IntervalVector& dx):
	ThickTest(2),	P(&P), dX(dx) {}

	ThickBoolean test(const IntervalVector& box){
    IntervalVector Xm = box + dX.lb();
    IntervalVector Xp = box + dX.ub();
    return P->Intersect(Xm, Xp);
    // return P->contains(box+dX);
  }
private:
	ThickPaving* P;
	IntervalVector dX;
};

// class ThickTranslate : public ThickTest{
//
// public:
// 	ThickTranslate(ThickTest& T, const IntervalVector& dX):
// 	ThickTest(T.nb_var),	T(T), dX(dX) {}
//
// 	ThickBoolean test(const IntervalVector& box){
//     IntervalVector Xm = box + dX.lb();
//     IntervalVector Xp = box + dX.ub();
//     // return P->Intersect(Xm, Xp);
//     ThickBoolean b1 = T.test(Xm);
//     ThickBoolean b2 = T.test(Xp);
//     // return opAnd(b1,b2);
//     if (b1 == b2)
//       return b1;
//     if (b1 == opNot(b2))
//       return MAYBE;
//
//     return UNK;
//
//     // return b1;
//   }
// private:
// 	ThickTest& T;
// 	const  IntervalVector dX;
// };



class ThickErode: public ThickTest {
  /// Compute B-A
public:
	ThickErode (ThickTest& A, ThickTest& B, const IntervalVector& X0,const double& eps_max): A(A), B(B), eps_max(eps_max), X0(X0), ThickTest(A.nb_var), size(0) {}
	ThickBoolean test(const IntervalVector& X){
    ThickPaving P_lb( X0, MAYBE);
    ThickPaving P_ub( X0, MAYBE);
    this->B.setParams(X);

    double eps =  std::max(X.max_diam()/8, this->eps_max);

    FuncTest test_lb = [this](const IntervalVector& box) {return opRestrict(opLower(this->B.test(box)),  this->A.test(box) ); };
    FuncTest test_ub = [this](const IntervalVector& box) {return opRestrict(opUpper(this->B.test(box)),  this->A.test(box) ); };



    P_lb.FastSivia(test_lb, eps, ibex::opInter);
    P_ub.FastSivia(test_ub, eps, ibex::opInter);

    size = P_lb.root.countLeaves() + P_ub.root.countLeaves();

    if ( P_ub.check_empty() == OUT ) return IN ;
    if ( P_lb.check_empty() == IN  ) return OUT;
    return UNK;

  }
	int size;
private:
	ThickTest &A, &B;
	double eps_max;
  IntervalVector X0;
};

class ThickNot : public ThickTest{
public:
  ThickNot(ThickTest& t): ThickTest(t.nb_var), t(t){}
  ThickBoolean test(const IntervalVector& box){ return opNot(t.test(box)); }
  void setParams(const IntervalVector& params) { t.setParams(params); }
private:
  ThickTest &t;
};

class ThickMinus : public ThickTest{
public:
  ThickMinus(ThickTest& t): ThickTest(t.nb_var), t(t){}
  ThickBoolean test(const IntervalVector& box){ return t.test(-box); }
  void setParams(const IntervalVector& params) { t.setParams(params); }
private:
  ThickTest &t;
};


class ThickDilate: public ThickTest {
public:
	ThickDilate (ThickTest& A, ThickTest& B, const IntervalVector& X0, const double& eps_max): A(A), B(B), eps_max(eps_max), ThickTest(A.nb_var), size(0),
        tErode( *(new ThickNot(A)),*(new ThickMinus(B)), X0, eps_max) {}

	ThickBoolean test(const IntervalVector& X0){

    return opNot (tErode.test(X0) );
    // ThickPaving P_lb( X0, MAYBE);
    // ThickPaving P_ub( X0, MAYBE);
    //
    //
    // FuncTest test_lb = [this](const IntervalVector& box) {return opRestrict(opLower(opNot(this->B.test(box))),  this->A.test(-box) ); };
    // FuncTest test_ub = [this](const IntervalVector& box) {return opRestrict(opUpper(opNot(this->B.test(box))),  this->A.test(-box) ); };
    //
    //
    //
    // P_lb.FastSivia(test_lb, this->eps_max, opInter);
    // P_ub.FastSivia(test_ub, this->eps_max, opInter);
    //
    // size = P_lb.root.countLeaves() + P_ub.root.countLeaves();
    //
    // if ( P_ub.check_empty() == OUT ) return OUT ;
    // if ( P_lb.check_empty() == IN  ) return IN;
    // return UNK;
  }

	int size;
private:
	ThickTest &A, &B;
  ThickErode tErode;
	double eps_max;
};


class ThickAEqualB : public ThickTest{

public:
  ThickAEqualB(ThickPaving& A, ThickPaving&B, double eps ) :ThickTest(2), A(A), B(B), eps(eps){}

  ThickBoolean is_AsubsetB_2(ThickPaving& A, ThickPaving& B, const  IntervalVector& dX, double eps){
    IntervalVector X0 = A.root.getBox();
    FuncTest getAub = [&A](const IntervalVector& X){ return opLower(A.contains(X)); };
    ThickPaving A_lb = ThickPaving(X0, MAYBE);
    A_lb.Sivia( getAub , eps, ibex::opInter);
    X0 = B.root.getBox();
    ThickPaving P_lb(X0, MAYBE);
    ThickPaving P_ub(X0, MAYBE);
    ThickTranslateInPaving T(A_lb, dX);
    FuncTest test_lb = [&T, &B](const IntervalVector &X) { return opRestrict(opLower(T.test(X)), opUpper(B.contains(X)));};
    FuncTest test_ub = [&T, &B](const IntervalVector &X) { return opRestrict(opUpper(T.test(X)), opUpper(B.contains(X)));};
    P_lb.FastSivia(test_lb, eps, ibex::opInter);
    P_ub.FastSivia(test_ub, eps, ibex::opInter);
    ThickBoolean b1 = (P_ub.check_empty());
    ThickBoolean b2 = (P_lb.check_empty());

    if (b2 == IN)
      return OUT;
    if (b1 == OUT)
      return IN;
    return UNK;
  }

  ThickBoolean is_AsubsetB(ThickPaving& A, ThickPaving& B, const  IntervalVector& dX, double eps){
    IntervalVector X0 = A.root.getBox();
    FuncTest getAub = [&A](const IntervalVector& X){ return opLower(A.contains(X)); };
    ThickPaving A_lb(A); //X0, MAYBE);
    UNARY_OP op = [] (const ThickBoolean& b) { return opLower(b);};
    A_lb.change(op);
    // A_lb.Sivia( getAub , eps, opInter);
    X0 = B.root.getBox();
    ThickPaving P_lb(X0, MAYBE);
    // ThickPaving P_ub(X0, MAYBE);
    ThickTranslateInPaving T(A_lb, dX);
    FuncTest test_lb = [&T, &B](const IntervalVector &X) { return opRestrict(opLower(T.test(X)), opUpper(B.contains(X)));};
    // FuncTest test_ub = [&T, &B](const IntervalVector &X) { return opRestrict(opUpper(T.test(X)), opUpper(B.contains(X)));};
    P_lb.FastSivia(test_lb, eps, ibex::opInter);
    // P_ub.FastSivia(test_ub, eps, opInter);
    // ThickBoolean b1 = (P_ub.check_empty());
    ThickBoolean b2 = (P_lb.check_empty());

    if (b2 == IN)
      return OUT;
    // if (b1 == OUT)
      // return IN;
    return UNK;
  }


  ThickBoolean test(const IntervalVector& dX){

    ThickBoolean b1 = is_AsubsetB(A, B, dX, eps);
    ThickBoolean b2 = is_AsubsetB(B, A, -dX, eps);
    // return b1;
    return opAnd(b1, b2);

    // return res;
  }
private:
  ThickPaving &A, &B;
  double eps;
};



class CtcAEqualB /*public Ctc*/{
public:
  CtcAEqualB(ThickPaving& A, ThickPaving& B, double eps ) :
  /*Ctc(2),*/ A(A), B(B), eps(eps), test(A,B,0.3/2.)
  {}

  void contract(IntervalVector& X0){
    std::list<IntervalVector> L= {{X0}};
    IntervalVector X_res = IntervalVector::empty(2);
    while (!L.empty()){
      IntervalVector X = L.front();    L.pop_front();
      if ( X.is_subset(X_res) ) continue;

      ThickBoolean t = test.test(X);

      if ( t == UNK ) {
        if ( X.max_diam() > eps){
          std::pair<ibex::IntervalVector,ibex::IntervalVector> boxes = lf.bisect(X);
          L.push_back(boxes.first);
          L.push_back(boxes.second);
        } else {
          X_res |= X;
        }
      }
    }
    X0 &= X_res;
  }

private:
  ThickPaving &A, &B;
  ThickAEqualB test;
  ibex::LargestFirst lf;
  double eps;
};


unsigned long NbIteration(const IntervalVector& X0, ThickTest& t, double eps){
  std::stack<IntervalVector> s; s.push(X0);
  ibex::LargestFirst bsc(eps/2.);
  unsigned long nIter = 0;
  while (!s.empty()){
    IntervalVector X = s.top();
    ThickBoolean res = t.test(X);
    s.pop();
    nIter++;
    if (!is_singleton(t.test(X)) && X.max_diam() > eps ){
      std::pair<ibex::IntervalVector,ibex::IntervalVector> boxes = bsc.bisect(X);
      s.push(boxes.first);
      s.push(boxes.second);
    }
  }
  return nIter;
}



// void Sivia_wrapper(ThickPaving& X, InclTest& pdc,double eps){
//     X.Sivia(pdc, opInter, eps);
// }
//
// void Sivia2_wrapper(ThickPaving& X, InclTest& pdc,double eps){
//     X.Sivia2(pdc, eps);
// }


ThickPaving my_copy(ThickPaving& p){
    return ThickPaving(p);
}

IntervalVector getX0(ThickPaving &p){
    return p.root.getBox();
}



// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(visit_overload, visit, 1,2)
// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(change_overload, change, 2,3)
// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ctcInside_overload, ctcInside, 1,2)
// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ctcOutside_overload, ctcOutside, 1,2)
// BOOST_PYTHON_FUNCTION_OVERLOADS(thickNotInclude_overload, thickNotInclude, 3,4)
// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(contains_overload, contains, 1,2)
extern void export_thickInterval(py::module& m);
//PYBIND11_MODULE(thickset, m)
//{
void export_unsupported_thickset(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<ibex::Sep, pySep>& sep)
{
  // py::module m("thickset", "python binding ThickPaving Class");

  // export thickInterval module
  export_thickInterval(m);

    // Export comparaison constant
    py::enum_<ThickBoolean>(m, "ThickBoolean")
    .value("IN", ibex::IN)
    .value("OUT", ibex::OUT)
    .value("MAYBE", ibex::MAYBE)
    .value("UNK", ibex::UNK)
    .value("MAYBE_IN", ibex::MAYBE_IN)
    .value("MAYBE_OUT", ibex::MAYBE_OUT)
		.value("EMPTY", ibex::EMPTY)
    // .value("op_Or", ibex::op_Or)
    // .value("op_leq", ibex::op_leq)
    // .value("op_geq", ibex::op_geq)
    // .value("op_Restrict", ibex::op_Restrict)
    // .value("op_Xor", ibex::op_Xor)
    .export_values()
    ;

    m.def("opAnd", &ibex::opAnd);
    m.def("opOr", &ibex::opOr);
    m.def("opInter", &ibex::opInter);
    m.def("opUnion", &ibex::opUnion);
    m.def("opNot", &ibex::opNot);
    m.def("opRestrict", &ibex::opRestrict);
    m.def("opInterUB", &ibex::opInterUB);
    m.def("opInterLB", &ibex::opInterLB);
    m.def("opLower", &ibex::opLower);
    m.def("opUpper", &ibex::opUpper);
    m.def("opInner", &ibex::opInner);
    m.def("opUNK", &ibex::opUNK);
    m.def("opXor", &ibex::opXor);
    m.def("opSupSub", &ibex::opSupSub);
    m.def("is_singleton", &ibex::is_singleton);
    m.def("NbIteration", &NbIteration);
    m.def("thickBooleanSum", &ibex::thickBooleanSum);




    bool (*isThickNotInclude_1)(const IntervalVector&, const IntervalVector&, const IntervalVector&) = &isThickNotInclude;
    bool (*isThickNotInclude_2)(const IntervalVector&, const IntervalVector&, const IntervalVector&, const IntervalVector&) = &isThickNotInclude;
    m.def("isThickNotInclude", isThickNotInclude_1);
    m.def("isThickNotInclude", isThickNotInclude_2);

    bool (*isThickIntersect_1)(const IntervalVector&, const IntervalVector&, const IntervalVector&) = &isThickIntersect;
    bool (*isThickIntersect_2)(const IntervalVector&, const IntervalVector&, const IntervalVector&, const IntervalVector&) = &isThickIntersect;
    m.def("isThickIntersect", isThickIntersect_1);
    m.def("isThickIntersect", isThickIntersect_2);

    // def<const IntervalVector&, const IntervalVector&, const IntervalVector&>("thickNotInclude",  &ibex::thickNotInclude);

    // def<const IntervalVector&, const IntervalVector&, const IntervalVector&, const IntervalVector&>("thickNotInclude",  &ibex::thickNotInclude);
    // def<const IntervalVector&, const IntervalVector&, const IntervalVector&>("thickNotDisjoint", &ibex::thickNotDisjoint);
    // def<const IntervalVector&, const IntervalVector&, const IntervalVector&, const IntervalVector&>("thickNotDisjoint", &ibex::thickNotDisjoint);

    class_<ThickTest, std::unique_ptr<ThickTest>, pyThickTest> TT(m, "ThickTest");
      TT.def(init<int>())
        .def("test",  &ThickTest::test)
        .def("setParams",  &ThickTest::setParams)
        .def("__call__", [](ThickTest& o, const IntervalVector& X){return o.test(X);})
        .def_readonly("nb_var", &ThickTest::nb_var)
        // .def_readwrite("is_enable", &ThickTest::is_enable);
        ;

		class_<ThickDisk>(m, "ThickDisk", TT)
				.def(init<Interval&, Interval&, Interval&, Interval&>())
        .def(init<double&, double&, double&>(), "cx"_a, "cy"_a, "r"_a)
				.def("test", &ThickDisk::test)
				;

		class_<ThickQInter>(m, "ThickQInter", TT)
				.def(init<const ibex::Array<ThickTest>&, int>(), py::keep_alive<1,2>())
				.def("test", &ThickQInter::test)
				;

		class_<ThickInter>(m, "ThickInter", TT)
				.def(init<const ibex::Array<ThickTest>&>(), py::keep_alive<1,2>())
				.def("test", &ThickInter::test)
				;

		class_<ThickOr>(m, "ThickOr", TT)
				.def(init<const ibex::Array<ThickTest>&>(), py::keep_alive<1,2>())
				.def("test", &ThickOr::test)
				;

    class_<ThickTranslateInPaving>(m, "ThickTranslateInPaving", TT)
				.def(init<ThickPaving&, IntervalVector&>(), py::keep_alive<1,2>())
				.def("test", &ThickTranslateInPaving::test)
				;

    class_<ThickNot>(m, "ThickNot", TT)
				.def(init<ThickTest& >(), py::keep_alive<1,2>())
				.def("test", &ThickNot::test)
				;

    class_<ThickMinus>(m, "ThickMinus", TT)
				.def(init<ThickTest& >(), py::keep_alive<1,2>())
				.def("test", &ThickMinus::test)
				;

    class_<ThickAEqualB>(m, "ThickAEqualB", TT)
				.def(init<ThickPaving&, ThickPaving&, double  >(), py::keep_alive<1,2>(),  py::keep_alive<1,3>())
				.def("test", &ThickAEqualB::test)
				;

    // [REMOVED] Provide wrong results !!!

    // class_<ThickTranslate>(m, "ThickTranslate", TT)
		// 		.def(init<ThickTest&, IntervalVector&>(), py::keep_alive<1,2>())
		// 		.def("test", &ThickTranslate::test)
		// 		;

    class_<ThinInverse>(m, "ThinInverse", TT)
        .def(init<Function&, ThickTest&>(), py::keep_alive<1,2>(), py::keep_alive<1,3>())
        .def("test", &ThinInverse::test)
        ;

    class_<ThinfIn>(m, "ThinfIn", TT)
        .def(init<Function&, IntervalVector&>(), py::keep_alive<1,2>())
        .def("test", &ThinfIn::test)
        ;

    class_<ThickSector>(m, "ThickSector", TT)
        .def(init<const Interval&, const Interval, const Interval, const Interval&>(), "x"_a, "y"_a, "theta"_a, "d"_a)
        .def(init<const Interval&, const Interval, const Interval, const Interval, const Interval&>(), "x"_a, "y"_a, "theta"_a, "d1"_a, "d2"_a)
        .def("testPlan",  &ThickSector::testPlan)
        .def("test", &ThickSector::test)
        ;


    class_<ThickErode>(m, "ThickErode", TT)
        .def(init<ThickTest&, ThickTest&, const IntervalVector&, const double& >(), "A"_a, "B"_a, "X0"_a, "eps_max"_a=0.001, py::keep_alive<1,2>(), py::keep_alive<1,3>() )
        .def("test", &ThickErode::test)
        .def_readonly( "size", &ThickErode::size)
        ;
    class_<ThickDilate>(m, "ThickDilate", TT)
        .def(init<ThickTest&, ThickTest&, const IntervalVector&, const double& >(), "A"_a, "B"_a, "X0"_a, "eps_max"_a=0.001, py::keep_alive<1,2>(), py::keep_alive<1,3>() )
        .def("test", &ThickDilate::test)
        .def_readonly( "size", &ThickDilate::size)
        ;

		// class_<ThickInverse>(m, "ThickInverse", TT)
		// 		.def(init<Function&, Function&, ThickTest&>(), py::keep_alive<1,2>(), py::keep_alive<1,3>())
		// 		.def("test", &ThickInverse::test)
		// 		;

    class_<ThickTranslateInBox>(m, "ThickTranslateInBox", TT)
				.def(init<const IntervalVector&, const IntervalVector&>() )
				.def("test", &ThickTranslateInBox::test)
				;



		class_<ThickfIn>(m, "ThickfIn", TT)
				.def(init<GenericFunc, GenericFunc, IntervalVector&>(), py::keep_alive<1,2>(),  py::keep_alive<1,3>())
        .def(init<Function&, Function&, IntervalVector&>(), py::keep_alive<1,2>(),  py::keep_alive<1,3>())
				.def("test", &ThickfIn::test)
				;

    class_<CtcAEqualB>(m, "CtcAEqualB", TT)
        .def(init<ThickPaving&, ThickPaving&, double>(), py::keep_alive<1,2>(), py::keep_alive<1,3>())
        .def("contract", &CtcAEqualB::contract)
        ;


    // class_<SepPaving>(m, "SepPaving")
    //     // .def(init<int>())
    //     .def(init<IntervalVector&> (), "X0"_a)
		// 		.def(init<IntervalVector&, LargestFirst& > (), "X0"_a, "bsc"_a)
		// 		.def(init<IntervalVector&, Sep&,double, bool> () , "X0"_a, "sep"_a, "eps"_a, "display"_a=false )
    //     .def(init<const std::string&> (), "filename"_a)
    //     .def( "Reunite", &SepPaving::Reunite )
    //     .def( "Sivia", ( SepPaving& (SepPaving::*) (Sep& ,double)) &SepPaving::Sivia, py::return_value_policy::take_ownership)
    //     .def( "visit", &SepPaving::visit, py::arg("visitor"))
    //     .def( "save", &SepPaving::save)
    //     ;

    class_<ThickPaving>(m, "ThickPaving")
        // .def(init<int>())
        .def(init<IntervalVector&, ThickBoolean> (), "X0"_a, "val"_a=MAYBE)
				.def(init<IntervalVector&, ThickBoolean, ibex::LargestFirst& > (), "X0"_a, "val"_a, "bsc"_a)
        .def(init<IntervalVector&, ThickTest&,double, BINARY_OP, bool> (), "X0"_a, "test"_a, "eps"_a, py::arg("binary_op")=BINARY_OP(&ibex::opInter), "display"_a=false )
				.def(init<IntervalVector&, FuncTest&,double, BINARY_OP, bool> () , "X0"_a, "test"_a, "eps"_a, py::arg("binary_op")=BINARY_OP(&ibex::opInter), "display"_a=false )
        .def(init<const std::string&> (), "filename"_a)
        // .def(init<IntervalVector&, InclTest&, double>())
        // .def( "Expand", &ThickPaving::Expand )
        // .def( "Remove_sons", &ThickPaving::Remove_sons )
        // .def( "Clean", &ThickPaving::Clean )
        .def( "Reunite", &ThickPaving::Reunite )
        .def( "contains", &ThickPaving::contains)
        .def( "change", &ThickPaving::change)
        .def( "Intersect", &ThickPaving::Intersect)
        // .def( "ctcOutside", &ThickPaving::ctcOutside, py::arg("box"), py::arg("k")=0)
        // .def( "ctcInside", &ThickPaving::ctcInside, py::arg("box"), py::arg("k")=0)
        .def( "Sivia", ( ThickPaving& (ThickPaving::*) (ThickTest& ,double, BINARY_OP )) &ThickPaving::Sivia, py::return_value_policy::take_ownership)
				.def( "Sivia", ( ThickPaving& (ThickPaving::*) (FuncTest&  ,double, BINARY_OP )) &ThickPaving::Sivia, py::return_value_policy::take_ownership)
        .def( "Sivia_visu", ( ThickPaving& (ThickPaving::*) (ThickTest& ,double, BINARY_OP op )) &ThickPaving::Sivia_visu, py::return_value_policy::take_ownership)
				.def( "Sivia_visu", ( ThickPaving& (ThickPaving::*) (FuncTest&  ,double, BINARY_OP op )) &ThickPaving::Sivia_visu, py::return_value_policy::take_ownership)
        .def( "FastSivia", ( ThickPaving& (ThickPaving::*) (ThickTest& ,double, BINARY_OP op )) &ThickPaving::FastSivia, py::return_value_policy::take_ownership)
				.def( "FastSivia", ( ThickPaving& (ThickPaving::*) (FuncTest&  ,double, BINARY_OP op )) &ThickPaving::FastSivia, py::return_value_policy::take_ownership)

        .def( "erode", ( ThickBoolean (ThickPaving::*) (ThickTest& ,double, BINARY_OP op )) &ThickPaving::erode, py::return_value_policy::take_ownership)
        .def( "erode", ( ThickBoolean (ThickPaving::*) (FuncTest&  ,double, BINARY_OP op )) &ThickPaving::erode, py::return_value_policy::take_ownership)

        .def( "visit", &ThickPaving::visit, py::arg("visitor"))
        .def( "ctcTransform", &ThickPaving::ctcTransform)
        .def( "check_empty", &ThickPaving::check_empty)

        .def( "Contract_distance_gt_ThickPaving", ( void (ThickPaving::*) ( double, IntervalVector&) ) &ThickPaving::Contract_distance_gt_ThickPaving)
        // .def( "Contract_distance_gt_ThickPaving", ( void (ThickPaving::*) (Node& , double, IntervalVector&) ) &ThickPaving::Contract_distance_gt_ThickPaving)

        .def( "copy", [](ThickPaving& o){return ThickPaving(o);})
        .def( "save", &ThickPaving::save)
        // .def( "is_empty", &ThickPaving::is_empty)
        .def_property_readonly( "X0", &getX0)
        .def_property_readonly( "size", [](ThickPaving& o){ return o.root.countLeaves();})
        // .def( repr(self))
        ;
    // def( "op_binaire", &op_binaire_wrapper);
    // def( "op_unaire", &op_unaire_wrapper);
    // def( "Set_Contractor_Inter", &ibex::Set_Contractor_Inter);
    // def( "Set_Contractor_Subset", &ibex::Set_Contractor_Subset);
    // def( "Set_Contractor_Disjoint", &ibex::Set_Contractor_Disjoint);
    // def( "Set_Contractor_Overlap", &ibex::Set_Contractor_Overlap);
    // def( "Overlap", &ibex::Overlap);
    // def( "Sqcap", &ibex::Sqcap);
    // def( "Contract_distance_gt_ThickPaving", &ibex::Contract_distance_gt_ThickPaving);
    // def( "Complement", &ibex::Complement);

    // class_<SepVisitor, std::unique_ptr<SepVisitor>, pySepVisitor>(m, "SepVisitor")
    //     .def(init<>())
    //     .def("visit_leaf", &SepVisitor::visit_leaf)
    //     .def("visit_node", ( void (SepVisitor::*) ( const IntervalVector&, const IntervalVector&) )&SepVisitor::visit_node)
    //     .def("pre_visit",  &SepVisitor::pre_visit)
    //     .def("post_visit", &SepVisitor::post_visit)
    //     ;

    class_<ThickVisitor, std::unique_ptr<ThickVisitor>, pyThickVisitor>(m, "PavingVisitor")
        .def(init<>())
        .def("visit_leaf", &ThickVisitor::visit_leaf)
        .def("visit_node", ( void (ThickVisitor::*) ( const IntervalVector&) ) &ThickVisitor::visit_node)
        .def("pre_visit",  &ThickVisitor::pre_visit)
        .def("post_visit", &ThickVisitor::post_visit)
        ;
      // return m.ptr();




  class_<ThickGeoImage>(m, "ThickGeoImage", TT)
      .def(init<py::array_t<DATA_TYPE>&, py::array_t<DATA_TYPE>&, double, double, double, double, ThickBoolean>(),
           py::keep_alive<1,2>(), py::keep_alive<1,3>(),
           "img_in"_a, "img_out"_a, "x0"_a, "y0"_a, "dx"_a, "dy"_a, "outerVal"_a=MAYBE)
      .def("test", &ThickGeoImage::test)
      .def("intersects", &ThickGeoImage::intersects)
      .def_readonly("img_in", &ThickGeoImage::img_in)
      .def_readonly("img_out", &ThickGeoImage::img_out)
      .def_readwrite("timestamp", &ThickGeoImage::timestamp)
      // .def_property_readinly("boundingBox", [](ThickGeoImage& self){return self.boundingBox;})
  ;

  // class_<GeoMapper>(m, "GeoMapper")
  //     .def(init<double, double, int, int, double, double>(), DOC_GEOMAPPER_CONSTRUCTOR, "x0"_a, "y0"_a, "x_size"_a, "y_size"_a, "dx"_a, "dy"_a)
  //     .def("world_to_grid", &GeoMapper::world_to_grid, DOC_GEOMAPPER_CONSTRUCTOR, "box"_a)
  //     .def("grid_to_world", &GeoMapper::grid_to_world, DOC_GEOMAPPER_GRID_TO_WORLD, "pixel_coords"_a)
  //     ;

  class_<GeoImageTranslate>(m, "GeoImageTranslate", TT)
      .def(init<ThickGeoImage&, IntervalVector&>(), py::keep_alive<1,2>())
      .def("test", &GeoImageTranslate::test)
      ;

    class_<ThickImgAEqualB>(m, "ThickImgAEqualB", TT)
        .def(init<ThickGeoImage&,ThickGeoImage&, double>(), py::keep_alive<1,2>())
        .def("test", &ThickImgAEqualB::test)
        ;
}
