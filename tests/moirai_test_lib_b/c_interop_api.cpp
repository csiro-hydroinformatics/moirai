#include "./include/moirai_test_lib_b/c_interop_api.h"

#include "moirai/reference_handle_map_export.h"

#ifdef _WIN32
#define STRDUP _strdup
#else
#include <string.h>
#define STRDUP strdup
#endif

#define STLSTR_TO_ANSISTR(x) STRDUP(x.c_str())

char* get_name_b(B_PTR x)
{
	auto s = x->get_ptr()->get_name();
	return STLSTR_TO_ANSISTR(s);
}

B_PTR create_b_obj()
{
	return new reference_handle<B>(new B());
}

B_PTR clone_handle_b_in_domain_b(B_PTR b)
{
	return b->new_reference_handle();
}

B_PTR new_handle_b_in_domain_b(B_PTR b)
{
	return new reference_handle<B>(b->get_ptr());
}

void free_string(char* s)
{
	delete s;
}

void release_handle(VOID_PTR_PROVIDER_PTR ptr)
{
	delete ptr;
}

int reference_count(VOID_PTR_PROVIDER_PTR ptr)
{
	return ptr->count();
}

void release_handle_domain_b_via_moirai(VOID_PTR_PROVIDER_PTR ptr)
{
	dispose_reference_handle(ptr);
}

