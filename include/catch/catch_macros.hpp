#pragma once

#include "catch/catch.hpp"

// Additional macros that were handy when migrating unit tests written with xUnit++ originally

inline bool WithinRelativeTolerance(double expected, double actual, double tolerance)
{
	if (expected == 0.0)
		throw std::logic_error("A test on a relative tolerance cannot be on an expected value of zero");
	return (std::abs((expected - actual) / expected) < std::abs(tolerance));
}

#define REQUIRE_EQUAL( expected, actual ) REQUIRE( expected == actual )
#define REQUIRE_EQUAL_COMPARER( expected, actual, comparer) REQUIRE( comparer( expected, actual ) )
#define REQUIRE_NOT_EQUAL( expected, actual ) REQUIRE( expected != actual )
#define REQUIRE_NULL( actual ) REQUIRE( nullptr == actual )
#define REQUIRE_NOT_NULL( actual ) REQUIRE( nullptr != actual )
#define REQUIRE_THROWS_EXCEPTION_TYPE(exceptionType, expr) REQUIRE_THROWS_AS( expr, exceptionType)
#define REQUIRE_WITHIN_ABSOLUTE_TOLERANCE( expected, actual, delta) REQUIRE( (abs(expected - actual) < delta) )
#define REQUIRE_WITHIN_RELATIVE_TOLERANCE( expected, actual, delta) REQUIRE( WithinRelativeTolerance(expected, actual, delta) )
