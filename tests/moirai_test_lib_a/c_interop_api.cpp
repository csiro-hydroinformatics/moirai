#include "./include/moirai_test_lib_a/c_interop_api.h"

#include "moirai/reference_handle_map_export.h"

#ifdef _WIN32
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif

#define STLSTR_TO_ANSISTR(x) STRDUP(x.c_str())

char* get_name_a(A_PTR x)
{
	auto s = x->get_ptr()->get_name();
	return STLSTR_TO_ANSISTR(s);
}

A_PTR create_a_obj()
{
	return new reference_handle<A>(new A());
}

A_PTR clone_handle_a(A_PTR a)
{
	return a->new_reference_handle();
}

B_PTR clone_handle_b_in_domain_a(B_PTR b)
{
	return b->new_reference_handle();
}

B_PTR new_handle_b_in_domain_a(B_PTR b)
{
	return new reference_handle<B>(b->get_ptr());
}

void meet_b(A_PTR a, B_PTR b) { a->get_ptr()->meet_b(*b->get_ptr()); }

B_PTR spawn_b(A_PTR a) { return new reference_handle<B>(a->get_ptr()->make_b()); }

void free_string(char* s)
{
	delete s;
}

void release_handle(VOID_PTR_PROVIDER_PTR ptr)
{
	delete ptr;
}

void release_handle_domain_a_via_moirai(VOID_PTR_PROVIDER_PTR ptr)
{
	dispose_reference_handle(ptr);
}

int reference_count(VOID_PTR_PROVIDER_PTR ptr)
{
	return ptr->count();
}
