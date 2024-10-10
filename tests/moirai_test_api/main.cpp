#define CATCH_CONFIG_MAIN  // This tells catch to provide a main() - only do this in one cpp file

#include "catch/catch.hpp"

#define USING_MOIRAI

#define USING_SPECIES_API
#include "../../tests/moirai_test_lib/c_interop_api.h"

#define USING_DOMAIN_A_API
#include "../../tests/moirai_test_lib_a/include/moirai_test_lib_a/c_interop_api.h"

#define USING_DOMAIN_B_API
#include "../../tests/moirai_test_lib_b/include/moirai_test_lib_b/c_interop_api.h"

#include "moirai/reference_handle_map_export.h"

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

TEST_CASE("Multiple APIs with opaque pointers")
{
	A_PTR a = create_a_obj();
	REQUIRE(reference_count(a) == 1);
	A_PTR a2 = clone_handle_a(a);
	REQUIRE(reference_count(a) == 2);
	release_handle(a2);
	REQUIRE(reference_count(a) == 1);
	release_handle(a);


	B_PTR b = create_b_obj();
	REQUIRE(reference_count(b) == 1);
	B_PTR bb3 = clone_handle_b_in_domain_b(b);
	REQUIRE(reference_count(b) == 2);
	release_handle(bb3);
	REQUIRE(reference_count(b) == 1);
	bb3 = new_handle_b_in_domain_b(b);
	REQUIRE(reference_count(b) == 2);
	release_handle(bb3);
	REQUIRE(reference_count(b) == 1);


	B_PTR b2 = clone_handle_b_in_domain_a(b);
	REQUIRE(reference_count(b) == 2);
	B_PTR b3 = new_handle_b_in_domain_a(b);
	REQUIRE(reference_count(b) == 3);
	release_handle(b3);
	REQUIRE(reference_count(b) == 2);
	release_handle(b2);
	REQUIRE(reference_count(b) == 1);
	release_handle(b);

}


TEST_CASE("Multiple APIs with opaque pointers released via Moirai C API")
{
	//see whether we can centralize API calls for disposal of objects,
	//	to limit duplications or errors in using's libraries
	A_PTR a = create_a_obj();
	REQUIRE(reference_count(a) == 1);
	REQUIRE(get_reference_count(a) == 1);
	A_PTR a2 = clone_handle_a(a);
	REQUIRE(reference_count(a) == 2);
	REQUIRE(get_reference_count(a) == 2);
	release_handle_domain_a_via_moirai(a2);
	REQUIRE(reference_count(a) == 1);
	REQUIRE(get_reference_count(a) == 1);
	release_handle_domain_a_via_moirai(a);


	B_PTR b = create_b_obj();
	REQUIRE(reference_count(b) == 1);
	REQUIRE(get_reference_count(b) == 1);
	B_PTR bb3 = clone_handle_b_in_domain_b(b);
	REQUIRE(reference_count(b) == 2);
	REQUIRE(get_reference_count(b) == 2);
	release_handle_domain_b_via_moirai(bb3);
	REQUIRE(reference_count(b) == 1);
	REQUIRE(get_reference_count(b) == 1);
	bb3 = new_handle_b_in_domain_b(b);
	REQUIRE(reference_count(b) == 2);
	REQUIRE(get_reference_count(b) == 2);
	release_handle_domain_b_via_moirai(bb3);
	REQUIRE(reference_count(b) == 1);
	REQUIRE(get_reference_count(b) == 1);


	B_PTR b2 = clone_handle_b_in_domain_a(b);
	REQUIRE(reference_count(b) == 2);
	REQUIRE(get_reference_count(b) == 2);
	B_PTR b3 = new_handle_b_in_domain_a(b);
	REQUIRE(reference_count(b) == 3);
	REQUIRE(get_reference_count(b) == 3);
	release_handle_domain_b_via_moirai(b3);
	REQUIRE(reference_count(b) == 2);
	REQUIRE(get_reference_count(b) == 2);
	release_handle_domain_b_via_moirai(b2);
	REQUIRE(reference_count(b) == 1);
	REQUIRE(get_reference_count(b) == 1);
	release_handle_domain_b_via_moirai(b);

}

#include "moirai/opaque_pointers.hpp"

TEST_CASE("Opaque pointers handlers meant for C++ projects accessing the C API")
{
	using wrapper = moirai::opaque_pointer_handle;
	CAT_PTR cat = create_cat();
	wrapper* cat_h = new wrapper(cat);
	REQUIRE(get_reference_count(cat) == 1);
	MAMAL_PTR mamal = create_mamal_ptr(cat);
	wrapper* mamal_h = new wrapper(mamal);
	REQUIRE(get_reference_count(cat) == 2);
	REQUIRE(get_reference_count(mamal) == 2);
	REQUIRE(cat_h->get_reference_count() == 2);
	REQUIRE(mamal_h->get_reference_count() == 2);
	delete cat_h;
	REQUIRE(get_reference_count(mamal) == 1);
	REQUIRE(mamal_h->get_reference_count() == 1);
	delete mamal_h;
}

static std::string last_message;

void native_exc_handler(const char* str)
{
	std::string msg(str);
	last_message = msg;
}

TEST_CASE("Registering a callback function works as expected")
{
	// trying to diagnose https://github.com/csiro-hydroinformatics/moirai/issues/1
	void* eh_func = (void*)(&native_exc_handler);
	REQUIRE(has_callback_registered() == 0);
	register_exception_callback_function(eh_func);
	REQUIRE(has_callback_registered() == 1);
}