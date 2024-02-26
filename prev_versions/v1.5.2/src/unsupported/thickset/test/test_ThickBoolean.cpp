
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../ThickBoolean.h"

using namespace ibex;


TEST_CASE( "Test operation Inter", "" ) {
	CHECK( (IN & EMPTY) == EMPTY );
	CHECK( (IN & OUT) == EMPTY );
	CHECK( (IN & IN) == IN );
	CHECK( (IN & UNK) == IN );
	CHECK( (IN & MAYBE_OUT) == EMPTY );
	CHECK( (IN & MAYBE_IN) == IN );
	CHECK( (IN & MAYBE) == IN );

	CHECK( (OUT & EMPTY) == EMPTY);
	CHECK( (OUT & OUT)	== OUT );
	CHECK( (OUT & UNK)	== OUT );
	CHECK( (OUT & MAYBE_OUT)	== OUT );
	CHECK( (OUT & MAYBE_IN)	== EMPTY );
	CHECK( (OUT & MAYBE)	== OUT );

	CHECK( (MAYBE & EMPTY) == EMPTY );
	CHECK( (MAYBE & UNK)	== MAYBE );
	CHECK( (MAYBE & MAYBE_OUT)	== MAYBE_OUT );
	CHECK( (MAYBE & MAYBE_IN)	== MAYBE_IN );
	CHECK( (MAYBE & MAYBE)	== MAYBE );

	CHECK( (MAYBE_IN & EMPTY) == EMPTY);
	CHECK( (MAYBE_IN & UNK)	== MAYBE_IN );
	CHECK( (MAYBE_IN & MAYBE_OUT)	== UNK );
	CHECK( (MAYBE_IN & MAYBE_IN)	== MAYBE_IN );

	CHECK( (MAYBE_OUT & EMPTY) == EMPTY);
	CHECK( (MAYBE_OUT & UNK)	== MAYBE_OUT );
	CHECK( (MAYBE_OUT & MAYBE_OUT)	== MAYBE_OUT );

	CHECK( (UNK & EMPTY) == EMPTY);
	CHECK( (UNK & UNK)	== UNK );

	CHECK( (EMPTY & EMPTY) == EMPTY);
}


TEST_CASE( "Test operation Union ", "" ) {
	CHECK( (IN | EMPTY) == IN );
	CHECK( (IN | OUT) == UNK );
	CHECK( (IN | IN) == IN );
	CHECK( (IN | UNK) == UNK );
	CHECK( (IN | MAYBE_OUT) == UNK );
	CHECK( (IN | MAYBE_IN) == MAYBE_IN );
	CHECK( (IN | MAYBE) == MAYBE_IN );

	CHECK( (OUT | EMPTY) == OUT);
	CHECK( (OUT | OUT)	== OUT );
	CHECK( (OUT | UNK)	== UNK );
	CHECK( (OUT | MAYBE_OUT)	== MAYBE_OUT );
	CHECK( (OUT | MAYBE_IN)	== UNK );
	CHECK( (OUT | MAYBE)	== MAYBE_OUT );

	CHECK( (MAYBE | EMPTY) == MAYBE );
	CHECK( (MAYBE | UNK)	== UNK );
	CHECK( (MAYBE | MAYBE_OUT)	== MAYBE_OUT );
	CHECK( (MAYBE | MAYBE_IN)	== MAYBE_IN );
	CHECK( (MAYBE | MAYBE)	== MAYBE );

	CHECK( (MAYBE_IN | EMPTY) == MAYBE_IN);
	CHECK( (MAYBE_IN | UNK)	== UNK );
	CHECK( (MAYBE_IN | MAYBE_OUT)	== UNK );
	CHECK( (MAYBE_IN | MAYBE_IN)	== MAYBE_IN );

	CHECK( (MAYBE_OUT | EMPTY) == MAYBE_OUT);
	CHECK( (MAYBE_OUT | UNK)	== UNK );
	CHECK( (MAYBE_OUT | MAYBE_OUT)	== MAYBE_OUT );

	CHECK( (UNK | EMPTY) == UNK);
	CHECK( (UNK | UNK)	== UNK );

	CHECK( (EMPTY | EMPTY) == EMPTY);
}

TEST_CASE( "Test operation And (logical) ", "" ) {
	CHECK( (IN && EMPTY) == EMPTY );
	CHECK( (IN && OUT) == OUT );
	CHECK( (IN && IN) == IN );
	CHECK( (IN && UNK) == UNK );
	CHECK( (IN && MAYBE_OUT) == MAYBE_OUT );
	CHECK( (IN && MAYBE_IN) == MAYBE_IN );
	CHECK( (IN && MAYBE) == MAYBE );

	CHECK( (OUT && EMPTY) == EMPTY );
	CHECK( (OUT && OUT)	== OUT );
	CHECK( (OUT && UNK)	== OUT );
	CHECK( (OUT && MAYBE_OUT)	== OUT );
	CHECK( (OUT && MAYBE_IN)	== OUT );
	CHECK( (OUT && MAYBE)	== OUT );

	CHECK( (MAYBE && EMPTY) == EMPTY );
	CHECK( (MAYBE && UNK)	== UNK );
	CHECK( (MAYBE && MAYBE_OUT)	== MAYBE_OUT );
	CHECK( (MAYBE && MAYBE_IN)	== MAYBE );
	CHECK( (MAYBE && MAYBE)	== MAYBE );

	CHECK( (MAYBE_IN && EMPTY) == EMPTY );
	CHECK( (MAYBE_IN && UNK)	== UNK );
	CHECK( (MAYBE_IN && MAYBE_OUT)	== MAYBE_OUT );
	CHECK( (MAYBE_IN && MAYBE_IN)	== MAYBE_IN );

	CHECK( (MAYBE_OUT && EMPTY) == EMPTY );
	CHECK( (MAYBE_OUT && UNK)	== UNK );
	CHECK( (MAYBE_OUT && MAYBE_OUT)	== MAYBE_OUT );

	CHECK( (UNK && EMPTY) == EMPTY );
	CHECK( (UNK && UNK)	== UNK );

	CHECK( (EMPTY && EMPTY) == EMPTY );
}


TEST_CASE( "Test operation Or (logical) ", "" ) {
	CHECK( (IN || EMPTY) == EMPTY );
	CHECK( (IN || OUT) == IN );
	CHECK( (IN || IN) == IN );
	CHECK( (IN || UNK) == IN );
	CHECK( (IN || MAYBE_OUT) == IN );
	CHECK( (IN || MAYBE_IN) == IN );
	CHECK( (IN || MAYBE) == IN );

	CHECK( (OUT || EMPTY) == EMPTY );
	CHECK( (OUT || OUT)	== OUT );
	CHECK( (OUT || UNK)	== UNK );
	CHECK( (OUT || MAYBE_OUT)	== MAYBE_OUT );
	CHECK( (OUT || MAYBE_IN)	== MAYBE_IN );
	CHECK( (OUT || MAYBE)	== MAYBE );

	CHECK( (MAYBE || EMPTY) == EMPTY );
	CHECK( (MAYBE || UNK)	== MAYBE );
	CHECK( (MAYBE || MAYBE_OUT)	== MAYBE );
	CHECK( (MAYBE || MAYBE_IN)	== MAYBE_IN );
	CHECK( (MAYBE || MAYBE)	== MAYBE );

	CHECK( (MAYBE_IN || EMPTY) == EMPTY );
	CHECK( (MAYBE_IN || UNK)	== MAYBE_IN );
	CHECK( (MAYBE_IN || MAYBE_OUT)	== MAYBE_IN );
	CHECK( (MAYBE_IN || MAYBE_IN)	== MAYBE_IN );

	CHECK( (MAYBE_OUT || EMPTY) == EMPTY );
	CHECK( (MAYBE_OUT || UNK)	== MAYBE_OUT );
	CHECK( (MAYBE_OUT || MAYBE_OUT)	== MAYBE_OUT );

	CHECK( (UNK || EMPTY) == EMPTY );
	CHECK( (UNK || UNK)	== UNK );

	CHECK( (EMPTY || EMPTY) == EMPTY );
}

TEST_CASE( "Test is singleton "){
	CHECK( is_singleton(IN) == true);
	CHECK( is_singleton(OUT) == true);
	CHECK( is_singleton(MAYBE) == true);
	CHECK( is_singleton(EMPTY) == true);
	CHECK( is_singleton(MAYBE_IN) == false);
	CHECK( is_singleton(MAYBE_OUT) == false);
	CHECK( is_singleton(UNK) == false);
}

TEST_CASE( " Test can be inside / outside"){

	CHECK( can_be_inside(IN) == true );
	CHECK( can_be_inside(MAYBE_IN) == true );
	CHECK( can_be_inside(MAYBE) == true );
	CHECK( can_be_inside(OUT) == false );
	CHECK( can_be_inside(MAYBE_OUT) == false );
	CHECK( can_be_inside(EMPTY) == false );
	CHECK( can_be_inside(UNK) == false );

	CHECK( can_be_outside(IN) == false );
	CHECK( can_be_outside(MAYBE_IN) == false );
	CHECK( can_be_outside(MAYBE) == true );
	CHECK( can_be_outside(OUT) == true );
	CHECK( can_be_outside(MAYBE_OUT) == true );
	CHECK( can_be_outside(EMPTY) == false );
	CHECK( can_be_outside(UNK) == false );
}
