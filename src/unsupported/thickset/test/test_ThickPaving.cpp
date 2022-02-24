#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../thickPaving.h"

#include <sstream>

using ibex::Interval;
using ibex::ThickPaving;
TEST_CASE("Test Constructor", ""){
	IntervalVector X0(2,Interval(-2, 3));
	SECTION(""){
		ThickPaving A(X0, UNK);
		CHECK(A.root.getBox() == IntervalVector(2, Interval(-2, 3)));
	}

	SECTION(""){
		ThickPaving A(X0, IN);
		CHECK(A.root.getBox() == X0);

	}

}

TEST_CASE(" simple bisection "){
	IntervalVector X0(2,Interval(0, 1));
	ibex::FuncTest f = [](const IntervalVector& X)->ThickBoolean {return UNK;};
	ThickPaving A(X0, UNK, ibex::LargestFirst(0, 0.5));

	SECTION(" test eps=0.5"){
		A.Sivia(f, 0.5);
		CHECK(A.root.countLeaves() == 4);
	}

	SECTION(" test eps = 0.25"){
		A.Sivia(f, 0.25);
		CHECK(A.root.countLeaves() == 16);
	}

	SECTION(" test eps = 0.125"){
		A.Sivia(f, 0.125);
		CHECK(A.root.countLeaves() == 64);
	}

	// SECTION(" test eps = 1/2**n"){
	// 	A.Sivia(f, 1.0/std::pow(2,10));
	// 	CHECK(A.root.countLeaves() == std::pow(2,10)*std::pow(2,10)	);
	// }

	// SECTION(" test eps = 1/2**n"){
	// 	A.Sivia(f, 1.0/std::pow(2,13));
	// 	CHECK(A.root.countLeaves() == std::pow(2,13)*std::pow(2,13)	);
	// }
}

TEST_CASE( " THICK DISk"){
	IntervalVector X0(2,Interval(-5, 5));
	Interval mx(0);
	Interval my(0);
	Interval Rmin(0, 4);
	Interval Rmax(0, 6);
	ThickDisk t(mx, my, Rmin, Rmax);
	ThickPaving A(X0, UNK);
	A.Sivia(t, 0.5);
}
