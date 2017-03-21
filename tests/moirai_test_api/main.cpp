#define CATCH_CONFIG_MAIN  // This tells catch to provide a main() - only do this in one cpp file

#include "catch.hpp"

#define USING_MOIRAI
#include "../../tests/moirai_test_lib/c_interop_api.h"

using std::vector;
using std::string;

TEST_CASE("Basic object lifetime")
{
	CAT_PTR cat = create_cat();
	DOG_PTR dog = create_dog();

	auto cn = get_name(cat);
	auto dn = get_name(dog);
	REQUIRE(string(cn) == "cat");
	REQUIRE(string(dn) == "dog");

	free_string(cn);
	free_string(dn);

	release_handle(cat);
	release_handle(dog);
}

TEST_CASE("Reference count")
{
	CAT_PTR cat = create_cat();
	REQUIRE(reference_count(cat) == 1);
	MAMAL_PTR mamal = create_mamal_ptr(cat);
	REQUIRE(reference_count(cat) == 2);
	release_handle(cat);
	REQUIRE(reference_count(mamal) == 1);
	release_handle(mamal);
}


