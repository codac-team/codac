#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../ThickInterval.h"
#include <sstream>
// using ibex::ThickInterval;
using ibex::Interval;

TEST_CASE("Test Constructor", ""){
	SECTION(""){
		ThickInterval a(Interval(1, 2), Interval(2,3));
		CHECK(a.lb() == Interval(1, 2));
		CHECK(a.ub() == Interval(2, 3));
	}

	SECTION(""){
		ThickInterval a(1,2);
		CHECK(a.lb() == Interval(1, 1));
		CHECK(a.ub() == Interval(2, 2));
	}

	SECTION(""){
		ThickInterval a(Interval(1,2));
		CHECK(a.lb() == Interval(1, 1));
		CHECK(a.ub() == Interval(2, 2));
	}

	SECTION(""){
		ThickInterval a(Interval(0, 3), Interval::POS_REALS);
		CHECK(a.lb() == Interval(0, 3));
		CHECK(a.ub() == Interval::POS_REALS);
	}

	SECTION(""){
		ThickInterval a(Interval::NEG_REALS, Interval(-4, 0));
		CHECK(a.lb() == Interval::NEG_REALS);
		CHECK(a.ub() == Interval(-4, 0));
	}

	SECTION(""){
		ThickInterval a(Interval::NEG_REALS, Interval(-4, 0));
		CHECK(a.lb() == Interval::NEG_REALS);
		CHECK(a.ub() == Interval(-4, 0));
	}

	SECTION(""){
		ThickInterval a(Interval::ALL_REALS, Interval::ALL_REALS);
		CHECK(a.lb() == Interval::ALL_REALS);
		CHECK(a.ub() == Interval::ALL_REALS);
	}

	SECTION(""){
		ThickInterval a(Interval::EMPTY_SET, Interval::EMPTY_SET);
		CHECK( a.is_empty() );
	}


	// SECTION("Cst constructor"){
	// 	ThickInterval res(Interval::EMPTY_SET, Interval::EMPTY_SET);
	// 	CHECK(ThickInterval::EMPTY_THICK == res);
	// }
}

TEST_CASE("Test Invalid constructor", ""){
	SECTION(""){
		ThickInterval a(Interval(2,-2));
		CHECK( a.is_empty() );
	}

	SECTION(""){
		ThickInterval a(Interval(2,3), Interval(1, 2));
		CHECK( a.is_empty() );
	}

	SECTION(""){
		ThickInterval a(Interval(1,3), Interval(1, 2));
		CHECK( a.is_empty() );
	}

	SECTION(""){
		ThickInterval a(Interval(1,2), Interval(1, 2));
		CHECK(a.lb() == Interval(1,2));
		CHECK(a.ub() == Interval(1,2));
	}


	SECTION(""){
		ThickInterval a(Interval::POS_REALS, Interval::NEG_REALS);
		CHECK(a.is_empty() );
		// CHECK(a.ub() == Interval::EMPTY_SET);
	}
}


TEST_CASE("Intersects"){

	ThickInterval a(Interval(-2, 1), Interval(0, 3));
	ThickInterval b(Interval(-2, 1), Interval(2, 3));
	ThickInterval c(Interval(1, 2), Interval(1, 2));
  CHECK( a.intersects( Interval(0, 1) ) == true );
  CHECK( a.intersects( Interval(1,2) )  == false);
  CHECK( b.intersects( Interval(1,2) )  == true);
  CHECK( c.intersects( Interval::EMPTY_SET) == false);
  CHECK( c.intersects( Interval(1,2) ) ==true );

  CHECK( a.intersects( ThickInterval( Interval(-0.0001, 0.5),Interval(2, 3) ) ) == false );
	CHECK( a.intersects( ThickInterval( Interval(-1, 0), Interval(2, 3) ) )  == true );

}

TEST_CASE("test is degenerated"){

	CHECK( ThickInterval(Interval(-2,-2),Interval(-1,3)).is_degenerated() == true);
	CHECK( ThickInterval(Interval(-2,-2),Interval(-1,3)).is_interval()  == false);

	CHECK( ThickInterval(Interval(-3,-1),Interval(4,4)).is_degenerated()  == true);
	CHECK( ThickInterval(Interval(-3,-1),Interval(4,4)).is_interval()  == false);

	CHECK( ThickInterval(Interval(-1,-1),Interval(4,4)).is_degenerated()  == true);
	CHECK( ThickInterval(Interval(-1,-1),Interval(4,4)).is_interval()  == true);

}


TEST_CASE(" test supset "){
	CHECK( ThickInterval(Interval(-2, 1), Interval(0, 4)).superset() == Interval(-2, 4) );
	// empty set
	CHECK( ThickInterval(Interval(-2, 1), Interval(5, 4)).superset() == Interval::EMPTY_SET );
}

TEST_CASE(" test subset "){
  CHECK( ThickInterval(Interval(-2, 1), Interval(0, 4)).subset() == Interval::EMPTY_SET );
  CHECK( ThickInterval(Interval(-2, 1), Interval(4, 5)).subset() == Interval(1, 4) );
}

TEST_CASE(" test Intersection "){
	ThickInterval a( Interval(0, 2), Interval(4, 5));
	ThickInterval b( Interval(1,2), Interval(4, 10));

	SECTION(" intersection 1 "){
			CHECK( a.inter(b) == ThickInterval(Interval(1,2), Interval(4, 5)) );
	}
	SECTION(" interseciont 2 "){
		CHECK( (a & b) ==  ThickInterval(Interval(1, 2), Interval(4, 5)) );
	}
	// SECTION(" intersection 3 "){
		// ThickInterval c( Interval(-2,-1), Interval(3, 4	));
		// CHECK( (a & c) == ThickInterval(ibex::Interval::EMPTY_SET,ibex::Interval::EMPTY_SET) );
	// }
}
