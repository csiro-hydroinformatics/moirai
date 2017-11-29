#include "c_interop_api.h"

#include "moirai/reference_handle_map_export.h"


#ifdef _WIN32
#define STRDUP _strdup
#else
#include <string.h>
#define STRDUP strdup
#endif

#define STLSTR_TO_ANSISTR(x) STRDUP(x.c_str())

char* get_name(ANIMAL_PTR x)
{
	auto s = x->get_ptr()->get_name();
	return STLSTR_TO_ANSISTR(s);
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

MAMAL_PTR create_mamal_ptr(CAT_PTR cat)
{
	return new reference_handle<mamal>(cat->get_ptr());
}

CAT_PTR create_cat()
{
	return new reference_handle<cat>(new cat());
}

DOG_PTR create_dog()
{
	return new reference_handle<dog>(new dog());
}

HOMO_ERECTUS_PTR create_homo_erectus()
{
	return new reference_handle<homo_erectus>(new homo_erectus());
}

int put_in_dog_kenel(DOG_PTR d)
{
	dog* d_ptr = as_raw_pointer<dog>(d);
	// just for the sake of mocking up:
	if (d_ptr == nullptr)
		return 1;
	return 0;
}

void release_handle_species_via_moirai(VOID_PTR_PROVIDER_PTR ptr)
{
	dispose_reference_handle(ptr);
}
