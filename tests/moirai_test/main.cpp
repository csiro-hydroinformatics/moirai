#define CATCH_CONFIG_MAIN  // This tells catch to provide a main() - only do this in one cpp file

#include "catch/catch.hpp"
#include "../moirai_test_lib/species.h"

#define USING_MOIRAI_API
#include "../../include/moirai/reference_handle.hpp"

using std::vector;
using namespace moirai;
using namespace moirai::tests;

// #define CHECKED_RETRIEVE_PTR(T, x)	(as_raw_pointer<T>(x))

TEST_CASE("Checks that reference_handle can be checked to be of an expected type without crash if using CHECKED_RETRIEVE_PTR", "[Moirai behavior]")
{
	cat* p = new cat();

	reference_handle<mamal> mamal_sp(p);
	opaque_ptr_provider * opaquePtr = &mamal_sp;

	homo_erectus* h;
	dog* d;
	REQUIRE_THROWS(h = CHECKED_RETRIEVE_PTR(homo_erectus, opaquePtr));
	REQUIRE_THROWS(d = CHECKED_RETRIEVE_PTR(dog, opaquePtr));
}

TEST_CASE("reference_handles perform type conversions of raw pointers if possible", "[Moirai behavior]")
{
	cat* c = new cat();

	reference_handle<cat> scat(c);
	REQUIRE(scat.can_cast_to<cat>());
	REQUIRE(scat.can_cast_to<mamal>());
	REQUIRE(scat.can_cast_to<animal>());

	REQUIRE_FALSE(scat.can_cast_to<dog>());
	REQUIRE_FALSE(scat.can_cast_to<homo_erectus>());

	REQUIRE(nullptr != scat.dynamic_cast_to<cat>());
	REQUIRE(nullptr != scat.dynamic_cast_to<mamal>());
	REQUIRE(nullptr != scat.dynamic_cast_to<animal>());

	REQUIRE(nullptr == scat.dynamic_cast_to<dog>());
	REQUIRE(nullptr == scat.dynamic_cast_to<homo_erectus>());

	mamal* m = new mamal();
	reference_handle<mamal> smamal(m);
	REQUIRE_FALSE(smamal.can_cast_to<cat>());
	REQUIRE(smamal.can_cast_to<mamal>());
	REQUIRE(smamal.can_cast_to<animal>());

	m = new cat();
	smamal = reference_handle<mamal>(m);
	REQUIRE(smamal.can_cast_to<mamal>());
	REQUIRE(smamal.can_cast_to<animal>());

	// While the mamal is a cat, we cannot cast to cat
	// because  reference_handle<mamal> looks at static type information only.
	REQUIRE_FALSE(smamal.can_cast_to<cat>());
	// This behavior may change in the future - not needed for now and may be undesirable.

}

TEST_CASE("Reference counting via the API", "[Moirai behavior]")
{
	auto h = new homo_erectus();
	reference_handle<homo_erectus>* p = new reference_handle<homo_erectus>(h);

	REQUIRE(1 == p->count());

	reference_handle<homo_erectus>* p2 = new reference_handle<homo_erectus>(*p);

	REQUIRE(2 == p->count());

	homo_erectus * pmr = p->get_ptr();
	reference_handle<homo_erectus>* p3 = new reference_handle<homo_erectus>(pmr);

	REQUIRE(3 == p->count());

	mamal * pm = p->get_ptr();
	auto spm = new reference_handle<mamal>(pm);

	REQUIRE(4 == p->count());
	REQUIRE(4 == spm->count());
	delete spm;

	REQUIRE(3 == p->count());
	delete p;
	REQUIRE(2 == p2->count());
	delete p2;
	REQUIRE(1 == p3->count());
	REQUIRE(reference_handle_map::instance().has_handle(h));
	delete p3;
	REQUIRE_FALSE(reference_handle_map::instance().has_handle(h));
}

TEST_CASE("Checking the status of a reference handle via the API", "[Moirai behavior]")
{
	auto h = new homo_erectus();
	reference_handle<homo_erectus>* p = new reference_handle<homo_erectus>(h);

	string status = p->get_status();
	REQUIRE(status == "Reference handle for class moirai::tests::homo_erectus. Reference count is 1. Raw pointer appears valid.");
	delete h;
	status = p->get_status();
	REQUIRE(status == "Woah! FUBAR!");
	delete p;
}

